<div align="center">

# 🐚 Minishell

### *A minimalist shell implementation in C that mimics the behavior of bash, featuring pipelines, redirections, environment variables, and built-in commands.*

[![42 School](https://img.shields.io/badge/School-000000?style=for-the-badge&logo=42&logoColor=white)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge)](https://github.com/42School/norminette)
![Grade](https://img.shields.io/badge/Grade-100%2F100-success?style=for-the-badge)

</div>

---

## 📋 Table of Contents

- [About](#-about)
- [Features](#-features)
- [Shell Mechanics](#-shell-mechanics)
- [Installation](#-installation)
- [Usage](#-usage)
- [Built-in Commands](#-built-in-commands)
- [Operators & Redirections](#-operators--redirections)
- [Project Structure](#-project-structure)
- [Implementation Details](#-implementation-details)
- [Make Commands](#️-make-commands)
- [Testing Examples](#-testing-examples)
- [Known Limitations](#-known-limitations)
- [Author](#-author)
- [License](#-license)

---

## 🎯 About

**Minishell** is a 42 School project that implements a simplified Unix shell similar to bash. It provides a command-line interface where users can execute commands, manage processes, handle redirections, and work with environment variables. The project demonstrates deep understanding of process management, file descriptors, parsing, and Unix system calls.

### Key Objectives:
- Implement a functional command-line interpreter
- Handle process creation and management with `fork()`, `execve()`, and `wait()`
- Parse complex command lines with pipes and redirections
- Manage environment variables and built-in commands
- Implement proper signal handling
- Ensure memory leak-free execution

---

## ✨ Features

- ✅ **Interactive prompt** with readline library (command history and editing)
- ✅ **Command execution** with absolute/relative paths or PATH resolution
- ✅ **Pipelines** - unlimited pipes (`|`) for command chaining
- ✅ **Redirections** - input (`<`), output (`>`), append (`>>`), heredoc (`<<`)
- ✅ **Environment variables** - expansion with `$VAR` syntax
- ✅ **Exit status** - `$?` expands to last command's exit code
- ✅ **Quote handling** - single quotes (`'`) and double quotes (`"`)
- ✅ **Signal handling** - `Ctrl+C`, `Ctrl+D`, `Ctrl+\` like bash
- ✅ **7 built-in commands** - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- ✅ **Heredoc processing** - multi-line input with variable expansion
- ✅ **Abstract Syntax Tree (AST)** - proper parsing and execution
- ✅ **Memory leak free** - comprehensive cleanup of all resources

---

## 🔧 Shell Mechanics

### Command Execution Flow

1. **Lexical Analysis** - Input string is tokenized into words, operators, and quotes
2. **Syntax Analysis** - Tokens are parsed into an Abstract Syntax Tree (AST)
3. **Expansion** - Environment variables and special parameters are expanded
4. **Execution** - AST is traversed and commands are executed

### Process Management

- Commands are executed in child processes using `fork()` and `execve()`
- Parent process waits for children and collects exit status
- Built-in commands execute in the shell process (no fork)
- Pipelines create multiple processes connected via pipes

### Environment

- Environment variables inherited from parent shell
- Can be modified with `export` and `unset`
- Variables expand in double quotes but not in single quotes
- `$?` expands to the last exit status

---

## 🔧 Installation

```bash
# Clone the repository
git clone https://github.com/Adavitas/minishell.git
cd minishell

# Compile the project
make
```

### Compilation Flags
The project is compiled with: `-Wall -Wextra -Werror -g`

### Dependencies
- **GNU Readline** library (`-lreadline`)
- **libft** (custom C library - included)
- **Standard C library**

### Installing readline (if needed)
```bash
# Ubuntu/Debian
sudo apt-get install libreadline-dev

# macOS
brew install readline
```

---

## 🚀 Usage

```bash
# Run the shell
./minishell

# You'll see the prompt
minishell$

# Execute commands
minishell$ echo Hello World!
Hello World!

minishell$ ls -la | grep minishell
-rwxr-xr-x  1 user  staff  123456 Jan 13 12:00 minishell

minishell$ cat << EOF > file.txt
> This is a heredoc
> Multiple lines supported
> EOF

minishell$ exit
```

### Example Session
```bash
minishell$ export USER=adavitas
minishell$ echo "Hello, $USER!"
Hello, adavitas!
minishell$ pwd
/home/adavitas/minishell
minishell$ cd ..
minishell$ pwd
/home/adavitas
minishell$ echo $?
0
```

---

## 💻 Built-in Commands

| Command | Description | Usage |
|---------|-------------|-------|
| `echo` | Display text with optional `-n` flag | `echo [-n] [text...]` |
| `cd` | Change directory | `cd [path]` |
| `pwd` | Print working directory | `pwd` |
| `export` | Set environment variable | `export VAR=value` |
| `unset` | Unset environment variable | `unset VAR` |
| `env` | Display environment variables | `env` |
| `exit` | Exit the shell | `exit [status]` |

### Built-in Details

#### `echo`
```bash
minishell$ echo Hello World
Hello World
minishell$ echo -n No newline
No newline minishell$
```

#### `cd`
```bash
minishell$ cd /tmp           # Absolute path
minishell$ cd ../home        # Relative path
minishell$ cd                # Go to HOME directory
minishell$ cd -              # Go to previous directory (not implemented)
```

#### `export`
```bash
minishell$ export PATH=/usr/bin:/bin
minishell$ export MY_VAR=hello
minishell$ export             # Display all exported variables
```

#### `exit`
```bash
minishell$ exit              # Exit with last status
minishell$ exit 42           # Exit with status 42
minishell$ exit hello        # Error: numeric argument required
```

---

## ⚙️ Operators & Redirections

### Pipes (`|`)
Chain commands by connecting output of one to input of next:
```bash
minishell$ ls -l | grep minishell | wc -l
minishell$ cat file.txt | sort | uniq | wc -l
minishell$ echo "test" | cat | cat | cat | cat
```

### Input Redirection (`<`)
Redirect file content to command's standard input:
```bash
minishell$ cat < input.txt
minishell$ wc -l < file.txt
```

### Output Redirection (`>`)
Redirect command's output to file (overwrite):
```bash
minishell$ echo "Hello" > output.txt
minishell$ ls -la > files.txt
```

### Append Redirection (`>>`)
Redirect command's output to file (append):
```bash
minishell$ echo "Line 1" > file.txt
minishell$ echo "Line 2" >> file.txt
```

### Heredoc (`<<`)
Multi-line input until delimiter:
```bash
minishell$ cat << EOF
> First line
> Second line
> EOF
First line
Second line

minishell$ cat << DELIMITER > output.txt
> Content with $USER expansion
> DELIMITER
```

### Complex Examples
```bash
# Multiple redirections
minishell$ < input.txt grep "pattern" > output.txt

# Pipes with redirections
minishell$ < file.txt cat | grep test > result.txt

# Multiple pipes and redirections
minishell$ cat << EOF | grep "hello" | wc -l > count.txt
> hello world
> test
> hello again
> EOF
```

---

## 📂 Project Structure

```
minishell/
├── 📄 Makefile                        # Build configuration
├── 📂 includes/                       # Header files
│   ├── minishell.h                # Main header
│   ├── tokenizer.h                # Tokenizer definitions
│   ├── parser.h                   # Parser definitions
│   └── libft.h                    # Libft header
├── 📂 libft/                          # Custom C library
│   ├── ft_atoi.c
│   ├── ft_split.c
│   ├── ft_strdup.c
│   ├── ft_strjoin.c
│   ├── get_next_line.c
│   └── ...                        # Other libft functions
└── 📂 src/                            # Source files
    ├── minishell.c                # Main entry point & REPL
    ├── 📂 lex_analysis/               # Lexical analysis (tokenization)
    │   ├── tokenizer.c            # Main tokenization logic
    │   ├── tokens_types.c         # Token type identification
    │   ├── process_tokens.c       # Token processing
    │   ├── pro_single.c           # Single quote handling
    │   ├── pro_double.c           # Double quote handling
    │   └── manage_lists.c         # Token list management
    ├── 📂 syntax_analysis/            # Syntax analysis (parsing)
    │   ├── syntax_check.c         # Syntax validation
    │   ├── syntax_analysis.c      # Main parsing logic
    │   ├── parsers.c              # AST construction
    │   ├── parser_helper.c        # Parsing utilities
    │   ├── collect_argvs.c        # Argument collection
    │   ├── store_quotes.c         # Quote chain storage
    │   └── ast_node_utils.c       # AST node utilities
    ├── 📂 executor/                   # Command execution
    │   ├── execute_cmd.c          # Command execution
    │   ├── execute_utils.c        # Execution utilities
    │   ├── path.c                 # PATH resolution
    │   ├── redirections.c         # Redirection setup
    │   ├── redir_files.c          # File operations
    │   ├── heredoc.c              # Heredoc processing
    │   ├── heredoc_utils.c        # Heredoc utilities
    │   ├── heredoc_parse.c        # Heredoc parsing
    │   ├── ast_executor.c         # AST traversal & execution
    │   ├── ast_pipe.c             # Pipeline execution
    │   ├── ast_builtin.c          # Built-in execution
    │   ├── ast_helpers.c          # Execution helpers
    │   ├── ast_expand.c           # Variable expansion
    │   └── ast_expand_utils.c     # Expansion utilities
    ├── 📂 builtins/                   # Built-in commands
    │   ├── builtin_utils.c        # Built-in utilities
    │   ├── builtin_echo.c         # echo command
    │   ├── builtin_cd.c           # cd command
    │   ├── builtin_pwd.c          # pwd command
    │   ├── builtin_export.c       # export command
    │   ├── builtin_env.c          # env & unset commands
    │   └── builtin_exit.c         # exit command
    ├── 📂 env/                        # Environment management
    │   ├── env_init.c             # Environment initialization
    │   ├── env_utils.c            # Environment utilities
    │   └── env_expand.c           # Variable expansion
    └── 📂 utils/                      # Utilities
        ├── signals.c              # Signal handling
        └── utils.c                # General utilities
```

---

## 🔬 Implementation Details

### Lexical Analysis (Tokenization)

The tokenizer breaks input into tokens:
- **Words** - Command names, arguments, filenames
- **Operators** - `|`, `<`, `>`, `>>`, `<<`
- **Quotes** - Single (`'`) and double (`"`) quotes

Quoted strings are handled specially:
- Single quotes preserve literal values
- Double quotes allow variable expansion

### Syntax Analysis (Parsing)

The parser builds an Abstract Syntax Tree (AST):
- **Command nodes** - Simple commands with arguments
- **Pipe nodes** - Pipeline connections
- **Redirection nodes** - I/O redirections

Syntax validation includes:
- Checking for unclosed quotes
- Validating operator placement
- Detecting syntax errors

### Expansion

Variable expansion happens before execution:
- `$VAR` - Environment variable expansion
- `$?` - Exit status of last command
- Variables not expanded in single quotes
- Tilde expansion not implemented

### Execution

The executor traverses the AST:
1. **Built-in commands** execute in parent process
2. **External commands** execute in child process via `fork()` and `execve()`
3. **Pipelines** create child processes connected with `pipe()`
4. **Redirections** modify file descriptors before execution

### Signal Handling

Signals are handled to mimic bash behavior:
- **`Ctrl+C` (SIGINT)** - Interrupt current command, display new prompt
- **`Ctrl+D` (EOF)** - Exit shell when buffer is empty
- **`Ctrl+\` (SIGQUIT)** - Ignored in interactive mode

---

## 🛠️ Make Commands

```bash
make          # Compile the program
make clean    # Remove object files
make fclean   # Remove object files and executable
make re       # Recompile everything
```

---

## 🧪 Testing Examples

### Basic Commands
```bash
./minishell
minishell$ ls
minishell$ ls -la
minishell$ pwd
minishell$ echo "Hello, World!"
```

### Environment Variables
```bash
minishell$ export TEST=hello
minishell$ echo $TEST
hello
minishell$ export PATH=/usr/bin:/bin
minishell$ env | grep TEST
TEST=hello
minishell$ unset TEST
```

### Pipes
```bash
minishell$ ls | grep minishell
minishell$ cat /etc/passwd | grep root | wc -l
minishell$ echo "test" | cat | cat | cat
```

### Redirections
```bash
minishell$ echo "Hello" > test.txt
minishell$ cat < test.txt
Hello
minishell$ echo "World" >> test.txt
minishell$ cat test.txt
Hello
World
```

### Heredoc
```bash
minishell$ cat << EOF
> Line 1
> Line 2
> EOF
Line 1
Line 2

minishell$ cat << STOP | grep "test"
> this is a test
> another line
> test again
> STOP
this is a test
test again
```

### Complex Commands
```bash
minishell$ < infile grep "pattern" | sort | uniq > outfile
minishell$ echo "USER=$USER" | cat | cat > info.txt
minishell$ (cat << EOF | grep hello) > result.txt
> hello world
> goodbye
> hello again
> EOF
```

### Exit Status
```bash
minishell$ ls
minishell$ echo $?
0
minishell$ ls nonexistent
ls: nonexistent: No such file or directory
minishell$ echo $?
1
```

### Signal Handling
```bash
minishell$ cat          # Press Ctrl+C
^C
minishell$ sleep 100    # Press Ctrl+C
^C
minishell$              # Press Ctrl+D
exit
```

---

## ⚠️ Known Limitations

The project implements a subset of bash functionality. Not implemented:
- **Logical operators** (`&&`, `||`)
- **Wildcards** (`*`, `?`, `[]`)
- **Subshells** (`()`, backticks, `$()`)
- **Tilde expansion** (`~`)
- **Special characters** (`;`, `&`, `\`, etc.)
- **Job control** (`bg`, `fg`, `jobs`)
- **Alias** and **functions**
- **Advanced redirections** (`>&`, `<&`, etc.)
- **History expansion** (`!`, `!!`, etc.)

---

## 👨‍💻 Author

**Aleksandre Davitashvili** (Adavitas) - *42 Student*

[![GitHub](https://img.shields.io/badge/GitHub-Adavitas-181717?style=flat&logo=github)](https://github.com/Adavitas)

**Zhanat Zhyrgalbayeva** (zzhyrgal) - *42 Student*

[![GitHub](https://img.shields.io/badge/GitHub-zhibeeek-181717?style=flat&logo=github)](https://github.com/zhibeeek)

---

## 📝 License

This project is part of the 42 School curriculum.

---
