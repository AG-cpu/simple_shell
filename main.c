#include "shell.h"

void signaler(int sig);
int _exe_(char **args, char **front);
int log_len(char *str, char *delim);
int c_log_len(char *str, char *delim);
char **str_log(char *line, char *delim);

/**
 * signaler - Print pre signal.
 * @sig: signal.
 */
void signaler(int sig)
{
	char *new_prompt = "\n$ ";

	(void)sig;
	signal(SIGINT, signaler);
	write(STDIN_FILENO, new_prompt, 3);
}

/**
 * _exe_ - Execute comm child proc.
 * @args: array of arguments.
 * @front: args start.
 * Return: last _exe_.
 */
int _exe_(char **args, char **front)
{
	pid_t child_pid;
	int status, flag = 0, ret = 0;
	char *command = args[0];

	if (command[0] != '/' && command[0] != '.')
	{
		flag = 1;
		command = get_location(command);
	}

	if (!command || (access(command, F_OK) == -1))
	{
		if (errno == EACCES)
			ret = (error_prt(args, 126));
		else
			ret = (error_prt(args, 127));
	}
	else
	{
		child_pid = fork();
		if (child_pid == -1)
		{
			if (flag)
				free(command);
			perror("Error child:");
			return (1);
		}
		if (child_pid == 0)
		{
			execve(command, args, environ);
			if (errno == EACCES)
				ret = (error_prt(args, 126));
			free_env();
			free_arg(args, front);
			free_l_list(aliases);
			_exit(ret);
		}
		else
		{
			wait(&status);
			ret = WEXITSTATUS(status);
		}
	}
	if (flag)
		free(command);
	return (ret);
}

/**
 * main - unix comm env.
 * @argc: arg count.
 * @argv: pointer arc.
 * Return: last _exe_ comm.
 */
int main(int argc, char *argv[])
{
	int ret = 0, retn;
	int *lst_com = &retn;
	char *prompt = "$ ", *new_line = "\n";

	name = argv[0];
	hist = 1;
	aliases = NULL;
	signal(SIGINT, signaler);

	*lst_com = 0;
	environ = cp_env();
	if (!environ)
		exit(-100);

	if (argc != 1)
	{
		ret = files_com_hndler(argv[1], lst_com);
		free_env();
		free_l_list(aliases);
		return (*lst_com);
	}

	if (!isatty(STDIN_FILENO))
	{
		while (ret != END_OF_FILE && ret != EXIT)
			ret = _h_arg(lst_com);
		free_env();
		free_l_list(aliases);
		return (*lst_com);
	}

	while (1)
	{
		write(STDOUT_FILENO, prompt, 2);
		ret = _h_arg(lst_com);
		if (ret == END_OF_FILE || ret == EXIT)
		{
			if (ret == END_OF_FILE)
				write(STDOUT_FILENO, new_line, 1);
			free_env();
			free_l_list(aliases);
			exit(*lst_com);
		}
	}

	free_env();
	free_l_list(aliases);
	return (*lst_com);
}
/**
 * log_len - first token in string.
 * @str: string.
 * @delim: char.
 * Return: index.
 */
int log_len(char *str, char *delim)
{
	int index = 0, len = 0;

	while (*(str + index) && *(str + index) != *delim)
	{
		len++;
		index++;
	}

	return (len);
}

/**
 * c_log_len - counts in string.
 * @str: string.
 * @delim: char.
 * Return: number.
 */
int c_log_len(char *str, char *delim)
{
	int index, tokens = 0, len = 0;

	for (index = 0; *(str + index); index++)
		len++;

	for (index = 0; index < len; index++)
	{
		if (*(str + index) != *delim)
		{
			tokens++;
			index += log_len(str + index, delim);
		}
	}

	return (tokens);
}

/**
 * str_log - string tokenizing.
 * @line: string.
 * @delim: char.
 * Return: pointer.
 */
char **str_log(char *line, char *delim)
{
	char **ptr;
	int index = 0, tokens, t, letters, l;

	tokens = c_log_len(line, delim);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (t = 0; t < tokens; t++)
	{
		while (line[index] == *delim)
			index++;

		letters = log_len(line + index, delim);

		ptr[t] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[t])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (l = 0; l < letters; l++)
		{
			ptr[t][l] = line[index];
			index++;
		}

		ptr[t][l] = '\0';
	}
	ptr[t] = NULL;
	ptr[t + 1] = NULL;

	return (ptr);
}
