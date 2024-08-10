/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 render_data.c										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/08/01 16:33:37 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/08/05 17:04:27 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"

static float	float_abs(float nbr)
{
	if (nbr < 0)
		return (nbr * (-1));
	return (nbr);
}

typedef struct s_line_params
{
    int x;
    int y;
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    int max_steps;
    int current_step;
} t_line_params;

// convert colour value to the right position of 32-bit pixel 
// Calculate the pixel index in the buffer
// Handle big-edian case
// Handle little-edian case
// NOTES
//	to be honest, && 0xff is like a subtraction to keep the value you want 
//	delete the value you don't want
static void	set_pixel_colour(t_img *img, float col, float row, int colour)
{
	int	pixel;

	pixel = ((int)row * img->line_len) + ((int)col * 4);
	if (img->endian == 1)
	{
		img->mem_addr[pixel + 0] = (colour >> 24);
		img->mem_addr[pixel + 1] = (colour >> 16) & 0xff;
		img->mem_addr[pixel + 2] = (colour >> 8) & 0xff;
		img->mem_addr[pixel + 3] = colour & 0xff;
	}
	else if (img->endian == 0)
	{
		img->mem_addr[pixel + 0] = colour & 0xff;
		img->mem_addr[pixel + 1] = (colour >> 8) & 0xff;
		img->mem_addr[pixel + 2] = (colour >> 16) & 0xff;
		img->mem_addr[pixel + 3] = (colour >> 24);
	}
	return ;
}

// static void	line_params_init(t_line_params *p, t_cartesian start, t_cartesian end)
// {
// 	p->x = (int )start.x;
// 	p->y = (int )start.y;
// 	p->dx = abs((int)end.x - (int)start.x);
// 	p->dy = abs((int)end.y - (int)start.y);
// 	p->sx = 1;
// 	p->sy = 1;
// 	if (start.x >= end.x)
// 		p->sx = -1;
// 	if (start.y >= end.y)
// 		p->sy = -1;
// 	p->err = p->dx + p->dy;
// 	p->max_steps = max(abs(p->dx), abs(p->dy));
// 	p->current_step = 0;
// }

// static void	draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
// {
// 	t_line_params params;
// 	t_colour	*colour;
// 	int		current_colour;

// 	line_params_init(&params, start, end);
// 	int pixels_drawn = 0;
// 	int	pixels_skipped = 0;
// 	printf("Bresenham start: (%f, %f) to (%f, %f)\n", 
// 		start.x, start.y, end.x, end.y);
// 	colour = colour_init(start, end);
// 	if (!colour)
// 		clean_free(fdf);
// 	while (1)
// 	{
// 		current_colour = get_colour(colour, params.current_step, params.max_steps);
// 		if (params.x >= 0 && params.y >= 0 && params.x < WINDOW_WIDTH && params.y < WINDOW_HEIGHT)
// 		{
// 			set_pixel_colour(fdf->img_ptr, params.x, params.y, current_colour);
// 			pixels_drawn++;
//             if (pixels_drawn <= 5 || pixels_drawn % 100 == 0)
//                 printf("Pixel %d: (%d, %d) color 0x%X\n", pixels_drawn, params.x, params.y, current_colour);
// 		}
// 		else
//         {
//             pixels_skipped++;
//             if (pixels_skipped <= 5)
//                 printf("Skipped (%d, %d) - out of bounds\n", params.x, params.y);
//         }

// 		if (params.x == (int)end.x && params.y == (int)end.y)
// 			break ;
// 		params.e2 = 2 * params.err;
// 		if (params.e2 >= params.dy)
// 		{
// 			params.err += params.dy;
// 			params.x += params.sx;
// 		}
// 		if (params.e2 <= params.dx)
// 		{
// 			params.err += params.dx;
// 			params.y += params.sy;
// 		}
// 		params.current_step++;
// 	}
// 	printf("Bresenham end. Drawn: %d, Skipped: %d\n", pixels_drawn, pixels_skipped);
// 	free(colour);
// }

static float	absolute(float nbr)
{
	if (nbr < 0)
		return (-nbr);
	else
		return (nbr);
}

