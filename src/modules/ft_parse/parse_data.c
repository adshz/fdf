/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parse_data.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 16:14:47 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/01 11:48:26 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"

static void	process_line(char **line_ptr, t_map *data, \
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

static void	count_line(char *line, t_map *data)
{
	if (data->max_m == 0)
		data->max_n = count_cols(line, ' ');
	data->max_m++;
}

static void	count_dimensions(char *content, t_map *data)
{
	char	*line;

	line = content;
	while (line && *line)
		process_line(&line, data, count_line);
}

static void	parse_content(char *content, t_map *data)
{
	char	*line;
	int		row;

	row = 0;
	line = content;
	while (line && *line && row < data->max_m)
		process_line(&line, data, parse_line_wrapper);
}

t_map	*parse_data(char *filepath)
{
	t_map	*data;
	char	*file_content;

	data = map_init();
	if (!data)
		return (NULL);
	file_content = get_whole_file(filepath);
	if (!file_content)
	{
		free(data);
		return (NULL);
	}
	count_dimensions(file_content, data);
	data->points = cartesian_init(data->max_n, data->max_m);
	if (!data->points)
	{
		free(file_content);
		free(data);
		return (NULL);
	}
	parse_content(file_content, data);
	free(file_content);
	return (data);
}
