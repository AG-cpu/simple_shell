#include "shell.h"

char *env_error(char **args);
char *alias_error(char **args);
char *ext_error(char **args);
char *cd_error(char **args);
char *syntax_error(char **args);
/**
 * env_error: env errors.
 * @args: An array of arguments.
 * Return: string.
 */
char *env_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

	args--;
	len = str_len(name) + str_len(hist_str) + str_len(args[0]) + 45;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	cp_str(error, name);
	lin_str(error, ": ");
	lin_str(error, hist_str);
	lin_str(error, ": ");
	lin_str(error, args[0]);
	lin_str(error, ": Cannot add or remove from environment\n");

	free(hist_str);
	return (error);
}

/**
 * alias_error: alias errors.
 * @args: An array of arguments.
 * Return: string.
 */
char *alias_error(char **args)
{
	char *error;
	int len;

	len = str_len(name) + str_len(args[0]) + 13;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
		return (NULL);

	cp_str(error, "alias: ");
	lin_str(error, args[0]);
	lin_str(error, " not found\n");

	return (error);
}

/**
 * ext_error: exit errors.
 * @args: An array of arguments.
 * Return: string.
 */
char *ext_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

	len = str_len(name) + str_len(hist_str) + str_len(args[0]) + 27;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	cp_str(error, name);
	lin_str(error, ": ");
	lin_str(error, hist_str);
	lin_str(error, ": op-dreped: unknown number: ");
	lin_str(error, args[0]);
	lin_str(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * cd_error: cd errors.
 * @args: An array of arguments.
 * Return: The error string.
 */
char *cd_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

	if (args[0][0] == '-')
		args[0][2] = '\0';
	len = str_len(name) + str_len(hist_str) + str_len(args[0]) + 24;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	cp_str(error, name);
	lin_str(error, ": ");
	lin_str(error, hist_str);
	if (args[0][0] == '-')
		lin_str(error, ": cd: unknown option ");
	else
		lin_str(error, ": cd: cannot cd to ");
	lin_str(error, args[0]);
	lin_str(error, "\n");

	free(hist_str);
	return (error);
}

/**
 * syntax_error: syntax errors.
 * @args: An array of arguments.
 * Return: string.
 */
char *syntax_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

	len = str_len(name) + str_len(hist_str) + str_len(args[0]) + 33;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (NULL);
	}

	cp_str(error, name);
	lin_str(error, ": ");
	lin_str(error, hist_str);
	lin_str(error, ": Syntax error: \"");
	lin_str(error, args[0]);
	lin_str(error, "\" missplaced\n");

	free(hist_str);
	return (error);
}
