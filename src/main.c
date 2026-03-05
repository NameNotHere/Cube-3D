/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msokolov <msokolov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 15:17:09 by msokolov          #+#    #+#             */
/*   Updated: 2026/02/13 14:55:55 by msokolov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	clear_screen(t_data *data)
{
	uint32_t		*p;
	unsigned int	x;
	unsigned int	y;

	p = (uint32_t *)data->img->pixels;
	y = 0;
	while (y < data->img->height)
	{
		x = 0;
		while (x < data->img->width)
		{
			if (y < data->img->height / 2)
				p[y * data->img->width + x] = 0xFF000000
					| (data->ceiling & 0x00FFFFFF);
			else
				p[y * data->img->width + x] = 0xFF000000
					| (data->floor & 0x00FFFFFF);
			x++;
		}
		y++;
	}
}

void	game_loop(void *param)
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	move_player(data);
	mouse_look(data);
	arrow_look(data);
	door(data, data->player);
	clear_screen(data);
	raycast_and_draw(data, data->player);
	draw_minimap(data);
}

void	find_player(t_data *data, t_player *player)
{
	int	x;
	int	y;

	y = 0;
	while (data->map[y])
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'P')
			{
				player->pos.x = x + 0.5;
				player->pos.y = y + 0.5;
			}
			x++;
		}
		y++;
	}
}

int	main(int ac, char **av)
{
	t_data		data;
	t_player	player;

	if (ac < 2)
		return (printf("Program requires MORE than 1 argument\n"), 1);
	if (ac > 2)
		return (printf("Program requires ONLY 1 argument\n"), 1);
	if (prefix_check(av[1]))
		return (-1);
	data_init(&data);
	if (!map_reader(av[1], &data))
		return (clean_data(&data), -1);
	if (!map_init(&data))
		return (clean_data(&data), -1);
	if (map_y_check(&data) == false || map_x_check(&data) == false)
		return (clean_data(&data), false);
	find_player(&data, &player);
	find_door(&data);
	if (!player_init(&player, &data))
		return (clean_data(&data), -1);
	if (!game_init(&data, &player))
		return (printf("Game init failed\n"), -1);
	return (clean_data(&data), 0);
}
