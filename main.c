#include "SDL_events.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;


int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL initialization failed.");
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("DINO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Failed to create SDL window");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create renderer.");
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = IMG_Load("assets\\dino.png");
    if (surface == NULL) {
        printf("Failed to load image.");
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int dinoWidth, dinoHeight;
    SDL_QueryTexture(texture, NULL, NULL, &dinoWidth, &dinoHeight);
    int dinoX = 128;
    printf("%d", dinoX);
    int dinoY = (WINDOW_HEIGHT / 2 )- 20;

    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dinoRect = {dinoX, dinoY, dinoWidth, dinoHeight};
        SDL_RenderCopy(renderer, texture, NULL, &dinoRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
