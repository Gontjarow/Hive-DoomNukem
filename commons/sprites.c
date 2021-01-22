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

static int	sprite_amounts(int category)
{
	if (category == FRONT_ATTACK)
		return (1);
}

static int	load_ranged_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_ranged_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
			return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/ranged/front/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_front_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_ranged_side_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
			return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/ranged/side/walk_", i + 1, ".png");
		doom->sprites->txt_ranged_side_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_ranged_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 7)))
			return (-1);
	i = '0';
	while (i < '7')
	{
		path = extension_num_path("img/sprites/ranged/death/death_", i + 1, ".png");
		doom->sprites->txt_ranged_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_ranged_front_attack = load_texture(doom, "img/sprites/ranged/front/attack_1.png");
	doom->sprites->txt_ranged_side_attack = load_texture(doom, "img/sprites/ranged/side/attack_1.png");
	doom->sprites->txt_ranged_front_idle = load_texture(doom, "img/sprites/ranged/front/idle.png");
	doom->sprites->txt_ranged_side_idle = load_texture(doom, "img/sprites/ranged/side/idle.png");
	return (1);
}

static int	load_melee_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_melee_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
			return (-1);
	i = '0';
	while (i < '3')
	{
		path = extension_num_path("img/sprites/melee/front/attack_", i + 1, ".png");
		doom->sprites->txt_melee_front_attack[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_melee_side_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 3)))
			return (-1);
	i = '0';
	while (i < '3')
	{
		path = extension_num_path("img/sprites/melee/side/attack_", i + 1, ".png");
		doom->sprites->txt_melee_side_attack[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_melee_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
			return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/melee/front/walk_", i + 1, ".png");
		doom->sprites->txt_melee_front_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_melee_side_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 4)))
			return (-1);
	i = '0';
	while (i < '4')
	{
		path = extension_num_path("img/sprites/melee/side/walk_", i + 1, ".png");
		doom->sprites->txt_melee_side_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_melee_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
			return (-1);
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/melee/death/death_", i + 1, ".png");
		doom->sprites->txt_melee_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_melee_front_idle = load_texture(doom, "img/sprites/melee/front/idle.png");
	doom->sprites->txt_melee_side_idle = load_texture(doom, "img/sprites/melee/side/idle.png");
	return (1);
}

static int	load_boss_sprite(t_doom *doom, char *path, int i)
{
	if (!(doom->sprites->txt_boss_front_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
			return (-1);
	i = '0';
	while (i < '2')
	{
		path = extension_num_path("img/sprites/boss/front/attack_", i + 1, ".png");
		doom->sprites->txt_boss_front_attack[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_boss_side_attack = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 2)))
			return (-1);
	i = '0';
	while (i < '2')
	{
		path = extension_num_path("img/sprites/boss/side/attack_", i + 1, ".png");
		doom->sprites->txt_boss_side_attack[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_boss_front_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
			return (-1);
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/boss/front/walk_", i + 1, ".png");
		doom->sprites->txt_boss_front_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_boss_side_walk = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 6)))
			return (-1);
	i = '0';
	while (i < '6')
	{
		path = extension_num_path("img/sprites/boss/side/walk_", i + 1, ".png");
		doom->sprites->txt_boss_side_walk[i - 48] = load_texture(doom, path);
		i++;
	}
	if (!(doom->sprites->txt_boss_death = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10)))
			return (-1);
	i = '0';
	while (i < '9')
	{
		path = extension_num_path("img/sprites/boss/death/death_", i + 1, ".png");
		doom->sprites->txt_boss_death[i - 48] = load_texture(doom, path);
		i++;
	}
	doom->sprites->txt_boss_front_idle = load_texture(doom, "img/sprites/boss/front/idle.png");
	doom->sprites->txt_boss_side_idle = load_texture(doom, "img/sprites/boss/side/idle.png");
	return (1);
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
	// doom->sprites->txt_front_attack_sprites = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * sprite_amounts(FRONT_ATTACK));
	// doom->sprites->txt_front_attack_sprites[0] = doom->sprites->txt_ranged_front_attack;
	// doom->sprites->txt_front_attack_sprites[1] = doom->sprites->txt_melee_front_attack;
	// doom->sprites->txt_front_attack_sprites[2] = doom->sprites->txt_boss_front_attack;
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
	// int i;

	// SDL_FreeSurface(doom->sprites->txt_ranged_front_attack);
	// i = sprite_amounts(FRONT_ATTACK);
	// while (i)
	// 	doom->sprites->txt_front_attack_sprites[i--] = NULL;
	// free(doom->sprites->txt_front_attack_sprites);
	free(doom->sprites);
	doom->sprites = NULL;
	return (1);
}
