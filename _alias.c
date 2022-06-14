#include "shell.h"

int nshell_alias(char **args, char __attribute__((__unused__)) **front);
void set_alias(char *var_name, char *value);
void prt_alias(e_alias *alias);
void help_env(void);
void help_setenv(void);
void help_unsetenv(void);
void help_history(void);

/**
 * nshell_alias - show aliases or set one.
 * @args: array of arguments.
 * @front: pointer.
 * Return: 0.
 */
int nshell_alias(char **args, char __attribute__((__unused__)) **front)
{
	e_alias *temp = aliases;
	int i, ret = 0;
	char *value;

	if (!args[0])
	{
		while (temp)
		{
			prt_alias(temp);
			temp = temp->next;
		}
		return (ret);
	}
	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		value = char_loc(args[i], '=');
		if (!value)
		{
			while (temp)
			{
				if (vs_str(args[i], temp->name) == 0)
				{
					prt_alias(temp);
					break;
				}
				temp = temp->next;
			}
			if (!temp)
				ret = error_prt(args + i, 1);
		}
		else
			set_alias(args[i], value);
	}
	return (ret);
}

/**
 * set_alias - creating or setting aliases.
 * @var_name: the alias name.
 * @value: the alias value.
 */
void set_alias(char *var_name, char *value)
{
	e_alias *temp = aliases;
	int len, j, k;
	char *new_value;

	*value = '\0';
	value++;
	len = str_len(value) - px_str(value, "'\"");
	new_value = malloc(sizeof(char) * (len + 1));
	if (!new_value)
		return;
	for (j = 0, k = 0; value[j]; j++)
	{
		if (value[j] != '\'' && value[j] != '"')
			new_value[k++] = value[j];
	}
	new_value[k] = '\0';
	while (temp)
	{
		if (vs_str(var_name, temp->name) == 0)
		{
			free(temp->value);
			temp->value = new_value;
			break;
		}
		temp = temp->next;
	}
	if (!temp)
		node_to_alias(&aliases, var_name, new_value);
}

/**
 * prt_alias - Print formated alias.
 * @alias: alias Pointer.
 */
void prt_alias(e_alias *alias)
{
	char *alias_string;
	int len = str_len(alias->name) + str_len(alias->value) + 4;

	alias_string = malloc(sizeof(char) * (len + 1));
	if (!alias_string)
		return;
	cp_str(alias_string, alias->name);
	lin_str(alias_string, "='");
	lin_str(alias_string, alias->value);
	lin_str(alias_string, "'\n");

	write(STDOUT_FILENO, alias_string, len);
	free(alias_string);
}
/**
 * rep_aliases - replace aliase
 * @args: pointer of arguments.
 * Return: pointer.
 */
char **rep_aliases(char **args)
{
	e_alias *temp;
	int i;
	char *new_value;

	if (vs_str(args[0], "alias") == 0)
		return (args);
	for (i = 0; args[i]; i++)
	{
		temp = aliases;
		while (temp)
		{
			if (vs_str(args[i], temp->name) == 0)
			{
				new_value = malloc(sizeof(char) * (str_len(temp->value) + 1));
				if (!new_value)
				{
					free_arg(args, args);
					return (NULL);
				}
				cp_str(new_value, temp->value);
				free(args[i]);
				args[i] = new_value;
				i--;
				break;
			}
			temp = temp->next;
		}
	}

	return (args);
}

/**
 * help_env - info builtin command 'env'.
 */
void help_env(void)
{
	char *msg = "env: env\n\tcurrent environment.\n";

	write(STDOUT_FILENO, msg, str_len(msg));
}

/**
 * help_setenv - info builtin command 'setenv'.
 */
void help_setenv(void)
{
	char *msg = "setenv: setenv [ENV-VAR] [VALUE]\n\tset a new";

	write(STDOUT_FILENO, msg, str_len(msg));
	msg = "env-var, or edite.\n\n";
	write(STDOUT_FILENO, msg, str_len(msg));
	msg = "\tif failed, print message.\n";
	write(STDOUT_FILENO, msg, str_len(msg));
}

/**
 * help_unsetenv - info builtin command.
 * 'unsetenv'.
 */
void help_unsetenv(void)
{
	char *msg = "unsetenv: unsetenv [VARIABLE]\n\tRemoves : ";

	write(STDOUT_FILENO, msg, str_len(msg));
	msg = "env-var.\n\n\tif failed, print : ";
	write(STDOUT_FILENO, msg, str_len(msg));
	msg = "message.\n";
	write(STDOUT_FILENO, msg, str_len(msg));
}
