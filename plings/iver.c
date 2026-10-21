#include "pling.h"


void iver_init(unsigned int id)
{
    (void)id;
}

enum PATCHLING_ACTION iver_update(unsigned int id, unsigned int boardstate[])
{
    int my_p;
    int j;
    int i;

    int my_r;
    int my_c;

    int target_r;
    int target_c;

    int delta_r;
    int delta_c;

    my_p = 0;

    for (i = 0; i < 64; i++)
    {
        if (boardstate[i] == id)
        {
            my_p = i;
            continue;
        }
    }


    for (j = 0; j < 64; j++)
    {

        if (boardstate[j] == id)
        {
            continue;
        }
        if (get_patchling(boardstate[j]))
        {
            break;
        }
    }

    my_r = my_p / 8;
    my_c = my_p % 8;

    target_r = j / 8;
    target_c = j % 8;

    delta_r = target_r - my_r;
    delta_c = target_c - my_c;

    if ((delta_r * delta_r) + (delta_c * delta_c) == 1)
    {
        /* neighbour, we can attack */
        if (delta_r == 1)
        {
            return PATCHLING_ACTION_ATTACK_DOWN;
        }

        if (delta_r == -1)
        {
            return PATCHLING_ACTION_ATTACK_UP;
        }

        if (delta_c == 1)
        {
            return PATCHLING_ACTION_ATTACK_RIGHT;
        }

        if (delta_c == -1)
        {
            return PATCHLING_ACTION_ATTACK_LEFT;
        }
    }


    if (delta_r > 0)
    {
        return PATCHLING_ACTION_MOVE_DOWN;
    }
    if (delta_r < 0)
    {
        return PATCHLING_ACTION_MOVE_UP;
    }
    if (delta_c < 0)
    {
        return PATCHLING_ACTION_MOVE_LEFT;
    }
    if (delta_c > 0)
    {
        return PATCHLING_ACTION_MOVE_RIGHT;
    }

    /* this should be unreachable, but you never know */
    return PATCHLING_ACTION_NONE;
}

struct patchling *iver_create()
{
    struct patchling *p;

    p = create_patchling();
    p->init = iver_init;
    p->update = iver_update;
    p->sprite_idx = 2;

    SDL_snprintf(p->name, 256, "iver");

    return p;
}
