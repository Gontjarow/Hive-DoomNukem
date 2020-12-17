#include "doom-nukem.h"
/*
 * Singleton that can be set once to point at the main() functions stack reserved s_doom struct.
 * set_doom_singleton(&doom);
 * Retrieve the singletonized pointer to the main() s_doom with doom_ptr().
 */
t_doom      *set_doom_singleton(t_doom *doom)
{
    static t_doom *doom_singleton = NULL;

    if (!doom_singleton)
        doom_singleton = doom;
    return (doom_singleton);
}

t_doom      *doom_ptr(void)
{
    static t_doom *ptr = NULL;

    if (!ptr)
        ptr = set_doom_singleton(NULL);
    return (ptr);
}

/*
 * Singleton pointer that simplifies calls and returns the sub-singleton doom->mdl
 * Retrieve the pointer with get_model().
 */

t_model     *get_model(void)
{
    static t_model *mdl_ptr = NULL;
    if (!mdl_ptr)
        mdl_ptr = doom_ptr()->mdl;
    return (mdl_ptr);
}
