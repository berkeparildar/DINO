#include "SDL_render.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
int main(int argc, char *argv[])
{

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("SDL Image Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    // Load image using SDL_image
    SDL_Surface* imageSurface = IMG_Load("dino.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    // Copy the texture onto the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Render the result
    SDL_RenderPresent(renderer);

    SDL_Delay(3000); // Wait for 3 seconds

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
    return 0;
}
