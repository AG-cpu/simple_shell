#include "shell.h"

char *char_loc(char *s, char c);
int px_str(char *s, char *accept);
int vs_str(char *s1, char *s2);
int vs_tw_str(const char *s1, const char *s2, size_t n);
int str_len(const char *s);
char *cp_str(char *dest, const char *src);
char *lin_str(char *dest, const char *src);
char *lin_tw_str(char *dest, const char *src, size_t n);

/**
 * char_loc - Locate the character.
 * @s: The string.
 * @c: The character.
 * Return: pointer.
 */
char *char_loc(char *s, char c)
{
	int index;

	for (index = 0; s[index]; index++)
	{
		if (s[index] == c)
			return (s + index);
	}

	return (NULL);
}

/**
 * px_str - prefix striong len.
 * @s: string.
 * @accept: evalue prefix .
 * Return: bytes count.
 */
int px_str(char *s, char *accept)
{
	int bytes = 0;
	int index;

	while (*s)
	{
		for (index = 0; accept[index]; index++)
		{
			if (*s == accept[index])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * vs_str - Compare strings.
 * @s1: string1 pointer.
 * @s2: string2 pointer.
 * @n: string1 type.
 * Return: < || > 0.
 */
int vs_str(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * vs_tw_str - Compare two strings.
 * @s1: string1 pointer.
 * @s2: string2 pointer.
 * @n: string1 type.
 * Return: < || > 0.
 */
int vs_tw_str(const char *s1, const char *s2, size_t n)
{
	size_t i;

	for (i = 0; s1[i] && s2[i] && i < n; i++)
	{
		if (s1[i] > s2[i])
			return (s1[i] - s2[i]);
		else if (s1[i] < s2[i])
			return (s1[i] - s2[i]);
	}
	if (i == n)
		return (0);
	else
		return (-15);
}
/**
 * str_len - string len.
 * @s: string pointer.
 * Return: len.
 */
int str_len(const char *s)
{
	int length = 0;

	if (!s)
		return (length);
	for (length = 0; s[length]; length++)
		;
	return (length);
}

/**
 * cp_str - Copy string.
 * @dest: string pointer.
 * @src: coped string pointer.
 * Return: Pointer.
 */
char *cp_str(char *dest, const char *src)
{
	size_t i;

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return (dest);
}

/**
 * lin_str - chain link two strings.
 * @dest: string pointer.
 * @src: src string pointer.
 * Return: Pointer.
 */
char *lin_str(char *dest, const char *src)
{
	char *destTemp;
	const char *srcTemp;

	destTemp = dest;
	srcTemp =  src;

	while (*destTemp != '\0')
		destTemp++;

	while (*srcTemp != '\0')
		*destTemp++ = *srcTemp++;
	*destTemp = '\0';
	return (dest);
}

/**
 * lin_tw_str - chain link two strings.
 * @dest: string pointer.
 * @src: src string pointer.
 * @n: copy byetes.
 * Return: Pointer.
 */
char *lin_tw_str(char *dest, const char *src, size_t n)
{
	size_t dest_len = str_len(dest);
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[dest_len + i] = src[i];
	dest[dest_len + i] = '\0';

	return (dest);
}
