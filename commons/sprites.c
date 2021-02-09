#include "doom-nukem.h"

static char *extension_num_path(char *folder, char c, char *extension)
{
	char *path;
	char *join;

	join = ft_strnew(1);
	join[0] = c;
	path = ft_strjoin(folder, join);
	free(join);
	join = path;
	path = ft_strjoin(path, extension);
	free(join);
	return (path);
}

static int	load_ranged_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_ranged_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_ranged_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 7)))
		return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/ranged/front/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_front_walk[i - 48] = load_texture(doom, path);
		path = extension_num_path("img/sprites/ranged/side/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_left_walk[i - 48] = load_texture(doom, path);
		doom->sprites->txt_ranged_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_ranged_left_walk[i - 48]);
		path = extension_num_path("img/sprites/ranged/back/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_back_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	i = '0';
	while (i < '7')
	{
		path = extension_num_path("img/sprites/ranged/death/death_", i + 1, ".png");
		doom->sprites->txt_ranged_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_ranged_front_attack = load_texture(doom, "img/sprites/ranged/front/attack_1.png");
	doom->sprites->txt_ranged_left_attack = load_texture(doom, "img/sprites/ranged/side/attack_1.png");
	doom->sprites->txt_ranged_right_attack = flip_horizontal(doom->sprites->txt_ranged_left_attack);
	doom->sprites->txt_ranged_front_idle = load_texture(doom, "img/sprites/ranged/front/idle.png");
	doom->sprites->txt_ranged_left_idle = load_texture(doom, "img/sprites/ranged/side/idle.png");
	doom->sprites->txt_ranged_right_idle = flip_horizontal(doom->sprites->txt_ranged_left_idle);
	doom->sprites->txt_ranged_back_idle = load_texture(doom, "img/sprites/ranged/back/idle.png");
	return (1);
}

static void		copy_from_atlas(int x, int y, SDL_Surface *atlas, SDL_Surface *buff)
{
	int *pix[2];
	int i;
	int iter;
	int limit_x;
	int limit_total;
	int to;
	int from;

	iter = 0;
	i = x;
	limit_x = x + buff->w;
	limit_total = buff->w * buff->h;
	pix[0] = (int*)buff->pixels;
	pix[1] = (int*)atlas->pixels;
	while (iter < limit_total)
	{
		x = i;
		while (x < limit_x)
		{
			from = (atlas->w * y) + x;
			to = iter;
				//printf("from %d,%d to %d, %d\n",x,y,iter % 128, iter / 128);
			pix[0][to] = pix[1][from];
			x++;
			iter++;
		}
		y++;
	}
	puts("Nice!");
}

static int	load_melee_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_melee_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_left_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_right_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
		return (-1);
	if (!(doom->sprites->txt_melee_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
		return (-1);
	if (!(doom->sprites->txt_melee_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	i = '0';
	while (i < '3')
	{
		path = extension_num_path("img/sprites/melee/front/attack_", i + 1, ".png");
		doom->sprites->txt_melee_front_attack[i - 48] = load_texture(doom, path);
		path = extension_num_path("img/sprites/melee/side/attack_", i + 1, ".png");
		doom->sprites->txt_melee_left_attack[i - 48] = load_texture(doom, path);
		doom->sprites->txt_melee_right_attack[i - 48] = flip_horizontal(doom->sprites->txt_melee_left_attack[i - 48]);
		i++;
	}
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/melee/front/walk_", i + 1, ".png");
		doom->sprites->txt_melee_front_walk[i - 48] = load_texture(doom, path);
		path = extension_num_path("img/sprites/melee/side/walk_", i + 1, ".png");
		doom->sprites->txt_melee_left_walk[i - 48] = load_texture(doom, path);
		doom->sprites->txt_melee_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_melee_left_walk[i - 48]);
		path = extension_num_path("img/sprites/melee/back/walk_", i + 1, ".png");
		doom->sprites->txt_melee_back_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/melee/death/death_", i + 1, ".png");
		doom->sprites->txt_melee_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_melee_front_idle = load_texture(doom, "img/sprites/melee/front/idle.png");
	doom->sprites->txt_melee_left_idle = load_texture(doom, "img/sprites/melee/side/idle.png");
	doom->sprites->txt_melee_right_idle = flip_horizontal(doom->sprites->txt_melee_left_idle);
	doom->sprites->txt_melee_back_idle = load_texture(doom, "img/sprites/melee/back/idle.png");
	return (1);
}

static int	load_boss_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_boss_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_left_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_right_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
		return (-1);
	if (!(doom->sprites->txt_boss_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_left_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_right_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
		return (-1);
	if (!(doom->sprites->txt_boss_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 9)))
		return (-1);
	//if (!(doom->sprites->txt_boss_back_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
	//	return (-1);
	//if (!(doom->sprites->txt_boss_back_walk_atlas = load_xpm("img/sprites/boss/back/walk.xpm")))
	//	return (-1);
	//printf("Loaded back walk atlas W: %d H: %d\n", doom->sprites->txt_boss_back_walk_atlas->w, doom->sprites->txt_boss_back_walk_atlas->h);
	//i = '0';
	//doom->sprites->txt_boss_back_walk[0] = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_ARGB32);
	//copy_from_atlas(0, 0, doom->sprites->txt_boss_back_walk_atlas, doom->sprites->txt_boss_back_walk[0]);
	//while (i < '6')
	//{
	//	doom->sprites->txt_boss_back_walk[(i - 48)] = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_ARGB32);
	//	if (doom->sprites->txt_boss_back_walk[(i - 48)] == NULL)
	//		ft_die("Fatal error: Could not malloc SDL_Surface for sprite");
	//	copy_from_atlas(128 * (i - 48), 0, doom->sprites->txt_boss_back_walk_atlas, doom->sprites->txt_boss_back_walk[(i - 48)]);
	//	i++;
	//}

	i = '0';
	while (i < '2')
	{
		path = extension_num_path("img/sprites/boss/front/attack_", i + 1, ".png");
		doom->sprites->txt_boss_front_attack[i - 48] = load_texture(doom, path);
		path = extension_num_path("img/sprites/boss/side/attack_", i + 1, ".png");
		doom->sprites->txt_boss_left_attack[i - 48] = load_texture(doom, path);
		doom->sprites->txt_boss_right_attack[i - 48] = flip_horizontal(doom->sprites->txt_boss_left_attack[i - 48]);
		i++;
	}
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/boss/front/walk_", i + 1, ".png");
		doom->sprites->txt_boss_front_walk[i - 48] = load_texture(doom, path);
		path = extension_num_path("img/sprites/boss/side/walk_", i + 1, ".png");
		doom->sprites->txt_boss_left_walk[i - 48] = load_texture(doom, path);
		doom->sprites->txt_boss_right_walk[i - 48] = flip_horizontal(doom->sprites->txt_boss_left_walk[i - 48]);
		path = extension_num_path("img/sprites/boss/back/walk_", i + 1, ".png");
		doom->sprites->txt_boss_back_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	i = '0';
	while (i < '9')
	{
		path = extension_num_path("img/sprites/boss/death/death_", i + 1, ".png");
		doom->sprites->txt_boss_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_boss_front_idle = load_texture(doom, "img/sprites/boss/front/idle.png");
	doom->sprites->txt_boss_left_idle = load_texture(doom, "img/sprites/boss/side/idle.png");
	doom->sprites->txt_boss_right_idle = flip_horizontal(doom->sprites->txt_boss_left_idle);
	doom->sprites->txt_boss_back_idle = load_texture(doom, "img/sprites/boss/back/idle.png");
	return (1);
}

void		init_enemy_sprite(t_doom *doom)
{
	int		ec;
	t_enemy	*enemy;

	ec = doom->mdl->enemy_count;
	if (ec == 0)
		return ;
	enemy = doom->mdl->enemy_first;
	while (ec--)
	{
		if (enemy->ai.type_id == RANGED)
			enemy->active_sprite = doom->sprites->txt_ranged_front_idle;
		else if (enemy->ai.type_id == MELEE)
			enemy->active_sprite = doom->sprites->txt_melee_front_idle;
		else if (enemy->ai.type_id == BOSS)
			enemy->active_sprite = doom->sprites->txt_boss_front_idle;
		enemy = enemy->next;
	}
}

int			load_pickup_sprite(t_doom *doom)
{
	doom->sprites->txt_health_pickup = load_texture(doom, "img/pickups/health.png");
	doom->sprites->txt_smg_ammo_pickup = load_texture(doom, "img/pickups/smgammo.png");
	doom->sprites->txt_assault_ammo_pickup = load_texture(doom, "img/pickups/akammo.png");
	doom->sprites->txt_key_hud = load_texture(doom, "img/pickups/key.png");
	return (1);
}

void		load_health_bars(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_health_bar = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 11)))
		return ;
	i = 0;
	while (i < 10)
	{
		path = extension_num_path("img/sprites/hud/healthbar/health_", i + '0', ".png");
		doom->sprites->txt_health_bar[i] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_health_bar[10] = load_texture(doom, "img/sprites/hud/healthbar/health_10.png");
}

void		load_ammo_bars(t_doom *doom)
{
	doom->sprites->txt_pistol_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/pistolammo.png");
	doom->sprites->txt_smg_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/smgammo.png");
	doom->sprites->txt_assault_ammo_bar = load_texture(doom, "img/sprites/hud/ammobar/assaultammo.png");
	doom->sprites->txt_clip_bar = load_texture(doom, "img/sprites/hud/ammobar/cliptest.png");
}

int			load_sprites(t_doom *doom)
{
	char	i;
	char	*path;

	if (doom->sprites == NULL)
		doom->sprites = (t_sprites*)malloc(sizeof(t_sprites));
	else
		ft_putendl("Warning: Doom->sprites was not NULL when load_sprites");
	if (doom->sprites == NULL)
		ft_putendl("Warning: Doom->sprites was not malloced at load_sprites");
	load_ranged_sprite(doom, path, i);
	load_melee_sprite(doom, path, i);
	load_boss_sprite(doom, path, i);
	load_pickup_sprite(doom);
	load_health_bars(doom, path, i);
	load_ammo_bars(doom);
	return (1);
}

void			print_minimap_mult_sprite(t_doom *doom, SDL_Surface **sprite, int i)		//debugging only
{
	unsigned int	*pixels;
	unsigned int	*ref;
	int				x;
	int				y;
	int				k;
	int				cut;

	pixels = doom->minimap->buff->pixels;
	ref = sprite[i]->pixels;
	cut = 15 * MWIN_WIDTH;
	y = (MWIN_WIDTH - sprite[i]->w) + ((MWIN_HEIGHT - sprite[i]->h) * MWIN_WIDTH) - cut;
	y += cut;
	x = 0;
	k = 0;
	while (k < ((sprite[i]->w * sprite[i]->h)))
	{
		y = x == sprite[i]->w ? y + MWIN_WIDTH : y;
		x = x == sprite[i]->w ? 0 : x;
		pixels[x++ + y] = ref[k++];
	}
}

void		print_minimap_single_sprite(t_doom *doom, SDL_Surface *sprite)					//debugging only
{
	unsigned int	*pixels;
	unsigned int	*ref;
	int				x;
	int				y;
	int				k;
	int				cut;

	pixels = doom->minimap->buff->pixels;
 	ref = sprite->pixels;
	cut = 15 * MWIN_WIDTH;
	y = (MWIN_WIDTH - sprite->w) + ((MWIN_HEIGHT - sprite->h) * MWIN_WIDTH) - cut;
	y += cut;
	x = 0;
	k = 0;
	while (k < ((sprite->w * sprite->h)))
	{
		y = x == sprite->w ? y + MWIN_WIDTH : y;
		x = x == sprite->w ? 0 : x;
		pixels[x++ + y] = ref[k++];
	}
}

int			destroy_sprites(t_doom *doom)
{
	free(doom->sprites);
	doom->sprites = NULL;
	return (1);
}
