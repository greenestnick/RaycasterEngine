#include "main.h"

//TODO: Must change screen sizes for window resize events

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

    SDL_Surface* textureSurf = IMG_Load("./wolftextures.png");
    if(textureSurf->format->format != SDL_PIXELFORMAT_ABGR8888){
        textureSurf = SDL_ConvertSurfaceFormat(textureSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    }
  
    SDL_Surface* spriteSurf = IMG_Load("./wolfsprites.png");
    if(spriteSurf->format->format != SDL_PIXELFORMAT_ABGR8888){
        spriteSurf = SDL_ConvertSurfaceFormat(spriteSurf, SDL_PIXELFORMAT_ABGR8888, 0);
    }

    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks(), lastTimeFrame = SDL_GetTicks();

    Player player = {2.5, 2.5, 0.707, 0.707};
    Mouse mouse = {0, 0};
    Uint8 keys[6] = {0,0,0,0,0,0};
    while(1){
        Uint32 msPassed = SDL_GetTicks() - lastTimeFrame; 
        lastTimeFrame = SDL_GetTicks();

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
        }else if(event.type == SDL_MOUSEMOTION){
            int xNew = 0, yNew = 0;
            SDL_GetMouseState(&xNew, NULL);
            mouse.xVel = xNew - mouse.x;
            mouse.x = xNew;

            float oldDirX = player.xDir;
            float angVel = 3.14 * mouse.xVel/(SCREEN_WIDTH/2);
            mouse.xVel = 0;
            player.xDir = cos(angVel) * player.xDir - sin(angVel) * player.yDir;
            player.yDir = sin(angVel) * oldDirX + cos(angVel) * player.yDir;

            char titleString[20];
            sprintf(titleString, "FPS: %05d", msPassed * 1000);
            SDL_SetWindowTitle(window, titleString);
        }
        if(event.window.event == SDL_WINDOWEVENT_ENTER){
            SDL_GetMouseState(&mouse.x, NULL);
        }
        

        //Update
        if(SDL_GetTicks() - lastTime >= UPDATE_TIMER_MS){
            lastTime = SDL_GetTicks();

            player.xPos += player.xDir/15 * (keys[0] - keys[2]);   
            player.yPos += player.yDir/15 * (keys[0] - keys[2]);

            player.xPos -= player.yDir/15 * (keys[3] - keys[1]);   
            player.yPos += player.xDir/15 * (keys[3] - keys[1]);
        }
        

        //Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        float xPlane = -player.yDir;
        float yPlane = player.xDir;

        //Rendering the floor and ceiling
        for(Uint16 i = 0; i < SCREEN_HEIGHT; i++){
            const float zPlayer = SCREEN_HEIGHT / 2.0;

            //Horizontal distance from player to floor.
            //As if we projected a vector from the player (through the camera plane) to the floor midpoint onto the floor. 
            int screenPitch = i - SCREEN_HEIGHT / 2.0;
            float rowDist = zPlayer / screenPitch;
            if(screenPitch == 0) continue;

            //First vector starts looking at leftmost side of the camera plane
            float xFloor = player.xPos + rowDist * (player.xDir - xPlane);
            float yFloor = player.yPos + rowDist * (player.yDir - yPlane);
            float xFloorStep = rowDist * 2*xPlane / SCREEN_WIDTH;
            float yFloorStep = rowDist * 2*yPlane / SCREEN_WIDTH;

            //Scanline across the screen
            for(Uint16 j = 0; j < SCREEN_WIDTH; j++){
                //We could have a floor/ceiling tile map using the truncated xFloor/yFloor to get custom tiles
                
                float texCol = (xFloor - (int)xFloor) * TEX_SIZE;
                float texRow = (yFloor - (int)yFloor) * TEX_SIZE;

                Uint32 floorColor = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * 7) + (textureSurf->w) * (int)texRow);
                Uint32 ceilingColor = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * 6) + (textureSurf->w) * (int)texRow);
                //Render the floor
                pixels[j + SCREEN_WIDTH * i] = floorColor;
                //render the ceiling
                pixels[j + SCREEN_WIDTH * (SCREEN_HEIGHT - i - 1)] = (ceilingColor>>1) & 0x7F7F7F7F;

                xFloor += xFloorStep;
                yFloor += yFloorStep;    
            }
        }


        //Raycast
        float zBuffer[SCREEN_WIDTH];
        for(Uint16 i = 0; i < SCREEN_WIDTH; i++){
            float camPlaneNorm = i / (SCREEN_WIDTH / 2.0) - 1; //normalize screen columns into range [-1, 1]

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
            
            float xExtend = ((xOffset < 0) ? -xOffset : xOffset) * xStep;
            float yExtend = ((yOffset < 0) ? -yOffset : yOffset) * yStep;

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
            zBuffer[i] = perpDist;
            float wallHeight = (float)SCREEN_HEIGHT / perpDist;
            int drawStart = (SCREEN_HEIGHT - wallHeight) / 2;

            float texRow = 0; 
            //If the wall is larger than the screen, we clamp the height and start the texture sampling further down and end further up
            if(wallHeight >= SCREEN_HEIGHT){
                texRow = (1 - SCREEN_HEIGHT/wallHeight) * 0.5 * TEX_SIZE;
                wallHeight = SCREEN_HEIGHT;
                drawStart = 0;
            }

            Uint16 j = drawStart;
            //for(; j < drawStart; j++) pixels[i + SCREEN_WIDTH * j] = 0x330000;
            
            //Get textureColumn
            float texCol = (steppingInX) ? (player.yPos + yFinish) : (player.xPos + xFinish);
            texCol -= (int)texCol; //only get the decimal part
            texCol *= TEX_SIZE;

            float texRowStep = (TEX_SIZE - 2*texRow) / wallHeight;
            for(; j < drawStart + wallHeight; j++){
                Uint32 color = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * map[xTile + MAPSIZE * yTile]) + (textureSurf->w) * (int)texRow);
                if((!steppingInX && yRay > 0) || steppingInX && xRay > 0) color = (color >> 1) & 0x7F7F7F7F;
                
                pixels[i + SCREEN_WIDTH * j] = color; //ADD SHADING
                texRow += texRowStep;
            }
            
            //for(; j < drawStart + wallHeight; j++) pixels[i + SCREEN_WIDTH * j] = 0x0000AA - 0x000011*steppingInX;

            //for(; j < SCREEN_HEIGHT; j++) pixels[i + SCREEN_WIDTH * j] = 0x008800;
        }



        //Render Sprites
        float xSprite = 8.5, ySprite = 3.5;

        //Get Relative dist to player
        float xDiff = xSprite - player.xPos, yDiff = ySprite - player.yPos;
        //Transform sprite world coords into camera coords (direction and plane as basis vecs)
        float denom = xPlane * player.yDir - yPlane * player.xDir;
        float xSpriteCam = (player.yDir * xDiff - player.xDir * yDiff) / denom;
        float ySpriteCam = (-yPlane * xDiff + xPlane * yDiff) / denom;
       
        //Only render if in front of the player
        if(ySpriteCam <= 0) goto exit_sprite_render;

        //get the screen width and height
        float spriteSize = (float)SCREEN_HEIGHT / ySpriteCam;
        int xScreenPos = (SCREEN_WIDTH / 2.0) * (1 + xSpriteCam/ySpriteCam); //for a 90deg FOV, the view triangle's slope is 1, compare that to the slope of the triangle formed by the player and object
        
        //render
        int xStart = (int)(xScreenPos - spriteSize/2.0);
        int xEnd = xStart + spriteSize;
        int yStart = (int)(SCREEN_HEIGHT/2.0 - spriteSize/2.0);
        int yEnd = yStart + spriteSize;
        
        float texCol = 0, texColStep = TEX_SIZE / spriteSize;
        if(xStart < 0){
            texCol = -xStart/spriteSize * TEX_SIZE;
            xStart = 0;
            xEnd += xStart;
        }else if(xStart > SCREEN_WIDTH){
            goto exit_sprite_render;
        }
        if(xEnd < 0) goto exit_sprite_render;
        else if(xEnd > SCREEN_WIDTH) xEnd = SCREEN_WIDTH;

        float texRowStart = 0, texRowStep = TEX_SIZE / spriteSize;
        if(yStart < 0){
            texRowStart = -yStart / spriteSize * TEX_SIZE;
            yStart = 0;
        }else if(yStart > SCREEN_HEIGHT) goto exit_sprite_render;
        
        if(yEnd > SCREEN_HEIGHT){
            yEnd = SCREEN_HEIGHT;
        }else if(yEnd < 0) goto exit_sprite_render;


        for(Uint16 x = (Uint16)xStart; x < xEnd; x++){
            if(zBuffer[x] < ySpriteCam){
                texCol += texColStep;
                continue;
            }

            float texRow = texRowStart;
            for(Uint16 y = (Uint16)yStart; y < yEnd; y++){
                Uint32 color = *((Uint32*)spriteSurf->pixels + (int)texCol + (TEX_SIZE*1) + (spriteSurf->w) * (int)texRow);
                texRow += texRowStep;
                
                if(color >> 24 == 0) continue;
                pixels[x + SCREEN_WIDTH * y] = color;
                
            }
            texCol += texColStep;
        }
        
        exit_sprite_render:
        SDL_UpdateTexture(screenTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    //Exit Clean Up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(screenTexture);
    SDL_FreeSurface(textureSurf);
    SDL_Quit();
    return 0;
}

