#include "cub_3d.h"

///////////////////////////////////////

int	lines(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

int		is_end_window(t_info *m, double x, double y)
{
	if (x < 0 || x >= (WALL_SIZE * WALL_SIZE) ||
		y < 0 || y >= lines(m->map) * WALL_SIZE)
		return (TRUE);
	return (FALSE);
}

int		is_wall(t_info *m, int x, int y, char identf)
{
	int	grid_x;
	int	grid_y;

	grid_x = (int)floor(x / WALL_SIZE);
	grid_y = (int)floor(y / WALL_SIZE);
	if (is_end_window(m, x, y))
	{
		return (TRUE);
	}
	// if (grid_x >= 63 || grid_y >= lines(m->map))
	// 	return (TRUE);
	if (m->map[grid_y][grid_x] == identf)
	{
		return (TRUE);
	}
	return (FALSE);
}

int			ray_facing(float angle, int way)
{
	int	up;
	int	down;
	int	left;
	int	right;

	down = (angle > 0 && angle < PI) ? TRUE : FALSE;
	up = !(down) ? TRUE : FALSE;
	left = (angle > PI / 2 && angle < 3 * PI / 2) ? TRUE : FALSE;
	right = !left ? TRUE : FALSE;
	if (way == ray_up)
		return (up);
	else if (way == ray_down)
		return (down);
	else if (way == ray_left)
		return (left);
	else if (way == ray_right)
		return (right);
	return (-1);
}

void drawDDA(int xA,int yA,int xB,int yB,t_info *game)
{

	int dx = xB - xA;
	int dy = yB - yA;

	float step = fmaxf(abs(dx),abs(dy));
	float xinc = dx/step;
	float yinc = dy/step;

	float x = xA,y = yA;

	while (step >= 0)
	{
		// if(game->map[(int)y/16][(int)x/16] == '1')
		// 	break;
		// puts ("yoooo");
		mlx_pixel_put(game->ml,game->window,round(x),round(y),0xff3300);
		// puts ("koooo");
		x += xinc;
		y += yinc;
		step--;
	}
}

float normaliseangle(float angle)
{
	angle = remainder(angle ,(2*PI));
	if(angle < 0)
	{
		angle = (2*PI) + angle;
	}
	return (angle);
}

float distancebetwenpoint(float x1,float y1,float x2,float y2)
{
	return(sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1)));
}

