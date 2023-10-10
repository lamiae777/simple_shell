#ifndef SHELL_H
#define SHELL_H

#include <stdio.h> /* for printf */
#include <unistd.h> /* for fork, execve */
#include <stdlib.h>
#include <string.h> /* for strtok */
#include <stddef.h>
#include <errno.h> /* for errno and perror */
#include <sys/types.h> /* for type pid */
#include <sys/wait.h> /* for wait */
#include <sys/stat.h> /* for use of stat function */
#include <signal.h> /* for signal management */
#include <fcntl.h> /* for open files */

/************* MACROS **************/

#include "macros.h" /* for message constants and prompts */

/************* STRUCTURES **************/

/**
 * struct ShellData - Structure for shell-related data.
 * @program_name: The name of the executable
 * @input_line: Pointer to the input read from _getline
 * @command_name: Pointer to the first command typed by the user
 * @exec_count: Number of executed commands
 * @file_desc: File descriptor for input commands
 * @tokens: Pointer to an array of tokenized input
 * @env_vars: Copy of the environment variables
 * @aliases: Array of pointers to aliases
 */
typedef struct ShellData
{
    char *program_name;
    char *input_line;
    char *command_name;
    int exec_count;
    int file_desc;
    char **tokens;
    char **env_vars;
    char **aliases;
} ShellData;

/*========  shell.c  ========*/

/* Initialize the ShellData structure */
void initialize_shell_data(ShellData *data, int argc, char *argv[], char **env);

/* Run the main shell loop */
void run_shell_loop(char *prompt, ShellData *data);

/* Handle Ctrl+C signal */
void handle_ctrl_c(int operation UNUSED);

/*========  _getline.c  ========*/

/* Read one line from standard input */
int read_input_line(ShellData *data);

/* Split the input line based on logical operators if they exist */
int split_logical_operators(char *commands[], int i, char operators[]);

/*======== expansions.c ========*/

/* Expand variables in the input line */
void expand_variables(ShellData *data);

/* Expand aliases in the input line */
void expand_aliases(ShellData *data);

/* Append a string to the end of the buffer */
int append_to_buffer(char *buffer, char *str_to_add);

/*======== str_tok.c ========*/

/* Tokenize the input line using a specified delimiter */
void tokenize_input(ShellData *data);

/* Create a pointer to a part of a string */
char *custom_strtok(char *line, char *delimiter);

/*======== execute.c ========*/

/* Execute a command with its full path */
int execute_command(ShellData *data);

/*======== builtins_list.c ========*/

/* Check if a command is a built-in and execute it if it is */
int execute_builtin(ShellData *data);

/*======== find_in_path.c ========*/

/* Create an array of directories from the PATH environment variable */
char **tokenize_path_directories(ShellData *data);

/* Search for a program in the directories listed in PATH */
int find_program_in_path(ShellData *data);

/* ... Rest of the code remains unchanged ... */

#endif /* SHELL_H */

