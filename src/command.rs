use std::error::Error;

pub trait Command {
    async fn run(&self, ctx: CommandContext) -> Result<(), Box<dyn Error>>;
}

pub struct CommandContext {}
