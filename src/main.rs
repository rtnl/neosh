use crate::shell::Shell;
use std::sync::Arc;

mod background;
mod command;
mod shell;
mod shell_builtin;
mod shell_builtin_cd;
mod shell_builtin_exit;
mod shell_builtin_export;
mod shell_io;
mod shell_process;
mod shell_state;

#[tokio::main]
async fn main() {
    let shell = Arc::new(Shell::new());

    shell.run().await;
}
