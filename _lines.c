#include "shell.h"

void *_sallc(void *ptr, unsigned int old_size, unsigned int new_size);
void reg_sline(char **p_sline, size_t *n, char *buffer, size_t b);
ssize_t gline_t(char **p_sline, size_t *n, FILE *stream);

/**
 * _sallc - memory malloc and free.
 * @ptr: mem pointer .
 * @old_size: old ptr .
 * @new_size: new ptr.
 * Return: ptr.
 */
void *_sallc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *mem;
	char *ptr_copy, *filler;
	unsigned int index;

	if (new_size == old_size)
		return (ptr);

	if (ptr == NULL)
	{
		mem = malloc(new_size);
		if (mem == NULL)
			return (NULL);

		return (mem);
	}

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}

	ptr_copy = ptr;
	mem = malloc(sizeof(*ptr_copy) * new_size);
	if (mem == NULL)
	{
		free(ptr);
		return (NULL);
	}

	filler = mem;

	for (index = 0; index < old_size && index < new_size; index++)
		filler[index] = *ptr_copy++;

	free(ptr);
	return (mem);
}

/**
 * reg_sline - Reassigns the p_sline.
 * @p_sline: buffer.
 * @n: size p_sline.
 * @buffer: to p_sline.
 * @b: The size of buffer.
 */
void reg_sline(char **p_sline, size_t *n, char *buffer, size_t b)
{
	if (*p_sline == NULL)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*p_sline = buffer;
	}
	else if (*n < b)
	{
		if (b > 120)
			*n = b;
		else
			*n = 120;
		*p_sline = buffer;
	}
	else
	{
		cp_str(*p_sline, buffer);
		free(buffer);
	}
}

/**
 * gline_t - Reads input from a stream.
 * @p_sline: A buffer to store the input.
 * @n: The size of p_sline.
 * @stream: stream.
 * Return: number of bytes.
 */
ssize_t gline_t(char **p_sline, size_t *n, FILE *stream)
{
	static ssize_t input;
	ssize_t ret;
	char c = 'x', *buffer;
	int r;

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * 120);
	if (!buffer)
		return (-1);

	while (c != '\n')
	{
		r = read(STDIN_FILENO, &c, 1);
		if (r == -1 || (r == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (r == 0 && input != 0)
		{
			input++;
			break;
		}

		if (input >= 120)
			buffer = _sallc(buffer, input, input + 1);

		buffer[input] = c;
		input++;
	}
	buffer[input] = '\0';

	reg_sline(p_sline, n, buffer, input);

	ret = input;
	if (r != 0)
		input = 0;
	return (ret);
}
