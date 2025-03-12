use crate::shell_builtin::{Command, CommandContext};
use std::error::Error;

pub struct CommandExit {}

impl CommandExit {
    pub fn new() -> Self {
        Self {}
    }
}

impl Command for CommandExit {
    fn get_key(&self) -> Vec<&str> {
        vec!["Exit", "exit"]
    }

    fn run(&self, ctx: CommandContext) -> Result<bool, Box<dyn Error>> {
        Ok(false)
    }
}
