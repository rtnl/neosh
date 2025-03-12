use crate::shell_builtin::{Command, CommandContext};
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

    fn run(&self, ctx: CommandContext) -> Result<bool, Box<dyn Error>> {
        let home = ctx.get_env("HOME").unwrap_or("~".to_string());
        let arg_0 = ctx.get_arg(0).unwrap_or(&home);

        let path = PathBuf::from(arg_0);

        match ctx.get_shell().update_path(path) {
            true => {}
            false => {
                return Err(Box::from("invalid path"));
            }
        }

        Ok(true)
    }
}
