
# Banger-Shell Project

This is a simple command-line shell program written in C that mimics the behavior of a Unix shell. It provides basic shell functionality such as executing commands, changing directories (`cd`), and exiting the shell. It also demonstrates process control via `fork()` and `execvp()` for running external commands.

### Features

- **Command execution**: The shell can execute any command available in the system's `PATH`.
- **Change Directory (`cd`)**: The shell supports changing directories.
- **Exit**: The shell can be exited by typing `exit`.
- **Process Management**: The shell spawns a child process to execute commands using `fork()` and `execvp()`.
- **Status reporting**: After executing a command, it reports if the process terminated normally or due to a signal.
- **Input parsing**: The shell parses the input to handle multiple arguments for each command.

### Usage

1. **Run the Shell**:  
   To start the shell, compile and run the C code:
   
   ```bash
   gcc -o banger_shell banger_shell.c
   ./banger_shell
   ```

2. **Basic Commands**:
   - To execute a command, simply type the command and press Enter.
     Example:
     ```bash
     Banger-shell⟁ ls
     Banger-shell⟁ cd /home/user/
     ```
   - To exit the shell, type `exit` and press Enter:
     ```bash
     Banger-shell⟁ exit
     ```

3. **Change Directory**:
   - Use the `cd` command to change directories:
     ```bash
     Banger-shell⟁ cd /path/to/directory
     ```
   - If no directory is specified, it will print a missing argument error.

4. **GIF Animation**:  
   The `shell.gif` file is included to represent an animated loading screen or any interactive media for the shell. You can display the gif using any image viewer or media player on your terminal or through a GUI tool.
   
   The gif can be integrated visually in your shell using a library like `ncurses` if desired, but it is currently outside the scope of the provided shell functionality.

---

### Code Walkthrough

- **Input Parsing**:  
  The shell waits for user input in a loop using `fgets()` and breaks the input into tokens using `strtok()`. The tokens are then stored in an array of arguments (`args[]`), and the first token (command) is executed via `execvp()` in a child process.

- **Process Control**:  
  The shell creates a child process using `fork()`. In the child process, it attempts to run the command using `execvp()`. If successful, the shell's execution continues. If there is an error with `execvp()`, it prints the error and exits the child process.

- **Built-in Commands**:  
  The shell implements built-in commands like `cd` for changing directories. If the command is `cd`, the shell calls `chdir()` to change the directory, otherwise, it executes external programs.

- **Signal Handling**:  
  After the command finishes execution, the shell waits for the child process to terminate using `waitpid()`. It checks the exit status and reports whether the process exited normally or was terminated by a signal.

### File Structure

```
banger_shell.c      # Source code for the shell
shell.gif           # GIF file for the shell's media or animation
README.md           # This README file
```

### Example Interaction

```bash
Banger-shell⟁ ls
file1.txt  file2.txt  directory1/

Banger-shell⟁ cd /home/user/
Banger-shell⟁ pwd
/home/user

Banger-shell⟁ exit
```

---

### Notes
- Make sure the `shell.gif` file is in the same directory as the compiled `banger_shell` executable to maintain consistency in your project structure.
- The shell does not yet handle more complex features like piping, redirection, or background processes, but those can be added as future enhancements.

