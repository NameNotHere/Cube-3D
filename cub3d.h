/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msokolov <msokolov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 11:10:06 by orhan             #+#    #+#             */
/*   Updated: 2026/02/13 14:04:19 by msokolov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# define MAX_MAP_LINES 2000
# define TITLE_SIZE 64
# define MINIMAP_SIZE 10
# define OFFSET_X 0
# define OFFSET_Y 0
# define SCREEN_WIDTH 1080
# define SCREEN_HEIGHT 1080
# define CEILING_COLOUR 120
# define PLAYER_COLLISION_BUFFER 0.3
# define NUMBER_OF_THREADS	4
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include "libft/libft.h"
# include <math.h>
# include "MLX42/include/MLX42/MLX42.h"

typedef struct t_data	t_data;


typedef struct t_point
{
	double	x;
	double	y;
}	t_point;

typedef struct t_ipoint
{
	int	x;
	int	y;
}	t_ipoint;

typedef struct t_player
{
	t_point	pos;
	double	speed;
	t_point	dir;
	t_point	plane;
	t_data	*data;
}	t_player;

typedef struct t_data
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_texture_t	*wall[5]; // 0 = north, 1 = south, 2 = west, 3 = east
	mlx_image_t		*wall_img[5];
	mlx_image_t		*minimap_i;
	char			**map;
	int				width;
	int				height;
	char			*no;
	char			*so;
	char			*we;
	char			*ea;
	uint32_t		floor;
	uint32_t		ceiling;
	int				n_flag;
	int				we_found;
	char			**tmp_floor;
	char			**tmp_ceiling;
	int				we_flag;
	int				no_flag;
	int				ea_flag;
	int				so_flag;
	int				wall_found;
	int				door_count;
	int				door_flag;
	int				key_e_was_pressed;
	int				door_y;
	int				door_x;
	int				ceiling_flag;
	int				floor_flag;
	t_player		*player;
}	t_data;

typedef struct t_ray
{
	t_point		raydir;
	t_point		sidedist;
	t_point		deltadist;
	double		perpwalldist;
	t_ipoint	map;
	t_point		step;
	int			side;
	int			hit;
}	t_ray;

typedef struct t_stripe
{
	int	drawstart;
	int	drawend;
	int	lineheight;
	int	texx;
	int	x;
}	t_stripe;

typedef	struct	s_useless_thread_task
{
	t_data		*data;
	t_player	*player;
	int			start_x;
	int			end_x;
} t_thread_task;

/* ===================== PARSING / MAP ===================== */

// char	color_set(char *filename, t_data *data);
char	**map_reader(char *filename, t_data *data);
char	**cordinates_check(char *filename, t_data *data);
char	prefix_check(char *filename);
char	dublicate_check(t_data *data);
char	map_validation(t_data *data);
char	map_char_check(t_data *data);
// char	wall_height_check(t_data *data);
// char	map_witdh_check(t_data *data);
bool    column_check_helper(t_data *data, int x, int y);
int     get_map_width(char **map);
bool	map_x_check(t_data *data);
bool	map_y_check(t_data *data);

char	**trim_floor(char **floor);
char	**trim_celing(char **ceiling);
char	**set_map(int fd, t_data *data, char **temp, int *i);

// bool	load_map(t_data *data);
bool	map_init(t_data *data);
bool	additional_check(char *line, t_data *data);
bool	collums_check(t_data *data);
bool	island_check(t_data *data, size_t y, size_t x);

bool	map_up_check(t_data *data, size_t x, size_t y);
bool	map_down_check(t_data *data, size_t x, size_t y);
bool	map_left_check(t_data *data, size_t x, size_t y);
bool	map_right_check(t_data *data, size_t x, size_t y);

int		get_map_height(char **map);
int		get_map_width(char **str);
void	find_door(t_data *data);
/* ===================== TEXTURES / COLORS ===================== */

bool	set_walls_texture(t_data *data, char *ptr);
// bool	walls_set(char *filename, t_data *data);

bool	parse_floor_ceiling(char *path, t_data *data, char **line);
bool	floor_ceiling_check(char **path);
bool	set_collor(t_data *data, int y, int x);
int		rgb(int r, int g, int b);

bool	texture_load(t_data *texture);
bool	load_image(t_data *texture);

/* ===================== INIT / CLEAN ===================== */

void	data_init(t_data *data);
void	clean_data(t_data *data);
void	clean_ceiling(t_data *data);
void	clean_floor(char **data);
void	change_perspective(t_data *data);
bool	game_init(t_data *data, t_player *player);
void	game_loop(void *param);

/* ===================== WINDOW / RENDER ===================== */

// bool	window_witdh(t_data *data);
bool	window_hight(t_data *data);

// void	render_map(t_data *data);
bool	put_pixel(t_data *data, int x, int y, int color);

/* ===================== MINIMAP ===================== */

bool	set_minimap_img(t_data *data);
bool	draw_minimap(t_data *data);

/* ===================== PLAYER ===================== */

int		player_init(t_player *player, t_data *data);
void	clear_screen(t_data *data);
void	move_player(t_data *data);
void	mouse_look(t_data *data);
void	arrow_look(t_data *data);
void	door(t_data *data, t_player *player);
void	rotate_camera(t_player *p, double rot);
/* ===================== RAYCASTING ===================== */

void	raycast_and_draw(t_data *data, t_player *player);

/* ray init */
void	init_ray(t_ray *ray, t_player *player, int x, int width);
void	calculate_step_and_side_dist(t_ray *ray, t_player *player);

/* ray calc */
void	dda_loop(t_ray *ray, t_data *data);
void	calculate_perp_wall_dist(t_ray *ray, t_player *player);

#endif
