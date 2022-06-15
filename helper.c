#include "shell.h"

void free_arg(char **args, char **front);
char *_g_proc_id(void);
char *_g_value_env(char *beginning, int len);
void _alt_var(char **args, int *lst_com);

/**
 * free_arg - free arg.
 * @args: comm pointer.
 * @front: arg start pointer.
 */
void free_arg(char **args, char **front)
{
	size_t i;

	for (i = 0; args[i] || args[i + 1]; i++)
		free(args[i]);

	free(front);
}

/**
 * _g_proc_id - process ID.
 * Description: information about the current process.
 * Return: proces ID.
 */
char *_g_proc_id(void)
{
	size_t i = 0;
	char *buffer;
	ssize_t file;

	file = open("/proc/own/st", O_RDONLY);
	if (file == -1)
	{
		perror("Cannot read file");
		return (NULL);
	}
	buffer = malloc(120);
	if (!buffer)
	{
		close(file);
		return (NULL);
	}
	read(file, buffer, 120);
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';

	close(file);
	return (buffer);
}

/**
 * _g_value_env - env value.
 * @beginning: var.
 * @len:var len.
 * Return: env value.
 * Description: var = value.
 */
char *_g_value_env(char *beginning, int len)
{
	char **var_addr;
	char *replacement = NULL, *temp, *var;

	var = malloc(len + 1);
	if (!var)
		return (NULL);
	var[0] = '\0';
	lin_tw_str(var, beginning, len);

	var_addr = _g_env(var);
	free(var);
	if (var_addr)
	{
		temp = *var_addr;
		while (*temp != '=')
			temp++;
		temp++;
		replacement = malloc(str_len(temp) + 1);
		if (replacement)
			cp_str(replacement, temp);
	}

	return (replacement);
}

/**
 * _alt_var - alternate variable replacement.
 * @line: comm pointer.
 * @lst_com: last _exe_d comm pointer.
 * Description: alternate and replaces values.
 */
void _alt_var(char **line, int *lst_com)
{
	int j, k = 0, len;
	char *replacement = NULL, *old_line = NULL, *new_line;

	old_line = *line;
	for (j = 0; old_line[j]; j++)
	{
		if (old_line[j] == '$' && old_line[j + 1] &&
				old_line[j + 1] != ' ')
		{
			if (old_line[j + 1] == '$')
			{
				replacement = _g_proc_id();
				k = j + 2;
			}
			else if (old_line[j + 1] == '?')
			{
				replacement = conv_i(*lst_com);
				k = j + 2;
			}
			else if (old_line[j + 1])
			{
				/* var name */
				for (k = j + 1; old_line[k] &&
						old_line[k] != '$' &&
						old_line[k] != ' '; k++)
					;
				len = k - (j + 1);
				replacement = _g_value_env(&old_line[j + 1], len);
			}
			new_line = malloc(j + str_len(replacement)
					  + str_len(&old_line[k]) + 1);
			if (!line)
				return;
			new_line[0] = '\0';
			lin_tw_str(new_line, old_line, j);
			if (replacement)
			{
				lin_str(new_line, replacement);
				free(replacement);
				replacement = NULL;
			}
			lin_str(new_line, &old_line[k]);
			free(old_line);
			*line = new_line;
			old_line = new_line;
			j = -1;
		}
	}
}
