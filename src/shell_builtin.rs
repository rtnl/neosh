use crate::shell::Shell;
use std::collections::HashMap;
use std::error::Error;
use std::sync::Arc;

pub trait Command {
    fn get_key(&self) -> Vec<&str>;

    fn run(&self, ctx: CommandContext) -> Result<bool, Box<dyn Error>>;
}

pub struct CommandContext<'a> {
    shell: Arc<Shell>,

    name: String,
    args: Vec<&'a str>,
    envs: HashMap<String, String>,
}

impl<'a> CommandContext<'a> {
    pub fn new(
        shell: Arc<Shell>,
        name: &str,
        args: Vec<&'a str>,
        envs: HashMap<String, String>,
    ) -> Self {
        Self {
            shell,
            name: name.to_string(),
            args,
            envs,
        }
    }

    pub fn get_shell(&self) -> &Shell {
        &self.shell
    }

    pub fn get_arg(&self, index: usize) -> Option<&str> {
        self.args.get(index).cloned()
    }

    pub fn get_env(&self, key: &str) -> Option<String> {
        self.envs.get(key).cloned()
    }
}
