#include "plings/pling.h"
#include "plings/flower.h"
#include "plings/bee.h"
#include "plings/cloud.h"
#include "plings/iver.h"
#include "plings/fish.h"


/* PREMAKE TEXTURES  -------------------------------------------------------- */

/*
    The spritemap is a file in the "res" folder that can hold a 8*8 grid
    of 32*32 pixels. you can edit it with ie gimp, please keep the 8*8 alignment
    when / if you make any art or art-related changes
*/

SDL_Texture *load_from_spritemap(SDL_Renderer *renderer, unsigned int index)
{
    SDL_Surface *s_load;
    SDL_Surface *s_target;
    SDL_Texture *t;
    SDL_Rect src;

    unsigned int r;
    unsigned int c;

    r = index / 8;
    c = index % 8;

    src.w = 32;
    src.h = 32;
    src.x = 32 * c;
    src.y = 32 * r;

    s_load = SDL_LoadBMP("res/spritemap.bmp");
    s_target = SDL_CreateSurface(32, 32, SDL_PIXELFORMAT_ARGB32);
    SDL_BlitSurface(s_load, &src, s_target, NULL);

    t = SDL_CreateTextureFromSurface(renderer, s_target);

    SDL_DestroySurface(s_load);
    SDL_DestroySurface(s_target);

    return t;
}

SDL_Texture *premake_checkerboard(SDL_Renderer *renderer)
{
    SDL_Surface *s;
    SDL_Texture *t;

    int x;
    int y;
    int c;
    int r;

    SDL_Color tile_color;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};

    s = SDL_CreateSurface(256, 256, SDL_PIXELFORMAT_ARGB32);

    for (x = 0; x < 256; x++)
    {
        for (y = 0; y < 256; y++)
        {
            /*
                we figure out what row and column we're on, assuming 32*32 cells.
                parities between cell and row will alter between matching and
                mismatching, sufficient to determine the color of our board.
            */

            c = x / 32;
            r = y / 32;
            tile_color = (r % 2) == (c % 2) ? white : red;
            SDL_WriteSurfacePixel(s, x, y, tile_color.r, tile_color.g, tile_color.b, tile_color.a);
        }
    }

    t = SDL_CreateTextureFromSurface(renderer, s);
    SDL_DestroySurface(s);
    return t;
}

/* STOPWATCH ---------------------------------------------------------------- */

struct stopwatch
{
    unsigned long start_ns;
    unsigned long lap_ns;
};

double stopwatch_start(struct stopwatch *sw)
{
    unsigned long ns;
    ns = SDL_GetTicksNS();

    SDL_memset(sw, 0, sizeof(struct stopwatch));

    sw->start_ns = ns;
    sw->lap_ns = sw->start_ns;

    return 0.0;
}

double stopwatch_lap(struct stopwatch *sw)
{
    unsigned long ns;
    unsigned long delta;
    ns = SDL_GetTicksNS();

    delta = ns - sw->lap_ns;
    sw->lap_ns = ns;

    return (double)delta / 1000000000.00;
}

double stopwatch_stop(struct stopwatch *sw)
{
    unsigned long ns;
    unsigned long delta;
    ns = SDL_GetTicksNS();

    delta = ns - sw->start_ns;
    return (double)delta / 1000000000.00;
}

