#include "doom-nukem.h"

static int	sprite_amounts(int category)
{
	if (category == FRONT_ATTACK)
		return (1);
}

int			load_sprites(t_doom *doom)
{
	if (doom->sprites == NULL)
		doom->sprites = (t_sprites*)malloc(sizeof(t_sprites));
	else
		ft_putendl("Warning: Doom->sprites was not NULL when load_sprites");
	if (doom->sprites == NULL)
		ft_putendl("Warning: Doom->sprites was not malloced at load_sprites");
	doom->sprites->txtRangedFrontAttack = load_texture(doom, "img/sprites/ranged/front/attack_1.png");

	doom->sprites->txtFrontAttackSprites = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * sprite_amounts(FRONT_ATTACK));
	doom->sprites->txtFrontAttackSprites[0] = doom->sprites->txtRangedFrontAttack;
	return (1);
}

int			destroy_sprites(t_doom *doom)
{
	int i;

	SDL_DestroySurface(doom->sprites->txtRangedFrontAttack);
	i = sprite_amounts(FRONT_ATTACK);
	while (i)
		doom->sprites->txtFrontAttackSprites[i--] = NULL;
	free(doom->sprites->txtFrontAttackSprites);
	doom->sprites = NULL;
	return (1);
}


