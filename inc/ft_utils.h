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
void	key_close(t_fdf *fdf);
void	esc_close(int keycode, t_fdf *fdf);
int		close_window(t_fdf *fdf);
int		key_handler(int keycode, t_fdf *fdf);
void	print_menu(t_fdf *fdf);
void	key_rotate(int keycode, t_fdf *fdf);
void	key_scale(int keycode, t_fdf *fdf);
void	key_translate(int keycode, t_fdf *fdf);
void	key_projection(int keycode, t_fdf *fdf);
void	key_change_colour(int keycode, t_fdf *fdf);
void	error_handler(int code);

#endif
