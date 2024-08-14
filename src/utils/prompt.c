/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 15:52:07 by szhong            #+#    #+#             */
/*   Updated: 2024/08/14 16:22:36 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "fdf.h"

static char *get_projection_type(t_fdf *fdf)
{
    char    *projection_name;

    projection_name = "";
    if (fdf->cam_ptr->projection == ISOMETRIC)
        projection_name = "View Mode: Isometric View";
    else if (fdf->cam_ptr->projection == PERSPECTIVE)
        projection_name = "View Mode: Prospective View";
    else if (fdf->cam_ptr->projection == TOP)
        projection_name = "View Mode: Top View";
    return (projection_name);
}

void    print_menu(t_fdf *fdf)
{
    int     y;
    char    *projection_name;
    void    *mlx;
    void    *win;

    mlx = fdf->mlx_ptr;
    win = fdf->win_ptr;
    y = 0;
    projection_name = get_projection_type(fdf);
    mlx_string_put(mlx, win, 50, y += 50, DEFAULT_COLOUR, projection_name);
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "ESC to exit");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Zoom in: press 'Page Up'");
    mlx_string_put(mlx, win, 50, y += 20, DEFAULT_COLOUR, "Zoom Out: press 'Page Down'");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Move: press 'w'/'s' Up/Down");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Move: press 'a'/'d' Left/Right");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Rotate: press arrow key Rotate");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Bold: press '+'/'-' Thick/Think");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "View: press 'i' Isometric View");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "View: press 'p' Perspective View");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "View: press 't' Top View");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Colour: press 'Space'");
    mlx_string_put(mlx, win, 50, y += 25, DEFAULT_COLOUR, "Reset: press 'R'");
}

