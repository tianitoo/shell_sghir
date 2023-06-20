/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnait <hnait@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 10:50:23 by hnait             #+#    #+#             */
/*   Updated: 2023/06/12 14:05:33 by hnait            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "./libft/libft.h"
# include "./ft_printf/ft_printf.h"
 
# define MAX_PATH 1024

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
	char				*input;
	char				*output;
	char				*append;
	struct s_cmd		*next;
}t_cmd,	*t_cmd_list;

typedef struct s_data{
	t_cmd_list	cmd_list;
	t_params	params;
	int			last_command_status;
	char		*commande_line;
	char		**env;
}t_data;

void		get_input(t_data *data);
char		**pipe_split(char *input);
t_cmd_list	get_cmd_list(t_params params);
void		handle_dollar(t_data *data);
void		add_cmd(t_cmd_list cmd_list, t_params param);
char		*get_commande_line(void);
char		*handle_dollar_in_quotes(char *param, t_data *data);
int			is_redirect_operator(t_params param);
void		add_output(t_params params, t_cmd_list cmd_list);
void		add_input(t_params params, t_cmd_list cmd_list);
void		add_param(t_params *params, char *param);
void		add_operator(t_data *data, char operatorm, int *i);
t_cmd		*new_cmd(void);
void		handle_normal_char(t_data *data, int *i, int *param_len);
void		handle_append(t_params params, t_cmd_list cmd_list);
t_params	new_param(char *param);
void		treat_input(t_data *data);
char		*get_operator(char *input, int *i, char operator);
int			is_operator(char c);
void		prompt_error(char *error);
char		*get_param(char *input, int *i, char operator);
void		handle_quotes(t_data *data, int *i);
void		show_command(t_cmd_list cmd_list);
void		handle_redirection(t_params params, t_cmd_list cmd_list);
void		handle_params(t_cmd_list *cmd_list);

#endif

