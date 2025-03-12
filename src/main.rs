use crate::shell::Shell;

mod shell;

#[tokio::main]
async fn main() {
    let shell = Shell::new();

    shell.run().await;
}
