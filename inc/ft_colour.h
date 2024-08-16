/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_colour.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:30:59 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:28:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_COLOUR_H
# define FT_COLOUR_H
# define LOW_COLOUR 0x008080   // Teal
# define MID_COLOUR 0xFFA500   // Orange
# define HIGH_COLOUR 0xFF00FF  // Magenta
# include "fdf_types.h"
# include <stdlib.h>

int		get_colour(t_colour *colour, int i_line, int line_size);
void	apply_colours(t_fdf *fdf, t_cartesian *point);
#endif
