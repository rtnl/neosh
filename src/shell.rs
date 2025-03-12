use crate::shell_state::ShellState;
use slog::Drain;
use slog::{info, o, Logger};
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

    pub async fn run(&self) {
        loop {
            match self.run_cycle().await {
                Ok(_) => (),
                Err(err) => eprintln!("failed at running cycle: {}", err),
            };
        }
    }

    async fn run_cycle(&self) -> Result<(), Box<dyn Error>> {
        let input = self.io_pull_input().await?;
        let input_split = input.split_ascii_whitespace().collect::<Vec<_>>();

        info!(self.logger, "input_split: {:?}", input_split);

        Ok(())
    }
}
