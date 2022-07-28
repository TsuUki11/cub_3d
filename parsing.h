#ifndef PARSING_H
# define PARSING_H

# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
#include <errno.h>
# include "./libft/libft.h"

typedef struct 
{
	char			*n_path;
	char			*s_path;
	char			*w_path;
	char			*e_path;
	unsigned long	f_hexa;
	unsigned long	c_hexa;
	char			**map;
} data;


char	**read_map(int fd);
void	parsing(char *map);
void	print_error(int error);
int		is_map(char *str);
void	parse_line(char *line, data *d);
int		arrlen(char **arr);
unsigned long	color_parse(char *str);
unsigned long	rgb_to_hexa(int r, int g, int b);
char	**map_checker(char **str);

#endif
