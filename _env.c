#include "shell.h"

int nshell_env(char **args, char __attribute__((__unused__)) **front);
int nshell_setenv(char **args, char __attribute__((__unused__)) **front);
int nshell_unsetenv(char **args, char __attribute__((__unused__)) **front);
char **cp_env(void);
void free_env(void);
char **_g_env(const char *var);

/**
 * nshell_env - Print environment.
 * @args: An array of arguments.
 * @front: arg pointer.
 * Return: 0.
 * Description: one line by one var
 */
int nshell_env(char **args, char __attribute__((__unused__)) **front)
{
	int index;
	char nc = '\n';

	if (!environ)
		return (-1);

	for (index = 0; environ[index]; index++)
	{
		write(STDOUT_FILENO, environ[index], str_len(environ[index]));
		write(STDOUT_FILENO, &nc, 1);
	}

	(void)args;
	return (0);
}

/**
 * nshell_setenv - set environment.
 * @args: An array of arguments.
 * @front: arg pointer.
 * Description: args[1] to reset by args[2].
 * Return: 0.
 */
int nshell_setenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var = NULL, **nenv, *new_value;
	size_t size;
	int index;

	if (!args[0] || !args[1])
		return (error_prt(args, -1));

	new_value = malloc(str_len(args[0]) + 1 + str_len(args[1]) + 1);
	if (!new_value)
		return (error_prt(args, -1));
	cp_str(new_value, args[0]);
	lin_str(new_value, "=");
	lin_str(new_value, args[1]);

	env_var = _g_env(args[0]);
	if (env_var)
	{
		free(*env_var);
		*env_var = new_value;
		return (0);
	}
	for (size = 0; environ[size]; size++)
		;

	nenv = malloc(sizeof(char *) * (size + 2));
	if (!nenv)
	{
		free(new_value);
		return (error_prt(args, -1));
	}

	for (index = 0; environ[index]; index++)
		nenv[index] = environ[index];

	free(environ);
	environ = nenv;
	environ[index] = new_value;
	environ[index + 1] = NULL;

	return (0);
}

/**
 * nshell_unsetenv - remove environment.
 * @args: An array of arguments.
 * @front: arg pointer
 * Description: removeing args[1].
 * Return: 0.
 */
int nshell_unsetenv(char **args, char __attribute__((__unused__)) **front)
{
	char **env_var, **nenv;
	size_t size;
	int index, index2;

	if (!args[0])
		return (error_prt(args, -1));
	env_var = _g_env(args[0]);
	if (!env_var)
		return (0);

	for (size = 0; environ[size]; size++)
		;

	nenv = malloc(sizeof(char *) * size);
	if (!nenv)
		return (error_prt(args, -1));

	for (index = 0, index2 = 0; environ[index]; index++)
	{
		if (*env_var == environ[index])
		{
			free(*env_var);
			continue;
		}
		nenv[index2] = environ[index];
		index2++;
	}
	free(environ);
	environ = nenv;
	environ[size - 1] = NULL;

	return (0);
}
/**
 * cp_env - copy the environment.
 * Return: copy pointer.
 */
char **cp_env(void)
{
	char **nenv;
	size_t size;
	int index;

	for (size = 0; environ[size]; size++)
		;

	nenv = malloc(sizeof(char *) * (size + 1));
	if (!nenv)
		return (NULL);

	for (index = 0; environ[index]; index++)
	{
		nenv[index] = malloc(str_len(environ[index]) + 1);

		if (!nenv[index])
		{
			for (index--; index >= 0; index--)
				free(nenv[index]);
			free(nenv);
			return (NULL);
		}
		cp_str(nenv[index], environ[index]);
	}
	nenv[index] = NULL;

	return (nenv);
}

/**
 * free_env - Frees last copy.
 */
void free_env(void)
{
	int index;

	for (index = 0; environ[index]; index++)
		free(environ[index]);
	free(environ);
}

/**
 * _g_env - get the env.
 * @var: env name.
 * Return: env pointer.
 */
char **_g_env(const char *var)
{
	int index, len;

	len = str_len(var);
	for (index = 0; environ[index]; index++)
	{
		if (vs_tw_str(var, environ[index], len) == 0)
			return (&environ[index]);
	}

	return (NULL);
}
