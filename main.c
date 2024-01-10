/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:26:51 by rluiz             #+#    #+#             */
/*   Updated: 2024/01/10 02:44:11 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx-linux/mlx.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
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
	void		*enemy_img;
	void		*exitnplayer_img;
	t_player	*player;
	char		**map;
	char		*map_file;
	int			width;
	int			height;
	int			collect;
	int			move_count;
	void		*mlx;
	void		*mlx_win;
}				t_data;

void	safeexit(t_data *data)
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

int	is_rectangular(t_data *data)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (data->map[i])
	{
		while (data->map[i][j])
			j++;
		if (k == 0)
			k = j;
		if (k != j)
			return (0);
		j = 0;
		i++;
	}
	return (1);
}

int	check_border_wall(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->map[i])
	{
		while (data->map[i][j])
		{
			if (i == 0 || j == 0 || i == ft_strstrlen(data->map) - 1
				|| j == ft_strlen(data->map[0]) - 1)
			{
				if (data->map[i][j] != '1')
					return (0);
			}
			j++;
		}
		i++;
		j = 0;
	}
	return (1);
}

int	is_solvable(t_data *data)
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
			if (data->map[i][j] == 'P')
				count++;
			j++;
		}
		i++;
		j = 0;
	}
	if (count != 1)
		return (0);
	return (1);
}

void	check_map(t_data *data)
{
	FILE	*file;
	int		count;
	char	buffer[BUFFER_SIZE];
	if (is_rectangular(data) == 0)
	{
		printf("Error\n");
		exit(0);
	}
	if (data->map == NULL)
	{
		printf("Error\n");
		exit(0);
	}
	if (check_border_wall(data) == 0)
	{
		printf("Error\n");
		exit(0);
	}
	if (is_solvable(data) == 0)
	{
		printf("Error\n");
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
		printf("%s", data->map[i]);
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
		if (data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] == 'F')
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] = 'E';
		else
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] = '0';
		data->player->collect++;
		printf("collect: %d / %d\n", data->player->collect, data->collect);
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == 'E')
	{
		printf("collect: %d / %d\n", data->player->collect, data->collect);
		if (data->player->collect < data->collect)
		{
			data->map[data->player->pos.y / 50][data->player->pos.x / 50] = 'F';
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] = '0';
			printf("You need to collect all the collectibles before exiting!\n");
		}
		else
		{
			printf("You win!\n");
			safeexit(data);
		}
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == 'X')
	{
		printf("You lose!\n");
		safeexit(data);
	}
	if (data->map[data->player->pos.y / 50][data->player->pos.x / 50] == '0')
	{
		if (data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] == 'F')
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] = 'E';
		else
			data->map[data->player->perv_pos.y / 50][data->player->perv_pos.x / 50] = '0';
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
				mlx_put_image_to_window(data->mlx, data->mlx_win,
						data->player_img, i, j);
			else if (data->map[j / 50][i / 50] == 'F')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->exitnplayer_img, i, j);
			else if (data->map[j / 50][i / 50] == '1')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->enemy_img, i, j);
			else if (data->map[j / 50][i / 50] == '0' || data->map[j / 50][i / 50] == 'C')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->void_img, i, j);
			else if (data->map[j / 50][i / 50] == 'X')
				mlx_put_image_to_window(data->mlx, data->mlx_win,
					data->enemy_img, i, j);
			j += 50;
		}
		i += 50;
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
	printf("move count: %d\n", data->move_count);
	check_player_pos(data);
	refresh_window(data);
}

void	hook(int keycode, void *param)
{
	t_data		*data;
	t_player	*player;

	data = (t_data *)param;
	player = data->player;
	if (keycode == 65307)
		safeexit(param);
	if (keycode == 65362 || keycode == 65364 || keycode == 65361
		|| keycode == 65363)
		player_move(data, keycode);
}

t_pos	find_player(t_data *data)
{
	int		i;
	int		j;
	t_pos	pos;

	i = 0;
	j = 0;
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

int		count_collect(t_data *data)
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
	data->collect_img = mlx_xpm_file_to_image(data->mlx, "imgs/collect2.xpm",
		&img_width, &img_height);
	data->exit_img = mlx_xpm_file_to_image(data->mlx, "imgs/exit.xpm",
		&img_width, &img_height);
	data->enemy_img = mlx_xpm_file_to_image(data->mlx, "imgs/enemy.xpm",
		&img_width, &img_height);
	data->exitnplayer_img = mlx_xpm_file_to_image(data->mlx, "imgs/exitnplayer.xpm",
		&img_width, &img_height);
	data->player->pos = find_player(data);
	data->player_img = mlx_xpm_file_to_image(data->mlx, "imgs/player.xpm",
		&img_width, &img_height);
	data->player->collect = 0;
	data->player->perv_pos = data->player->pos;
	data->move_count = 0;
	data->collect = count_collect(data);
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

int	main(void)
{
	t_data		*data;
	t_player	*player;
	int			i;
	int			j;

	data = (t_data *)malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->map_file = "map5.ber";
	parse_map(data);
	data->player = (t_player *)malloc(sizeof(t_player));
	creat_img(data);
	data->width = ft_strlen(data->map[0]) * 50;
	data->height = ft_strstrlen(data->map) * 50;
	printf("width: %d\n", data->width);
	printf("height: %d\n", data->height);
	data->mlx_win = mlx_new_window(data->mlx, data->width, data->height,
		"so_long");
	refresh_window(data);
	mlx_hook(data->mlx_win, 33, 1L << 17, safeexit, data);
	mlx_key_hook(data->mlx_win, hook, data);
	mlx_loop(data->mlx);
	return (0);
}
