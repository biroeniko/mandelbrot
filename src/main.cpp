/* Copyright (C) 2016 Biro Eniko - All Rights Reserved
 * You may use, distribute and modify this code under the
 * terms of the license.
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE.txt', which is part of this source code package.
 */

#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

// x,y,w,h
SDL_Rect windowRect = { 0, 0, 800, 600 };
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;

Uint32 *myPixels;

const int iterMax = 128;

SDL_Color pal[iterMax + 1];

bool InitializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        return false;

    window = SDL_CreateWindow("Fractal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowRect.w, windowRect.h, 0);
    if (!window)
        return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        return false;

    SDL_RenderSetLogicalSize(renderer, windowRect.w, windowRect.h);
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    return true;
}

void Quit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void CalculatePixel(int x, int y, double midX, double midY, double alt)
{
    // based on the [-1;1] interval
    double lat = windowRect.w * alt / windowRect.h;

    double cR = (x * (alt / windowRect.h)) + (midX - lat / 2.0);
    double cI = (y * (alt / windowRect.h)) + (midY - alt / 2.0);

    double zR = 0.0;
    double zI = 0.0;

    int iterNum = 0;
    int r = 0;
    int g = 64;
    int b = 128;
    while (((zR*zR + zI*zI) < 2 * 2) && iterNum < iterMax)
    {
        double new_zx = zR*zR - zI*zI + cR;
        zI = 2 * zR*zI + cI;
        zR = new_zx;
        iterNum++;
    }

    /* black and white
    if (iterNum == iterMax)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    */

    /*
    if (iterNum == iterMax)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    else
    SDL_SetRenderDrawColor(renderer, pal[iterNum].r, pal[iterNum].g, pal[iterNum].b, 255);

    SDL_RenderDrawPoint(renderer, x, y);
    */

    if (iterNum == iterMax)
        myPixels[y*windowRect.w + x] = 0;
    else
        myPixels[y*windowRect.w + x] = (pal[iterNum].r << 16) | (pal[iterNum].g << 8) | (pal[iterNum].b);
}

void RenderImage(double midX, double midY, double alt)
{
#pragma omp parallel for
    for (int y = 0; y < windowRect.h; y++)
    {
        for (int x = 0; x < windowRect.w; x++)
        {
            CalculatePixel(x, y, midX, midY, alt);
        }
    }
    SDL_UpdateTexture(texture, NULL, myPixels, windowRect.w * sizeof(Uint32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// very basic palette
/*
void fillPalette()
{
    int base = 40;
    for (int i = 0; i < iterMax; i++)
    {
        pal[i].r = (base + i) / 3;
        pal[i].g = (base + i) / 2;
        pal[i].b = base + i;
    }
}
*/

// palette inspiration from: http://www.gamedev.net/topic/344763-better-gradients-resolved/
void fillPalette()
{
    int startR = 10;
    int startG = 36;
    int startB = 106;

    int endR = 166;
    int endG = 202;
    int endB = 240;

    for (int i = 0; i < iterMax; i++)
    {
        float t = ((float)(i - 3)) / ((float)(windowRect.w / 8 - 6));
        pal[i].r = ((float)startR)*(1.0f - t) + ((float)endR)*t;
        pal[i].g = ((float)startG)*(1.0f - t) + ((float)endG)*t;
        pal[i].b = ((float)startB)*(1.0f - t) + ((float)endB)*t;
    }
}



int main(int argc, char* argv[])
{
    InitializeSDL();

    double midX = -0.5;
    double midY = 0;
    double alt = 2;

    texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        windowRect.w, windowRect.h);

    myPixels = new Uint32[windowRect.w*windowRect.h];

    fillPalette();
    RenderImage(midX, midY, alt);

    bool loop = true;
    int scale = 10;				// pixel number
    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                loop = false;
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_DOWN:
                    midY += alt / windowRect.h * scale;
                    break;
                case SDLK_UP:
                    midY -= alt / windowRect.h * scale;
                    break;
                case SDLK_RIGHT:
                    midX += alt / windowRect.h * scale;
                    break;
                case SDLK_LEFT:
                    midX -= alt / windowRect.h * scale;
                    break;
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                if (event.wheel.y > 0)
                    alt /= 1.01;
                else
                    alt *= 1.01;
            }
        }
        RenderImage(midX, midY, alt);
    }

    Quit();
    return 0;
}
