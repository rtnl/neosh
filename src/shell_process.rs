use crate::shell::Shell;
use crate::shell_builtin::CommandContext;
use is_executable::IsExecutable;
use std::error::Error;
use std::path::{Path, PathBuf};
use tokio::process::Command;

impl Shell {
    pub async fn process_input(&self, input_split: Vec<&str>) -> Result<bool, Box<dyn Error>> {
        if input_split.is_empty() {
            return Ok(true);
        }

        let input_head = input_split.first().unwrap().to_string();
        match self.find_command(input_head.as_str()) {
            Some(command) => {
                let ctx = CommandContext::new(
                    self,
                    input_head.as_str(),
                    input_split[1..].to_vec(),
                    self.get_envs(),
                );

                return match command.run(ctx) {
                    Ok(flag_continue) => Ok(flag_continue),
                    Err(err) => {
                        eprintln!("failed at executing command: {}", err);
                        Ok(true)
                    }
                };
            }
            None => {}
        }

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
        command.spawn()?.wait().await?;

        Ok(true)
    }

    fn process_derive_path(&self, value: &str) -> Vec<Box<Path>> {
        let path_list = vec![
            "/bin",
            "/sbin",
            "/usr/bin",
            "/usr/sbin",
            "/usr/local/bin",
            "/usr/local/sbin",
        ];

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