void cast_ray(float rayangel,int i,t_info *m)
{
	m->rays[i].rayAngle = rayangel;
	m->rays[i].washitvertical = FALSE;
	rayangel = normaliseangle(rayangel);
	int wallhitx;
	int wallhity;
	int rayisup;
	int rayisdown;
	int rayisleft;
	int rayisright;
	float distance;
	float ystep;
	float xstep; 
	float yintercept;
	float xintercept;
	float hhitx;
	float hhity;
	int findhorzwall_hit;
	findhorzwall_hit = FALSE;
	rayisdown = 0;
	rayisright = 0;
	wallhitx = 0;
	wallhity =0;
	distance = 0;
	if(rayangel  > 0 && rayangel < PI)
		rayisdown = 1;
	rayisup = !rayisdown;
	if(rayangel < 0.5 * PI || rayangel > 1.5 * PI)
		rayisright = 1;
	rayisleft = !rayisright;
	//drawDDA(m->pplayer->x*16,m->pplayer->y*16,m->pplayer->x*16+cos(rayangel)*30,m->pplayer->y*16+sin(rayangel)*30,m);
	////////////////////////////////////////////
	//////HORIZONTAL RAY !//////////////////////
	////////////////////////////////////////////
	
	yintercept = floor(m->py/WALL_SIZE) * WALL_SIZE;
	if(rayisdown)
		yintercept += WALL_SIZE;
	xintercept =  m->px + (yintercept - m->py) / tan(rayangel);
	 /////////////////////////////////////////
	ystep = WALL_SIZE;
	if(rayisup)
		ystep *= -1;
	xstep = WALL_SIZE/tan(rayangel);
	if(rayisleft && xstep > 0)
		xstep *= -1;
	if(rayisright && xstep < 0)
		xstep *= -1;
	float	x_chk;
	float	y_chk;

	x_chk = xintercept;
	y_chk = yintercept;

	if(rayisup)
		y_chk--;
	while (1)
	{
		if (is_wall(m, x_chk, y_chk, '1'))
		{
			hhitx = x_chk;
			hhity = y_chk;
			findhorzwall_hit = TRUE;
			//drawDDA(m->pplayer->x,m->pplayer->y,x_chk,y_chk,m);
			break ;
		}
		else
		{
			x_chk += xstep;
			y_chk += ystep;
		}
	}
	//////////////////////////////////////////////////
	////////////////VIRTICAL//////////////////////////
	//////////////////////////////////////////////////
	int findvir_wall;
	float vhitx;
	float vhity;
	
	findvir_wall = FALSE;

	
	xintercept = floor(m->px/WALL_SIZE) * WALL_SIZE;
	if(rayisright)
		xintercept += WALL_SIZE;
		
	yintercept =  m->py + (xintercept - m->px) * tan(rayangel);
	 /////////////////////////////////////////
	xstep = WALL_SIZE;
	if(rayisleft)
		xstep *= -1;
	ystep = WALL_SIZE*tan(rayangel);
	if(rayisup && ystep > 0)
		ystep *= -1;
	if(rayisdown && ystep < 0)
		ystep *= -1;
	float	vx_chk;
	float	vy_chk;

	vx_chk = xintercept;
	vy_chk = yintercept;

	if(rayisleft)
		vx_chk--;
	while (1)
	{
		if (is_wall(m, vx_chk, vy_chk, '1'))
		{
			// printf ("x >> %f, y >> %f\n", vx_chk, vy_chk);
			vhitx = vx_chk;
			vhity = vy_chk;
			findvir_wall = TRUE;
			// drawDDA(m->px,m->py,vx_chk,vy_chk,m);
			break ;
		}
		else
		{
			vx_chk += xstep;
			vy_chk += ystep;
		}
	}
	// printf ("xchck %f, xvchc %f\n", x_chk, y_chk);
	////////////call distance///////////////////////////////
	double horz_dist;
	double vert_dist;
	if(findhorzwall_hit)
		horz_dist = distancebetwenpoint(m->px,m->py,x_chk,y_chk);
	else
		horz_dist = INT_MAX;
	if(findvir_wall)
		vert_dist = distancebetwenpoint(m->px,m->py,vx_chk,vy_chk);
	else
		vert_dist = INT_MAX;
	
	if(horz_dist <= vert_dist)
	{
		wallhitx = x_chk;
		wallhity = y_chk;
	}
	else
	{
		wallhitx = vx_chk;
		wallhity = vy_chk;
	}
	///drawDDA(m->pplayer->x,m->pplayer->y,wallhitx,wallhity,m);
	m->rays[i].w_x = wallhitx;
	m->rays[i].w_y = wallhity;
	if(horz_dist < vert_dist)
		m->rays[i].distance = horz_dist;
	else
		m->rays[i].distance = vert_dist;
		m->rays[i].washitvertical = (vert_dist < horz_dist);
}

void rays(t_info *m)
{
	float ra;
	ra = m->pa - (FOV/2);
	int i;
	i = 0;
	while(i < 320)
	{
		// puts ("yoooo");
		cast_ray(ra, i, m);
		// puts ("koooo");
		// printf("x >> %d y >> %d\n", m->rays[i].w_x, m->rays[i].w_y);
		// drawDDA(m->px,m->py,m->rays[i].w_x,m->rays[i].w_y,m);
		i++;
		ra += FOV/320;
	}
}


/////////////////////////////////



