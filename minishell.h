/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 05:35:50 by hnait             #+#    #+#             */
/*   Updated: 2023/09/19 19:37:25 by hnait            ###   ########.fr       */
/*                                                                            */
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

typedef struct s_garbage{
	void				*ptr;
	struct s_garbage	*next;
}	t_garbage;

typedef struct s_exit{
	int					g_exit_status;
	int					in_exec_mode;
	t_garbage			*garbage;
	int					heredoc_ctrlc;
	int					number_of_lines;
}t_exit;

extern t_exit	*g_exit;

typedef struct s_env{
	char			*key;
	char			*value;
	int				exported;
	int				show_value;
	int				hidden;
	int				unsetable;
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
	char		*commande_line;
	char		*original_commande_line;
	int			parsing_error;
}t_data;

//void rl_replace_line (const char *, int);

// parsing
void		handler(int arg);
int			get_input(t_data *data);
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
int			handle_redirection(t_params params, \
						t_cmd_list cmd_list, t_data *data);
int			handle_params(t_cmd_list *cmd_list, t_data *data);
char		*ft_strjoin_char(char *s1, char c, t_data *data);
void		free_params(t_params *params);

//builtins
t_data		*ft_cd(t_params params, t_data *data);
void		*write_env(t_data *data);
t_params	ft_echo(t_params params, t_data *data);
char		*find_pwd(t_data *data);
char		*ft_pwd(t_data *data);
int			key_exists(t_env *env_params, char *key_to_find);
t_env		*ft_export(t_cmd_list cmd_list, t_data *data);
void		*ft_exit(t_params params, t_data *data);
t_env		*ft_unset(t_data *data, t_cmd *cmd_list);
t_env		*remove_variable(t_data *data, char *var_name);

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
void		prompt_error(char *error, \
				t_cmd_list cmd_list, t_data *data, int exit_status);
t_params	delete_param(t_params *params);
t_env		*new_env(char *key, char *value, t_data *data);
char		*get_value(char *variable, t_data *data);
t_env		*get_env(char **envp, t_data *data);
t_garbage	*add_garbage(t_data *data, void *ptr);
void		free_garbage(void);
void		*garbage(void *garbage, t_data *data);
void		*ft_malloc(size_t size);
char		*get_next_line(int fd);

//redirection
int			open_file(char *file, t_cmd_list cmd_list, int redirect_type);
void		skip_riderection(t_params params, t_cmd_list cmd_list);
int			cwd(t_data *data);
int			key_exists_hidden(t_env *env_params, char *key_to_find);
t_env		*add_to_env(t_cmd_list cmd_list, t_data *data);
t_env		*get_env_by_key(t_env *env, char *key);

int			ft_only_dig(char *str);
t_cmd_list	init_pipes(t_data *data);
int			check_min(char *arg);
int			check_max(char *arg);
void		free_enve(t_data *data);
t_params	add_param_to_cmd(t_cmd_list cmd_list, t_params param, t_data *data);
int			env_len(t_env *env);
void		move_node(t_env *tmp);
char		*update_param(t_data *data, char *key, char *new_param);
void		close_file_descriptors(t_data *data);
int			set_up_execve(t_cmd_list cmd_list, t_data *data);
int			execute_cmd(t_data *data, t_cmd_list cmd_list);
t_cmd_list	execute_builtin(t_data *data, t_cmd_list cmd_list);
void		check_quote(char *command_line, int *i, int *quotes);
char		*check_heredoc(char *command_line, int *i, t_data *data,
				int *quotes);
int			is_builtin(char *cmd);
void		exit_error(char *str);
char		*expand_join_variable(char *command_line, char *new_command_line,
				int *i, t_data *data);
char		*expand_variable(char *param, int *i, t_data *data);
void		empty_pipe(char *line, int *pip);
int			*create_heredoc_pipe(t_params next, t_data *data);
t_env		*get_linked_env(char **envp, t_data *data);
char		**get_paths(t_data *data, t_cmd_list cmd_list, char *cmd);
void		signalher(int sig);
int			get_exitstate(int wait_status);
int			set_g_exit(void);

#endif
