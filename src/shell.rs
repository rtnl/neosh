use crate::shell_state::ShellState;
use slog::Drain;
use slog::{o, Logger};
use std::collections::HashMap;
use std::error::Error;
use std::sync::Arc;
use tokio::sync::Mutex;

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

    pub async fn get_envs(&self) -> HashMap<String, String> {
        let state = self.state.lock().await;

        state.get_envs().clone()
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
    }

    async fn run_cycle(&self) -> Result<bool, Box<dyn Error>> {
        self.io_push_prompt().await?;

        let input = self.io_pull_input().await?;
        if input.is_empty() {
            return Ok(false);
        }

        let input_split = input.split_ascii_whitespace().collect::<Vec<_>>();

        self.process_input(input_split).await?;

        Ok(true)
    }
}
