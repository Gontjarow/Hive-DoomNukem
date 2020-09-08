#include "doom_nukem.h"

int		window_close(t_doom *p)
{
	(void)p;
	exit(0);
}

int		keyboard_press(int key, t_doom *p)
{
	(void)key;
	(void)p;
	return (1);
}
int		keyboard_release(int key, t_doom *p)
{
	(void)key;
	(void)p;
	return (1);
}

int		movement(t_doom *p)
{
	(void)p;
	return (1);
}

int		mouse_key(int key, int x, int y, t_doom *p)
{
	(void)key;
	(void)x;
	(void)y;
	(void)p;
	return (TRUE);
}

int		mouse_move(int x, int y, t_doom *p)
{
	(void)x;
	(void)y;
	(void)p;
	return (TRUE);
}
