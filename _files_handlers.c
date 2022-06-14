#include "shell.h"

int cannot_op(char *file_path);
int files_com_hndler(char *file_path, int *lst_com);

/**
 * cannot_op -  error print.
 * @file_path: file path.
 * Return: 127.
 */

int cannot_op(char *file_path)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (127);

	len = str_len(name) + str_len(hist_str) + str_len(file_path) + 16;
	error = malloc(sizeof(char) * (len + 1));
	if (!error)
	{
		free(hist_str);
		return (127);
	}

	cp_str(error, name);
	lin_str(error, ": ");
	lin_str(error, hist_str);
	lin_str(error, ": Can't open ");
	lin_str(error, file_path);
	lin_str(error, "\n");

	free(hist_str);
	write(STDERR_FILENO, error, len);
	free(error);
	return (127);
}

/**
 * files_com_hndler - run comm in file.
 * @file_path: file path.
 * @lst_com: last comm.
 * Return: last comm.
 */
int files_com_hndler(char *file_path, int *lst_com)
{
	ssize_t file, b_read, i;
	unsigned int line_size = 0;
	unsigned int old_size = 120;
	char *line, **args, **front;
	char buffer[120];
	int ret;

	hist = 0;
	file = open(file_path, O_RDONLY);
	if (file == -1)
	{
		*lst_com = cannot_op(file_path);
		return (*lst_com);
	}
	line = malloc(sizeof(char) * old_size);
	if (!line)
		return (-1);
	do {
		b_read = read(file, buffer, 119);
		if (b_read == 0 && line_size == 0)
			return (*lst_com);
		buffer[b_read] = '\0';
		line_size += b_read;
		line = _sallc(line, old_size, line_size);
		lin_str(line, buffer);
		old_size = line_size;
	} while (b_read);
	for (i = 0; line[i] == '\n'; i++)
		line[i] = ' ';
	for (; i < line_size; i++)
	{
		if (line[i] == '\n')
		{
			line[i] = ';';
			for (i += 1; i < line_size && line[i] == '\n'; i++)
				line[i] = ' ';
		}
	}
	_alt_var(&line, lst_com);
	line_check(&line, line_size);
	args = str_log(line, " ");
	free(line);
	if (!args)
		return (0);
	if (_f_arg(args) != 0)
	{
		*lst_com = 2;
		free_arg(args, args);
		return (*lst_com);
	}
	front = args;

	for (i = 0; args[i]; i++)
	{
		if (vs_tw_str(args[i], ";", 1) == 0)
		{
			free(args[i]);
			args[i] = NULL;
			ret = _c_arg(args, front, lst_com);
			args = &args[++i];
			i = 0;
		}
	}

	ret = _c_arg(args, front, lst_com);

	free(front);
	return (ret);
}
