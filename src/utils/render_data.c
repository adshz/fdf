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

# define LINE_THICKNESS 1.5
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

static float	absolute(float nbr)
{
    if (nbr < 0)
        return (-nbr);
    else
        return (nbr);
}

static void draw_thick_pixel(t_fdf *fdf, int x, int y, int colour, double thickness)
{
    double dx;
    double dy;
    int temp_x;
    int temp_y;

    dy = -thickness / 2;
    while (dy <= thickness / 2)
    {
        dx = -thickness / 2;
        while (dx <= thickness / 2)
        {
            temp_x = x + (int)dx;
            temp_y = y + (int)dy;
            if (temp_x >= 0 && temp_x < WINDOW_WIDTH && temp_y >= 0 && temp_y < WINDOW_HEIGHT)
                set_pixel_colour(fdf->img_ptr, temp_x, temp_y, colour);
            dx += 0.5;
        }
        dy += 0.5;
    }
}

static void draw_line_low(t_fdf *fdf, t_cartesian start, t_cartesian end, t_colour *color)
{
    int dx;
    int dy;
    int yi;
    int D;
    int x;
    int y;

    dx = end.x - start.x;
    dy = end.y - start.y;
    yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    D = (2 * dy) - dx;
    x = start.x;
    y = start.y;

    while (x <= end.x)
    {
        draw_thick_pixel(fdf, x, y, get_colour(color, x - start.x, end.x - start.x), \
                fdf->cam_ptr->line_thickness);
        if (D > 0)
        {
            y = y + yi;
            D = D + (2 * (dy - dx));
        }
        else
            D = D + 2*dy;
        x++;
    }
}

static void draw_line_high(t_fdf *fdf, t_cartesian start, t_cartesian end, t_colour *color)
{
    int dx;
    int dy;
    int xi;
    int D;
    int x;
    int y;

    dx = end.x - start.x;
    dy = end.y - start.y;
    xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    D = (2 * dx) - dy;
    x = start.x;
    y = start.y;

    while (y <= end.y)
    {
        draw_thick_pixel(fdf, x, y, get_colour(color, y - start.y, end.y - start.y), \
                fdf->cam_ptr->line_thickness);
        if (D > 0)
        {
            x = x + xi;
            D = D + (2 * (dx - dy));
        }
        else
            D = D + 2*dx;
        y++;
    }
}

static void draw_line_bresenham(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
    t_colour *color;

    color = colour_init(start, end);
    if (!color)
        clean_free(fdf);
    if (absolute(end.y - start.y) < absolute(end.x - start.x))
    {
        if (start.x > end.x)
            draw_line_low(fdf, end, start, color);
        else
            draw_line_low(fdf, start, end, color);
    }
    else
    {
        if (start.y > end.y)
            draw_line_high(fdf, end, start, color);
        else
            draw_line_high(fdf, start, end, color);
    }

    free(color);
}

static void draw_line_segment(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
    if (!fdf || !fdf->cam_ptr || !fdf->img_ptr)
        return ;
    start.z *= fdf->cam_ptr->scale_z;
    end.z *= fdf->cam_ptr->scale_z;
    apply_colours(fdf, &start);
    apply_colours(fdf, &end);
    fdf->img_ptr->line_segment = line_init(start, end, fdf);
    if (!fdf->img_ptr->line_segment)
        clean_free(fdf);
    rotate(fdf->cam_ptr, fdf->img_ptr->line_segment);
    project(fdf->cam_ptr, fdf->img_ptr->line_segment);
    view_transform(fdf->cam_ptr, fdf->img_ptr->line_segment);
    draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start, \
            fdf->img_ptr->line_segment->end);
    free(fdf->img_ptr->line_segment);
}
// static void	draw_line_segment(t_fdf *fdf, t_cartesian start, t_cartesian end)
// {
//	if (!fdf || !fdf->cam_ptr || !fdf->img_ptr)
//		return ;
//     printf("Draw line segment start\n");
//     printf("Original - Start: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", start.x, start.y, start.z, start.colour);
//     printf("Original - End: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", end.x, end.y, end.z, end.colour);

//	start.z *= fdf->cam_ptr->scale_z;
//	end.z *= fdf->cam_ptr->scale_z;
    
//	printf("After z-scale - Start: x=%.2f, y=%.2f, z=%.2f\n", start.x, start.y, start.z);
//     printf("After z-scale - End: x=%.2f, y=%.2f, z=%.2f\n", end.x, end.y, end.z);

//	apply_colours(fdf, &start);
//	apply_colours(fdf, &end);
//	printf("After apply_colours - Start color: 0x%X\n", start.colour);
//     printf("After apply_colours - End color: 0x%X\n", end.colour);

//	fdf->img_ptr->line_segment = line_init(start, end, fdf);
//	if (!fdf->img_ptr->line_segment)
//		clean_free(fdf);
//	printf("Applying transformations...\n");
//	rotate(fdf->cam_ptr, fdf->img_ptr->line_segment);
//	project(fdf->cam_ptr, fdf->img_ptr->line_segment);
//	transform(fdf->cam_ptr, fdf->img_ptr->line_segment);
//	printf("After transformations:\n");
//     printf("Start: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", 
//            fdf->img_ptr->line_segment->start.x, 
//            fdf->img_ptr->line_segment->start.y, 
//            fdf->img_ptr->line_segment->start.z, 
//            fdf->img_ptr->line_segment->start.colour);
//     printf("End: x=%.2f, y=%.2f, z=%.2f, color=0x%X\n", 
//            fdf->img_ptr->line_segment->end.x, 
//            fdf->img_ptr->line_segment->end.y, 
//            fdf->img_ptr->line_segment->end.z, 
//            fdf->img_ptr->line_segment->end.colour);
//	printf("Calling draw_line_bresenham\n");
//	draw_line_bresenham(fdf, fdf->img_ptr->line_segment->start, \
//			fdf->img_ptr->line_segment->end);
//	free(fdf->img_ptr->line_segment);
//	printf("Draw line segment end\n\n");
// }

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
    canvas_setup(fdf->img_ptr, PIXEL_COUNTS * 4);
    while (row < fdf->map_data->max_m)
    {
        col = 0;
        while (col < fdf->map_data->max_n)
        {
            if (col < fdf->map_data->max_n - 1)
                    draw_line_segment(fdf, fdf->map_data->points[row][col], \
                        fdf->map_data->points[row][col + 1]);
			if (row < fdf->map_data->max_m - 1)
				draw_line_segment(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row + 1][col]);
			col++;
		}
		row++;
	}
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img_ptr->img_buff, \
			0, 0);
	print_menu(fdf);
}