void raydraw(int xA,int yA,int xB,int yB,t_info *game, int i, float npa)
{
	int dx = xB - xA;
	int dy = yB - yA;
	t_win	brr;

	float step = fmaxf(abs(dx),abs(dy));
	float xinc = dx/step;
	float yinc = dy/step;

	float x = xA,y = yA;

	while (step >= 0)
	{
		if(game->map[(int)floor(y / 16)][(int)floor(x / 16)] == '1')
		{
			game->rays[i].w_x = (int)(x);
			game->rays[i].w_y = (int)(y);
			if (sin(npa) > 0.1 | sin(npa) < -0.1)
			{
				// printf ("%f\n", sin(npa));
				game->rays[i].distance = fabs((y - game->py) / sin(npa));
			}
			else
				game->rays[i].distance = fabsf(x - game->px);
			// if (!(int)game->rays[i].distance && i)
			// 	game->rays[i].distance = game->rays[i - 1].distance;
			// printf ("%d > distance > %f x > %d px > %d y > %d py > %d npa > %f\n", i, game->rays[i].distance, (int)x, game->px, (int)y, game->py, npa);
			break ;
		}
		brr.mlx = game->ml;
		brr.mlx_win = game->window;
		// bresenham(&brr, game->px, game->py, xB, xA, 0x000000);
		mlx_pixel_put(game->ml,game->window,round(x),round(y),0x000000);
		x += xinc;
		y += yinc;
		step--;
	}
}

char	*cpy(char *dest, char *src)
{
	int		i;
	char	*tmp;

	(void)dest;
	tmp = malloc (sizeof (char) * (ft_strlen(src) + 2));
	i = -1;
	while (src[++i])
		tmp[i] = src[i];
	tmp[i] = '\0';
	return (tmp);
}

char	**cpy_2(char **str, int j)
{
	int		i;
	int		k;
	char	**ret;

	i = -1;
	while (str[++i])
		i = i + 1 - 1;
	ret = malloc (sizeof (char *) * (i + 1));
	i = j - 2;
	k = -1;
	while (str[++i])
	{
		k++;
		ret[k] = cpy(ret[k], str[i]);
	}
	ret[k + 1] = NULL;
	return (ret);
}

void	my_mlx_pixel_put(t_img *data, int i, int color)
{
	char	*dst;
	int		x;
	int		y;

	x = 0;
	y = 0;
	if (i == 1)
	{
		while (y != 240)
		{
			x = 0;
			while (x != 852)
			{
				dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
				*(unsigned int*)dst = color;
				x++;
			}
			y++;
		}
	}
	else if (i == 2)
	{
		while (y != WALL_SIZE)
		{
			x = 0;
			while (x != WALL_SIZE)
			{
				dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
				x++;
				*(unsigned int*)dst = color;
			}
			y++;
		}

	}
	else if (i == 3)
	{
		while (y != 120)
		{
			x = 0;
			while (x != 856 - 120)
			{
				dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
				x++;
				*(unsigned int*)dst = color;
			}
			y++;
		}

	}
	else if (i == 4)
	{
		while (y != 240 - 120)
		{
			x = 0;
			while (x != 856)
			{
				dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
				x++;
				*(unsigned int*)dst = color;
			}
			y++;
		}

	}
	else if (i == 5)
	{
		while (y != 240)
		{
			x = 0;
			while (x != 852)
			{
				dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
				x++;
				*(unsigned int*)dst = color;
			}
			y++;
		}

	}
}

void	rander_view(t_info *info)
{
	float	npa;
	int		i;

	i = 320;
	// npa = info->pa;
	npa = info->pa - (FOV / 2);
	while (i)
	{
		raydraw(info->px, info->py, info->px + cos(npa) * ((12 * 16) + (8 * 16)), info->py + sin(npa) * ((12 * 16) + (8 * 16)), info, 320 - i, npa);
		// printf ("%d distance >>%f\n", 320 - i, info->rays[320 - i].distance);
		npa += FOV/320;
		i--;
	}
	// printf ("the wall x >>%d\n the wall y >>%d\n", info->rays[0].w_x, info->rays[0].w_y);
	// printf ("distance >> %f\n", info->rays[0].distance);
}


