/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_ai2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 16:20:42 by msuarez-          #+#    #+#             */
/*   Updated: 2021/01/15 18:56:52 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

void		ai_ranged(t_doom *doom)
{
	/*
		Set enemy shoot cd
		If aggro or distance >= 70 && distance <= 200
		- Move forward player at normal speed
		- Once distance < 70, start shooting player, dealing 5dmg per sec?
		- HP normal
	*/
	
}

void		ai_melee(t_doom *doom)
{
	/*
		Set enemy shoot cd
		If aggro or distance >= 100 && distance <= 300 (or 200)
		- Move forward player at a higher speed than other enemies
		- Once collided, start dealing 5-10 dmg per sec? Prob more dmg than ranged
		- HP lower than ranged?
		- Players should definitely avoid letting them get near to survive
	*/
}

void		ai_boss(t_doom *doom)
{
	/*
		Set enemy shoot cd
		If aggro or distance (*)
		- Move forward player at slower speed?
		- Once distance (*) or collided, start dealing dmg, lower than most but depending
		of what is its kind, we could decide a balanced dmg rate
		- HP Very big pool - gotta make the fight last longer than usual
	*/
}

void		handle_enemy_ai(t_doom *doom)
{
	enemy_update_cooldown(doom);
	rotate_enemy_towards_player(doom);
	move_enemy_towards_player(doom);
	handle_enemy_shooting(doom);
}
