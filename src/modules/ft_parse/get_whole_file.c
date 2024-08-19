/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 get_whole_file.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <marvin@42.fr>						+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2024/07/29 13:27:24 by szhong			   #+#	  #+#			  */
/*	 Updated: 2024/07/29 15:40:55 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "fdf.h"
#include "libft.h"
#include <fcntl.h>

char	*ft_concatenate(char *s1, char *s2)
{
	char	*result;
	int		len1;
	int		len2;

	if (!s1)
		return (s2);
	if (!s2)
		return (s1);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	result = malloc(len1 + len2 + 1);
	if (!result)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2 + 1);
	free(s1);
	free(s2);
	return (result);
}

static char	*read_chunk(int fd, ssize_t *bytes_read)
{
	char	*buff;

	buff = malloc(sizeof(char) * (BUFF_SIZE + 1));
	if (!buff)
		return (NULL);
	*bytes_read = read(fd, buff, BUFF_SIZE);
	if (*bytes_read <= 0)
	{
		free(buff);
		return (NULL);
	}
	buff[*bytes_read] = '\0';
	return (buff);
}

static char	*process_chunks(int fd)
{
	ssize_t		bytes_read;
	char		*content;
	char		*chunk;

	content = NULL;
	while (1)
	{
		chunk = read_chunk(fd, &bytes_read);
		if (!chunk)
		{
			free(content);
			return (NULL);
		}
		if (!content)
			content = chunk;
		else
			content = ft_concatenate(content, chunk);
		if (!content)
			return (NULL);
		if (bytes_read < BUFF_SIZE)
			break ;
	}
	return (content);
}

char	*get_whole_file(const char *filename)
{
	int		fd;
	char	*content;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	content = process_chunks(fd);
	close(fd);
	return (content);
}
