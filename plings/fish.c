#include "pling.h"

void fish_init(unsigned int id)
{
    (void)id;
}

enum PATCHLING_ACTION fish_update(unsigned int id,unsigned int boardstate[]){
    static int direction = 1;
    unsigned int i;

    for (i = 56; i < 64; i++)
    {
        if (boardstate[i] == id)
        {
            if (i == 63)
            {
                direction = -1;
            }
            else if (i == 56)
            {
                direction = 1;
            }

            if (direction == 1)
            {
                return PATCHLING_ACTION_MOVE_RIGHT;
            }
            else
            {
                return PATCHLING_ACTION_MOVE_LEFT;
            }
        }
    }

    return PATCHLING_ACTION_NONE;
}

struct patchling *fish_create()
{
    struct patchling *p;

    p = create_patchling();
    p->init = fish_init;
    p->update = fish_update;
    p->sprite_idx = 15;

    SDL_snprintf(p->name, 256, "fish");

    return p;
}