static void init_line_params(t_line_params *params, t_cartesian start, t_cartesian end)
{
    params->dx = abs((int)end.x - (int)start.x);
    params->dy = -abs((int)end.y - (int)start.y);
    params->sx = -1;
    params->sy = -1;
    if (start.x < end.x)
        params->sx = 1;
    if (start.y < end.y)
        params->sy = 1;
    params->err = params->dx + params->dy;
    params->x = (int)start.x;
    params->y = (int)start.y;
    params->max_steps = max(abs(params->dx), abs(params->dy));
    params->current_step = 0;
}


static void draw_pixel(t_fdf *fdf, t_line_params *params, t_colour *colour)
{
    int current_colour;

    current_colour = get_colour(colour, params->current_step, params->max_steps);
    if (params->x >= 0 && params->y >= 0 && params->x < WINDOW_WIDTH && params->y < WINDOW_HEIGHT)
        set_pixel_colour(fdf->img_ptr, params->x, params->y, current_colour);
}


static void update_line_params(t_line_params *params)
{
    int e2;

    e2 = 2 * params->err;
    if (e2 >= params->dy)
    {
        params->err += params->dy;
        params->x += params->sx;
    }
    if (e2 <= params->dx)
    {
        params->err += params->dx;
        params->y += params->sy;
    }
    params->current_step++;
}

static void draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
    t_line_params params;
    t_colour *colour;

    init_line_params(&params, start, end);
    colour = colour_init(start, end);
    if (!colour)
        clean_free(fdf);
    while (1)
    {
        draw_pixel(fdf, &params, colour);
        if (params.x == (int)end.x && params.y == (int)end.y)
            break;
        update_line_params(&params);
    }
    free(colour);
}
// void	draw_line_bresenham(t_fdf *fdf, int x0, int y0, int x1, int y1, int colour_value)
// {
// 	int	dx;
// 	int	dy;
// 	int	sx;
// 	int	sy;
// 	int	err;
// 	int	e2;
// 	t_colour	*colour;
// 	int	max_steps;
// 	int	line_idx;

// 	dx = abs(x1 - x0);
// 	dy = abs(y1 - y0);
// 	sx = -1;
// 	sy = -1;
// 	max_steps = (int)max(absolute(dx), absolute(dy));
// 	if (x0 < x1)
// 		sx = 1;
// 	if (y0 < y1)
// 		sy = 1;
// 	err = dx - dy;
// 	colour = colour_init(fdf->img_ptr->line_segment->start, fdf->img_ptr->line_segment->end);
// 	if (!colour)
//  		clean_free(fdf);
// 	line_idx = 0;
// 	while (line_idx < max_steps)
// 	{
//  		colour_value = get_colour(colour, line_idx++, max_steps);
//  		if (x0 > 0 && y0 > 0 && x0 < WINDOW_WIDTH && y0 < \
//  				WINDOW_HEIGHT)
//  			set_pixel_colour(fdf->img_ptr, x0, y0, colour_value);
// 		if (x0 == x1 && y0 == y1)
// 			break ;
// 		e2 = 2 * err;
// 		if (e2 > -dy)
// 		{
// 			err -= dy;
// 			x0 += sx;
// 		}
// 		if (e2 < dx)
// 		{
// 			err += dx;
// 			y0 += sy;
// 		}

// 	}
// }

// to be aborted!
// void	draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
// {
// 	float	x_step;
// 	float	y_step;
// 	int		max_steps;
// 	int		i_line;
// 	t_colour	*color;

// 	x_step = end.x - start.x;
// 	y_step = end.y - start.y;
// 	max_steps = (int)max(absolute(x_step), absolute(y_step));
// 	x_step /= max_steps;
// 	y_step /= max_steps;
// 	color = colour_init(start, end);
// 	if (!color)
// 		clean_free(fdf);
// 	i_line = 0;
// 	while (i_line < max_steps)
// 	{
// 		start.colour = get_colour(color, i_line++, max_steps);
// 		if (start.x > 0 && start.y > 0 && start.x < WINDOW_WIDTH && start.y < \
// 				WINDOW_HEIGHT)
// 			set_pixel_colour(fdf->img_ptr, start.x, start.y, start.colour);
// 		start.x += x_step;
// 		start.y += y_step;
// 	}
// 	free(color);
// }

