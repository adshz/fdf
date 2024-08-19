/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:38:00 by szhong            #+#    #+#             */
/*   Updated: 2024/08/19 20:16:25 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PARSE_H
# define FT_PARSE_H
# define PARSE_FAILURE 0
# define PARSE_SUCCESS 1
# define TRUE 1
# define FALSE 0

char	*ft_concatenate(char *s1, char *s2);
char	*get_whole_file(const char *filename);
t_map	*parse_data(char *filepath);
void	parse_line_wrapper(char *line, t_map *data);
size_t	count_cols(char const *s, char c);
void	process_line(char **line_ptr, t_map *data, \
		void (*process_func)(char *, t_map *));
void	count_line(char *line, t_map *data);
void	count_dimensions(char *content, t_map *data);
void	parse_content(char *content, t_map *data);
#endif
