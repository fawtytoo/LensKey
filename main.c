// lenskey

// LensKey: A Lenslok cracker
// by Steve Clark (C) 2021

// based on:
// LensKey - a Lenslok Emulator
// (c) 2002-2008 Simon Owen <simon@simonowen.com>
// simonowen.com/spectrum/lenskey

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "common.h"

#define SAVECAT(cat)        fprintf(file, "[%s]", cat)
#define SAVEINT(key, value) fprintf(file, "\n%s = %i", key, value)

typedef struct
{
    char        *name;
    int         x, y;
    int         w, h;
    int         decode[16];
} GAME;

GAME        lenskey_Game[9] =
{
    {"ACE (Air Combat Emulator)", 0, 0, 0, 0, {0, -81, -31,  13, -62, -41,  22, 0,  0, -22,  39, 58, -12, 29, 70, 0}},
    {"Art Studio", 0, 0, 0, 0, {0, -41, -30, -68, -52, -11, -20, 0,  0,  32,  60, 11,  22, 49, 71, 0}},
    {"Elite", 0, 0, 0, 0, {0, -41, -57, -77,  10, -28, -19, 0,  0,  43, -10, 22,  32, 77, 58, 0}},
    {"Graphic Adventure Creator", 0, 0, 0, 0, {0, -77, -28,  -4, -19, -59, -39, 0,  0,  20,  51, 10,  10, 66, 28, 0}},
    {"Jewels of Darkness", 0, 0, 0, 0, {0, -40, -57, -71,  14, -27, -21, 0,  0,  42, -12, 22,  27, 67, 53, 0}},
    {"Mooncresta", 0, 0, 0, 0, {0, -79, -31,  -7, -22, -61, -44, 0,  0,  18,  50,  7,  67, 39, 27, 0}},
    {"Price of Magik", 0, 0, 0, 0, {0, -27, -39, -71, -6,  -17, -48, 0,  0,  51,  64,  7,  40, 17, 79, 0}},
    {"Tomahawk", 0, 0, 0, 0, {0, -82, -31, -58, -20, -42,  10, 0,  0, -10,  32, 65,  20, 44, 80, 0}},
    {"TT Racer", 0, 0, 0, 0, {0, -20, -41, -69, -53,   6, -29, 0,  0,  -9,  64, 20,  46, 33, 81, 0}}
};

void LoadConfig(int game)
{
    lenskey_Game[game].x = Cfg_GetInteger(lenskey_Game[game].name, "Left", 0);
    lenskey_Game[game].y = Cfg_GetInteger(lenskey_Game[game].name, "Top", 0);
    lenskey_Game[game].w = Cfg_GetInteger(lenskey_Game[game].name, "Width", 0);
    lenskey_Game[game].h = Cfg_GetInteger(lenskey_Game[game].name, "Height", 0);
}

void SaveConfig(char *config)
{
    FILE        *file;
    int         g;

    if ((file = fopen(config, "wb")) == NULL)
        return;

    for (g = 0; g < 9; g++)
    {
        SAVECAT(lenskey_Game[g].name);
        SAVEINT("Left", lenskey_Game[g].x);
        SAVEINT("Top", lenskey_Game[g].y);
        SAVEINT("Width", lenskey_Game[g].w);
        SAVEINT("Height", lenskey_Game[g].h);
        fprintf(file, "\n");
    }

    fclose(file);
}

void SurfaceRect(SDL_Surface *surface, int x, int y, int w, int h)
{
    SDL_Rect    rect = {x, y, w, h};

    SDL_FillRect(surface, &rect, 0x0);
}

