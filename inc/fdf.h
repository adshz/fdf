/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 10:25:53 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:30:28 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FDF_H
# define FDF_H
# define HEXADECIMAL "0123456789abcdef"
# define BUFF_SIZE 4096
# define WINDOW_WIDTH 1680 
# define WINDOW_HEIGHT 980
# define PIXEL_COUNTS 1646400
# define DEFAULT_BACKGROUND 0x010203
# define DEFAULT_COLOUR 0xFFFFFF
// system library
# include <stdlib.h>
# include <error.h>
# include <stdio.h>
# include <math.h>

// minilibx
# include "../minilibx-linux/mlx.h"

// X11
# include <X11/keysym.h>
# include <X11/X.h>

// libft
# include "libft.h"

// fdf libray
# include "ft_init.h"
# include "ft_render.h"
# include "ft_transform.h"
# include "ft_colour.h"
# include "ft_parse.h"
# include "ft_utils.h"

#endif
