#include "pling.h"

void flower_init(unsigned int id)
{
    (void)id;
}

enum PATCHLING_ACTION flower_update(unsigned int id, unsigned int boardstate[])
{
    (void)id;
    (void)boardstate;
    return PATCHLING_ACTION_NONE;
}

struct patchling *flower_create()
{
    struct patchling *p;

    p = create_patchling();
    p->init = flower_init;
    p->update = flower_update;
    p->sprite_idx = 0;
    
    SDL_snprintf(p->name, 256, "flower");

    return p;
}
