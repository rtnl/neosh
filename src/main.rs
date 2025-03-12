use crate::shell::Shell;

mod command;
mod shell;
mod shell_io;
mod shell_process;
mod shell_state;

#[tokio::main]
async fn main() {
    let shell = Shell::new();

    shell.run().await;
}
