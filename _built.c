#include "shell.h"

int (*get_builtin(char *command))(char **args, char **front);
int nshell_exit(char **args, char **front);
int nshell_cd(char **args, char __attribute__((__unused__)) **front);
int nshell_help(char **args, char __attribute__((__unused__)) **front);

/**
 * get_builtin - check the commands.
 * @command: checker.
 * Return: pointer.
 */
int (*get_builtin(char *command))(char **args, char **front)
{
	end_built_com funcs[] = {
		{ "exit", nshell_exit },
		{ "env", nshell_env },
		{ "setenv", nshell_setenv },
		{ "unsetenv", nshell_unsetenv },
		{ "cd", nshell_cd },
		{ "alias", nshell_alias },
		{ "help", nshell_help },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; funcs[i].name; i++)
	{
		if (vs_str(funcs[i].name, command) == 0)
			break;
	}
	return (funcs[i].f);
}

/**
 * nshell_exit - exit process of nshell shell.
 * @args: array of arguments.
 * @front: pointer.
 * Return: given value.
 * Description: if returning -2 exit.
 */
int nshell_exit(char **args, char **front)
{
	int i, len_of_int = 10;
	unsigned int num = 0, max = 1 << (sizeof(int) * 8 - 1);

	if (args[0])
	{
		if (args[0][0] == '+')
		{
			i = 1;
			len_of_int++;
		}
		for (; args[0][i]; i++)
		{
			if (i <= len_of_int && args[0][i] >= '0' && args[0][i] <= '9')
				num = (num * 10) + (args[0][i] - '0');
			else
				return (error_prt(--args, 2));
		}
	}
	else
	{
		return (-3);
	}
	if (num > max - 1)
		return (error_prt(--args, 2));
	args -= 1;
	free_arg(args, front);
	free_env();
	free_l_list(aliases);
	exit(num);
}

/**
 * nshell_cd - directory change.
 * @args: array of arguments.
 * @front: pointer.
 * Return: 0.
 */
int nshell_cd(char **args, char __attribute__((__unused__)) **front)
{
	char **dir_info, *new_line = "\n";
	char *oldpwd = NULL, *pwd = NULL;
	struct stat dir;

	oldpwd = getcwd(oldpwd, 0);
	if (!oldpwd)
		return (-1);

	if (args[0])
	{
		if (*(args[0]) == '-' || vs_str(args[0], "--") == 0)
		{
			if ((args[0][1] == '-' && args[0][2] == '\0') ||
					args[0][1] == '\0')
			{
				if (_g_env("OLDPWD") != NULL)
					(chdir(*_g_env("OLDPWD") + 7));
			}
			else
			{
				free(oldpwd);
				return (error_prt(args, 2));
			}
		}
		else
		{
			if (stat(args[0], &dir) == 0 && S_ISDIR(dir.st_mode)
					&& ((dir.st_mode & S_IXUSR) != 0))
				chdir(args[0]);
			else
			{
				free(oldpwd);
				return (error_prt(args, 2));
			}
		}
	}
	else
	{
		if (_g_env("HOME") != NULL)
			chdir(*(_g_env("HOME")) + 5);
	}

	pwd = getcwd(pwd, 0);
	if (!pwd)
		return (-1);

	dir_info = malloc(sizeof(char *) * 2);
	if (!dir_info)
		return (-1);

	dir_info[0] = "OLDPWD";
	dir_info[1] = oldpwd;
	if (nshell_setenv(dir_info, dir_info) == -1)
		return (-1);

	dir_info[0] = "PWD";
	dir_info[1] = pwd;
	if (nshell_setenv(dir_info, dir_info) == -1)
		return (-1);
	if (args[0] && args[0][0] == '-' && args[0][1] != '-')
	{
		write(STDOUT_FILENO, pwd, str_len(pwd));
		write(STDOUT_FILENO, new_line, 1);
	}
	free(oldpwd);
	free(pwd);
	free(dir_info);
	return (0);
}

/**
 * nshell_help - nshell builtin commands.
 * @args: array of arguments.
 * @front: pointer.
 * Return: 0.
 */
int nshell_help(char **args, char __attribute__((__unused__)) **front)
{
	if (!args[0])
		help_all();
	else if (vs_str(args[0], "alias") == 0)
		help_alias();
	else if (vs_str(args[0], "cd") == 0)
		help_cd();
	else if (vs_str(args[0], "exit") == 0)
		help_exit();
	else if (vs_str(args[0], "env") == 0)
		help_env();
	else if (vs_str(args[0], "setenv") == 0)
		help_setenv();
	else if (vs_str(args[0], "unsetenv") == 0)
		help_unsetenv();
	else if (vs_str(args[0], "help") == 0)
		help_help();
	else
		write(STDERR_FILENO, name, str_len(name));

	return (0);
}
