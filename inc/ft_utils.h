/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:50:41 by szhong            #+#    #+#             */
/*   Updated: 2024/08/21 22:49:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_UTILS_H
# define FT_UTILS_H

void	free_arr(char **arr);
void	free_points(t_cartesian **points, int max_depth);
float	min(float a, float b);
float	max(float a, float b);
void	clean_up(t_fdf **fdf);
void	clean_free(t_fdf *fdf);
void	win_clean_free(t_fdf *fdf);
void	close_window(void *param);
void	key_handler(mlx_key_data_t keydata, void *param);
void	print_menu(t_fdf *fdf);
void	key_rotate(keys_t keycode, t_fdf *fdf);
void	key_scale(keys_t keycode, t_fdf *fdf);
void	key_translate(keys_t keycode, t_fdf *fdf);
void	key_projection(keys_t keycode, t_fdf *fdf);
void	key_change_colour(keys_t keycode, t_fdf *fdf);
void	error_handler(int code);

#endif
