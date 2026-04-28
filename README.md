# Mini Shell

A lightweight, feature-rich command shell implementation in C that mimics the behavior of traditional Unix shells like bash. This project demonstrates core shell concepts including command parsing, process management, piping, redirection, and built-in commands.

## Features

- **Interactive Mode**: Full interactive shell with command history and line editing (powered by GNU Readline)
- **Non-Interactive Mode**: Execute commands from scripts or piped input
- **Piping**: Connect multiple commands using pipes (`|`) to chain operations
- **Input/Output Redirection**: 
  - Input redirection with `<` operator
  - Output redirection with `>` operator
- **Background Process Execution**: Run commands in the background using `&` operator
- **Environment Variables**: Set and use environment variables within the shell
- **Built-in Commands**: Support for shell built-in commands like `cd`, `exit`, and `help`
- **Signal Handling**: Graceful handling of SIGINT (Ctrl+C) for process management
- **Command History**: Interactive mode maintains command history for easy recall
- **Debug Mode**: Compile with debugging symbols to trace token parsing and command execution

## Compilation

The project supports multiple compilation modes via the Makefile:

### 1. **Standard Release Mode**
```bash
make main
```
Compiles the shell optimized for production use with readline support.
- **Output**: `./main` executable
- **Compilation Command**: `gcc -o main main.c shell.c parser.c -lreadline`

### 2. **Debug Mode**
```bash
make debug
```
Compiles with debugging symbols and enables debug output tracing.
- **Output**: `./main` executable with debug information
- **Compilation Command**: `gcc -o main main.c shell.c parser.c debug.c -DDEBUG_SHELL -lreadline -g`
- **Features**: Shows token parsing and command structure during execution

### 3. **Test Mode**
```bash
make test
```
Runs comprehensive tests comparing shell output with bash output.
- **Process**: 
  - Executes commands from `test.txt` through mini_shell and generates `mini_shell.txt`
  - Executes the same commands through bash and generates `bash.txt`
  - Compares outputs using `diff` to ensure compatibility

### 4. **Clean**
```bash
make clean
```
Removes compiled executable and generated test files.

## How to Run

### Interactive Mode
Simply execute the shell:
```bash
./main
```
The prompt will display the current working directory, e.g., `[/home/user]>`

### Non-Interactive Mode (Script/Piped Input)
Execute commands from a file or pipe:
```bash
cat commands.txt | ./main
./main < commands.txt
echo "ls -la" | ./main
```

## Usage Examples

### Basic Commands
```bash
[/home]>ls -la
[/home]>pwd
[/home]>echo "Hello World"
```

### Piping
```bash
[/home]>cat file.txt | grep "search_term" | wc -l
```

### Output Redirection
```bash
[/home]>ls -la > output.txt
[/home]>echo "data" > file.txt
```

### Input Redirection
```bash
[/home]>sort < unsorted.txt
```

### Background Execution
```bash
[/home]>sleep 100 &
[/home]>./long_running_process &
```

### Environment Variables
```bash
[/home]>MY_VAR = "Hello"
[/home]>printenv MY_VAR
```

### Directory Navigation
```bash
[/home]>cd /tmp
[/tmp]>cd ..
```

## Built-in Commands

The mini shell provides the following built-in commands:

### 1. **exit**
- **Usage**: `exit`
- **Description**: Terminates the shell session with message "Bye!"
- **Example**: 
  ```bash
  [/home]>exit
  Bye!
  ```

### 2. **cd [directory]**
- **Usage**: `cd [directory]`
- **Description**: Changes the current working directory
- **Arguments**:
  - `directory`: Path to change to (relative or absolute)
  - If no argument provided: Attempts to change directory
- **Example**:
  ```bash
  [/home]>cd /tmp
  [/tmp]>cd ..
  [/]>
  ```

### 3. **help**
- **Usage**: `help`
- **Description**: Displays information about all available built-in commands
- **Example**:
  ```bash
  [/home]>help
  
  Builtins
  1.Exit: To exit the shell
  2.cd[dir]: To change current directory
  3.help: to print this message
  ```

## Architecture

### Core Components

1. **main.c**: Entry point handling interactive and non-interactive modes
2. **shell.c**: Process execution, piping, redirection, and built-in command implementation
3. **parser.c**: Tokenization and command parsing logic
4. **shell.h**: Header file with data structures and function declarations
5. **debug.c**: Debug output functions for development

### Data Structures

- **Token**: Represents parsed tokens with type information (COMMAND, ARGUMENT, PIPE, etc.)
- **Command**: Represents a parsed command with arguments, input/output files, and job type
- **E_table**: Environment variable table for storing variable assignments

## Testing

### Run Tests
```bash
make test
```

### Test Process
The test target:
1. Executes commands from `test.txt` using mini_shell and pipes output to `mini_shell.txt`
2. Executes the same commands using bash and pipes output to `bash.txt`
3. Compares both outputs using `diff` to verify compatibility

### Expected Result
```
0 files differ  (no differences found)
```

## Requirements

- **GCC**: GNU C Compiler
- **Readline Library**: For interactive line editing (`libreadline-dev` on Ubuntu/Debian)
- **Standard POSIX Utilities**: fork, exec, pipe, dup2, signal handling

### Installation on Ubuntu/Debian
```bash
sudo apt-get install build-essential libreadline-dev
```

## Future Enhancements

- Support for more advanced redirections (e.g., `>>`, `2>`, `&>`)
- Job control and foreground/background job management
- Command substitution and glob patterns
- Additional built-in commands (`echo`, `export`, `set`, etc.)
- Alias support
- Script file execution

## License

This is an educational project demonstrating shell implementation concepts.
