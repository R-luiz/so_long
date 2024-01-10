/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:26:51 by rluiz             #+#    #+#             */
/*   Updated: 2024/01/10 20:08:09 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx-linux/mlx.h"
#include <fcntl.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct
{
	int			x;
	int			y;
}				t_pos;

typedef struct
{
	t_pos		pos;
	t_pos		perv_pos;
	int			collect;
}				t_player;

typedef struct
{
	void		*void_img;
	void		*wall_img;
	void		*collect_img;
	void		*exit_img;
	void		*player_img;
	void		*playerm_img;
	void		*enemy_img;
	void		*enemym_img;
	void		*exitnplayer_img;
	void		*zero_img;
	void		*one_img;
	void		*two_img;
	void		*three_img;
	void		*four_img;
	void		*five_img;
	void		*six_img;
	void		*seven_img;
	void		*eight_img;
	void		*nine_img;
	t_player	*player;
	char		**map;
	char		*map_file;
	int			nb_map;
	int			width;
	int			height;
	int			collect;
	int			move_count;
	void		*mlx;
	void		*mlx_win;
}				t_data;

int	safeexit(t_data *data)
{
	mlx_destroy_image(data->mlx, data->void_img);
	mlx_destroy_image(data->mlx, data->wall_img);
	mlx_destroy_image(data->mlx, data->collect_img);
	mlx_destroy_image(data->mlx, data->exit_img);
	mlx_destroy_image(data->mlx, data->player_img);
	mlx_destroy_image(data->mlx, data->enemy_img);
	mlx_destroy_window(data->mlx, data->mlx_win);
	mlx_destroy_display(data->mlx);
	free(data);
	exit(0);
}

void	ft_putchar(char c)
{
	ssize_t	bytes_written;

	bytes_written = write(1, &c, 1);
	if (bytes_written == -1)
		perror("write failed");
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		ft_putchar(str[i]);
		i++;
	}
	return ;
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i - 1);
}

int	ft_strstrlen(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_putnbr(int nb)
{
	ssize_t	bytes_written;

	bytes_written = 0;
	if (nb == -2147483648)
	{
		bytes_written = write(1, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		bytes_written = write(1, "-", 1);
		nb = -nb;
	}
	if (bytes_written == -1)
		perror("write failed");
	if (nb > 9)
		ft_putnbr(nb / 10);
	ft_putchar(nb % 10 + '0');
	return ;
}

void	ft_putnbr_base(unsigned int nb, char *base, unsigned int size)
{
	if (nb >= size)
		ft_putnbr_base(nb / size, base, size);
	ft_putchar(base[nb % size]);
	return ;
}

void	ft_pourcent(const char *src, va_list list, int i)
{
	if (src[i] == 's')
		ft_putstr(va_arg(list, char *));
	else if (src[i] == 'd')
		ft_putnbr(va_arg(list, int));
	return ;
}

int	ft_printf(const char *src, ...)
{
	int		i;
	va_list	list;

	i = -1;
	va_start(list, src);
	while (src[++i])
	{
		if (src[i] == '%')
			ft_pourcent(src, list, ++i);
		else
			ft_putchar(src[i]);
	}
	va_end(list);
	return (0);
}

int	is_rectangular(t_data *data)
{
	int	i;
	int	k;
	int	j;

	i = 0;
	k = 0;
	j = 0;
	k = ft_strlen(data->map[i]);
	while (++i < ft_strstrlen(data->map))
	{
		while (j < ft_strlen(data->map[i]))
		{
			if (data->map[i][j] != '1' && data->map[i][j] != '0'
				&& data->map[i][j] != 'C' && data->map[i][j] != 'E'
				&& data->map[i][j] != 'P' && data->map[i][j] != 'X')
				return (0);
			j++;
		}
		j = 0;
		if (k != ft_strlen(data->map[i]) || i == ft_strstrlen(data->map) - 1)
		{
			if (i == ft_strstrlen(data->map) - 1 && (k
					- 1 == ft_strlen(data->map[i])))
				return (1);
			else
				return (0);
		}
	}
	return (1);
}

int	check_border_wall(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < ft_strlen(data->map[0]))
	{
		if (data->map[0][i] != '1')
			return (0);
		i++;
	}
	while (data->map[j])
	{
		if (data->map[j][0] != '1')
			return (0);
		j++;
	}
	i = 0;
	while (i < ft_strlen(data->map[0]))
	{
		if (data->map[j - 1][i] != '1')
			return (0);
		i++;
	}
	j = 0;
	while (data->map[j])
	{
		if (data->map[j][ft_strlen(data->map[0]) - 1] != '1')
			return (0);
		j++;
	}
	return (1);
}

t_pos	find_player(t_data *data)
{
	int		i;
	int		j;
	t_pos	pos;

	i = 0;
	j = 0;
	pos = (t_pos){0, 0};
	while (data->map[i])
	{
		while (data->map[i][j])
		{
			if (data->map[i][j] == 'P')
			{
				pos.x = j * 50;
				pos.y = i * 50;
				return (pos);
			}
			j++;
		}
		i++;
		j = 0;
	}
	return (pos);
}

int	is_solvable(t_data *data)
{
	t_pos	pos;
	int		i;
	int		j;

	pos = find_player(data);
	// check if there is a void around the player
	i = pos.y / 50;
	j = pos.x / 50;
	if (data->map[i - 1][j] == '0' || data->map[i + 1][j] == '0'
		|| data->map[i][j - 1] == '0' || data->map[i][j + 1] == '0')
		return (1);
	if (data->map[i - 1][j] == 'C' || data->map[i + 1][j] == 'C'
		|| data->map[i][j - 1] == 'C' || data->map[i][j + 1] == 'C')
		return (1);
	if (data->map[i - 1][j] == 'X' || data->map[i + 1][j] == 'X'
		|| data->map[i][j - 1] == 'X' || data->map[i][j + 1] == 'X')
		return (1);
	
	return (0);
}

void	check_map(t_data *data)
{
	if (is_rectangular(data) == 0)
	{
		ft_printf("Error\n");
		exit(0);
	}
	if (data->map == NULL)
	{
		ft_printf("Error\n");
		exit(0);
	}
	if (check_border_wall(data) == 0)
	{
		ft_printf("Error\n");
		exit(0);
	}
	if (is_solvable(data) == 0)
	{
		ft_printf("Error\n");
		exit(0);
	}
}

int	get_line_count(const char *filename)
{
	FILE	*file;
	int		count;
	char	*buffer;

	file = fopen(filename, "r");
	if (!file)
		return (-1);
	count = 0;
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (!buffer)
		return (-1);
	while (fgets(buffer, BUFFER_SIZE, file) != NULL)
		count++;
	fclose(file);
	free(buffer);
	return (count);
}

void	parse_map(t_data *data)
{
	int		line_count;
	FILE	*file;
	char	buffer[BUFFER_SIZE];
	int		i;

	if (!data || !data->map_file)
		return ;
	line_count = get_line_count(data->map_file);
	if (line_count <= 0)
		return ;
	data->map = malloc(sizeof(char *) * (line_count + 1));
	if (!data->map)
		return ;
	file = fopen(data->map_file, "r");
	if (!file)
		return ;
	i = 0;
	while (fgets(buffer, BUFFER_SIZE, file) != NULL)
	{
		data->map[i] = strdup(buffer);
		if (!data->map[i])
			return ;
		i++;
	}
	data->map[i] = NULL; // Null-terminate the array
	fclose(file);
}

void	print_map(t_data *data)
{
	int	i;

	i = 0;
	while (data->map[i])
	{
		ft_printf("%s", data->map[i]);
		i++;
	}
}

void	check_player_pos(t_data *data)
{
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == '1')
		data->player->pos = data->player->perv_pos;
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == 'C')
	{
		data->map[data->player->pos.y / 50][data->player->pos.x / 50] = 'P';
		if (data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
			/ 50] == 'F')
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
				/ 50] = 'E';
		else
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
				/ 50] = '0';
		data->player->collect++;
		ft_printf("collect: %d / %d\n", data->player->collect, data->collect);
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == 'E')
	{
		ft_printf("collect: %d / %d\n", data->player->collect, data->collect);
		if (data->player->collect < data->collect)
		{
			data->map[data->player->pos.y / 50][data->player->pos.x / 50] = 'F';
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
				/ 50] = '0';
			ft_printf("You need to collect all the collectibles before exiting!\n");
		}
		else
		{
			ft_printf("You win!\n");
			safeexit(data);
		}
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == 'X')
	{
		ft_printf("You lose!\n");
		safeexit(data);
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == '0')
	{
		if (data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
			/ 50] == 'F')
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
				/ 50] = 'E';
		else
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x
				/ 50] = '0';
		data->map[data->player->pos.y / 50][data->player->pos.x / 50] = 'P';
	}
}

void	refresh_window(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < data->width)
	{
		while (j < data->height)
		{
			if (data->map[j / 50][i / 50] == '1')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->wall_img, i, j);
			else if (data->map[j / 50][i / 50] == 'C')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->collect_img, i, j);
			else if (data->map[j / 50][i / 50] == 'E')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->exit_img, i, j);
			else if (data->map[j / 50][i / 50] == 'P')
				{
					if (data->player->pos.x > data->player->perv_pos.x || data->player->pos.y > data->player->perv_pos.y)
						mlx_put_image_to_window(data->mlx, data->mlx_win,
							data->player_img, i, j);
					else
						mlx_put_image_to_window(data->mlx, data->mlx_win,
							data->playerm_img, i, j);
				}
			else if (data->map[j / 50][i / 50] == 'F')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->exitnplayer_img, i, j);
			else if (data->map[j / 50][i / 50] == '1')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->enemy_img, i, j);
			else if (data->map[j / 50][i / 50] == 'X')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->enemym_img, i, j);
			else
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->void_img, i, j);
			j += 50;
		}
		i += 50;
		j = 0;
	}
}

void	creat_number_img(t_data *data)
{
	int	img_width;
	int	img_height;

	data->zero_img = mlx_xpm_file_to_image(data->mlx, "imgs/0.xpm", &img_width,
		&img_height);
	data->one_img = mlx_xpm_file_to_image(data->mlx, "imgs/1.xpm", &img_width,
		&img_height);
	data->two_img = mlx_xpm_file_to_image(data->mlx, "imgs/2.xpm", &img_width,
		&img_height);
	data->three_img = mlx_xpm_file_to_image(data->mlx, "imgs/3.xpm", &img_width,
		&img_height);
	data->four_img = mlx_xpm_file_to_image(data->mlx, "imgs/4.xpm", &img_width,
		&img_height);
	data->five_img = mlx_xpm_file_to_image(data->mlx, "imgs/5.xpm", &img_width,
		&img_height);
	data->six_img = mlx_xpm_file_to_image(data->mlx, "imgs/6.xpm", &img_width,
		&img_height);
	data->seven_img = mlx_xpm_file_to_image(data->mlx, "imgs/7.xpm", &img_width,
		&img_height);
	data->eight_img = mlx_xpm_file_to_image(data->mlx, "imgs/8.xpm", &img_width,
		&img_height);
	data->nine_img = mlx_xpm_file_to_image(data->mlx, "imgs/9.xpm", &img_width,
		&img_height);
}

void	number_put_img(t_data *data, int number, int x, int y)
{
	if (number == 0)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->zero_img, x, y);
	if (number == 1)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->one_img, x, y);
	if (number == 2)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->two_img, x, y);
	if (number == 3)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->three_img, x,
			y);
	if (number == 4)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->four_img, x, y);
	if (number == 5)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->five_img, x, y);
	if (number == 6)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->six_img, x, y);
	if (number == 7)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->seven_img, x,
			y);
	if (number == 8)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->eight_img, x,
			y);
	if (number == 9)
		mlx_put_image_to_window(data->mlx, data->mlx_win, data->nine_img, x, y);
}

void	count_put_img(t_data *data)
{
	int	x;
	int	y;
	int	count;

	x = data->width - 50;
	y = data->height - 50;
	count = data->move_count;
	while (count > 0)
	{
		number_put_img(data, count % 10, x, y);
		count /= 10;
		x -= 50;
	}
}

void	collect_put_img(t_data *data)
{
	int	x;
	int	y;
	int	count;

	x = (int)(log(data->player->collect) / log(10)) * 50;
	y = data->height - 50;
	count = data->player->collect;
	while (count > 0)
	{
		number_put_img(data, count % 10, x, y);
		count /= 10;
		x -= 50;
	}
}

void	move_enemy(t_data *data, int direction)
{
	//enemies move right and left in void then switch direction when they hit a wall
	int	i;
	int	j;
	
	i = 0;
	j = 0;
	direction = direction % 9;
	direction = direction % 2;
	printf("direction: %d\n", direction);
	while (data->map[i])
	{
		while (data->map[i][j])
		{
			if (data->map[i][j] == 'X')
			{
				if (data->map[i][j + 1 * (-1 * -1 * direction)] == '0' || data->map[i][j + 1 * (-1 *-1 * direction)] == 'P')
				{
					if (data->map[i][j + 1] == 'P')
					{
						ft_printf("You lose!\n");
						safeexit(data);
					}
					data->map[i][j] = '0';
					mlx_put_image_to_window(data->mlx, data->mlx_win,
						data->void_img, j * 50, (i + 1) * 50);
					data->map[i][j + 1] = 'X';
					mlx_put_image_to_window(data->mlx, data->mlx_win,
						data->enemy_img, j * 50, (i + 1) * 50);
					
					break;
				}
				if (data->map[i][j - 1 * (-1 * -1 * direction)] == '0' || data->map[i][j - 1 * (-1 * -1 * direction)] == 'P')
				{
					if (data->map[i][j - 1] == 'P')
					{
						ft_printf("You lose!\n");
						safeexit(data);
					}
					data->map[i][j] = '0';
					mlx_put_image_to_window(data->mlx, data->mlx_win,
						data->void_img, j * 50, (i + 1) * 50);
					data->map[i][j - 1] = 'X';
					mlx_put_image_to_window(data->mlx, data->mlx_win,
						data->enemym_img, j * 50, (i - 1) * 50);
					break;
				}
			}
			j++;
		}
		i++;
		j = 0;
	}
}

void	player_move(t_data *data, int keycode)
{
	data->player->perv_pos = data->player->pos;
	if (keycode == 65362)
		data->player->pos.y -= 50;
	if (keycode == 65364)
		data->player->pos.y += 50;
	if (keycode == 65361)
		data->player->pos.x -= 50;
	if (keycode == 65363)
		data->player->pos.x += 50;
	data->move_count++;
	check_player_pos(data);
	move_enemy(data, data->move_count);
	refresh_window(data);
	count_put_img(data);
	collect_put_img(data);
	// ft_printf("move count: %d\n", data->move_count);
}

int	hook(int keycode, void *param)
{
	if (keycode == 65307)
		safeexit(param);
	if (keycode == 65362 || keycode == 65364 || keycode == 65361
		|| keycode == 65363)
		player_move((t_data *)param, keycode);
	return (0);
}

int	count_collect(t_data *data)
{
	int	i;
	int	j;
	int	count;

	i = 0;
	j = 0;
	count = 0;
	while (data->map[i])
	{
		while (data->map[i][j])
		{
			if (data->map[i][j] == 'C')
				count++;
			j++;
		}
		i++;
		j = 0;
	}
	return (count);
}

void	creat_img(t_data *data)
{
	int	img_width;
	int	img_height;

	data->void_img = mlx_xpm_file_to_image(data->mlx, "imgs/void.xpm",
		&img_width, &img_height);
	data->wall_img = mlx_xpm_file_to_image(data->mlx, "imgs/wall.xpm",
		&img_width, &img_height);
	data->collect_img = mlx_xpm_file_to_image(data->mlx, "imgs/collect.xpm",
		&img_width, &img_height);
	data->exit_img = mlx_xpm_file_to_image(data->mlx, "imgs/exit.xpm",
		&img_width, &img_height);
	data->enemy_img = mlx_xpm_file_to_image(data->mlx, "imgs/enemy.xpm",
		&img_width, &img_height);
	data->enemym_img = mlx_xpm_file_to_image(data->mlx, "imgs/enemym.xpm",
		&img_width, &img_height);
	data->exitnplayer_img = mlx_xpm_file_to_image(data->mlx,
		"imgs/exitnplayer.xpm", &img_width, &img_height);
	data->player->pos = find_player(data);
	data->player_img = mlx_xpm_file_to_image(data->mlx, "imgs/player.xpm",
		&img_width, &img_height);
	data->playerm_img = mlx_xpm_file_to_image(data->mlx, "imgs/playerm.xpm",
		&img_width, &img_height);
	data->player->collect = 0;
	data->player->perv_pos = data->player->pos;
	data->move_count = 0;
	data->collect = count_collect(data);
}

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		if (src[i] == '\n')
			dest[i] = '\0';
		else
			dest[i] = src[i];
		i++;
	}
	return (dest);
}

int	ft_atoi(char *str)
{
	int	i;
	int	nb;

	i = 0;
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb *= 10;
		nb += ((int)str[i] - '0');
		i++;
	}
	return (nb);
}

void	ft_strcpy2(t_data *data, char *map, int i)
{
	while (*map)
	{
		data->map_file[i] = *map;
		map++;
		i++;
	}
}

void	create_file_name(t_data *data, char **argv, int argc)
{
	char	*map;

	if (argc == 1)
	{
		data->map_file = "maps/map1.ber";
		return ;
	}
	if (ft_atoi(argv[1]) == 0 || (ft_atoi(argv[1]) > data->nb_map - 1))
	{
		ft_printf("Error input file\n");
		safeexit(data);
	}
	map = "maps/map";
	data->map_file = (char *)malloc(sizeof(char) * 12 + ft_strlen(argv[1]));
	ft_strcpy2(data, map, 0);
	map = argv[1];
	ft_strcpy2(data, map, 8);
	map = ".ber";
	ft_strcpy2(data, map, 9 + ft_strlen(argv[1]));
}

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc > 2)
		ft_printf("Error input\n");
	data = (t_data *)malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->nb_map = 10;
	create_file_name(data, argv, argc);
	parse_map(data);
	check_map(data);
	data->player = (t_player *)malloc(sizeof(t_player));
	creat_img(data);
	creat_number_img(data);
	data->width = ft_strlen(data->map[0]) * 50;
	data->height = ft_strstrlen(data->map) * 50;
	data->mlx_win = mlx_new_window(data->mlx, data->width, data->height,
		"so_long");
	refresh_window(data);
	mlx_hook(data->mlx_win, 33, 1L << 17, safeexit, data);
	mlx_key_hook(data->mlx_win, hook, (void *)data);
	mlx_loop(data->mlx);
	return (0);
}
