/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 09:50:50 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/07 12:32:54 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	in_quote(const char *str, t_parse *data)
{
	if (!data->in_quote)
	{
		data->in_quote = 1;
		data->quote_char = *str;
	}
	else if (*str == data->quote_char)
	{
		data->in_quote = 0;
		data->quote_char = '\0';
	}
}

static int	count_arguments(t_parse *data, const char *str)
{
	init_parse_data(data);
	while (*str)
	{
		if (ft_isspace(*str) && !data->in_quote)
			str++;
		else
		{
			data->count++;
			while (*str && (!ft_isspace(*str) || data->in_quote))
			{
				if (*str == '\'' || *str == '"')
					in_quote(str, data);
				str++;
			}
		}
	}
	return (data->count);
}

static char	*extract_argument(t_parse *data, const char **str)
{
	int		i;
	char	*result;

	i = 0;
	init_parse_data(data);
	while (**str && (data->in_quote || !ft_isspace(**str)))
	{
		if (**str == '\\' && data->quote_char != '\'')
		{
			(*str)++;
			if (**str)
				data->buffer[i++] = **str;
		}
		else if ((**str == '\'' || **str == '"')
			&& (!data->in_quote || **str == data->quote_char))
			in_quote(*str, data);
		else
			data->buffer[i++] = **str;
		(*str)++;
	}
	data->buffer[i] = '\0';
	while (**str && ft_isspace(**str))
		(*str)++;
	result = ft_strdup(data->buffer);
	return (result);
}

char	**parse_cmd(const char *str)
{
	t_parse	data;
	char	**result;
	int		arg_count;
	int		i;

	if (!str)
		return (NULL);
	while (*str && ft_isspace(*str))
		str++;
	arg_count = count_arguments(&data, str);
	result = malloc((arg_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*str)
	{
		result[i] = extract_argument(&data, &str);
		if (result[i] == NULL)
			return (NULL);
		i++;
	}
	result[i] = NULL;
	return (result);
}
