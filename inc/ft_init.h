/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:43:40 by szhong            #+#    #+#             */
/*   Updated: 2024/08/19 18:33:09 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_INIT_H
# define FT_INIT_H
# include "ft_transform.h"
# include "ft_colour.h"
# include "fdf_types.h"

t_cartesian	**cartesian_init(int max_width, int max_depth);
t_fdf		*fdf_init(char *filepath);
t_map		*map_init(void);
t_img		*img_init(void	*mlx);
t_cam		*cam_init(t_map *data);
t_line		*line_init(t_cartesian start, t_cartesian end, t_fdf *fdf);
float		scale_to_fit(t_map *data);
t_colour	*colour_init(t_cartesian start, t_cartesian end);
void		move_origin(t_map *data);

#endif
