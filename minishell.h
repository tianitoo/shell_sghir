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
# include "./libft/libft.h"
# include "./ft_printf/ft_printf.h"

#include <string.h>

# define MAX_PATH 1024

// t_exit	*g_exit;
typedef struct s_env{
	char	*key;
	char	*value;
	int		exported;
	struct s_env	*next;
}t_env;

typedef struct s_param{
	int				is_operator;
	int				in_quote;
	int				in_double_quote;
	char			*parameter;
	struct s_param	*next;
	struct s_param	*prev;
}t_param,	* t_params;

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

// parcing
void		get_input(t_data *data);
char		**pipe_split(char *input);
void		handle_heredoc(t_params params, t_cmd_list cmd_list, t_data *data);
t_cmd_list	get_cmd_list(t_data *data);
void		handle_dollar(char **heredoc_input, t_data *data);
void		add_cmd(t_cmd_list cmd_list, t_params param);
char		*get_commande_line(void);
char		*handle_dollar_in_quotes(char *param, t_data *data);
int			is_redirect_operator(t_params param);
void		add_output(t_params params, t_cmd_list cmd_list, t_data *data);
void		add_input(t_params params, t_cmd_list cmd_list, t_data *data);
void		add_param(t_params *params, char *param);
void		add_operator(t_data *data, char operatorm, int *i);
t_cmd		*new_cmd(t_data *data);
void		handle_normal_char(t_data *data, int *i, int *param_len);
void		handle_append(t_params params, t_cmd_list cmd_list, t_data *data);
t_params	new_param(char *param);
void		treat_input(t_data *data);
char		*get_operator(char *input, int *i, char operator);
int			is_operator(char c);
char		*get_param(char *input, int *i, char operator);
t_params	get_last_param(t_params params);
void		handle_quotes(t_data *data, int *i);
void		show_command(t_cmd_list cmd_list);
void		handle_redirection(t_params params, t_cmd_list cmd_list, t_data *data);
void		handle_params(t_cmd_list *cmd_list, t_data *data);


//builtins
void		ft_cd(t_params params, t_data *data);
void		write_env(t_data *data);
int			ft_echo(t_params params);
void		ft_pwd(void);
void		ft_export(t_cmd_list cmd_list, t_data *data);
void		ft_exit(t_data *data);
void		ft_unset(t_data *data, t_cmd *cmd_list);


//execution
void		execute(t_data *data);
void		add_env(t_env *env, char *key, char *value);

//utils
t_params	double_pointer_to_args(char **double_pointer);
char		**env_to_double_pointer(t_env *env);
char		**args_to_double_pointer(t_params params);
char		*get_env_value(char *param, t_data *data);
char		*get_variable(t_env *env, char *var);
char		*find_key(char *str, t_data *data);
void		prompt_error(char *error, t_cmd_list cmd_list, t_data *data);
t_params	delete_param(t_params *params);
t_env		*new_env(char *key, char *value);
char		*get_value(char *variable);
t_env		*get_env(char **envp);
void		add_garbage(void *ptr);
void		free_garbage(void);



#endif

