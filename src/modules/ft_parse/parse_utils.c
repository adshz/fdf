/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_utils.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 16:19:07 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 11:50:32 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

//int	ft_arrlen(char **arr)
//{
//	size_t	i;
//
//	i = 0;
//	while (arr[i])
//		i++;
//	return (i);
//}
void	process_line(char **line_ptr, t_map *data, \
		void (*process_func)(char *, t_map *))
{
	char	*ptr_nextline;
	size_t	line_length;

	ptr_nextline = ft_strchr(*line_ptr, '\n');
	if (ptr_nextline)
	{
		line_length = ptr_nextline - *line_ptr;
		*ptr_nextline = '\0';
	}
	else
		line_length = ft_strlen(*line_ptr);
	if (line_length > 0 || data->max_m == 0)
		process_func(*line_ptr, data);
	if (ptr_nextline)
	{
		*ptr_nextline = '\n';
		*line_ptr = ptr_nextline + 1;
	}
	else
		*line_ptr = NULL;
}

size_t	count_cols(char const *s, char c)
{
	size_t	word_count;

	word_count = 0;
	while (s && *s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			while (*s && *s != c)
				s++;
			word_count++;
		}
	}
	return (word_count);
}

void	count_line(char *line, t_map *data)
{
	if (data->max_m == 0)
		data->max_n = count_cols(line, ' ');
	data->max_m++;
}

void	count_dimensions(char *content, t_map *data)
{
	char	*line;

	line = content;
	while (line && *line)
		process_line(&line, data, count_line);
}

void	parse_content(char *content, t_map *data)
{
	char	*line;
	int		row;

	row = 0;
	line = content;
	while (line && *line && row < data->max_m && !data->parse_error)
	{
		process_line(&line, data, parse_line_wrapper);
		row++;
	}
}