int main(int argc, char **argv)
{
    char        config[4096];
    char        title[34];
    SDL_Window  *window;
    SDL_Event   event;
    SDL_Surface *image, *surface, *result;
    SDL_Rect    rectImage, rectSrc, rectDest;

    int         window_W, window_H;
    int         mouse_Move = 0;

    int         update = 0, running = 1;

    int         i, game, help = 0;

    if (argc < 3)
        help = 1;
    else
    {
        game = atoi(argv[1]) - 1;
        if (game < 0 || game > 8)
            help = 1;
        else if ((image = IMG_Load(argv[2])) == NULL)
            help = 1;
    }

    if (help)
    {
        printf("LensKey: LensLok cracker by Steve Clark (C)2021\nUsage: %s GAME IMAGE\n\tGAME\t- One of the following:\n", argv[0]);
        for (i = 0; i < 9; i++)
            printf("\t\t%i. %s\n", i + 1, lenskey_Game[i].name);

        return 1;
    }

    sprintf(config, "%s/.lenskey", getenv("HOME"));

    if (Cfg_Open(config) == 0)
    {
        for (i = 0; i < 9; i++)
            LoadConfig(i);

        Cfg_Close();
    }

    if (lenskey_Game[game].w == 0 || lenskey_Game[game].h == 0)
    {
        lenskey_Game[game].w = image->w;
        lenskey_Game[game].h = image->h;
    }

    rectImage.x = lenskey_Game[game].x;
    rectImage.y = lenskey_Game[game].y;
    rectImage.w = image->w;
    rectImage.h = image->h;
    window_W = lenskey_Game[game].w;
    window_H = lenskey_Game[game].h;

    rectSrc.h = window_H;
    rectDest.h = 64;
    rectSrc.w = 1;
    rectDest.w = 8;
    rectSrc.y = rectImage.y;
    rectDest.y = 0;

    printf("%i. %s\n", game + 1, lenskey_Game[game].name);
    sprintf(title, "%s - Lenskey", lenskey_Game[game].name);

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_W, window_H, SDL_WINDOW_RESIZABLE);
    surface = SDL_GetWindowSurface(window);
    result = SDL_CreateRGBSurface(0, 128, 64, 32, 0, 0, 0, 0);

    while (running)
    {
        while (SDL_PollEvent(&event))
            switch (event.type)
            {
              case SDL_QUIT:
                running = 0;
                break;

              case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = 0;
                }
                else if (event.key.keysym.sym == SDLK_RETURN)
                {
                    // this bit does the work
                    for (i = 0; i < 16; i++)
                        if (lenskey_Game[game].decode[i])
                        {
                            rectSrc.x = rectImage.x + (window_W / 2) + (window_W / 2) * lenskey_Game[game].decode[i] / 100;
                            rectDest.x = i * 8;
                            SDL_BlitScaled(image, &rectSrc, result, &rectDest);
                        }
                    // done
                    update = 1;
                }
                break;

              case SDL_MOUSEBUTTONDOWN:
                mouse_Move = 1;
                update = 0;
                break;

              case SDL_MOUSEBUTTONUP:
                mouse_Move = 0;
                break;

              case SDL_MOUSEMOTION:
                if (mouse_Move)
                {
                    if ((rectImage.x -= event.motion.xrel) < 0)
                        rectImage.x = 0;

                    if ((rectImage.y -= event.motion.yrel) < 0)
                        rectImage.y = 0;

                    rectSrc.y = rectImage.y;
                }
                break;

              case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    window_W = event.window.data1;
                    window_H = event.window.data2;
                    rectSrc.h = window_H;
                    update = 0;
                }
                break;
            }

        surface = SDL_GetWindowSurface(window);

        SurfaceRect(surface, 0, 0, window_W, window_H);
        SDL_BlitSurface(image, &rectImage, surface, NULL);
        if (update)
            SDL_BlitSurface(result, NULL, surface, NULL);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(1);
    }

    lenskey_Game[game].x = rectImage.x;
    lenskey_Game[game].y = rectImage.y;
    lenskey_Game[game].w = window_W;
    lenskey_Game[game].h = window_H;

    SaveConfig(config);

    SDL_FreeSurface(result);
    SDL_FreeSurface(image);
    SDL_DestroyWindow(window);

    return 0;
}

// lenskey
