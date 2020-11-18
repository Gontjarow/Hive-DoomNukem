#include "doom-nukem.h"
// Ignore the void *doom, it is singleton linked from the doom-nukem.c third line of code once,
// USAGE: singleton_doom_pointer(NULL) after that.
t_doom      *set_singleton_doom_ptr(void *doom)
{
    static t_doom *doom_ptr = NULL;
    if (!doom_ptr)
        doom_ptr = (t_doom*)doom;
    return (doom_ptr);
}

t_doom      *doom_ptr(void)
{
    static t_doom *ptr = NULL;
    if (!ptr)
        ptr = set_singleton_doom_ptr(NULL);
    return (ptr);
}

t_model     *get_model(void)
{
    static t_model *mdl_ptr = NULL;
    if (!mdl_ptr)
        mdl_ptr = doom_ptr()->mdl;
    return (mdl_ptr);
}