/* MAIN --------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
    char title_buffer[1024];
    int should_run;
    unsigned int random;
    unsigned int i;
    unsigned int j;
    unsigned int r;
    unsigned int c;

    struct stopwatch sw_delta;
    struct stopwatch sw_second;
    double delta;
    unsigned long fps;

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    unsigned int boardstate[64];
    unsigned int boardstate_copy[64];
    enum PATCHLING_ACTION action;

    SDL_Texture *checkerboard;
    SDL_FRect patchling_rdst;
    SDL_FRect checkerboard_rdst = {192, 112, 256, 256};

    struct patchling *p;

    /* initialization */
    {
        SDL_srand(0xCAFEBABE); /* every good story ... */

        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();

        SDL_memset(title_buffer, 0, 1024);
        SDL_snprintf(title_buffer, 1024, "Patchlings");

        if (argc > 1 && argv[1][0] == 'f')
        {
            window = SDL_CreateWindow(title_buffer, 640, 480, SDL_WINDOW_FULLSCREEN);
        }
        else
        {
            window = SDL_CreateWindow(title_buffer, 640, 480, 0);
        }

        renderer = SDL_CreateRenderer(window, 0);

        if (argc > 1 && argv[1][0] == 'f')
        {
            SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);
            SDL_SetDefaultTextureScaleMode(renderer, SDL_SCALEMODE_NEAREST);
        }

        /* might be usefull later, trick compiler for now */
        checkerboard = premake_checkerboard(renderer);

        SDL_memset(boardstate, 0, sizeof(boardstate));

        /* create all the different patchlings */
        flower_create();
        bee_create();
        cloud_create();
        iver_create();
        fish_create();

        patchling_list_reset();
        while ((p = patchling_list_next()) != NULL)
        {
            if (!verify_patchling(p))
            {
                printf("INVALID PATCHLING %p\n", (void *)p);
                return 1;
            }

            p->init(p->id);
        }

        /* generic init for all patchlings inlcudes settig the texture */
        patchling_list_reset();
        while ((p = patchling_list_next()) != NULL)
        {
            random = SDL_rand(64);
            while (boardstate[random] != 0)
            {
                random = SDL_rand(64);
            }

            boardstate[random] = p->id;
            p->texture = load_from_spritemap(renderer, p->sprite_idx);
        }

        stopwatch_start(&sw_delta);
        stopwatch_start(&sw_second);
    }

    should_run = 1;
    while (should_run)
    {
        delta = stopwatch_lap(&sw_delta);
        /* Events */
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                should_run = 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_Q)
                {
                    should_run = 0;
                }
                if (event.key.key == SDLK_ESCAPE)
                {
                    should_run = 0;
                }
            }
        }

        /* Logical updates */
        {
            fps = (unsigned long)(1 / delta);
            SDL_memset(title_buffer, 0, 1024);
            SDL_snprintf(title_buffer, 1024, "Patchlings - FPS: %lu", fps);
            SDL_SetWindowTitle(window, title_buffer);

            /* trigger every 2 seconds */
            if (stopwatch_stop(&sw_second) > 2.0)
            {
                /* reset the timer */
                stopwatch_start(&sw_second);

                patchling_list_reset();
                while ((p = patchling_list_next()) != NULL)
                {
                    SDL_memcpy(boardstate_copy, boardstate, sizeof(boardstate));
                    action = p->update(p->id, boardstate_copy);

                    /* figure out where the patchling is */
                    for (i = 0; i < 64; i++)
                    {
                        if (boardstate[i] == p->id)
                        {
                            break;
                        }
                    }

                    r = i / 8;
                    c = i % 8;

                    switch (action)
                    {
                    case PATCHLING_ACTION_MOVE_UP:
                        if (r <= 0)
                        {
                            break;
                        }
                        r -= 1;
                        j = c + (8 * r);
                        if (boardstate[j] != 0)
                        {
                            break;
                        }

                        boardstate[j] = boardstate[i];
                        boardstate[i] = 0;
                        break;

                    case PATCHLING_ACTION_MOVE_DOWN:
                        if (r >= 7)
                        {
                            break;
                        }
                        r += 1;
                        j = c + (8 * r);
                        if (boardstate[j] != 0)
                        {
                            break;
                        }
                        boardstate[j] = boardstate[i];
                        boardstate[i] = 0;
                        break;

                    case PATCHLING_ACTION_MOVE_LEFT:
                        if (c <= 0)
                        {
                            break;
                        }
                        c -= 1;
                        j = c + (8 * r);
                        if (boardstate[j] != 0)
                        {
                            break;
                        }
                        boardstate[j] = boardstate[i];
                        boardstate[i] = 0;
                        break;

                    case PATCHLING_ACTION_MOVE_RIGHT:
                        if (c >= 7)
                        {
                            break;
                        }
                        c += 1;
                        j = c + (8 * r);
                        if (boardstate[j] != 0)
                        {
                            break;
                        }
                        boardstate[j] = boardstate[i];
                        boardstate[i] = 0;
                        break;

                    case PATCHLING_ACTION_ATTACK_UP:
                        if (r <= 0)
                        {
                            break;
                        }
                        r -= 1;
                        j = c + (8 * r);
                        if (boardstate[j] == 0)
                        {
                            break;
                        }

                        destroy_patchling(boardstate[j]);
                        boardstate[j] = 0;

                        break;

                    case PATCHLING_ACTION_ATTACK_DOWN:
                        if (r >= 7)
                        {
                            break;
                        }
                        r += 1;
                        j = c + (8 * r);
                        if (boardstate[j] == 0)
                        {
                            break;
                        }

                        destroy_patchling(boardstate[j]);
                        boardstate[j] = 0;
                        break;

                    case PATCHLING_ACTION_ATTACK_LEFT:
                        if (c <= 0)
                        {
                            break;
                        }
                        c -= 1;
                        j = c + (8 * r);
                        if (boardstate[j] == 0)
                        {
                            break;
                        }

                        destroy_patchling(boardstate[j]);
                        boardstate[j] = 0;
                        break;

                    case PATCHLING_ACTION_ATTACK_RIGHT:
                        if (c >= 7)
                        {
                            break;
                        }
                        c += 1;
                        j = c + (8 * r);
                        if (boardstate[j] == 0)
                        {
                            break;
                        }

                        destroy_patchling(boardstate[j]);
                        boardstate[j] = 0;
                        break;

                    case PATCHLING_ACTION_NONE:
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        /* Rendering */
        {
            SDL_RenderClear(renderer);
            SDL_RenderTexture(renderer, checkerboard, NULL, &checkerboard_rdst);

            for (i = 0; i < 64; i++)
            {
                if (boardstate[i] == 0)
                {
                    continue;
                }

                r = i / 8;
                c = i % 8;

                p = get_patchling(boardstate[i]);

                patchling_rdst.x = checkerboard_rdst.x + (32 * c);
                patchling_rdst.y = checkerboard_rdst.y + (32 * r);
                patchling_rdst.w = 32;
                patchling_rdst.h = 32;

                SDL_RenderTexture(renderer, p->texture, NULL, &patchling_rdst);
            }

            SDL_RenderPresent(renderer);
        }
    }

    return 0;
}
