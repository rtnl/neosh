use crate::background::BackgroundProcess;
use crate::shell_builtin::{Command, CommandContext};
use crate::shell_builtin_cd::CommandChangeDirectory;
use crate::shell_builtin_exit::CommandExit;
use crate::shell_builtin_export::CommandExport;
use parking_lot::Mutex;
use std::collections::{HashMap, VecDeque};
use std::env;
use std::env::current_dir;
use std::error::Error;
use std::path::PathBuf;
use std::sync::Arc;
use uuid::Uuid;

pub struct ShellState {
    path: PathBuf,
    envs: HashMap<String, String>,
    pub(crate) commands_builtin: Vec<Box<dyn Command>>,
    background_process: HashMap<Uuid, BackgroundProcess>,
    pub(crate) background_process_completed: Arc<Mutex<VecDeque<Uuid>>>,
}

impl ShellState {
    pub fn new() -> Self {
        let mut envs = HashMap::new();

        for (key, value) in env::vars() {
            envs.insert(key, value);
        }

        Self {
            path: current_dir().unwrap_or(PathBuf::new()),
            envs,
            commands_builtin: vec![
                Box::new(CommandChangeDirectory::new()),
                Box::new(CommandExit::new()),
                Box::new(CommandExport::new()),
            ],
            background_process: HashMap::new(),
            background_process_completed: Arc::new(Mutex::new(VecDeque::new())),
        }
    }

    pub fn get_path(&self) -> &PathBuf {
        &self.path
    }

    pub fn update_path(&mut self, value: PathBuf) -> bool {
        let path_new = self.path.join(value);

        match path_new.exists() {
            true => {
                self.path = path_new;
                true
            }
            false => false,
        }
    }

    pub fn update_env(&mut self, key: &str, value: &str) {
        if value.is_empty() {
            self.envs.remove(key);
        } else {
            self.envs.insert(key.to_string(), value.to_string());
        }
    }

    pub fn get_envs(&self) -> &HashMap<String, String> {
        &self.envs
    }

    pub fn execute_command(
        &self,
        key: &str,
        ctx: CommandContext,
    ) -> Result<(bool, bool), Box<dyn Error>> {
        let command = self
            .commands_builtin
            .iter()
            .find(|c| c.get_key().contains(&key));

        match command {
            Some(command) => {
                let flag_continue = command.run(ctx)?;

                Ok((flag_continue, true))
            }
            None => Ok((true, false)),
        }
    }

    pub fn add_background(&mut self, value: BackgroundProcess) {
        self.background_process.insert(value.get_id(), value);
    }

    pub fn take_background(&mut self, id: Uuid) -> Option<BackgroundProcess> {
        self.background_process.remove(&id)
    }
}
