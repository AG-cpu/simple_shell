#include "shell.h"

e_alias *node_to_alias(e_alias **head, char *name, char *value);
void free_l_list(e_alias *head);
end_nlist *node_to_end(end_nlist **head, char *dir);
void free_list(end_nlist *head);

/**
 * node_to_alias - adding nod to list end.
 * @head: pointer.
 * @name: new alias.
 * @value: the alias.
 * Return: node pointer.
 */
e_alias *node_to_alias(e_alias **head, char *name, char *value)
{
	e_alias *new_node = malloc(sizeof(e_alias));
	e_alias *last;

	if (!new_node)
		return (NULL);

	new_node->next = NULL;
	new_node->name = malloc(sizeof(char) * (str_len(name) + 1));
	if (!new_node->name)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = value;
	cp_str(new_node->name, name);

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * node_to_end - adding nod to linkedlist end.
 * @head: pointer.
 * @dir: node path dir.
 * Return: new node pointer.
 */
end_nlist *node_to_end(end_nlist **head, char *dir)
{
	end_nlist *new_node = malloc(sizeof(end_nlist));
	end_nlist *last;

	if (!new_node)
		return (NULL);

	new_node->dir = dir;
	new_node->next = NULL;

	if (*head)
	{
		last = *head;
		while (last->next != NULL)
			last = last->next;
		last->next = new_node;
	}
	else
		*head = new_node;

	return (new_node);
}

/**
 * free_l_list - free a e_alias list.
 * @head: pointer.
 */
void free_l_list(e_alias *head)
{
	e_alias *next;

	while (head)
	{
		next = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = next;
	}
}

/**
 * free_list - free end_nlist.
 * @head: pointer.
 */
void free_list(end_nlist *head)
{
	end_nlist *next;

	while (head)
	{
		next = head->next;
		free(head->dir);
		free(head);
		head = next;
	}
}
