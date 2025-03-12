use crate::command::{Command, CommandContext};
use std::error::Error;

pub struct CommandChangeDirectory {}

impl CommandChangeDirectory {
    pub fn new() -> Self {
        Self {}
    }
}

impl Command for CommandChangeDirectory {
    fn get_key(&self) -> Vec<&str> {
        vec!["Change-Directory", "cd"]
    }

    fn run(&self, ctx: CommandContext) -> Result<(), Box<dyn Error>> {
        println!("hello world");

        Ok(())
    }
}
