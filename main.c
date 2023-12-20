#include "main.h"

int main(int argc, char* argv[]){

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    if(!window){
        printf("error creating window\n");
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("error creating renderer\n");
        return 1;
    }

    SDL_Texture* screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!screenTexture){
        printf("error creating texture\n");
        return 1;
    }
    Uint32 pixels[SCREEN_WIDTH*SCREEN_HEIGHT];


    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks();

    Player player = {2.5, 2.5, 0.707, 0.707};
    Uint8 keys[6] = {0,0,0,0,0,0};
    while(1){
        
        //Event Handling 
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            printf("QUITTING WINDOW\n");
            break;
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_w: keys[0] = 1; break;
                case SDLK_a: keys[1] = 1; break;
                case SDLK_s: keys[2] = 1; break;
                case SDLK_d: keys[3] = 1; break;
                case SDLK_q: keys[4] = 1; break;
                case SDLK_e: keys[5] = 1; break;
            }
        }else if(event.type == SDL_KEYUP){
            switch(event.key.keysym.sym){
                case SDLK_w: keys[0] = 0; break;
                case SDLK_a: keys[1] = 0; break;
                case SDLK_s: keys[2] = 0; break;
                case SDLK_d: keys[3] = 0; break;
                case SDLK_q: keys[4] = 0; break;
                case SDLK_e: keys[5] = 0; break;
            }
        }

        //Update
        if(SDL_GetTicks() - lastTime >= UPDATE_TIMER_MS){
            lastTime = SDL_GetTicks();

            player.xPos += player.xDir/15 * (keys[0] - keys[2]);   
            player.yPos += player.yDir/15 * (keys[0] - keys[2]);

            float oldDirX = player.xDir;
            float angVel = 0.03 * (keys[5] - keys[4]);
            player.xDir = cos(angVel) * player.xDir - sin(angVel) * player.yDir;
            player.yDir = sin(angVel) * oldDirX + cos(angVel) * player.yDir;
        }
        

        //Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Raycast
        for(Uint16 i = 0; i < SCREEN_WIDTH; i++){
            float camPlaneNorm = i / (SCREEN_WIDTH / 2.0) - 1; //normalize screen columns into range [-1, 1]
            
            float xPlane = -player.yDir;
            float yPlane = player.xDir;

            float xRay = player.xDir + xPlane * camPlaneNorm;
            float yRay = player.yDir + yPlane * camPlaneNorm;

            //==================================================================DDA Raycast
            float slope = yRay / xRay;
            if(xRay == 0) slope = MAX_SLOPE;

            float inv_slope = xRay / yRay;
            if(yRay == 0) inv_slope = MAX_SLOPE;
            

            float xStep = sqrt(1 + slope * slope); 
            float yStep = sqrt(1 + inv_slope * inv_slope);

            Uint32 xTile = (Uint32)player.xPos; 
            Uint32 yTile = (Uint32)player.yPos;

            float xOffset = xTile - player.xPos + (xRay > 0);
            float yOffset = yTile - player.yPos + (yRay > 0);
            
            float xExtend = ((xOffset<0)?-xOffset:xOffset) * xStep;
            float yExtend = ((yOffset<0)?-yOffset:yOffset) * yStep;

            float xFinish, yFinish;

            Uint8 steppingInX = 0;
            while(1){
                //Determine step direction
                steppingInX = (xExtend < yExtend);
                
                //Get next tile
                if(steppingInX)
                    xTile += (xRay > 0) - (xRay < 0);
                else
                    yTile += (yRay > 0) - (yRay < 0);

                //Check Tile
                if(map[xTile + MAPSIZE * yTile] != 0){
                    float rayLength = steppingInX * xExtend + !steppingInX *  yExtend;
                    float rayNorm = sqrt(xRay * xRay + yRay * yRay);

                    xFinish = xRay/rayNorm * rayLength;
                    yFinish = yRay/rayNorm * rayLength;
                    break;
                }

                //Take step
                xExtend += steppingInX * xStep;
                yExtend += !steppingInX * yStep;
            }

            //================================================Rendering Wall
            float perpDist = player.xDir * xFinish + player.yDir * yFinish;
            float wallHeight = (float)SCREEN_HEIGHT / perpDist;
            int drawStart = (SCREEN_HEIGHT - wallHeight) / 2;

            //Clamp Wall Height if its larger than the screen
            if(wallHeight >= SCREEN_HEIGHT){
                wallHeight = SCREEN_HEIGHT;
                drawStart = 0;
            }

            Uint16 j = 0;
            for(; j < drawStart; j++) pixels[i + SCREEN_WIDTH * j] = 0x330000;

            
            for(; j < drawStart + wallHeight; j++) pixels[i + SCREEN_WIDTH * j] = 0x0000AA - 0x000011*steppingInX;

            for(; j < SCREEN_HEIGHT; j++) pixels[i + SCREEN_WIDTH * j] = 0x008800;
        }
        

        SDL_UpdateTexture(screenTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    //Exit Clean Up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(screenTexture);
    SDL_Quit();
    return 0;
}

