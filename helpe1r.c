#include "shell.h"

char *_g_arg(char *line, int *lst_com);
int _c_arg(char **args, char **front, int *lst_com);
int _r_arg(char **args, char **front, int *lst_com);
int _h_arg(int *lst_com);
int _f_arg(char **args);

/**
 * _g_arg - get input.
 * @line: get buffer.
 * @lst_com: last _exe_d comm.
 * Return: comm pointer.
 */
char *_g_arg(char *line, int *lst_com)
{
	size_t n = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = gline_t(&line, &n, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (_g_arg(line, lst_com));
	}

	line[read - 1] = '\0';
	_alt_var(&line, lst_com);
	line_check(&line, read);

	return (line);
}

/**
 * _c_arg - call comm.
 * @args: array of arguments.
 * @front: start pointer.
 * @lst_com: last _exe_d comm.
 * Return: last _exe_d comm.
 */
int _c_arg(char **args, char **front, int *lst_com)
{
	int ret, index;

	if (!args[0])
		return (*lst_com);
	for (index = 0; args[index]; index++)
	{
		if (vs_tw_str(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = rep_aliases(args);
			ret = _r_arg(args, front, lst_com);
			if (*lst_com != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (vs_tw_str(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = rep_aliases(args);
			ret = _r_arg(args, front, lst_com);
			if (*lst_com == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = rep_aliases(args);
	ret = _r_arg(args, front, lst_com);
	return (ret);
}

/**
 * _r_arg - run the comm.
 * @args: array of arguments.
 * @front: pointer.
 * @lst_com:  last _exe_d comm.
 * Return: last _exe_d command.
 */
int _r_arg(char **args, char **front, int *lst_com)
{
	int ret, i;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*lst_com = ret;
	}
	else
	{
		*lst_com = _exe_(args, front);
		ret = *lst_com;
	}

	hist++;

	for (i = 0; args[i]; i++)
		free(args[i]);

	return (ret);
}

/**
 * _h_arg - arg handler.
 * @lst_com:  last _exe_d comm.
 * Return:  last _exe_d comm.
 */
int _h_arg(int *lst_com)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = _g_arg(line, lst_com);
	if (!line)
		return (END_OF_FILE);

	args = str_log(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (_f_arg(args) != 0)
	{
		*lst_com = 2;
		free_arg(args, args);
		return (*lst_com);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (vs_tw_str(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = _c_arg(args, front, lst_com);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = _c_arg(args, front, lst_com);

	free(front);
	return (ret);
}

/**
 * _f_arg - find ';', ';;', '&&', or '||'.
 * @args: tokenize arg.
 * Return: 0.
 */
int _f_arg(char **args)
{
	size_t i;
	char *cur, *nex;

	for (i = 0; args[i]; i++)
	{
		cur = args[i];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (i == 0 || cur[1] == ';')
				return (error_prt(&args[i], 2));
			nex = args[i + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (error_prt(&args[i + 1], 2));
		}
	}
	return (0);
}
