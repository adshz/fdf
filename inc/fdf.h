/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:25:53 by szhong            #+#    #+#             */
/*   Updated: 2024/07/26 14:08:50 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

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

typedef struct	s_fdf
{
	t_map	*map_data;
	void	*mlx_ptr;
	int	win_w;
	int	win_h;
	void	*win_ptr;
}	t_fdf;

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

