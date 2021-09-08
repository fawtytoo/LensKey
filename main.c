// lenskey

// LensKey: A LensLok cracker
// by Steve Clark (C) 2021

// based on:
// LensKey - a Lenslok Emulator
// (c) 2002-2008 Simon Owen <simon@simonowen.com>
// simonowen.com/spectrum/lenskey

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

float       decode[][16] =
{
    { 0, -81, -31,  13, -62, -41,  22, 0,  0, -22,  39, 58, -12, 29, 70, 0 },   // ACE
    { 0, -41, -30, -68, -52, -11, -20, 0,  0,  32,  60, 11,  22, 49, 71, 0 },   // Art Studio
    { 0, -41, -57, -77,  10, -28, -19, 0,  0,  43, -10, 22,  32, 77, 58, 0 },   // Elite
    { 0, -77, -28,  -4, -19, -59, -39, 0,  0,  20,  51, 10,  10, 66, 28, 0 },   // Graphic Adventure Creator
    { 0, -40, -57, -71,  14, -27, -21, 0,  0,  42, -12, 22,  27, 67, 53, 0 },   // Jewels of Darkness
    { 0, -79, -31,  -7, -22, -61, -44, 0,  0,  18,  50,  7,  67, 39, 27, 0 },   // Mooncresta
    { 0, -27, -39, -71, -6,  -17, -48, 0,  0,  51,  64,  7,  40, 17, 79, 0 },   // Price of Magik
    { 0, -82, -31, -58, -20, -42,  10, 0,  0, -10,  32, 65,  20, 44, 80, 0 },   // Tomahawk
    { 0, -20, -41, -69, -53,   6, -29, 0,  0,  -9,  64, 20,  46, 33, 81, 0 }    // TT Racer
};

char        *game[] =
{
    "ACE (Air Combat Emulator)",
    "Art Studio",
    "Elite",
    "Graphic Adventure Creator",
    "Jewels of Darkness",
    "Mooncresta",
    "Price of Magik",
    "Tomahawk",
    "TT Racer"
};

int main(int argc, char **argv)
{
    SDL_Window  *window;
    SDL_Event   event;
    SDL_Surface *image, *surface;
    SDL_Rect    rectSrc, rectDest;
    int         running = 1;

    int         i, g, h = 0;

    if (argc < 3)
        h = 1;
    else
    {
        if ((image = IMG_Load(argv[2])) == NULL)
            h = 1;

        g = atoi(argv[1]) - 1;
        if (g < 0 || g > 8)
            h = 1;
    }

    if (h == 1)
    {
        printf("LensKey: LensLok cracker by Steve Clark (C)2021\nUsage: %s GAME IMAGE\n\tGAME\t- One of the following:\n", argv[0]);
        for (i = 0; i < 9; i++)
            printf("\t\t%i. %s\n", i + 1, game[i]);

        printf("\tIMAGE\t- Image is cropped within vertical end bars,\n\t\tand from horizontal bar to bottom of vertical bar.\n");
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("LensKey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 576, 390, 0);
    surface = SDL_GetWindowSurface(window);

    rectDest.x = 0;
    rectDest.y = 195;
    rectDest.w = 576;
    rectDest.h = 195;
    SDL_BlitScaled(image, NULL, SDL_GetWindowSurface(window), &rectDest);
    SDL_FreeSurface(image);

    rectSrc.h = rectDest.h = 96;
    rectSrc.w = 1;
    rectDest.w = 8;
    rectSrc.y = 201;
    rectDest.y = 24;

    // this bit does the work
    for (i = 0; i < 16; i++)
        if (decode[g][i])
        {
            rectSrc.x = 228.0f * decode[g][i] / 100.0f + 292;
            rectDest.x = 64 * (((float)i - 8.0f) / 8.0f) + 292;
            SDL_BlitScaled(surface, &rectSrc, surface, &rectDest);
        }
    // done

    SDL_UpdateWindowSurface(window);

    while (running)
    {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;

        SDL_Delay(1);
    }

    SDL_DestroyWindow(window);

    return 0;
}

// lenskey
