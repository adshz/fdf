/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_types.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:16:40 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:25:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_TYPES_H
# define FDF_TYPES_H
# include <stdbool.h>

typedef struct s_col_params
{
	int	r;
	int	g;
	int	b;
	int	end_r;
	int	end_g;
	int	end_b;
	int	start_r;
	int	start_g;
	int	start_b;
	int	start_colour;
	int	end_colour;
}	t_col_params;

typedef struct s_colour
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
			unsigned char	start_b:8;
			unsigned char	start_g:8;
			unsigned char	start_r:8;
			unsigned char	end_b:8;
			unsigned char	end_g:8;
			unsigned char	end_r:8;
		};
	};
	short	delta_r;
	short	delta_g;
	short	delta_b;
}	t_colour;

// transformation part
typedef enum e_projection
{
	ISOMETRIC,
	PERSPECTIVE,
	TOP
}	t_proj_type;

// rotate part
typedef enum e_axis
{
	X_AXIS,
	Y_AXIS,
	Z_AXIS
}	t_axis;

typedef struct s_rot_params
{
	float	*a;
	float	*b;
	int		sign1;
	int		sign2;
}	t_rot_params;

// projection
typedef struct s_proj_params
{
	float	*x;
	float	*y;
	float	*z;
	float	transform_z;
}	t_proj_params;
// initialsation part
typedef struct s_point
{
	float	x;
	float	y;
	float	z;
	int		colour;
}		t_cartesian;

typedef struct s_line
{
	t_cartesian	start;
	t_cartesian	end;
	float		transform_z;
}	t_line;

typedef struct s_map
{
	t_cartesian	**points;
	int			max_m;
	int			max_n;
	int			max_z;
	int			min_z;
	int			current_row;
}	t_map;

typedef struct s_img
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
	t_proj_type	projection;
	bool		colour_pallet;
	float		scale_factor;
	float		scale_z;
	float		cam_position_x;
	float		cam_position_y;
	double		alpha;
	double		beta;
	double		gamma;
	double		line_thickness;
}	t_cam;

typedef struct s_fdf
{
	t_map	*map_data;
	void	*mlx_ptr;
	void	*win_ptr;
	t_img	*img_ptr;
	t_cam	*cam_ptr;
}	t_fdf;

#endif
