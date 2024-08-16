/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_transform.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:09:09 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 16:22:44 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_TRANSFORM_H
# define FT_TRANSFORM_H
# define ANG_30	0.52359877
# define ANG_45 0.78539816
# include "ft_init.h"
# include "fdf_types.h"

// rotatation part
void	rotate(t_cam *cam, t_line *line);
void	x_rotation(t_cam *cam, t_line *line);
// projection part
void	project(t_cam *cam, t_line *line);
// view transform part
void	view_transform(t_cam *cam, t_line *line);
#endif
