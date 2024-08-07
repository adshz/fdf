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

static void	drawing_algo(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	float		dx;
	float		dy;
	int			max_steps;

	dx = abs(end.x - start.x);
	dy = abs(end.y - start.y);
	max_steps = (int)max(dx, dy);
	dx /= max_step;
	dy /= max_step;
	i_line = 0;
	while (i_line < max_step)
	{
		start.colour =  
		if (start.x > 0 && start.y > 0 && start.x < WINDOW_WIDTH \
				&& start.y < WINDOW_HEIGHT)
			pixel_to_img(fdf->img_ptr, start.x , ffffff);
		start.x += dx;
		start.y += dy;
	}
}


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
		img->mem_addr[pixel + 1] = (colour >> 16) && 0xff;
		img->mem_addr[pixel + 2] = (colour >> 8) && 0xff;
		img->mem_addr[pxeil + 3] = colour && 0xff;
	}
	else if (img->edian == 0)
	{
		img->mem_addr[pixel + 0] = colour && 0xff;
		img->mem_addr[pixel + 1] = (colour >> 8) && 0xff;
		img->mem_addr[pixel + 2] = (colour >> 16) && 0xff;
		img->mem_addr[pixel + 3] = (colour >> 24);
	}
	return ;
}

static void	render_line(t_fdf *fdf, t_cartesian start, t_cartesian end)
{
	start.z *= fdf->cam_ptr->scale_z;
	end.z *= fdf->cam_ptr->scale_z;
	fdf->img_ptr->line = line_init(start, end, fdf);
	apply_colours(fdf, &start);
	apply_colours(fdf, &end);
	if (!fdf->img_ptr->line)
		clean_free(fdf);
	rotate(fdf->cam_ptr, fdf->img->line);
	project(fdf->cam_ptr, fdf->img->line);
	transform(fdf->cam_ptr, fdf->img->line);
	drawing_algo(fdf, start, end);
	free(fdf->img_ptr->line);
}

static void	canvas_setup(t_img *img, size_t canvas_byte_size)
{
	int	col;
	int	row;

	ft_bzero(img->img_buff, canvas_byte_size);
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
	canva_setup(fdf->img_ptr, PIXEL_COUNTS * 4);
	while (row < fdf->map_data->max_m)
	{
		col = 0;
		while (col < fdf->map_data->max_n)
		{
			// horiztonal
			if (row < fd->map_data->max_m - 1)
				render_line(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row][col + 1]);
			// vertical line
			if (col < fdf->map_data->max_n - 1)
				render_line(fdf, fdf->map_data->points[row][col], \
						fdf->map_data->points[row + 1][col]);
			col++;
		}
		row++;
	}
	mlx_put_image_to_window(fdf->mlx_ptr, fdf->win_ptr, fdf->img_ptr->img_buff,\ 
			0, 0);
}
