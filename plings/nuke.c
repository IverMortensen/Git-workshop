#include "pling.h"


void nuke_init(unsigned int id)
{
    (void)id;
}

enum PATCHLING_ACTION nuke_update(unsigned int id, unsigned int boardstate[])
{
    int i;
    int j;
    
    int my_r;
    int my_c;

    int target_r;
    int target_c;

    int delta_r;
    int delta_c;

    int valid;

    struct patchling *p;

    valid = 0;

    for (i = 0; i < 64; i++)
    {
        if (boardstate[i] == id)
        {
            valid++;
            break;
        }
    }

    for (j = 0; j < 64; j++)
    {
        p = get_patchling(boardstate[j]);
        if (!p)
        {
            continue;
        }

        if (SDL_strcmp("bee", p->name) == 0)
        {
            valid++;
            break;
        }
    }

    if (valid < 2)
    {
        return PATCHLING_ACTION_NONE;
    }

    my_r = i / 8;
    my_c = i % 8;

    target_r = j / 8;
    target_c = j % 8;

    delta_r = target_r - my_r;
    delta_c = target_c - my_c;

    if ((delta_r * delta_r) + (delta_c * delta_c) == 1)
    {
        /* neighbour, we can attack */
        if (delta_r)
        {
            return NUKE;
        }
    }

    return PATCHLING_ACTION_NONE;
}

struct patchling *nuke_create()
{
    struct patchling *p;

    p = create_patchling();
    p->init = nuke_init;
    p->update = nuke_update;
    p->sprite_idx = 11;
    
    SDL_snprintf(p->name, 256, "nuke");

    return p;
}
