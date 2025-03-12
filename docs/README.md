# neosh

neosh (Neoshell) is a command line shell for Linux and macOS. neosh offers a coherent, deterministic and intuitive
experience with features like standard naming, typed data and scripting.

## Roadmap

- [ ] execution experience
    - [ ] program execution
        - [X] execute programs
        - [ ] execute in background
        - [ ] exit code handling
        - [ ] signal handling
    - [X] PATH support
        - [X] hardcoded PATH values
        - [X] use env
        - [X] relative file paths
    - [X] env support
        - [X] load env on start
        - [X] pass env to programs
        - [X] modify in memory
    - [ ] input handling
        - [X] basic args
        - [ ] multiple commands
        - [ ] file path query
        - [ ] io control

- [ ] built-in commands
    - [X] Exit
    - [X] Change-Directory (cd)
    - [ ] Set-Alias
    - [ ] Start-Timer
    - [ ] Stop-Timer
    - [X] Set-Env (set, export)

- [ ] settings
    - [ ] load from file
    - [ ] save to file
    - [ ] modify in memory
    - [ ] options
        - [ ] custom prompt
            - [ ] template support
        - [ ] execute with PATH
        - [ ] execute local file
        - [ ] toggle command history
        - [ ] toggle input suggestion
        - [ ] toggle llm integration

- [ ] intuitive experience
    - [ ] default programs
    - [ ] command history
    - [ ] input suggestion
    - [ ] colors
    - [ ] llm integration
        - [ ] request prompt
        - [ ] output summary
        - [ ] stream watching
        - [ ] magic rock hallucination for parsing data into supported formats

- [ ] data flow
    - [ ] types
        - [ ] string
        - [ ] number
        - [ ] list
        - [ ] table
        - [ ] map
        - [ ] object
        - [ ] uri
        - [ ] binary
    - [ ] format parsing
        - [ ] json
        - [ ] csv
        - [ ] logging
            - [ ] slog
    - [ ] data query

- [ ] security
    - [ ] permission system
    - [ ] directory restrictions
    - [ ] logging
    - [ ] testing
        - [ ] test script
        - [ ] valgrind test
        - [ ] full coverage

- [ ] meta
    - [ ] documentation page
    - [ ] built-in documentation
    - [ ] built-in tutorial
    - [ ] website
    - [ ] universal install script
    - [ ] distribute packages
        - [ ] voidlinux
        - [ ] archlinux
        - [ ] debian
        - [ ] ubuntu
        - [ ] docker
        - [ ] macos/brew
        - [ ] windows/winget

- [ ] code
    - [ ] macros
        - [ ] result code handling
    - [ ] logging
    - [ ] github workflows
        - [ ] run tests
        - [ ] export binary
        - [ ] build targets
            - [ ] linux/x86_64
            - [ ] linux/arm64
            - [ ] macos/arm64
            - [ ] windows/x86_64
            - [ ] windows/arm64
            - [ ] bsd?

## Author's note

This is a new project that is intentionally ambitious.
The roadmap is subject to change and there's always a chance for me to abandon the project.
That being said, I am already daily driving neosh to force myself into enhancing it.
I would be honored if you decide to give neosh a try, even if you are not a developer.
Naturally, contributions are welcome and you can find the discord invite link below.

## Links

- [Discord server](https://discord.gg/JMaXKWrTMZ)
