#include "SDL_stdinc.h"
#include <stdint.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;
const int ANIMATION_DELAY = 100;
const float GROUND_SCROLL_SPEED = 0.8f;


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
    
    SDL_Surface* idleSurface = IMG_Load("assets\\dino.png");
    SDL_Texture* idleTexture = SDL_CreateTextureFromSurface(renderer, idleSurface);
    SDL_FreeSurface(idleSurface);
    
    SDL_Surface* runningSurface1 = IMG_Load("assets\\dino2.png");
    SDL_Texture* runningTexture1 = SDL_CreateTextureFromSurface(renderer, runningSurface1);
    SDL_FreeSurface(runningSurface1);

    SDL_Surface* runningSurface2 = IMG_Load("assets\\dino3.png");
    SDL_Texture* runningTexture2 = SDL_CreateTextureFromSurface(renderer, runningSurface2);
    SDL_FreeSurface(runningSurface2);

    SDL_Surface* groundSurface = IMG_Load("assets\\ground.png");
    SDL_Texture* groundTexture = SDL_CreateTextureFromSurface(renderer, groundSurface);
    SDL_FreeSurface(groundSurface);

    int dinoWidth, dinoHeight;
    SDL_QueryTexture(idleTexture, NULL, NULL, &dinoWidth, &dinoHeight);
    int dinoX = 128;
    printf("%d", dinoX);
    int dinoY = (WINDOW_HEIGHT / 2 )- 20;

    int groundWidth = WINDOW_WIDTH;
    int groundHeight;
    SDL_QueryTexture(groundTexture, NULL, NULL, &groundWidth, &groundHeight);
    int groundX = 0;
    int groundY = dinoY + 80;
    float groundOffset = 0.0f;

    int isRunning = 0;
    SDL_Texture* currentTexture = idleTexture;
    SDL_Texture* runningTextures[] = {runningTexture1, runningTexture2};
    int runningCount = 2;
    int animFrame = 0;
    Uint32 lastAnimationTime = 0;
    Uint32 prevTicks = 0;

    int quit = 0;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    isRunning = 1;
                }
            }
        }

        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - prevTicks) ;
        prevTicks = currentTicks;
        if (isRunning && currentTicks - lastAnimationTime >= ANIMATION_DELAY) {
            animFrame = (animFrame + 1) % runningCount;
            currentTexture = runningTextures[animFrame];
            lastAnimationTime = currentTicks;
        }

        if (isRunning) {
            groundOffset += GROUND_SCROLL_SPEED * deltaTime;
            if (groundOffset >= groundWidth) {
                groundOffset = 0.0f;
            }
        }
        

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dinoRect = {dinoX, dinoY, dinoWidth, dinoHeight};
        SDL_RenderCopy(renderer, currentTexture, NULL, &dinoRect);

        SDL_Rect groundSrcRect = {groundX - groundOffset, groundY, WINDOW_WIDTH, groundHeight};
        SDL_Rect groundDestRect = {WINDOW_WIDTH - groundOffset, groundY, WINDOW_WIDTH, groundHeight};
        SDL_RenderCopy(renderer, groundTexture, NULL, &groundSrcRect);
        SDL_RenderCopy(renderer, groundTexture, NULL, &groundDestRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(runningTexture1);
    SDL_DestroyTexture(runningTexture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
