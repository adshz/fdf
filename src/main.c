/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:59:19 by szhong            #+#    #+#             */
/*   Updated: 2024/07/25 16:35:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>

#include <fcntl.h>
#include "libft.h"
#include "fdf.h"
#include <assert.h> 
#include <stdio.h>

#include "../minilibx-linux/mlx.h"

# define HEXADECIMAL "0123456789abcdef"

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	return ;
}

int	ft_arrlen(char **arr)
{
	size_t	i;

	i = 0;
	while (arr[i])
		i++;
	return (i);
}

static size_t	count_words(char const *s, char c)
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

int	get_n_cols(char	*filepath)
{
	int	fd;
	char	*data;
	int	max_n;
	int	next_n;
	
	fd = open(filepath, O_RDONLY);
	data = get_next_line(fd);
	if (!data)
		return (0);
	max_n = (int)count_words(data, ' ');
	free(data);
	while (1)
	{
		data = get_next_line(fd);
		if (data == NULL)
			break ;
		next_n = (int)count_words(data, ' ');
		free(data);
		if (max_n != next_n)
			return (0);
	}
	close(fd);
	return (max_n);
}

int	get_m_rows(char *filepath)
{
	int	fd;
	int	max_m;
	char	*data;

	fd = open(filepath, O_RDONLY);
	max_m = 0;
	while (1)
	{
		data = get_next_line(fd);
		if (!data)
			break ;
		if (ft_isprint(*data))
			max_m++;
		free(data);
	}
	close(fd);
	return (max_m);
}

void	free_points(t_cartesian **points, int max_depth)
{
	int	i;

	if (points == NULL)
		return ;
	i = 0;
	while (i < max_depth + 1)
	{
		free(points[i]);
		i++;
	}
	free(points);
	return ;
}

t_map	*map_init(void)
{
	t_map	*result;

	result = (t_map *)malloc(sizeof(t_map) * 1);
	if (!result)
		return (NULL);
	result->max_m = 0;
	result->max_n = 0;
	result->max_z = 0;
	result->min_z = 0;
	result->points = NULL;
	return (result);
}

void	print_map(t_cartesian **points, int max_width, int max_depth)
{
	int	i;
	int	j;

	i = 0;
	while (i < max_depth)
	{
		j = 0;
		while (j < max_width)
		{
			printf("x is %f\ty is %f\tz is %f\tcolour is %d\n", points[i][j].x, points[i][j].y, points[i][j].z, points[i][j].colour);
			j++;
		}
		i++;
	}
	return ;
}

t_cartesian	**cartesian_init(int max_width, int max_depth)
{
	t_cartesian	**points;
	int		i;
	int		j;

	points = (t_cartesian **)malloc(sizeof(t_cartesian *) * (max_depth + 1)); 
	if (points == NULL)
		return (NULL);
	i = 0;
	while (i < max_depth)
	{
		points[i]= (t_cartesian *)malloc(sizeof(t_cartesian ) * (max_width + 1));
		if (!points[i])
		{
			free_points(points, max_depth + 1);
			return (NULL);
		}
		j = 0;
		while (j < max_width)
		{
			points[i][j].x = 0.0f;
			points[i][j].y = 0.0f;
			points[i][j].z = 0.0f;
			points[i][j].colour = 0;
			j++;
		}
		points[i][max_width].x = -1;
		i++;
	}
	points[max_depth] = NULL;
	return (points);
}

#if 0 // old parsing logic read file line by line with get_next_line
void	parse_values(char *filepath, t_map *data)
{
	int	i;
	int	j;
	int	fd;
	char	*line;
	char	**split;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return ;
	i = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		split = ft_split(line, ' ');
		if (!split)
		{
			free(line);
			break ;
		}
		j = 0;
		while (j < data->max_n)
		{
			data->points[i][j].x = (float )j;
			data->points[i][j].y = (float )i;
			if (ft_strchr(split[j], ','))
			{
				char	**z_split;

				z_split = ft_split(split[j], ',');
				if (z_split == NULL)
					return ;
				data->points[i][j].z = (float )ft_atoi(z_split[0]);
				data->points[i][j].colour = ft_atoi_base(z_split[1], HEXADECIMAL);
				free_arr(z_split);
			}
			else
			{
				data->points[i][j].z = (float )ft_atoi(split[j]);
				data->points[i][j].colour = -1;
			}
			if (data->points[i][j].z > data->max_z)
				data->max_z = data->points[i][j].z;
			if (data->points[i][j].z < data->min_z)
				data->min_z = data->points[i][j].z;
			free(split[j]);
			j++;
		}
		free(line);
		free(split);
		i++;
	}
	close(fd);
}

t_map	*parse_data(char* filepath)
{
	t_map	*data;

	data = map_init();
	if (!data)
		return (NULL);
	data->max_n = get_n_cols(filepath);
	data->max_m = get_m_rows(filepath);
	data->points = cartesian_init(data->max_n, data->max_m);
	if (!data->points)
	{
		free(data);
		return (NULL);
	}
	parse_values(filepath, data);
	return (data);
}

#else
//new parsing logic - read the whole file
#define BUFF_SIZE 4096

static char	*ft_concatenate(char *s1, char *s2)
{
	char	*result;
	int	len1;
	int	len2;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	free(s2);
	return (result);
}

static char *read_chunk(int fd, ssize_t *bytes_read)
{
	char	*buff;

	buff = malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buff)
		return (NULL);
	*bytes_read = read(fd, buff, BUFF_SIZE);
	if (*bytes_read <= 0)
	{
		free(buff);
		return (NULL);
	}
	buff[*bytes_read] = '\0';
	return (buff);
}

static char *process_chunks(int fd)
{
	ssize_t		bytes_read;
	char		*content;
	char		*chunk;

	content = NULL;
	while (1)
	{
		chunk = read_chunk(fd, &bytes_read);
		if (!chunk)
			break ;
		if (!content)
			content = chunk;
		else
			content = ft_concatenate(content, chunk);
		if (!content)
			return (NULL);
		if (bytes_read < BUFF_SIZE)
			break ;
	}
	return (content);
}

char	*get_whole_file(const char *filename)
{
	int	fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (NULL);
	content = process_chunks(fd);
	close(fd);
	return (content);
}

static void parse_z_and_color(char *token, t_cartesian *point, t_map *data)
{
	char	*comma;

	comma = ft_strchr(token, ',');
	if (comma)
	{
		*comma = '\0';
		point->z = (float)ft_atoi(token);
		point->colour = ft_atoi_base(comma + 1, HEXADECIMAL);
	}
	else
	{
		point->z = (float)ft_atoi(token);
		point->colour = -1;
	}
	if (point->z > data->max_z)
		data->max_z = point->z;
	if (point->z < data->min_z)
		data->min_z = point->z;
}

/*
 * this function is actually quite smart
 * it actually uses a lot of other functions to work on the same issues
 *
 * for example, we have `ft_strlen`, it will stop when it has the `\0` null terminator
 * also the return value of `ft_strchr` is NULL when the targeted character isn't found
 * if  statement will not run if the condition is not ture,
 * so if the `ptr_nextline` was found by the `ft_strchr` then it won't be a NULL
 * so these two conditions will be true. 
 */
