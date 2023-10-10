i#include "shell.h"

/**
 * main - Initialize the variables of the program
 * @argc: Number of values received from the command line
 * @argv: Values received from the command line
 * @env: Environment variables passed to the program
 * Return: 0 on success.
 */
int main(int argc, char *argv[], char *env[])
{
    ShellData data_struct = {NULL}, *data = &data_struct;
    char *prompt = "";

    initialize_data(data, argc, argv, env);

    signal(SIGINT, handle_ctrl_c);

    if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && argc == 1)
    {
        /* We are in the terminal, interactive mode */
        errno = 2; /* Set errno to an appropriate value */
        prompt = PROMPT_MSG;
    }
    errno = 0;
    sisifo(prompt, data);
    return (0);
}

/**
 * handle_ctrl_c - Print the prompt in a new line when the signal SIGINT (ctrl + c) is sent to the program
 * @unused: Unused parameter
 */
void handle_ctrl_c(int unused)
{
    _print("\n");
    _print(PROMPT_MSG);
}

/**
 * initialize_data - Initialize the struct with the info of the program
 * @data: Pointer to the structure of data
 * @argc: Number of values received from the command line
 * @argv: Array of arguments passed to the program execution
 * @env: Environment passed to the program execution
 */
void initialize_data(ShellData *data, int argc, char *argv[], char **env)
{
    int i = 0;

    data->program_name = argv[0];
    data->input_line = NULL;
    data->command_name = NULL;
    data->exec_count = 0;

    if (argc == 1)
        data->file_desc = STDIN_FILENO;
    else
    {
        data->file_desc = open(argv[1], O_RDONLY);
        if (data->file_desc == -1)
        {
            _printe(data->program_name);
            _printe(": 0: Can't open ");
            _printe(argv[1]);
            _printe("\n");
            exit(127);
        }
    }

    data->tokens = NULL;

    data->env_vars = malloc(sizeof(char *) * 50);
    if (env)
    {
        for (; env[i]; i++)
        {
            data->env_vars[i] = str_duplicate(env[i]);
        }
    }
    data->env_vars[i] = NULL;
    env = data->env_vars;

    data->aliases = malloc(sizeof(char *) * 20);
    for (i = 0; i < 20; i++)
    {
        data->aliases[i] = NULL;
    }
}

/**
 * sisifo - An infinite loop that shows the prompt
 * @prompt: Prompt to be printed
 * @data: Pointer to the program's data
 */
void sisifo(char *prompt, ShellData *data)
{
    int error_code = 0, string_len = 0;

    while (++(data->exec_count))
    {
        _print(prompt);
        error_code = string_len = _getline(data);

        if (error_code == EOF)
        {
            free_all_data(data);
            exit(errno); /* If EOF is the first character of the string, exit */
        }

        if (string_len >= 1)
        {
            expand_aliases(data);
            expand_variables(data);
            tokenize(data);

            if (data->tokens[0])
            {
                /* If a text is given to prompt, execute */
                error_code = execute(data);
                if (error_code != 0)
                    _print_error(error_code, data);
            }
            free_recurrent_data(data);
        }
    }
}

