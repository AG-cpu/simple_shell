#include "shell.h"

int i_len(int num);
char *conv_i(int num);
int error_prt(char **args, int err);
char *acc_error(char **args);
char *e404_error(char **args);

/**
 * i_len - Counts the digit length of a number.
 * @num: len number.
 * Return: len.
 */
int i_len(int num)
{
unsigned int num1;
int len = 1;

if (num < 0)
{
len++;
num1 = num * -1;
}
else
{
num1 = num;
}
while (num1 > 9)
{
len++;
num1 /= 10;
}

return (len);
}

/**
 * conv_i - int Convertor.
 * @num: int.
 * Return: string.
 */
char *conv_i(int num)
{
char *buffer;
int len = i_len(num);
unsigned int num1;

buffer = malloc(sizeof(char) * (len + 1));
if (!buffer)
return (NULL);

buffer[len] = '\0';

if (num < 0)
{
num1 = num * -1;
buffer[0] = '-';
}
else
{
num1 = num;
}

len--;
do {
buffer[len] = (num1 % 10) + '0';
num1 /= 10;
len--;
} while (num1 > 0);

return (buffer);
}


/**
 * error_prt - prints error message.
 * @args: array of arguments.
 * @err: The error.
 * Return: The error.
 */
int error_prt(char **args, int err)
{
char *error;

switch (err)
{
case -1:
error = env_error(args);
break;
case 1:
error = alias_error(args);
break;
case 2:
if (*(args[0]) == 'e')
error = ext_error(++args);
else if (args[0][0] == ';' || args[0][0] == '&' || args[0][0] == '|')
error = syntax_error(args);
else
error = cd_error(args);
break;
case 126:
error = acc_error(args);
break;
case 127:
error = e404_error(args);
break;
}
write(STDERR_FILENO, error, str_len(error));

if (error)
free(error);
return (err);

}
/**
 * acc_error - access permission denied.
 * @args: An array of arguments.
 * Return: string.
 */
char *acc_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

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
	lin_str(error, ": ");
	lin_str(error, args[0]);
	lin_str(error, ": access denied\n");

	free(hist_str);
	return (error);
}

/**
 * e404_error - command not found .
 * @args: An array of arguments passed to the command.
 * Return: string.
 */
char *e404_error(char **args)
{
	char *error, *hist_str;
	int len;

	hist_str = conv_i(hist);
	if (!hist_str)
		return (NULL);

	len = str_len(name) + str_len(hist_str) + str_len(args[0]) + 16;
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
	lin_str(error, ": not found\n");

	free(hist_str);
	return (error);
}
