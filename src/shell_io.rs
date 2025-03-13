use crate::background::BackgroundProcess;
use crate::shell::Shell;
use colored::Colorize;
use std::error::Error;
use std::io::{stdin, stdout, Write};

impl Shell {
    pub(crate) async fn io_push_prompt(&self) -> Result<(), Box<dyn Error>> {
        let prompt = "neosh $ ".yellow();

        print!("{}", prompt);
        stdout().flush()?;

        Ok(())
    }

    pub(crate) async fn io_pull_input(&self) -> Result<String, Box<dyn Error>> {
        let mut input = String::new();

        stdin().read_line(&mut input)?;

        Ok(input)
    }

    pub(crate) async fn io_push_background_complete(
        &self,
        value: &BackgroundProcess,
    ) -> Result<(), Box<dyn Error>> {
        let msg = format!(
            "completed [id->{}] [pid->{}]",
            value.get_id(),
            value.get_child_id()
        );

        let msg = msg.truecolor(128, 128, 128);

        println!("{}", msg);

        Ok(())
    }
}
