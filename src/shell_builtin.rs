use crate::command::{Command, CommandContext};
use std::error::Error;
use std::path::PathBuf;

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
        let arg_0 = match ctx.get_arg(0) {
            Some(v) => v,
            None => return Err(Box::from("missing argument")),
        };

        let path = PathBuf::from(ctx.get_arg(0).unwrap());

        match ctx.get_shell().update_path(path) {
            true => {}
            false => {
                return Err(Box::from("invalid path"));
            }
        }

        Ok(())
    }
}
