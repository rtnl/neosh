use crate::shell::Shell;

mod shell;
mod shell_io;
mod shell_state;

#[tokio::main]
async fn main() {
    let shell = Shell::new();

    shell.run().await;
}
