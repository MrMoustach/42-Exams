#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define ERROR_FATAL write(2, "error: fatal\n", 13); exit(1)

typedef enum e_types {COMMAND, PIPE, END, RANDOM} types;

typedef struct 	s_token
{
	char	*str;
	types	type;
	int		arg_count;
	char	**args;
	int		fd[2];
	struct s_token *next;
	struct s_token *prev;
}				t_token;

t_token *tokens = NULL;

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

char *ft_strdup(char *s)
{
	int len;
	char *ret;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	ret = malloc(len + 1);
	int i = 0;
	while (s[i])
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return  ret;
}

int tablen(char **tab)
{
	int i = 0;
	while (tab[i])
		i++;
	return (i);
}

char **add_to_tab(char **tab, char *s)
{
	int len;
	char **ret;

	len = tablen(tab);
	ret = malloc (sizeof (char *) * (len + 2));
	int i = 0;
	while (tab[i])
	{
		ret[i] = ft_strdup(tab[i]);
		free (tab[i]);
		i++;
	}
	ret[i++] = ft_strdup(s);
	ret[i] = NULL;
	free (tab);
	return ret;
}

char *ft_strcchr(char *s, char c)
{
	int i;

	i = ft_strlen(s);
	while (i)
	{
		if (s[i] == c)
		{
			return (&s[i+1]);
		}
		i--;
	}
	return NULL;
}

void add_token(char *str)
{
	t_token *token;
	static int context = 0;

	if (context == 1 && strcmp(str, "|") && strcmp(str, ";"))
	{
		t_token *tmp;
		tmp = tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->arg_count++;
		tmp->args = add_to_tab(tmp->args, str);
		return ;
	}
	token = malloc(sizeof(t_token));
	token->str = str;
	token->arg_count = 1;
	token->next = NULL;
	token->prev = NULL;
	if (context == 0 && strcmp(str, "|") && strcmp(token->str, ";"))
	{
		token->type = COMMAND;
		token->args = malloc(sizeof(char *) * 2);
		token->args[0] = ft_strdup(ft_strcchr(str, '/'));
		token->args[1] = NULL;
		token->fd[0] = 0;
		token->fd[1] = 1;
		context = 1;
	}
	else
	{
		if (!strcmp(str, "|"))
			token->type = PIPE;
		else
			token->type = END;
		context = 0;
	}
	if (!tokens)
		tokens = token;
	else
	{
		t_token *tmp;
		tmp = tokens;
		while (tmp->next)
			tmp = tmp->next;
		token->prev = tmp;
		tmp->next = token;
	}
}

void	execute(t_token *token, char **envp)
{
	if (token->next && token->next->type == PIPE)
	{
		int fd[2];
		pipe(fd);
		token->fd[1] = fd[1];
		token->next->next->fd[0] = fd[0];
	}
	int pid = fork();
	if (pid == -1)
	{
		ERROR_FATAL;
	}
	else if (pid == 0)
	{
		//child
		if  (token->fd[0] != 0)
			dup2(token->fd[0], 0);
		if (token->fd[1] != 1)
		{
			dup2(token->fd[1], 1);
			close (token->next->next->fd[0]);
		}
		int ret = execve(token->str, token->args, envp);
		if (ret < 0)
		{
			write (2, "error: cannot execute ", 22);
			write (2, token->str, ft_strlen (token->str));
			write (2, "\n", 1);
			exit (1);
		}
	}
	else
	{
		//parent
		if (token->fd[0] != 0)
			close (token->fd[0]);
		if (token->fd[1] != 1)
			close (token->fd[1]);
	}
}

void builtin_cd(t_token *token)
{
	if (token->arg_count != 2)
	{
		write (2, "error: cd: bad arguments\n", 25);
		return ;
	}
	int ret = chdir(token->args[0]);
	if (ret < 0)
	{
		write (2, "error: cd: cannot change directory to ", 38);
		write (2, token->args[0], ft_strlen(token->args[0]));
		write (2, "\n", 1);
	}
}

int main(int ac, char **argv, char **envp)
{
	int i;

	if (ac < 2)
	{
		write (2, "Error: please provide arguments\n", 32);
		return (1);
	}
	i = 1;
	while (argv[i])
	{
		add_token(argv[i]);
		i++;
	}
	t_token *tmp = tokens;
	while (tmp)
	{
		if (tmp->type == COMMAND)
		{
			if (!strcmp(tmp->str, "cd"))
				builtin_cd(tmp);
			else
				execute(tmp, envp);
		}
		tmp = tmp->next;
	}
	int stat = 0;
	int ret = waitpid(-1, &stat, 0);
	while (ret > 0)
	{
		ret = waitpid(-1, &stat, 0);
	}
	tmp = tokens;
	while (tmp)
	{
		t_token *tmpn = tmp->next;
		if (tmp->type == COMMAND)
		{
			int i = 0;
			while (tmp->args[i])
			{
				free(tmp->args[i]);
				i++;
			}
			free(tmp->args);
		}
		free (tmp);
		tmp = tmpn;
	}
}