static void	draw_line_segment(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	if (!fdf || !fdf->cam_ptr || !fdf->img_ptr)
		return ;
	printf("Draw line segment start\n");
    printf("Original - Start: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", start.x, start.y, start.z, start.colour);
    printf("Original - End: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", end.x, end.y, end.z, end.colour);

	start.z *= fdf->cam_ptr->scale_z;
	end.z *= fdf->cam_ptr->scale_z;
	
	printf("After z-scale - Start: x=%.2f, y=%.2f, z=%.2f\n", start.x, start.y, start.z);
    printf("After z-scale - End: x=%.2f, y=%.2f, z=%.2f\n", end.x, end.y, end.z);
	
	apply_colours(fdf, &start);
	apply_colours(fdf, &end);
	printf("After apply_colours - Start color: 0x%X\n", start.colour);
    printf("After apply_colours - End color: 0x%X\n", end.colour);

	fdf->img_ptr->line_segment = line_init(start, end, fdf);
	if (!fdf->img_ptr->line_segment)
		clean_free(fdf);
	printf("Applying transformations...\n");
	rotate(fdf->cam_ptr, fdf->img_ptr->line_segment);
	project(fdf->cam_ptr, fdf->img_ptr->line_segment);
	transform(fdf->cam_ptr, fdf->img_ptr->line_segment);
	printf("After transformations:\n");
    printf("Start: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", 
           fdf->img_ptr->line_segment->start.x, 
           fdf->img_ptr->line_segment->start.y, 
           fdf->img_ptr->line_segment->start.z, 
           fdf->img_ptr->line_segment->start.colour);
    printf("End: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", 
           fdf->img_ptr->line_segment->end.x, 
           fdf->img_ptr->line_segment->end.y, 
           fdf->img_ptr->line_segment->end.z, 
           fdf->img_ptr->line_segment->end.colour);
	printf("Calling draw_line_bresenham\n");

	draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start, fdf->img_ptr->line_segment->end);
	// draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start.x, \
	// fdf->img_ptr->line_segment->start.y, \
	// fdf->img_ptr->line_segment->end.x, \
	// fdf->img_ptr->line_segment->end.y, \
	// fdf->img_ptr->line_segment->start.colour);
	free(fdf->img_ptr->line_segment);
	printf("Draw line segment end\n\n");
}

static void	canvas_setup(t_img *img, size_t canvas_byte_size)
{
	int	col;
	int	row;

	ft_bzero(img->mem_addr, canvas_byte_size);
	row = 0;
	while (row < WINDOW_HEIGHT)
	{
		col = 0;
		while (col < WINDOW_WIDTH)
		{
			set_pixel_colour(img, col, row, DEFAULT_BACKGROUND); 
			col++;
		}
		row++;
	}
}


void	render_data(t_fdf *fdf)
{
	int	row;
	int	col;

	row = 0;
	int line_segments = 0;
	printf("Starting render_data. Map size: %d x %d\n", fdf->map_data->max_m, fdf->map_data->max_n);
	canvas_setup(fdf->img_ptr, PIXEL_COUNTS * 4);
	printf("Canvas setup complete\n");
	while (row < fdf->map_data->max_m)
	{
		col = 0;
		while (col < fdf->map_data->max_n)
		{
			// horiztonal
			if (col < fdf->map_data->max_n - 1)
			{
					printf("Drawing horizontal line: row %d, col %d to %d\n", row, col, col + 1);
					draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row][col + 1]);
					line_segments++;
			
			}
			// vertical line
			if (row < fdf->map_data->max_m - 1)
			{
				printf("Drawing vertical line: row %d to %d, col %d\n", row, row + 1, col);
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row + 1][col]);
				line_segments++;
			}
			col++;
		}
		row++;
	}
	printf("Render complete. Total line segments: %d\n", line_segments);
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img_ptr->img_buff, \
			0, 0);
	printf("Image put to window\n");
}
