use crate::shell_builtin::{Command, CommandContext};
use std::error::Error;

pub struct CommandExport {}

impl CommandExport {
    pub fn new() -> Self {
        Self {}
    }
}

impl Command for CommandExport {
    fn get_key(&self) -> Vec<&str> {
        vec!["Set-Env", "set", "export"]
    }

    fn run(&self, ctx: CommandContext) -> Result<bool, Box<dyn Error>> {
        let arg_key = match ctx.get_arg(0) {
            Some(v) => v,
            None => return Ok(true),
        };

        let arg_value = ctx.get_arg(1).unwrap_or("");

        ctx.get_shell().update_env(arg_key, arg_value);

        Ok(true)
    }
}
