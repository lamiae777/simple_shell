#include "shell.h"

/**
 * _strtok - Separates strings with delimiters
 * @line: Pointer to the input string to be tokenized
 * @delim: Characters to be used as delimiters
 *
 * Return: A pointer to the next token or NULL if no more tokens are found.
 */
char *_strtok(char *line, const char *delim)
{
	static char *str;
	char *token_start;
	int is_delimiter;

	if (line != NULL)
	{
		str = line;
	}

	/* Skip leading delimiters */
	while (*str && strchr(delim, *str))
	{
		str++;
	}

	if (*str == '\0')
	{
		return (NULL); /* No more tokens found */
	}

	token_start = str; /* Starting position of the token */

	while (*str)
	{
		is_delimiter = strchr(delim, *str) != NULL;

		if (is_delimiter)
		{
			*str = '\0'; /* Null-terminate the token */
			str++;
			break;
		}

		str++;
	}

	return token_start;
}

