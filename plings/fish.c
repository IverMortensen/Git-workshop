#include "pling.h"

/* 1 = høyre, -1 = venstre */
static int fish_direction = 1;

void fish_init(unsigned int id)
{
    (void)id;
    fish_direction = 1;
}

enum PATCHLING_ACTION fish_update(unsigned int id, unsigned int boardstate[])
{
    unsigned int i;
    unsigned int column;

    /* Finn fisken på brettet (8 x 8 ruter). */
    for (i = 0; i < 64; i++)
    {
        if (boardstate[i] == id)
        {
            column = i % 8;

            if (column == 7)
            {
                fish_direction = -1;
            }
            else if (column == 0)
            {
                fish_direction = 1;
            }

            /* Angrip andre patchlings foran fisken. */
            if (fish_direction == 1 && column < 7)
            {
                if (boardstate[i + 1] != 0)
                {
                    return PATCHLING_ACTION_ATTACK_RIGHT;
                }
                return PATCHLING_ACTION_MOVE_RIGHT;
            }

            if (fish_direction == -1 && column > 0)
            {
                if (boardstate[i - 1] != 0)
                {
                    return PATCHLING_ACTION_ATTACK_LEFT;
                }
                return PATCHLING_ACTION_MOVE_LEFT;
            }

            return PATCHLING_ACTION_NONE;
        }
    }

    return PATCHLING_ACTION_NONE;
}

struct patchling *fish_create(void)
{
    struct patchling *p;

    p = create_patchling();
    p->init = fish_init;
    p->update = fish_update;
    p->sprite_idx = 15;

    SDL_snprintf(p->name, 256, "fish");

    return p;
}
