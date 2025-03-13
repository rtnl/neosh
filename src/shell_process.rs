use crate::shell::Shell;
use crate::shell_builtin::CommandContext;
use is_executable::IsExecutable;
use std::error::Error;
use std::path::{Path, PathBuf};
use std::sync::Arc;
use tokio::process::Command;

impl Shell {
    pub async fn process_input(
        self: Arc<Self>,
        input_split: Vec<&str>,
    ) -> Result<bool, Box<dyn Error>> {
        let mut input_split = input_split.clone();

        if input_split.is_empty() {
            return Ok(true);
        }

        let flag_background = if input_split.last().cloned().unwrap_or("") == "&" {
            input_split.pop();
            true
        } else {
            false
        };

        let input_head = input_split.first().unwrap().to_string();

        let command_ctx = CommandContext::new(
            self.clone(),
            input_head.as_str(),
            input_split[1..].to_vec(),
            self.get_envs(),
        );
        let (flag_continue, command_found) = self
            .clone()
            .execute_command(input_head.as_str(), command_ctx)?;
        match command_found {
            true => return Ok(flag_continue),
            false => {}
        };

        let path_list = self.process_derive_path(&input_head);

        let path_choice_list = path_list
            .into_iter()
            .filter(|p| p.is_executable())
            .collect::<Vec<_>>();

        let path_choice = match path_choice_list.first() {
            Some(value) => value.clone(),
            None => {
                println!("executable not found");
                return Ok(true);
            }
        };

        let exec_path = path_choice.as_os_str();
        let exec_args = input_split[1..].to_vec();
        let exec_envs = self.get_envs();

        let mut command = Command::new(exec_path);
        command.current_dir(self.get_path());
        command.args(exec_args);
        command.envs(exec_envs);

        let mut child = command.spawn()?;
        if flag_background {
            self.run_background(child).await;
        } else {
            child.wait().await?;
        }

        Ok(true)
    }

    fn process_derive_path(&self, value: &str) -> Vec<Box<Path>> {
        let path_default = vec![
            "/bin",
            "/sbin",
            "/usr/bin",
            "/usr/sbin",
            "/usr/local/bin",
            "/usr/local/sbin",
        ];

        let envs = self.get_envs();

        let path_list = envs
            .get("PATH")
            .map(|it| it.split(":").collect())
            .unwrap_or(path_default);

        let mut path_list: Vec<Box<Path>> = path_list
            .iter()
            .map(|it| {
                let mut p = PathBuf::from(it);

                p = p.join(value);

                Box::from(p.as_path())
            })
            .collect();

        if value.contains("/") {
            path_list.push(Box::from(Path::new(value)));
        }

        path_list
    }
}
