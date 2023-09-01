
#include "../../minishell.h"

int ft_keylen(char *key)
{
	int	i;

	i = 0;
	while (key[i] && key[i] != '=')
		i++;
	return (i);
}

char	*find_key(char *str)
{
	char	*key;
	int		i;

	i = 0;
	while (str[i] != '=' && str[i])
		i++;
	key = ft_substr(str, 0, i);
	if (key == NULL)
	{
		prompt_error("malloc error", NULL, data, 1);
		return (NULL);
	}
	return (key);
}

void	ft_export1(t_data *data)
{
	//t_env	*env;
	t_env	*declare;
	char	*key;
	// int     keylen;
	
	// env = data->linked_env;
	declare = data->declare;
	// keylen = ft_keylen(env);
	key = NULL;
	if (data->params->next)
	{
		key = find_key(data->params->next->parameter);
		// keylen = ft_keylen(key);l
	}
	else
	{
		while (declare)
		{
			ft_printf("declare -x %s=\"%s\"\n", declare->key, declare->value);
			declare = declare->next;
		}
	}

}
