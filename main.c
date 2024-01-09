/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 18:26:51 by rluiz             #+#    #+#             */
/*   Updated: 2024/01/09 18:27:41 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./minilibx-linux/mlx.h"

int main(void)
{
    void    *mlx;
    void    *mlx_win;
    void    *img;
    int     img_width, img_height;

    mlx = mlx_init();
    mlx_win = mlx_new_window(mlx, 800, 600, "so_long");

    // Load an image
    img = mlx_xpm_file_to_image(mlx, "image.xpm", &img_width, &img_height);

    // Put the image to the window
    mlx_put_image_to_window(mlx, mlx_win, img, 0, 0);

    // Handle the loop
    mlx_loop(mlx);
    return (0);
}
