/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:26:51 by rluiz             #+#    #+#             */
/*   Updated: 2024/01/09 19:48:22 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	void		*mlx;
	void		*win;
	char		**map;
}				t_game;

typedef struct
{
	int			x;
	int			y;
}				t_pos;

typedef struct
{
	t_pos		pos;
	int			direction;
	int			speed;
	void		*img;
}				t_player;

typedef struct
{
	void		*background;
	t_player	*player;
	char		*map;
	int			width;
	int			height;
	void		*mlx;
	void		*mlx_win;
}				t_data;

void	safeexit(t_data *data)
{
	mlx_destroy_image(data->mlx, data->background);
	mlx_destroy_window(data->mlx, data->mlx_win);
	mlx_destroy_display(data->mlx);
	free(data);
	exit(0);
}

void	hook(int keycode, void *param)
{
	t_data		*data;
	t_player	*player;

	data = (t_data *)param;
	player = data->player;
	if (keycode == 65307)
		safeexit(param);
	if (keycode == 65362)
		player->pos.y -= 10;
	if (keycode == 65364)
		player->pos.y += 10;
	if (keycode == 65361)
		player->pos.x -= 10;
	if (keycode == 65363)
		player->pos.x += 10;
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->background, 0, 0);
	mlx_put_image_to_window(data->mlx, data->mlx_win, player->img,
		player->pos.x, player->pos.y);
}

int	main(void)
{
	t_data		*data;
	t_player	*player;

	int img_width, img_height;
	data = (t_data *)malloc(sizeof(t_data));
	data->mlx = mlx_init();
	data->mlx_win = mlx_new_window(data->mlx, 1700, 900, "so_long");
	// put the background
	data->background = mlx_xpm_file_to_image(data->mlx, "foret.xpm", &img_width,
		&img_height);
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->background, 0, 0);
	// Load an image
	data->player = (t_player *)malloc(sizeof(t_player));
	data->player->pos = (t_pos){.x = 1000, .y = 550};
	data->player->img = mlx_xpm_file_to_image(data->mlx, "image2.xpm",
		&img_width, &img_height);
	// Put the image 104 by 104 to the window to cover the whole window
	mlx_put_image_to_window(data->mlx, data->mlx_win, data->player->img, 1000,
		550);
	// Handle the loop
	mlx_key_hook(data->mlx_win, hook, data);
	mlx_loop(data->mlx);
	return (0);
}
