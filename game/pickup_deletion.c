/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickup_deletion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuarez- <msuarez-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 18:15:41 by msuarez-          #+#    #+#             */
/*   Updated: 2021/02/01 18:25:39 by msuarez-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom-nukem.h"

t_pickup		*delete_first(t_doom *doom)
{
	t_pickup	*head;
	t_pickup	*temp;

	head = doom->mdl->pickup_first;
	if (head == NULL)
		return (NULL);
	temp = head;
	head = head->next;
	free(temp);
	doom->mdl->pickup_first = head;
	return (head);
}

t_pickup		*delete_node(t_pickup *cur, t_pickup *prev, t_pickup *pickup)
{
	while (cur != NULL && cur->id != pickup->id)
	{
		prev = cur;
		cur = cur->next;
	}
	prev->next = cur->next;
	free(cur);
	pickup = prev;
	return (pickup);
}
