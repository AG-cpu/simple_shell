#include "shell.h"

char *cp_dir_pth(char *path);
end_nlist *_g_dir_pth(char *path);

/**
 * get_location - get a command in the PATH.
 * @command: comm.
 * Return: comm path.
 */
char *get_location(char *command)
{
char **path, *temp;
end_nlist *dirs, *head;
struct stat st;

path = _g_env("PATH");
if (!path || !(*path))
return (NULL);

dirs = _g_dir_pth(*path + 5);
head = dirs;

while (dirs)
{
temp = malloc(str_len(dirs->dir) + str_len(command) + 2);
if (!temp)
return (NULL);

cp_str(temp, dirs->dir);
lin_str(temp, "/");
lin_str(temp, command);

if (stat(temp, &st) == 0)
{
free_list(head);
return (temp);
}

dirs = dirs->next;
free(temp);
}

free_list(head);

return (NULL);
}

/**
 * cp_dir_pth - path copy.
 * @path: paths.
 * Return: cpy dir path.
 */
char *cp_dir_pth(char *path)
{
	int i, length = 0;
	char *path_copy, *pwd;

	pwd = *(_g_env("PWD")) + 4;
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (path[i + 1] == ':' || i == 0 || path[i + 1] == '\0')
				length += str_len(pwd) + 1;
			else
				length++;
		}
		else
			length++;
	}
	path_copy = malloc(sizeof(char) * (length + 1));
	if (!path_copy)
		return (NULL);
	path_copy[0] = '\0';
	for (i = 0; path[i]; i++)
	{
		if (path[i] == ':')
		{
			if (i == 0)
			{
				lin_str(path_copy, pwd);
				lin_str(path_copy, ":");
			}
			else if (path[i + 1] == ':' || path[i + 1] == '\0')
			{
				lin_str(path_copy, ":");
				lin_str(path_copy, pwd);
			}
			else
				lin_str(path_copy, ":");
		}
		else
		{
			lin_tw_str(path_copy, &path[i], 1);
		}
	}
	return (path_copy);
}

/**
 * _g_dir_pth - get struct_list linked list token.
 * @path: paths.
 * Return: pointer.
 */
end_nlist *_g_dir_pth(char *path)
{
	int index;
	char **dirs, *path_copy;
	end_nlist *head = NULL;

	path_copy = cp_dir_pth(path);
	if (!path_copy)
		return (NULL);
	dirs = str_log(path_copy, ":");
	free(path_copy);
	if (!dirs)
		return (NULL);

	for (index = 0; dirs[index]; index++)
	{
		if (node_to_end(&head, dirs[index]) == NULL)
		{
			free_list(head);
			free(dirs);
			return (NULL);
		}
	}

	free(dirs);

	return (head);
}
