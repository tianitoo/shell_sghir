/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   minishell.h										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hnait <hnait@student.42.fr>				+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/06/02 10:50:23 by hnait			 #+#	#+#			 */
/*   Updated: 2023/07/09 16:25:06 by hnait			###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include "./libft/libft.h"
# include "./ft_printf/ft_printf.h"

# define MAX_PATH 1024

typedef struct s_file_descriptors{
	int							fd;
	struct s_file_descriptors	*next;
}	t_file_descriptors;

typedef struct s_garbage{
	void				*ptr;
	struct s_garbage	*next;
}	t_garbage;

typedef struct s_exit{
	int					g_exit_status;
	int					in_exec_mode;
	t_garbage			*garbage;
	t_file_descriptors	*file_descriptors;
	int					heredoc_statu;
	int					heredoc_fd;
	int					closeed;
}t_exit;

extern t_exit	*g_exit;

typedef struct s_env{
	char			*key;
	char			*value;
	int				exported;
	int				hidden;
	struct s_env	*next;
}t_env;

typedef struct s_param{
	int				is_operator;
	int				in_quote;
	int				in_double_quote;
	char			*parameter;
	struct s_param	*next;
	struct s_param	*prev;
}t_param,	*t_params;

typedef struct s_cmd{
	char				*cmd;
	struct s_param		*args;
	int					input;
	int					output;
	int					*pip;
	struct s_cmd		*next;
	struct s_cmd		*prev;
	int					parsing_error;
}t_cmd,	*t_cmd_list;

typedef struct s_data{
	t_cmd_list	cmd_list;
	t_params	params;
	t_env		*linked_env;
	t_env		*declare;
	char		*commande_line;
	int			last_command_status;
	int			parsing_error;
}t_data;

//void rl_replace_line (const char *, int);

// parsing
void		handler(int arg);
int		get_input(t_data *data);
char		**pipe_split(char *input);
int			handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data);
t_cmd_list	get_cmd_list(t_data *data);
char		*handle_dollar(char **heredoc_input, t_data *data);
void		add_cmd(t_cmd_list cmd_list, t_params param, t_data *data);
char		*get_commande_line(t_data *data);
int			is_redirect_operator(t_params param);
t_params	add_output(t_params params, t_cmd_list cmd_list, t_data *data);
t_params	add_input(t_params params, t_cmd_list cmd_list, t_data *data);
t_params	add_param(t_params *params, char *param, t_data *data);
t_params	add_operator(t_data *data, char operatorm, int *i);
t_cmd		*new_cmd(t_data *data);
char		*handle_normal_char(t_data *data, int *i, int *param_len);
t_params	handle_append(t_params params, t_cmd_list cmd_list, t_data *data);
t_params	new_param(char *param, t_data *data);
int			treat_input(t_data *data);
char		*get_operator(char *input, int *i, char operator);
int			is_operator(char c);
char		*get_param(char *input, int *i, char operator);
t_params	get_last_param(t_params params);
void		handle_quotes(t_data *data, int *i);
void		show_command(t_cmd_list cmd_list);
int			handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data);
int			handle_params(t_cmd_list *cmd_list, t_data *data);
char		*ft_strjoin_char(char *s1, char c, t_data *data);
void		free_params(t_params *params);

//builtins
t_data		*ft_cd(t_params params, t_data *data);
void		write_env(t_data *data);
t_params	ft_echo(t_params params, t_data *data);
char		*find_pwd(t_data *data);
char		*ft_pwd(t_data *data);
int			key_exists(t_env *env_params, char *key_to_find, t_data *data);
t_env		*ft_export(t_cmd_list cmd_list, t_data *data);
void		*ft_exit(t_params params, t_data *data);
t_env		*ft_unset(t_data *data, t_cmd *cmd_list);

//execution
void		execute(t_data *data);
void		pipes_work(t_cmd_list cmd_list);
t_env		*add_env(t_env *env, char *key, char *value, t_data *data);

//utils
t_env		*update_env_var(char *var_key, char *value, t_data *data);
t_params	double_pointer_to_args(char **double_pointer, t_data *data);
char		**env_to_double_pointer(t_env *env, t_data *data);
void		add_hidden_env(t_env *env, char *key, char *value, t_data *data);
char		**args_to_double_pointer(t_params params, t_data *data);
char		*get_env_value(char *param, t_data *data);
char		*get_variable(t_env *env, char *var);
char		*find_key(char *str, t_data *data);
void		prompt_error(char *error, t_cmd_list cmd_list, t_data *data, int exit_status);
t_params	delete_param(t_params *params);
t_env		*new_env(char *key, char *value, t_data *data);
char		*get_value(char *variable, t_data *data);
t_env		*get_env(char **envp, t_data *data);
t_garbage	*add_garbage(t_data *data, void *ptr);
void		free_garbage(void);
void		*garbage(void *garbage, t_data *data);


void		*ft_malloc(size_t size);

#endif
