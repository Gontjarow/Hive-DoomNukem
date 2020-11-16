#include "doom-nukem.h"
// Ignore the void *doom, it is singleton linked from the doom-nukem.c third line of code once,
// USAGE: singleton_doom_pointer(NULL) after that.
t_doom      *singleton_doom_pointer(void *doom)
{
    static t_doom *doom_ptr = NULL;
    if (!doom_ptr)
        doom_ptr = (t_doom*)doom;
    return (doom_ptr);
}

t_model     *get_model(void)
{
    static t_model *mdl_ptr = NULL;
    if (!mdl_ptr)
        mdl_ptr = singleton_doom_pointer(NULL)->mdl;
    return (mdl_ptr);
}

