use std::collections::HashMap;
use std::env;
use std::env::current_dir;
use std::path::PathBuf;

pub struct ShellState {
    path: PathBuf,
    envs: HashMap<String, String>,
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
}
