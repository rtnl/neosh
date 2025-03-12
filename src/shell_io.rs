use crate::shell::Shell;
use std::error::Error;
use std::io::stdin;

impl Shell {
    pub(crate) async fn io_pull_input(&self) -> Result<String, Box<dyn Error>> {
        let mut input = String::new();

        stdin().read_line(&mut input)?;

        Ok(input)
    }
}
