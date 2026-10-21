#include "pling.h"

struct listnode
{
    struct patchling *p;
    struct listnode *next;
};

/* -------------------------------------------------------------------------- */

static struct listnode *s_head = NULL;
static struct listnode *s_iter = NULL;
static unsigned int s_nextid = 1;

/* -------------------------------------------------------------------------- */

struct patchling *create_patchling()
{
    struct listnode *current;
    struct listnode *new_listnode;
    struct patchling *new_patchling;

    new_patchling = SDL_malloc(sizeof(struct patchling));
    memset(new_patchling, 0, sizeof(struct patchling));
    new_patchling->id = s_nextid++;
    new_patchling->sprite_idx = 256;

    new_listnode = SDL_malloc(sizeof(struct listnode));
    memset(new_listnode, 0, sizeof(struct listnode));

    new_listnode->next = NULL;
    new_listnode->p = new_patchling;

    if (!s_head)
    {
        /*
            if we dont have any patchlings we create one :)
        */
        s_head = new_listnode;
        return new_patchling;
    }

    /* otherwise we iterate to end of list before creating a new patchling */
    current = s_head;
    while (current->next)
    {
        current = current->next;
    }

    current->next = new_listnode;
    return new_patchling;
}

struct patchling *get_patchling(unsigned int id)
{
    struct listnode *current;

    if (s_head == NULL)
    {
        return NULL;
    }

    for (current = s_head; current != NULL; current = current->next)
    {
        if (current->p->id == id)
        {
            return current->p;
        }
    }
    return NULL;
}

void destroy_patchling(unsigned int id)
{
    struct listnode tmp;
    struct listnode *current;
    struct listnode *delete;

    /* voodoo */
    tmp.next = s_head;
    delete = NULL;

    for (current = &tmp; current->next; current = current->next)
    {
        if (current->next->p->id == id)
        {
            delete = current->next;
            current->next = delete->next;
            break;
        }
    }

    /* maybe we did too much voodoo */
    if (delete == s_head)
    {
        s_head = s_head->next;
    }

    if (s_iter == delete)
    {
        s_iter = s_iter->next;
    }

    if (delete)
    {
        SDL_free(delete->p);
        SDL_free(delete);
    }
}

int verify_patchling(struct patchling *p)
{
    int valid = 0;

    valid += (p->init != NULL);
    valid += (p->update != NULL);
    valid += (p->name[0] != 0);
    valid += (p->sprite_idx < 64);

    if (valid < 4)
    {
        return 0;
    }

    return 1;
}

/* -------------------------------------------------------------------------- */

void patchling_list_reset()
{
    s_iter = s_head;
}

struct patchling *patchling_list_next()
{
    struct patchling *p;

    if (!s_iter)
    {
        return NULL;
    }

    p = s_iter->p;
    s_iter = s_iter->next;
    return p;
}
