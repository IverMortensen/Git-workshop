#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>

#ifndef _PLING_H
#define _PLING_H

enum PATCHLING_ACTION
{
    PATCHLING_ACTION_NONE,
    PATCHLING_ACTION_MOVE_UP,
    PATCHLING_ACTION_MOVE_DOWN,
    PATCHLING_ACTION_MOVE_LEFT,
    PATCHLING_ACTION_MOVE_RIGHT,
    PATCHLING_ACTION_ATTACK_UP,
    PATCHLING_ACTION_ATTACK_DOWN,
    PATCHLING_ACTION_ATTACK_LEFT,
    PATCHLING_ACTION_ATTACK_RIGHT
};

struct patchling
{
    char name[256];
    unsigned int sprite_idx;
    void (*init)(unsigned int id);
    enum PATCHLING_ACTION (*update)(unsigned int id, unsigned int boardstate[]);

    /* --- treat anything below this line as "READ ONLY" (dont modify it) --- */
    unsigned int id;
    SDL_Texture *texture;
};

struct patchling *create_patchling();
struct patchling *get_patchling(unsigned int id);
void destroy_patchling(unsigned int id);
int verify_patchling(struct patchling *p);

void patchling_list_reset();
struct patchling *patchling_list_next();

#endif /* _PLING_H */
