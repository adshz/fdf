/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:25:53 by szhong            #+#    #+#             */
/*   Updated: 2024/08/12 18:01:43 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_H
# define FDF_H
# define HEXADECIMAL "0123456789abcdef"
# define BUFF_SIZE 4096
# include <stdlib.h>
# include <stdbool.h>
# include <error.h>
# include <stdio.h>
# include <math.h>
# include "../minilibx-linux/mlx.h"
# include "libft.h"
# define WINDOW_WIDTH 1680 
# define WINDOW_HEIGHT 980
# define PIXEL_COUNTS 1646400

# define DEFAULT_BACKGROUND 0x010203
# define DEFAULT_COLOUR 0xFFFFFF
# define ANG_30	0.52359877
# define ANG_45 0.78539816




typedef enum e_projection
{
	ISOMETRIC,
	PERSPECTIVE,
	TOP
}	t_projection_type;

typedef	struct	s_colour
{
	union
	{
		struct
		{
			unsigned int	start_colour:24;
			unsigned int	end_colour:24;
		};
		struct
		{
			unsigned char	start_b: 8;
			unsigned char	start_g: 8;
			unsigned char	start_r: 8;
			unsigned char	end_b: 8;
			unsigned char	end_g: 8;
			unsigned char 	end_r:8;
		};
	};
	short	delta_r;
	short	delta_g;
	short	delta_b;
}	t_colour;

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

typedef struct s_line
{
	t_cartesian	start;
	t_cartesian	end;
	float	transform_z;
}	t_line;

typedef struct	s_img
{
	void	*img_buff;
	int		bits_per_pixel;
	int		line_len;
	int		endian;
	char	*mem_addr;
	t_line	*line_segment;
}	t_img;

typedef struct s_display
{
	t_projection_type	projection;
	bool	colour_pallet;
	float	scale_factor;
	float	scale_z;
	float	cam_position_x;
	float	cam_position_y;
	double	alpha;
	double	beta;
	double	gamma;
}	t_cam;

typedef struct	s_fdf
{
	t_map	*map_data;
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	*img_ptr;
	t_cam	*cam_ptr;
}	t_fdf;

t_fdf	*fdf_init(char *filepath);
t_map	*map_init(void);
t_img	*img_init(void	*mlx);
t_cam	*cam_init(t_map *data);
t_line	*line_init(t_cartesian start, t_cartesian end, t_fdf *fdf);
t_cartesian	**cartesian_init(int max_width, int max_depth);


float	scale_to_fit(t_map *data);
int	ft_arrlen(char **arr);
size_t	count_cols(char const *s, char c);
void	free_arr(char **arr);
void	free_points(t_cartesian **points, int max_depth);

char	*ft_concatenate(char *s1, char *s2);
char	*get_whole_file(const char *filename);
t_map	*parse_data(char *filepath);
void	parse_line_wrapper(char *line, t_map *data);
void	move_origin(t_map *data);
void	print_map(t_cartesian **point, int max_width, int max_depth);

float	min(float a, float b);
float	max(float a, float b);
void	clean_up(t_fdf **fdf);
void	clean_free(t_fdf *fdf);
void	key_close(t_fdf *fdf);
int	esc_close(int keycode, t_fdf *fdf);
int	click_close(t_fdf *fdf);

void	render_data(t_fdf *fdf);

void	move_origin(t_map *data);

t_colour	*colour_init(t_cartesian start, t_cartesian end);
int	get_colour(t_colour *colour, int i_line, int line_size);
void	apply_colours(t_fdf *fdf, t_cartesian *point);


void	rotate(t_cam *cam, t_line *line);
void	project(t_cam *cam, t_line *line);
void	x_rotation(t_cam *cam, t_line *line);
void	view_transform(t_cam *cam, t_line *line);


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

