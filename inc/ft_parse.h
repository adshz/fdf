/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:38:00 by szhong            #+#    #+#             */
/*   Updated: 2024/08/16 14:47:24 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PARSE_H
# define FT_PARSE_H

char	*ft_concatenate(char *s1, char *s2);
char	*get_whole_file(const char *filename);
t_map	*parse_data(char *filepath);
void	parse_line_wrapper(char *line, t_map *data);
void	move_origin(t_map *data);
size_t	count_cols(char const *s, char c);
#endif