void	put_wall(t_info *m, float wall_height, int i, unsigned int c)
{
	t_img	img;
	char	*dst;
	int		y;
	int		x;
	int		ix;
	int		iy;
	int		wall_h;
	// int		u;

	wall_h = (int)fabs((wall_height * 16));
	c = c+1;
	img.img = mlx_new_image(m->ml, 3, wall_h);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
	y = 0;
	if (!m->rays[i / 3].washitvertical)
		ix = (int)floor((m->rays[i / 3].w_x) % WALL_SIZE);
	else
		ix = (int)floor((m->rays[i / 3].w_y) % WALL_SIZE);
	while (y != wall_h)
	{
		iy = (y) * ((WALL_SIZE / (float)wall_h));
		x = 0;
		while (x != 3)
		{
			if ((WALL_SIZE *iy) > 120 && ix < 852 - 120)
			{
				dst = img.addr + (y * img.line_length + x * (img.bits_per_pixel / 8));
				*(unsigned int*)dst = m->buff[(WALL_SIZE *iy) + ix];
			}
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(m->ml, m->window, img.img, i, (480 / 2) - (wall_h / 2));
	mlx_destroy_image(m->ml, img.img);
}


void	rander_walls(t_info *info)
{
	int	i = 0;
	float	projection_distance;
	float	wall_height;
	unsigned int	color;
	float	dis;

	// c = 0x03bafc;
	// d = 0x0FFFF00;

	projection_distance = (852 / 2) / tan(60  / 2);
	while (i < 320)
	{
		if ((int)info->rays[i].distance)
		{
			if (info->rays[i].washitvertical)
				color = 0x0C0C0C0;
			else
				color = 0x0808080;
			dis = info->rays[i].distance * cos(info->rays[i].rayAngle - info->pa);
			wall_height = (WALL_SIZE / dis)  * projection_distance;
			// if (info->rays[i].w_x <= 32 && info->rays[i].w_y <= 32)
			put_wall(info, wall_height, i * 3, color);
		}
		i++;
	}
}

void projectewall3d(t_info *m)
{
	int i;
	i = 0;
	float raydist;
	float wallstripheigth;
	float distanceprojplan ;
	unsigned int	color;

	color = 0x03bafc;
	distanceprojplan = (1080/2)/tan(FOV/2);
	while(i < 320)
	{
		raydist = m->rays[i].distance;
		wallstripheigth = (16/raydist)*distanceprojplan;
		put_wall(m, (int)fabs((wallstripheigth)), i * 3, color);
		i++;
	}
}

void	printf_photo(t_info *info)
{
	int	i;
	int	j;
	int	k;

	i = -1;
	k = 0;
	while (++i != 64)
	{
		j = -1;
		while (++j != 64)
		{
			mlx_pixel_put(info->ml, info->window, j, i, info->buff[k]);
			k++;
		}
	}
}

int	get_color_map(t_info *info, int i, int j)
{
	if (info->map[(int)floor(i / 17)][(int)floor(j / 17)] == '0')
		return (0x0FFFFFF);
	if (info->map[(int)floor(i / 17)][(int)floor(j / 17)] == 'N')
		return (0x0FFFFFF);
	if (info->map[(int)floor(i / 17)][(int)floor(j / 17)] == '1')
		return (0x0FF00FF);
	return (0x000FFFF);
}

void	creat_mini_map(t_info *info)
{
	int			i;
	int			j;
	char		*dst;
	int			color;

	i = -1;
	info->mini_map.img = mlx_new_image(info->buff, info->map_w * 17, info->map_w * 17);
	info->mini_map.addr = mlx_get_data_addr(info->mini_map.img, &info->mini_map.bits_per_pixel, &info->mini_map.line_length, &info->mini_map.endian);
	while (++i < info->map_h * 17)
	{
		j = -1;
		while (j++ < info->map_w * 17)
		{
			dst = info->mini_map.addr + (i * info->mini_map.line_length + j * (info->mini_map.bits_per_pixel / 8));
			color = get_color_map(info, i, j);
			*(unsigned int*)dst = color;
		}
	}
}

int	get_x(t_info *info)
{
	int	x;

	// x = 852 - (60) - (int)floor((info->px / 64) * 17);
	x = (120) - (int)floor((info->px / 64) * 17);
	return (x);
}

int	get_y(t_info *info)
{
	int	y;

	y = 120 - (int)floor((info->py / 64) * 17);
	return (y);
}

int	draw(void *stru)
{
	t_info	*info;
	int		i;
	int		j;

	j = 0;
	i = -1;
	info = stru;
	mlx_clear_window(info->ml, info->window);
	update_player_cord(info);
	// printf_photo(info);
	// mlx_put_image_to_window(info->ml, info->window, info->img_u1.img, 0, 0);
	mlx_put_image_to_window(info->ml, info->window2, info->mini_map.img, get_x(info), get_y(info));
	mlx_put_image_to_window(info->ml, info->window, info->img_u1.img, 0, 0);
	mlx_put_image_to_window(info->ml, info->window, info->img_u2.img, 0, 240 - 120);
	mlx_put_image_to_window(info->ml, info->window, info->img_d.img, 0, 240);
	mlx_put_image_to_window(info->ml, info->window2, info->img3.img, 120, 120);
	// while (info->map[++i])
	// {
	// 	j = -1;
	// 	while (info->map[i][++j])
	// 	{
	// 		if (info->map[i][j] == '0' || info->map[i][j] == 'N')
	// 			mlx_put_image_to_window(info->ml, info->window, info->img1.img, j * WALL_SIZE, i * WALL_SIZE);
	// 		else if (info->map[i][j] == '1')
	// 			mlx_put_image_to_window(info->ml, info->window, info->img2.img, j * WALL_SIZE, i * WALL_SIZE);
	// 	}
	// }
	rays(info);
	// projectewall3d(info);
	// rander_view(info);
	rander_walls(info);
	return (0);
}

int	get_map_info(t_info *info, char *map)
{
	int		i;
	char	**splited;

	i = -1;
	splited = ft_split(map, '\n');
	info->map = cpy_2(splited, 7);
	return (1);
}

void	creat_imgs(t_info *info)
{
	creat_mini_map(info);
	info->img_u.img = mlx_new_image(info->ml, 856, 240);
	info->img_u1.img = mlx_new_image(info->ml, 856 - 120, 120);
	info->img_u2.img = mlx_new_image(info->ml, 856, 240 - 120);
	info->img_d.img = mlx_new_image(info->ml, 856, 240);
	info->img1.img = mlx_new_image(info->ml, WALL_SIZE, WALL_SIZE);
	info->img2.img = mlx_new_image(info->ml, WALL_SIZE, WALL_SIZE);
	info->img3.img = mlx_new_image(info->ml, 1, 1);
	info->img_d.addr = mlx_get_data_addr(info->img_d.img, &info->img_d.bits_per_pixel, &info->img_d.line_length, &info->img_d.endian);
	info->img_u.addr = mlx_get_data_addr(info->img_u.img, &info->img_u.bits_per_pixel, &info->img_u.line_length, &info->img_u.endian);
	info->img_u1.addr = mlx_get_data_addr(info->img_u1.img, &info->img_u1.bits_per_pixel, &info->img_u1.line_length, &info->img_u1.endian);
	info->img_u2.addr = mlx_get_data_addr(info->img_u2.img, &info->img_u2.bits_per_pixel, &info->img_u2.line_length, &info->img_u2.endian);
	info->img1.addr = mlx_get_data_addr(info->img1.img, &info->img1.bits_per_pixel, &info->img1.line_length, &info->img1.endian);
	info->img2.addr = mlx_get_data_addr(info->img2.img, &info->img2.bits_per_pixel, &info->img2.line_length, &info->img2.endian);
	info->img3.addr = mlx_get_data_addr(info->img3.img, &info->img3.bits_per_pixel, &info->img3.line_length, &info->img3.endian);
	my_mlx_pixel_put(&info->img_u, 5, 0x000FFFF);
	my_mlx_pixel_put(&info->img_d, 1, 0x00008000);
	my_mlx_pixel_put(&info->img_u1, 3, 0x000FFFF);
	my_mlx_pixel_put(&info->img_u2, 4, 0x000FFFF);
	my_mlx_pixel_put(&info->img1, 2, 0x0000FFFF);
	my_mlx_pixel_put(&info->img2, 2, 0x00FF0000);
	my_mlx_pixel_put(&info->img3, 0, 0x00000000);
}

void	get_player_position(t_info *info)
{
	int	i;
	int	j;

	i = -1;
	while (info->map[++i])
	{
		j = -1;
		while (info->map[i][++j])
		{
			if (info->map[i][j] == 'N')
			{
				info->px = j * WALL_SIZE;
				info->py = i * WALL_SIZE;
				return ;
			}
		}
	}
}

void	player_next_position(t_info *info, int i)
{
	int	x;
	int	y;

	if (i == 1)
	{
		x = round(info->px + (cos(info->pa)) * 3);
		y = round(info->py + (sin(info->pa)) * 3);
		if(info->map[(int)floor(y / WALL_SIZE)][(int)floor(x / WALL_SIZE)] != '1' &&
			info->map[(int)floor((y + 2) / WALL_SIZE)][(int)floor((x + 2) / WALL_SIZE)] != '1' && 
			info->map[(int)floor((y - 2) / WALL_SIZE)][(int)floor((x - 2) / WALL_SIZE)] != '1')
		{
			info->px = x;
			info->py = y;
		}
	}
	if (i == 2)
	{
		x = round(info->px + cos(info->pa) * -3);
		y = round(info->py + sin(info->pa) * -3);
		if(info->map[(int)floor(y / WALL_SIZE)][(int)floor(x / WALL_SIZE)] != '1' &&
			info->map[(int)floor((y + 2) / WALL_SIZE)][(int)floor((x + 2) / WALL_SIZE)] != '1' && 
			info->map[(int)floor((y - 2) / WALL_SIZE)][(int)floor((x - 2) / WALL_SIZE)] != '1')		{
			info->px = x;
			info->py = y;
		}
	}
	if (i == 3)
	{
		x = round(info->px + sin(info->pa) * 3);
		y = round(info->py - cos(info->pa) * 3);
		if(info->map[(int)floor(y / WALL_SIZE)][(int)floor(x / WALL_SIZE)] != '1' &&
			info->map[(int)floor((y + 2) / WALL_SIZE)][(int)floor((x + 2) / WALL_SIZE)] != '1' && 
			info->map[(int)floor((y - 2) / WALL_SIZE)][(int)floor((x - 2) / WALL_SIZE)] != '1')		{
			info->px = x;
			info->py = y;
		}
	}
	if (i == 4)
	{
		x = round(info->px - sin(info->pa) * 3);
		y = round(info->py + cos(info->pa) * 3);
		if(info->map[(int)floor(y / WALL_SIZE)][(int)floor(x / WALL_SIZE)] != '1' &&
			info->map[(int)floor((y + 2) / WALL_SIZE)][(int)floor((x + 2) / WALL_SIZE)] != '1' && 
			info->map[(int)floor((y - 2) / WALL_SIZE)][(int)floor((x - 2) / WALL_SIZE)] != '1')
		{
			info->px = x;
			info->py = y;
		}
	}
}

void	update_player_cord(t_info *info)
{
	if (info->up.k_w)
		player_next_position(info, 1);
	if (info->up.k_s)
		player_next_position(info, 2);
	if (info->up.k_a)
		player_next_position(info, 3);
	if (info->up.k_d)
		player_next_position(info, 4);
	if (info->up.k_right)
	{
		// info->pa = info->pa + (5 * (PI / 150));
		info->pa = info->pa + 0.041;
		// if (info->pa > PI * 2)
		// 	info->pa = info->pa - (2 * PI);
	}
	if (info->up.k_left)
	{
		// info->pa = info->pa - (5 * (PI / 150));
		info->pa = info->pa - 0.041;
		// if (info->pa < 0)
		// 	info->pa = info->pa + (2 * PI);
	}
}

int	key_press(int key, t_info *info)
{
	if (key == KEY_W)
		info->up.k_w = 1;
	if (key == KEY_S)
		info->up.k_s = 1;
	if (key == KEY_A)
		info->up.k_a = 1;
	if (key == KEY_D)
		info->up.k_d = 1;
	if (key == KEY_RIGHT)
		info->up.k_right = 1;
	if (key == KEY_LEFT)
		info->up.k_left = 1;
	return (0);
}

int	key_release(int key, t_info *info)
{
	if (key == KEY_W)
		info->up.k_w = 0;
	if (key == KEY_S)
		info->up.k_s = 0;
	if (key == KEY_A)
		info->up.k_a = 0;
	if (key == KEY_D)
		info->up.k_d = 0;
	if (key == KEY_RIGHT)
		info->up.k_right = 0;
	if (key == KEY_LEFT)
		info->up.k_left = 0;
	return (0);
}

void	get_window_info(t_info *info)
{
	int	i;
	int	j;
	int	k;

	k = 0;
	i = -1;
	while (info->map[++i])
	{
		j = 0;
		while (info->map[i][j])
			j++;
		if (j > k)
			k = j;
	}
	info->map_h = i;
	info->map_w = k;
	info->window_h = i * WALL_SIZE;
	info->window_w = k * WALL_SIZE;
}

unsigned int	get_color(t_img img, int x, int y)
{
	char	*ptr;
	int		pixel;

	pixel = y * img.line_length + x * 4;
	ptr = img.addr + pixel;
	return ((((unsigned char)ptr[2]) << 16) + (((unsigned char)ptr[1]) << 8) + ((unsigned char)ptr[0]));
}

void	get_texture_buff(t_info *info)
{
	int		img_width;
	int		img_height;
	int	i = -1;
	int	j;
	int	k = 0;
	t_img	img1;

	img1.img = mlx_xpm_file_to_image(info->ml, PATH, &img_width, &img_height);
	img1.addr = mlx_get_data_addr(img1.img, &img1.bits_per_pixel, &img1.line_length, &img1.endian);
	int *buff;
	buff = malloc(4 * img_height * img_width);
	while (++i < img_height)
	{
		j = 0;
		while (j < img_width)
		{
			buff[k] = get_color(img1, j, i);
			j++;
			k++;
		}
	}
	info->buff = buff;
}

void	ft_creat_window(t_info *info)
{
	info->ml = mlx_init();
	get_texture_buff(info);
	get_window_info(info);
	info->window = mlx_new_window(info->ml, 852, 480, "CUB_3D");
	info->window2 = mlx_new_window(info->ml, 240, 240, "mini_MAP :)");
	creat_imgs(info);
	get_player_position(info);
	mlx_loop_hook(info->ml, draw, info);
	mlx_hook(info->window, 2, 1L<<0, key_press, info);
	mlx_hook(info->window, 3, 1L<<1, key_release, info);
	mlx_loop(info->ml);
}

void	get_default(t_info *info)
{
	info->pa = PI / 2;
	info->up.k_a = 0;
	info->up.k_d = 0;
	info->up.k_left = 0;
	info->up.k_right = 0;
	info->up.k_s = 0;
	info->up.k_w = 0;
}

int	main(int ac, char **av)
{
	t_info	info;
	char	*map;
	int		fd;

	get_default(&info);
	if (ac != 2)
	{
		printf ("error\n");
		exit (1);
	}
	fd = open(av[1], O_RDONLY);
	read_map(&map, fd);
	if (!get_map_info(&info, map))
		return (0);
	fd = -1;
	while (info.map[++fd])
		printf ("%s\n", info.map[fd]);
	ft_creat_window(&info);
	return (0);
}