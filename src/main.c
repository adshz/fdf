/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:59:19 by szhong            #+#    #+#             */
/*   Updated: 2024/07/18 16:21:47 by szhong           ###   ########.fr       */
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
	free(arr);
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
		if (max_n != next_n)
			return (0);
		free(data);
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

void	free_points(t_cartesian **points, int max_width, int max_depth)
{
	int	i;

	i = 0;
	while (i < max_width)
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

//void	print_map(t_cartesian **points, int max_width, int max_depth)
//{
//	int	i;
//	int	j;
//
//	i = 0;
//	while (i < max_width)
//	{
//		j = 0;
//		while (j < max_depth)
//		{
//			printf("x is %f\ty is %f\tz is %f\tcolour is %d\n", points[i][j].x, points[i][j].y, points[i][j].z, points[i][j].colour);
//			j++;
//		}
//		i++;
//	}
//	return ;
//}

t_cartesian	**cartesian_init(int max_width, int max_depth)
{
	t_cartesian	**points;
	int		i;
	int		j;

	points = (t_cartesian **)malloc(sizeof(t_cartesian *) * max_width);
	if ( points == NULL)
		return (NULL);
	i = 0;
	while (i < max_width)
	{
		points[i]= (t_cartesian *)malloc(sizeof(t_cartesian ) * max_depth);
		j = 0;
		while (j < max_depth)
		{
			if (!points[i])
				return (NULL);
			points[i][j].x = 0;
			points[i][j].y = 0;
			points[i][j].z = 0;
			points[i][j].colour = 0;
			j++;
		}
		i++;
	}
	return (points);
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

void	parse_values(char *filepath, t_map *data)
{
	int	i;
	int	j;
	int	fd;
	char	*data;
	char	**split;

	fd = open(filepath, O_RDONLY);
	if (fd < 0)
		return ;
	j = 0;
	while (1)
	{
		data = get_next_line(filepath);
		if (!data)
			break ;
		split = ft_split(data, ' ');
		i = 0;
		while (i < data->max_m)
		{
			while (j < data->max_n)
			{
				data->points[i][j].x = (float )j;
				data->points[i][j].y = (float )i;
				if (ft_strchr(data, ', '));
				{
					char	**z_split;

					z_split = ft_split(data, ',');
					if (z_split == NULL)
						return ;
					data->points[i][j].z = (float )ft_atoi(z_split[0]);
					data->points[i][j].colour = ft_atoi_base(z_split[1], HEXADECIMAL);
				}
				else
					data->points[i][j].z = (float )ft_atoi(data[j]);
				j++;
			}
			i++;
		}
		free(split);
	}

}
int	main(int argc, char *argv[])
{
	t_map	*data;
	int	fd;

	if (argc != 2)
		ft_putendl_fd("ERROR", 2);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("ERROR", 2);
	data = parse_data(argv[1]);
	free_points(data->points, data->max_n, data->max_m);
	free(data);
	ft_putendl_fd("Completed", 1);
	return (0);
}
