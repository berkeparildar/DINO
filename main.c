#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <time.h>

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 1280;
const int ANIMATION_DELAY = 100;
const float SCROLL_SPEED = 600.0f;
const float JUMP_HEIGHT = 200.0f;
const float JUMP_DURATION = 0.6f;
const float DINO_Y = 360.0f;
const int MAX_CACTUS = 3;  
const int MIN_CACTUS_GAP = 450;

int RandomNumber(int min, int max){
    int random = (rand() % (max - min + 1) + min);
    return random;
}
struct Cactus {
    float posX;
    int posY;
    int width;
    int height;
    SDL_Texture* texture;
};

int main(int argc, char *argv[])
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window* window = SDL_CreateWindow("DINO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    SDL_Surface* idleSurface = IMG_Load("assets\\dino.png");
    SDL_Texture* idleTexture = SDL_CreateTextureFromSurface(renderer, idleSurface);
    SDL_FreeSurface(idleSurface);
    
    SDL_Surface* runningSurface1 = IMG_Load("assets\\dino2.png");
    SDL_Texture* runningTexture1 = SDL_CreateTextureFromSurface(renderer, runningSurface1);
    SDL_FreeSurface(runningSurface1);

    SDL_Surface* runningSurface2 = IMG_Load("assets\\dino3.png");
    SDL_Texture* runningTexture2 = SDL_CreateTextureFromSurface(renderer, runningSurface2);
    SDL_FreeSurface(runningSurface2);

    SDL_Surface* deadSurface = IMG_Load("assets\\dino4.png");
    SDL_Texture* deadTexture = SDL_CreateTextureFromSurface(renderer, deadSurface);
    SDL_FreeSurface(deadSurface);

    SDL_Surface* groundSurface = IMG_Load("assets\\ground.png");
    SDL_Texture* groundTexture = SDL_CreateTextureFromSurface(renderer, groundSurface);
    SDL_FreeSurface(groundSurface);

    SDL_Surface* cactusSurface = IMG_Load("assets\\cactus.png");
    SDL_Texture* cactusTexture = SDL_CreateTextureFromSurface(renderer, cactusSurface);
    SDL_FreeSurface(cactusSurface);

    SDL_Surface* cactusTwoSurface = IMG_Load("assets\\cactus2.png");
    SDL_Texture* cactusTwoTexture = SDL_CreateTextureFromSurface(renderer, cactusTwoSurface);
    SDL_FreeSurface(cactusTwoSurface);

    SDL_Surface* cactusThreeSurface = IMG_Load("assets\\cactus3.png");
    SDL_Texture* cactusThreeTexture = SDL_CreateTextureFromSurface(renderer, cactusThreeSurface);
    SDL_FreeSurface(cactusThreeSurface);

    SDL_Surface* gameOverSurface = IMG_Load("assets\\gameover.png");
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);

    SDL_Surface* restartSurface = IMG_Load("assets\\restart.png");
    SDL_Texture* restartTexture = SDL_CreateTextureFromSurface(renderer, restartSurface);
    SDL_FreeSurface(restartSurface);

    SDL_Surface* cloudSurface = IMG_Load("assets\\cloud.png");
    SDL_Texture* cloudTexture = SDL_CreateTextureFromSurface(renderer, cloudSurface);
    SDL_FreeSurface(cloudSurface);

    TTF_Font* font = TTF_OpenFont("assets\\PressStart2P.ttf", 24);
    SDL_Color textColor = {83, 83, 83, 255};
    float score = 0.0f;

    int dinoWidth, dinoHeight;
    SDL_QueryTexture(idleTexture, NULL, NULL, &dinoWidth, &dinoHeight);
    int dinoX = 128;
    float dinoY = DINO_Y;

    int groundWidth = WINDOW_WIDTH;
    int groundHeight;
    SDL_QueryTexture(groundTexture, NULL, NULL, &groundWidth, &groundHeight);
    int groundX = 0;
    int groundY = WINDOW_HEIGHT / 2 + 60;
    float groundOffset = 0.0f;

    int cactusWidth, cactusHeight;
    SDL_QueryTexture(cactusTexture, NULL, NULL, &cactusWidth, &cactusHeight);
    int cactusY = 365; 

    int cactusTwoWidth = 83;
    int cactusTwoHeight = 80;
    SDL_QueryTexture(cactusTwoTexture, NULL, NULL, &cactusTwoWidth, &cactusTwoHeight);
    int cactusTwoY = 345;

    int cactusThreeWidth = 102;
    int cactusThreeHeight = 70;
    SDL_QueryTexture(cactusThreeTexture, NULL, NULL, &cactusThreeWidth, &cactusThreeHeight);
    
    int overWidth, overHeight;
    SDL_QueryTexture(gameOverTexture, NULL, NULL, &overWidth, &overHeight);

    int restartWidth, restartHeight;
    SDL_QueryTexture(restartTexture, NULL, NULL, &restartWidth, &restartHeight);

    int cloudWidth, cloudHeight;
    SDL_QueryTexture(cloudTexture, NULL, NULL, &cloudWidth, &cloudHeight);
    int cloudY = 50; 
    int cloudX = RandomNumber(200, 1280);
    float cloudOffset = 0.0f;
    
    int isRunning = 0;
    int isDead = 0;
    SDL_Texture* currentTexture = idleTexture;
    SDL_Texture* runningTextures[] = {runningTexture1, runningTexture2};
    SDL_Texture* cactusTextures[] = {cactusTexture, cactusTwoTexture, cactusThreeTexture};
    int runningCount = 2;
    int animFrame = 0;
    Uint32 lastAnimationTime = 0;
    Uint32 prevTicks = 0;
    int totalGap = (MAX_CACTUS - 1) * MIN_CACTUS_GAP;
    int availableWidth = WINDOW_WIDTH - (MAX_CACTUS * cactusWidth) - totalGap;

    struct Cactus cacti[MAX_CACTUS];
    for (int i = 0; i < MAX_CACTUS; i++) {
        cacti[i].posX = WINDOW_WIDTH + (i * (cactusWidth + MIN_CACTUS_GAP) + RandomNumber(0, availableWidth));
        int random = RandomNumber(0, 2);
        if (random == 0) {
            cacti[i].width = cactusWidth;
            cacti[i].height = cactusHeight;
            cacti[i].posY = cactusY;
        }
        else if (random == 1) {
            cacti[i].width = cactusTwoWidth;
            cacti[i].height = cactusTwoHeight;
            cacti[i].posY = cactusTwoY;
        }
        else if (random == 2) {
            cacti[i].width = cactusThreeWidth;
            cacti[i].height = cactusThreeHeight;
            cacti[i].posY = cactusY;
        }
        cacti[i].texture = cactusTextures[random];
    }

    int isJumping = 0;
    float jumpTime = 0.0f;
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
                else if (event.key.keysym.sym == SDLK_SPACE) {
                    if (!isJumping) {
                        isJumping = 1;
                    }
                }
                else if (event.key.keysym.sym == SDLK_r && isDead) {
                    srand(time(NULL));
                    currentTexture = idleTexture;
                    score = 0.0f;
                    dinoY = DINO_Y;
                    cloudX = 1280;
                    groundOffset = 0.0f;
                    isRunning = 0;
                    isJumping = 0;
                    isDead = 0;
                    jumpTime = 0.0f;
                    for (int i = 0; i < MAX_CACTUS; i++) {
                        cacti[i].posX = WINDOW_WIDTH + (i * RandomNumber(300, 1000));
                        int random = RandomNumber(0, 2);
                        if (random == 0) {
                            cacti[i].width = cactusWidth;
                            cacti[i].height = cactusHeight;
                            cacti[i].posY = cactusY;
                        }
                        else if (random == 1) {
                            cacti[i].width = cactusTwoWidth;
                            cacti[i].height = cactusTwoHeight;
                            cacti[i].posY = cactusTwoY;
                        }
                        else if (random == 2) {
                            cacti[i].width = cactusThreeWidth;
                            cacti[i].height = cactusThreeHeight;
                            cacti[i].posY = cactusY;
                        }
                        cacti[i].texture = cactusTextures[random];                 
                    }
                }
            }
        }

        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - prevTicks) * 0.001f;
        prevTicks = currentTicks;

        if (!isDead && isRunning && currentTicks - lastAnimationTime >= ANIMATION_DELAY) {
            animFrame = (animFrame + 1) % runningCount;
            currentTexture = runningTextures[animFrame];
            lastAnimationTime = currentTicks;
            score += 0.25f;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_Rect dinoRect = {dinoX, dinoY, dinoWidth, dinoHeight};
        SDL_RenderCopy(renderer, currentTexture, NULL, &dinoRect);


        if (isJumping) {
            currentTexture = idleTexture;
            jumpTime += deltaTime;
            float jumpProgress = jumpTime / JUMP_DURATION;
            float jumpOffset = sin(jumpProgress * M_PI) * JUMP_HEIGHT;
            dinoY = ((float)WINDOW_HEIGHT / 2 - 20) - jumpOffset;
            if (jumpTime >= JUMP_DURATION) {
                dinoY = DINO_Y;
                jumpTime = 0.0f;
                isJumping = 0;
            }
        }
       
        if (isRunning) {
            if (!isDead) {
                groundOffset += SCROLL_SPEED * deltaTime;
                cloudOffset += SCROLL_SPEED / 4 * deltaTime;
            }
            if (groundOffset >= groundWidth) {
                groundOffset = 0.0f;
            }
            if (cloudX - cloudOffset <= -cloudWidth) {
                cloudOffset = 0.0f;
                cloudX = RandomNumber(1280, 2000);
            }

            for (int i = 0; i < MAX_CACTUS; i++) {
                if (!isDead) {
                    cacti[i].posX -= SCROLL_SPEED * deltaTime;
                }
                if (cacti[i].posX <= -cactusWidth) {
                    int previousIndex = (i == 0) ? MAX_CACTUS - 1 : i - 1;
                    cacti[i].posX = cacti[previousIndex].posX + cactusWidth + MIN_CACTUS_GAP + RandomNumber(0, availableWidth);
                    int random = RandomNumber(0, 2);
                    if (random == 0) {
                        cacti[i].width = cactusWidth;
                        cacti[i].height = cactusHeight;
                        cacti[i].posY = cactusY;
                    }
                    else if (random == 1) {
                        cacti[i].width = cactusTwoWidth;
                        cacti[i].height = cactusTwoHeight;
                        cacti[i].posY = cactusTwoY;
                    }
                    else if (random == 2) {
                        cacti[i].width = cactusThreeWidth;
                        cacti[i].height = cactusThreeHeight;
                        cacti[i].posY = cactusY;
                    }
                    cacti[i].texture = cactusTextures[random];
                }
                SDL_Rect cactusRect = {cacti[i].posX, cacti[i].posY, cacti[i].width, cacti[i].height};
                if (SDL_HasIntersection(&dinoRect, &cactusRect)) {
                    currentTexture = deadTexture;
                    isDead = 1;
                }
                SDL_RenderCopy(renderer, cacti[i].texture, NULL, &cactusRect);
            }

        }    

        char scoreText[20];
        sprintf(scoreText, "Score: %d", (int)score);
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText, textColor);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_FreeSurface(scoreSurface);
        SDL_Rect scoreRect = {10, 10, 0, 0};
        SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreRect.w, &scoreRect.h);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    
        SDL_Rect groundSrcRect = {groundX - groundOffset, groundY, WINDOW_WIDTH, groundHeight};
        SDL_Rect groundDestRect = {WINDOW_WIDTH - groundOffset, groundY, WINDOW_WIDTH, groundHeight};
        SDL_RenderCopy(renderer, groundTexture, NULL, &groundSrcRect);
        SDL_RenderCopy(renderer, groundTexture, NULL, &groundDestRect);

        SDL_Rect cloudRect = {cloudX - cloudOffset, cloudY, cloudWidth, cloudHeight};
        SDL_RenderCopy(renderer, cloudTexture, NULL, &cloudRect);

        if (isDead) {
            SDL_Rect overRect = {WINDOW_WIDTH / 2 - overWidth / 2, WINDOW_HEIGHT / 2 - overHeight, overWidth, overHeight};
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &overRect);
            SDL_Rect restartRect = {WINDOW_WIDTH / 2  - restartWidth / 2, WINDOW_HEIGHT / 2 + 100, restartWidth, restartHeight};
            SDL_RenderCopy(renderer, restartTexture, NULL, &restartRect);
        }

        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(scoreTexture);
    }

    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(runningTexture1);
    SDL_DestroyTexture(runningTexture2);
    SDL_DestroyTexture(groundTexture);
    SDL_DestroyTexture(cactusTexture);
    SDL_DestroyTexture(cactusTwoTexture);
    SDL_DestroyTexture(cactusThreeTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(restartTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
