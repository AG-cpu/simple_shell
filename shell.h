#ifndef _SHELL_H_
#define _SHELL_H_

#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define END_OF_FILE -2
#define EXIT -3

/* Global environemnt */
extern char **environ;
/* Global program name */
char *name;
/* Global history counter */
int hist;

/**
 * struct struc_list -  linked list.
 * @dir: dir path.
 * @next: list pointer.
 */
typedef struct struc_list
{
	char *dir;
	struct struc_list *next;
} end_nlist;

/**
 * struct struc_built - builtin comm.
 * @name: comm name.
 * @f: comm pointer.
 */
typedef struct struc_built
{
	char *name;
	int (*f)(char **argv, char **front);
} end_built_com;

/**
 * struct struc_alias - make alias .
 * @name: alias name.
 * @value: alias value.
 * @next: aliases pointer.
 */
typedef struct struc_alias
{
	char *name;
	char *value;
	struct struc_alias *next;
} e_alias;

/* Global aliases linkedlist */
e_alias *aliases;

/* for Main */
ssize_t gline_t(char **p_sline, size_t *n, FILE *stream);
void *_sallc(void *ptr, unsigned int old_size, unsigned int new_size);
char **str_log(char *line, char *delim);
char *get_location(char *command);
end_nlist *_g_dir_pth(char *path);
int _exe_(char **args, char **front);
void free_list(end_nlist *head);
char *conv_i(int num);

/* for Input */
void line_check(char **line, ssize_t read);
void _alt_var(char **args, int *lst_com);
char *_g_arg(char *line, int *lst_com);
int _c_arg(char **args, char **front, int *lst_com);
int _r_arg(char **args, char **front, int *lst_com);
int _h_arg(int *lst_com);
int _f_arg(char **args);
void free_arg(char **args, char **front);
char **rep_aliases(char **args);

/* for Strings */
int str_len(const char *s);
char *lin_str(char *dest, const char *src);
char *lin_tw_str(char *dest, const char *src, size_t n);
char *cp_str(char *dest, const char *src);
char *char_loc(char *s, char c);
int px_str(char *s, char *accept);
int vs_str(char *s1, char *s2);
int vs_tw_str(const char *s1, const char *s2, size_t n);

/* for Builtin */
int (*get_builtin(char *command))(char **args, char **front);
int nshell_exit(char **args, char **front);
int nshell_env(char **args, char __attribute__((__unused__)) **front);
int nshell_setenv(char **args, char __attribute__((__unused__)) **front);
int nshell_unsetenv(char **args, char __attribute__((__unused__)) **front);
int nshell_cd(char **args, char __attribute__((__unused__)) **front);
int nshell_alias(char **args, char __attribute__((__unused__)) **front);
int nshell_help(char **args, char __attribute__((__unused__)) **front);
char **cp_env(void);
void free_env(void);
char **_g_env(const char *var);

/* for Error */
int error_prt(char **args, int err);
char *env_error(char **args);
char *alias_error(char **args);
char *ext_error(char **args);
char *cd_error(char **args);
char *syntax_error(char **args);
char *acc_error(char **args);
char *e404_error(char **args);

/* for Linkedlist */
e_alias *node_to_alias(e_alias **head, char *name, char *value);
void free_l_list(e_alias *head);
end_nlist *node_to_end(end_nlist **head, char *dir);
void free_list(end_nlist *head);
void help_all(void);
void help_alias(void);
void help_cd(void);
void help_exit(void);
void help_help(void);
void help_env(void);
void help_setenv(void);
void help_unsetenv(void);
void help_history(void);
int files_com_hndler(char *file_path, int *lst_com);


#endif