static void	count_dimensions(char *content, t_map *data)
{
	
	char		*line;
	char		*ptr_nextline;
	size_t		line_length;

	line = content;
	
	while (line && *line)
	{
		ptr_nextline = ft_strchr(line, '\n');
		if (ptr_nextline)
		{
			line_length = ptr_nextline - line;
			*ptr_nextline = '\0';
		}
		else
			line_length = ft_strlen(line);
		if (line_length > 0 || data->max_m == 0)
		{
			if (data->max_m == 0)
				data->max_n = count_words(line, ' ');
			data->max_m++;
		}
		if (ptr_nextline)
		{
			*ptr_nextline = '\n';
			line = ptr_nextline + 1;
		}
		else
			break ;
	}
}

static void	parse_line(char *line, t_cartesian *point, int row, t_map *data)
{
	char	**split;
	int	col;

	split = ft_split(line, ' ');
	if (!split)
		return ;
	col = 0;
	while (split[col] && col < data->max_n && point[col].x != -1)
	{
		point[col].x = (float)col;
		point[col].y = (float)row;
		parse_z_and_color(split[col], &point[col], data);
		col++;
	}
	free_arr(split);
}

static void	parse_content(char *content, t_map *data)
{
	char		*line;
	char		*ptr_nextline;
	int		row;
	size_t		line_length;

	line = content;
	row = 0;
	while (line && *line && row < data->max_m)
	{
		ptr_nextline = ft_strchr(line, '\n');
		if (ptr_nextline)
		{
    			line_length = ptr_nextline - line;
			*ptr_nextline = '\0';
		}
		else
			line_length = ft_strlen(line);
		if (line_length > 0 || row == 0)
		{
			parse_line(line, data->points[row], row, data);
			row++;
		}
		if (ptr_nextline)
		{
			*ptr_nextline = '\n';
			line = ptr_nextline + 1;
		}
		else
			break;
	}
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

#endif
/* test parsing data */
//#include <time.h>
//#include <stdio.h>
//
//int	main(int argc, char *argv[])
//{
//	t_map	*data;
//	int	fd;
//	clock_t	start, end;
//	double cpu_time_used;
//
//	start = clock();
//	if (argc != 2)
//		ft_putendl_fd("ERROR", 2);
//	fd = open(argv[1], O_RDONLY);
//	if (fd < 0)
//		ft_putendl_fd("ERROR", 2);
//	data = parse_data(argv[1]);
//	print_map(data->points, data->max_n, data->max_m);
//	free_points(data->points, data->max_n);
//	free(data);
//	ft_putendl_fd("Completed", 1);
//	end = clock();
//	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
//	printf("Time taken: %f seconds\n", cpu_time_used);
//	return (0);
//}

int	main(int argc, char *argv[])
{
	t_fdf	*fdf;

	if (argc != 2)
		ft_putendl_fd("ERROR", 2);
	fdf = (t_fdf *)malloc(sizeof(t_fdf) * 1);
	if (fdf == NULL)
		return (-1);
	fdf->map_data = parse_data(argv[1]);
	print_map(fdf->map_data->points, fdf->map_data->max_n, fdf->map_data->max_m);
	fdf->mlx_ptr = mlx_init();

	mlx_destroy_display(fdf->mlx_ptr);
	free(fdf->mlx_ptr);
	free_points(fdf->map_data->points, fdf->map_data->max_m);
	free(fdf->map_data);
	free(fdf);
}
