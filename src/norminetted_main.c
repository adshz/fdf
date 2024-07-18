/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:59:19 by szhong            #+#    #+#             */
/*   Updated: 2024/07/17 12:50:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>

#include <fcntl.h>
#include "libft.h"
#include <assert.h> 
#include <stdio.h>

#if 0

typedef void *(*malloc_t)(size_t size);
typedef void (*free_t)(void *ptr);

malloc_t malloc_f = NULL;
free_t free_f = NULL;

int enable_malloc = 1;
int enable_free = 1;

void *TranslateToSymbol(void *addr) {

	Dl_info info;
	struct link_map *link;

	dladdr1(addr, &info, (void *)&link, RTLD_DL_LINKMAP);

	return (void *)(addr - link->l_addr);

}


// main() --> f1() --> f2() --> f3();

void *malloc(size_t size) {

	if (!malloc_f) {
		malloc_f = dlsym(RTLD_NEXT, "malloc");
	}

	void *ptr = NULL;

	if (enable_malloc) {
		enable_malloc = 0;

		ptr = malloc_f(size);

		void *caller = __builtin_return_address(0);

		char buff[128] = {0};
		snprintf(buff, 128, "./block/%p.mem", ptr);

		FILE* fp = fopen(buff, "w");
		if (!fp) {
			free(ptr);
			return NULL;
		}

		fprintf(fp, "[+][%p] %p: %ld malloc\n", TranslateToSymbol(caller), ptr, size);
		fflush(fp);

		enable_malloc = 1;
	} else {
		ptr = malloc_f(size);
	}

	return ptr;
}

void free(void *ptr) {
	if (!free_f) {
		free_f = dlsym(RTLD_NEXT, "free");
	}

	char buff[128] = {0};
	snprintf(buff, 128, "./block/%p.mem", ptr);

	if (unlink(buff) < 0) { // no exist
		printf("double free: %p\n", ptr);
		return ;
	}

	return free_f(ptr);
}

#endif

typedef struct s_point
{
	float	x;
	float	y;
	float	z;
	int		colour;
}		t_cartesian;

typedef struct s_map
{
	t_cartesian	**points;
	int		max_m;
	int		max_n;
	int		max_z;
	int		min_z;
}	t_map;

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

//void	free_points(t_row **row, int max_width)
//{
//	int	i;
//
//	i = 0;
//	while (i < max_width)
//	{
//		free(row[i]->point);
//		i++;
//	}
//	return ;
//}

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

int	main(int argc, char *argv[])
{
	t_map	*data;
	int	fd;

	if (argc != 2)
		ft_putendl_fd("ERROR", 2);
	fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		ft_putendl_fd("ERROR", 2);
	data = map_init();
	if (!data)
		return (-1);
	data->max_n = get_n_cols(argv[1]);
	data->max_m = get_m_rows(argv[1]);

	printf("max n is %d\t,max m is %d\n", data->max_n, data->max_m);
	free(data);
	return (0);
}
