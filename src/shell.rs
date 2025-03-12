use crate::shell_builtin::Command;
use crate::shell_builtin_cd::CommandChangeDirectory;
use crate::shell_builtin_exit::CommandExit;
use crate::shell_builtin_export::CommandExport;
use crate::shell_state::ShellState;
use parking_lot::Mutex;
use slog::Drain;
use slog::{o, Logger};
use std::collections::HashMap;
use std::error::Error;
use std::path::PathBuf;
use std::process::exit;
use std::sync::Arc;

pub struct Shell {
    pub logger: Logger,

    state: Arc<Mutex<ShellState>>,

    commands_builtin: Vec<Box<dyn Command>>,
}

impl Shell {
    pub fn new() -> Self {
        let drain = slog_json::Json::default(std::io::stderr()).map(slog::Fuse);
        let drain = slog_async::Async::new(drain).build().fuse();
        let logger = Logger::root(drain, o!());

        Self {
            logger,

            state: Arc::new(Mutex::new(ShellState::new())),

            commands_builtin: vec![
                Box::new(CommandChangeDirectory::new()),
                Box::new(CommandExit::new()),
                Box::new(CommandExport::new()),
            ],
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

    pub fn find_command(&self, key: &str) -> Option<&Box<dyn Command>> {
        let l: Vec<&Box<dyn Command>> = self
            .commands_builtin
            .iter()
            .filter(|command| command.get_key().contains(&key).clone())
            .collect();

        l.first().cloned()
    }

    pub async fn run(&self) {
        loop {
            match self.run_cycle().await {
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

    async fn run_cycle(&self) -> Result<bool, Box<dyn Error>> {
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
