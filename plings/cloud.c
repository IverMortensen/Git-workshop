#include "pling.h"

static int dir = 0;

void cloud_init(unsigned int id)
{
    (void)id;
}

enum PATCHLING_ACTION cloud_update(unsigned int id, unsigned int boardstate[])
{

    int i;

    for (i = 0; i < 64; i++)
    {
        if (boardstate[i] == id)
        {
            break;
        }
    }

    if (i > 7)
    {
        return PATCHLING_ACTION_MOVE_UP;
    }


    if (i == 0){
        dir = 0;
    }

    
    if (i == 7){
        dir = 1;
    }    

    return dir % 2 ? PATCHLING_ACTION_MOVE_LEFT : PATCHLING_ACTION_MOVE_RIGHT;

}

struct patchling *cloud_create()
{
    struct patchling *p;

    p = create_patchling();
    p->init = cloud_init;
    p->update = cloud_update;
    p->sprite_idx = 9;

    SDL_snprintf(p->name, 256, "cloud");

    return p;
}
