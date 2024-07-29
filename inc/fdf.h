/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:25:53 by szhong            #+#    #+#             */
/*   Updated: 2024/07/29 16:57:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_H
# define FDF_H
# define HEXADECIMAL "0123456789abcdef"
# define BUFF_SIZE 4096
# include <stdlib.h>

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
	int		current_row;
}	t_map;

typedef struct	s_fdf
{
	t_map	*map_data;
	void	*mlx_ptr;
	int	win_w;
	int	win_h;
	void	*win_ptr;
}	t_fdf;

t_map	*map_init(void);
t_cartesian	**cartesian_init(int max_width, int max_depth);


int	ft_arrlen(char **arr);
size_t	count_cols(char const *s, char c);
void	free_arr(char **arr);
void	free_points(t_cartesian **points, int max_depth);

char	*ft_concatenate(char *s1, char *s2);
char	*get_whole_file(const char *filename);
t_map	*parse_data(char *filepath);
void	parse_line_wrapper(char *line, t_map *data);
void	print_map(t_cartesian **point, int max_width, int max_depth);
#endif

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

