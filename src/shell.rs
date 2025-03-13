use crate::background::BackgroundProcess;
use crate::shell_builtin::CommandContext;
use crate::shell_state::ShellState;
use parking_lot::Mutex;
use slog::Drain;
use slog::{o, Logger};
use std::collections::HashMap;
use std::error::Error;
use std::path::PathBuf;
use std::process::exit;
use std::sync::Arc;
use tokio::process::Child;

pub struct Shell {
    pub logger: Logger,

    state: Arc<Mutex<ShellState>>,
}

impl Shell {
    pub fn new() -> Self {
        let drain = slog_json::Json::default(std::io::stderr()).map(slog::Fuse);
        let drain = slog_async::Async::new(drain).build().fuse();
        let logger = Logger::root(drain, o!());

        Self {
            logger,

            state: Arc::new(Mutex::new(ShellState::new())),
        }
    }

    pub fn get_path(&self) -> PathBuf {
        let state = self.state.lock();

        state.get_path().clone()
    }

    pub fn update_path(&self, value: PathBuf) -> bool {
        let mut state = self.state.lock();

        state.update_path(value)
    }

    pub fn update_env(&self, key: &str, value: &str) {
        let mut state = self.state.lock();

        state.update_env(key, value);
    }

    pub fn get_envs(&self) -> HashMap<String, String> {
        let state = self.state.lock();

        state.get_envs().clone()
    }

    pub fn execute_command(
        self: Arc<Self>,
        key: &str,
        ctx: CommandContext,
    ) -> Result<(bool, bool), Box<dyn Error>> {
        let state = self.state.lock();

        state.execute_command(key, ctx)
    }

    pub fn add_background(&self, value: BackgroundProcess) {
        let mut state = self.state.lock();

        state.add_background(value)
    }

    pub async fn run_background(self: Arc<Self>, mut child: Child) {
        let value = BackgroundProcess::new(child.id().unwrap_or(0));
        let value_id = value.get_id();

        self.add_background(value);

        let state = self.state.lock();
        let completed = state.background_process_completed.clone();

        tokio::spawn(async move {
            child.wait().await.unwrap();

            let mut c = completed.lock();

            c.push_back(value_id);
        });
    }

    pub async fn run(self: Arc<Self>) {
        loop {
            match self.clone().run_cycle().await {
                Ok(flag_continue) => {
                    match flag_continue {
                        false => break,
                        true => {}
                    };
                }
                Err(err) => eprintln!("failed at running cycle: {}", err),
            };
        }

        self.shutdown();
    }

    async fn run_cycle(self: Arc<Self>) -> Result<bool, Box<dyn Error>> {
        self.io_push_prompt().await?;

        let input = self.io_pull_input().await?;
        if input.is_empty() {
            return Ok(false);
        }

        let input_split = input.split_ascii_whitespace().collect::<Vec<_>>();

        let flag_continue = self.process_input(input_split).await?;
        Ok(flag_continue)
    }

    pub fn shutdown(&self) {
        exit(0);
    }
}
