use std::collections::HashMap;
use std::env;

pub struct ShellState {
    envs: HashMap<String, String>,
}

impl ShellState {
    pub fn new() -> Self {
        let mut envs = HashMap::new();

        for (key, value) in env::vars() {
            envs.insert(key, value);
        }

        Self { envs }
    }

    pub fn get_envs(&self) -> &HashMap<String, String> {
        &self.envs
    }
}
