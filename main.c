#include "main.h"

SDL_Surface* textureSurf;
SDL_Surface* spriteSurf;
Uint32 pixels[SCREEN_WIDTH*SCREEN_HEIGHT];
float zBuffer[SCREEN_WIDTH];


void RenderWall(const Player*const player, const RayHit*const rayhit, const WallPiece*const map){
    if(rayhit->xTile < 0 && rayhit->yTile < 0) return; //TODO: As of now we return a struct with negative tile position. Is there a better way to represent a "NULL Struct" without a null ptr
                        
    Uint8 transparencyColFlag = 0;
    float perpDist = player->xDir * rayhit->xRayLength + player->yDir * rayhit->yRayLength;
    zBuffer[rayhit->column] = perpDist;
    float wallHeight = (float)SCREEN_HEIGHT / perpDist;
                
    int drawStart = (SCREEN_HEIGHT - wallHeight) / 2 + player->pitch;
    drawStart -= wallHeight * 0;// * i; TODO: Place higher levels of map here, must pass in
    
    float texRow = 0;
    float texRowStep = TEX_SIZE;
    float wallEnd = drawStart + wallHeight;
    float wallHeightOld = wallHeight;
    //Clip Top
    if(drawStart < 0){
        texRow = -drawStart / wallHeight * TEX_SIZE;
        texRowStep -= texRow; 
        wallHeight += drawStart;
        drawStart = 0;
    }
    //Clip Bottom
    if(wallEnd >= SCREEN_HEIGHT){
        texRowStep -= (wallEnd - SCREEN_HEIGHT) / wallHeightOld * TEX_SIZE;
        wallHeight = SCREEN_HEIGHT - drawStart;
    }
    texRowStep /= wallHeight;

    float texCol = (rayhit->steppingInX) ? (player->yPos + rayhit->yRayLength) : (player->xPos + rayhit->xRayLength);
    texCol -= (int)texCol; //only get the decimal part
    texCol *= TEX_SIZE;

    for(Uint16 j = drawStart; j < drawStart + wallHeight; j++){
        Uint32 color = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * map[rayhit->xTile + MAPSIZE * rayhit->yTile].texID) + (textureSurf->w) * (int)texRow);
        //TODO: Better way to organize/implement the fog? We can also cull anything far enough away where the fog the only color
        float fogDist = perpDist / MAPSIZE * 255 * 2;
        if(fogDist > 255) fogDist = 255;
        Uint32 fogColor = (Uint8)fogDist << 24;

        if(color >> 24 < 255)
            pixels[rayhit->column + SCREEN_WIDTH * j] = AlphaBlend(fogColor, AlphaBlend(color, pixels[rayhit->column + SCREEN_WIDTH * j])); //TODO: Add directional shading
        else
            pixels[rayhit->column + SCREEN_WIDTH * j] = AlphaBlend(fogColor, color);

        texRow += texRowStep;
    }  
}

void RenderSprite(const Sprite*const sprite, float playerPitch){
    float xSpriteCam = sprite->xCamPos;
    float ySpriteCam = sprite->yCamPos;

    if(ySpriteCam <= 0) return;   //Only render if in front of the player

    //get the screen width and height
    float spriteSize = sprite->scale * (float)SCREEN_HEIGHT / ySpriteCam;
    int xScreenPos = (SCREEN_WIDTH / 2.0) * (1 + xSpriteCam/ySpriteCam); //for a 90deg FOV, the view triangle's slope is 1, compare that to the slope of the triangle formed by the player and object

    //adjust screen and texture bounds
    int xStart = (int)(xScreenPos - spriteSize/2.0);
    int xEnd = xStart + spriteSize;
    int yStart = (int)(SCREEN_HEIGHT/2.0 - spriteSize/2.0) - (sprite->heightAdjust * SCREEN_HEIGHT/2.0)/ySpriteCam + playerPitch;
    int yEnd = yStart + spriteSize;

    float texStep = TEX_SIZE / spriteSize;
    float texCol = 0;
    float texRowStart = 0;

    if(xStart < 0){
        texCol = -xStart/spriteSize * TEX_SIZE;
        xStart = 0;
        xEnd += xStart;
    }
    if(xEnd > SCREEN_WIDTH) xEnd = SCREEN_WIDTH;


    if(yStart < 0){
        texRowStart = -yStart / spriteSize * TEX_SIZE;
        yStart = 0;
    }
    if(yEnd > SCREEN_HEIGHT) yEnd = SCREEN_HEIGHT;


    //render
    for(int x = xStart; x < xEnd; x++){
        if(zBuffer[x] < ySpriteCam){
            texCol += texStep;
            continue;
        }

        float texRow = texRowStart;
        for(int y = yStart; y < yEnd; y++){
            Uint32 color = *((Uint32*)spriteSurf->pixels + (int)texCol + (TEX_SIZE*sprite->spriteTextureID) + (spriteSurf->w) * (int)texRow);
            texRow += texStep;

            float fogDist = ySpriteCam / MAPSIZE * 255 * 2;
            if(fogDist > 255) fogDist = 255;
            Uint32 fogColor = (Uint8)fogDist << 24;
            if(!color) continue;
            color = AlphaBlend(color, pixels[x + SCREEN_WIDTH * y]);
            color = AlphaBlend(fogColor, color);

            pixels[x + SCREEN_WIDTH * y] = color;
            
        }
        texCol += texStep;
    }
}

int main(int argc, char* argv[]){
    /*
    Uint32 top = 0xAFFF0000;
    Uint32 bot = 0xFF0000FF;
    Uint32 newCol = AlphaBlend(top, bot);
    printf("%x + %x = %x\n", bot, top, newCol);
    exit(0);
    */

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

    SDL_Texture* screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(!screenTexture){
        printf("error creating texture\n");
        return 1;
    }

    textureSurf = IMG_Load("./wolftextures.png");
    if(textureSurf->format->format != SDL_PIXELFORMAT_ARGB8888){
        textureSurf = SDL_ConvertSurfaceFormat(textureSurf, SDL_PIXELFORMAT_ARGB8888, 0);
    }
  
    spriteSurf = IMG_Load("./wolfsprites.png");
    if(spriteSurf->format->format != SDL_PIXELFORMAT_ARGB8888){
        spriteSurf = SDL_ConvertSurfaceFormat(spriteSurf, SDL_PIXELFORMAT_ARGB8888, 0);
    }

    SDL_Event event;
    Uint32 lastTime = SDL_GetTicks(), lastTimeFrame = 0;
    
    Player player = {17.5, 1.5, 0.707, 0.707, -0.707, 0.707, 0};
    Mouse mouse = {0, 0, 0, 0};

    WallPiece Map[MAPSIZE_ARRAY];
    Map_Init(Map, map);
    Map_AddDoor(Map, 16, 4, MakeDoor(0.25, 0.4, 1, 0, 1));
    Map_AddDoor(Map, 15, 5, MakeDoor(0.5, 1, 0, 1, 1));

    const Uint32 spriteCount = 4;
    Sprite sprites[] = {
        {1, 7.5, 3.5, 0, 1},
        {1, 8.5, 3.5, -(1-0.45), 0.45},
        {3, 9.5, 3.5, 0, 1},
        {0, 16.5, 5.5, 0, 1}
    };

    Uint8 keys[6] = {0,0,0,0,0,0};
    Uint8 running = 1;
    Uint8 timer = 0;
    RayHit hits[SCREEN_WIDTH];
    
    RenderList renderList = ListInit(SCREEN_WIDTH);
    Uint32 fps_last = SDL_GetTicks();
    Uint32 frameCounter = 0;
    while(running){
        
        //=============================FPS Counter=================================
        frameCounter++;
        if(SDL_GetTicks() - fps_last >= 250){
            printf("FPS: %f\n", (float)frameCounter/250.0 * 1000.0);
            frameCounter = 0;
            fps_last = SDL_GetTicks();
        }
        

        //===========================================Event Handling=================================================================
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                printf("QUITTING WINDOW\n");
                running = 0;
                continue;
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
                SDL_GetMouseState(&xNew, &yNew);
                mouse.xVel = xNew - mouse.x;
                mouse.yVel = yNew - mouse.y;
                mouse.x = xNew;
                mouse.y = yNew;

                //TODO: only change direction if the mouse is moving almost horizontally
                //TODO: either recenter mouse after every movement, or allow mouse to move on screen pacman torus style

                float oldDirX = player.xDir;
                float angVel = 3.14 * mouse.xVel/(SCREEN_WIDTH / 2.0);
                mouse.xVel = 0;
                player.xDir = cos(angVel) * player.xDir - sin(angVel) * player.yDir;
                player.yDir = sin(angVel) * oldDirX + cos(angVel) * player.yDir;

                player.pitch = SCREEN_HEIGHT/2.0 - mouse.y;
            }
            if(event.window.event == SDL_WINDOWEVENT_ENTER){
                SDL_GetMouseState(&mouse.x, NULL);
            }
            
        }
        
        //==========================================Update=====================================================================
        if(SDL_GetTicks() - lastTime >= UPDATE_TIMER_MS){
            lastTime = SDL_GetTicks();
            timer+=1;
            Map[16 + MAPSIZE * 4].door->width = timer/255.0;
            Map[16 + MAPSIZE * 4].texID = (Uint8)(timer/255.0 * 7);

            if(keys[0] || keys[2]){
                float xNew =  player.xPos + player.xDir/5 * (keys[0] - keys[2]);
                float yNew = player.yPos + player.yDir/5 * (keys[0] - keys[2]);
                if(Map[(Uint32)xNew + MAPSIZE * (Uint32)yNew].type == WALL_NULL){
                    player.xPos += player.xDir/15 * (keys[0] - keys[2]);   
                    player.yPos += player.yDir/15 * (keys[0] - keys[2]);
                }
            }else{
                float xNew = player.xPos - player.yDir/5 * (keys[3] - keys[1]);   
                float yNew = player.yPos + player.xDir/5 * (keys[3] - keys[1]);
                if(Map[(Uint32)xNew + MAPSIZE * (Uint32)yNew].type == WALL_NULL){
                    player.xPos -= player.yDir/15 * (keys[3] - keys[1]);   
                    player.yPos += player.xDir/15 * (keys[3] - keys[1]);
                }
            }


            float oldDirX = player.xDir;
            float angVel = 0.05 * (keys[5] - keys[4]);
            player.xDir = cos(angVel) * player.xDir - sin(angVel) * player.yDir;
            player.yDir = sin(angVel) * oldDirX + cos(angVel) * player.yDir;

            player.xPlane = -player.yDir;
            player.yPlane = player.xDir;
        }

        //Rendering
        //Clear Pixels
        for(Uint32 i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++){
            pixels[i] = 0xFF00FFFF;
        }
        

        //=========================================Rendering floor/ceiling===================================================
        for(Uint32 i = 0; i < SCREEN_HEIGHT; i++){
            Uint8 floorRender = (i > (float)(SCREEN_HEIGHT >> 1) + player.pitch);
            
            //============Setup
            //We need to get the horizontal distance from player to floor.
            //Calculated as if we projected a vector from the player (through the camera plane) to the floor midpoint onto the floor.
            //The vector from player through the screen forms a similar triangle formed by the vector from the player to the floor spot
            float playerHeight = (float)(SCREEN_HEIGHT >> 1);
            float screenPitch = (float)(SCREEN_HEIGHT >> 1) - i + player.pitch;
            if(floorRender) screenPitch = -screenPitch;
            float rowDist = playerHeight / screenPitch;
            if(!floorRender) rowDist *= 5;
            if(screenPitch == 0) continue;

            float xHit = player.xPos + rowDist * (player.xDir - player.xPlane);
            float yHit = player.yPos + rowDist * (player.yDir - player.yPlane);
            float xStep = (2 * player.xPlane * rowDist) / SCREEN_WIDTH;
            float yStep = (2 * player.yPlane * rowDist) / SCREEN_WIDTH;
      
            //Scanline across the screen
            for(Uint32 j = 0; j < SCREEN_WIDTH; j++){                 
                int xTile = (int)xHit;
                int yTile = (int)yHit;
                float texCol = (xHit - xTile) * TEX_SIZE;
                float texRow = (yHit - yTile) * TEX_SIZE;
                Uint8 tileID = 0;

                float fogDist = rowDist / MAPSIZE * 255 * 2;
                if(fogDist > 255) fogDist = 255;
                Uint32 fogColor = (Uint8)fogDist << 24;

                if(floorRender){
                    if(xTile < 0 || xTile >= MAPSIZE || yTile < 0 || yTile >= MAPSIZE) tileID = 0;
                    else tileID = floorMap[xTile + MAPSIZE * yTile];

                    Uint32 color = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * tileID) + (textureSurf->w) * (int)texRow);
                    color = AlphaBlend(fogColor, color);
                    pixels[j + SCREEN_WIDTH * i] = color;
                }else{
                    if(xTile < 0 || xTile >= MAPSIZE || yTile < 0 || yTile >= MAPSIZE) tileID = 0;
                    else tileID = ceilingMap[xTile + MAPSIZE * yTile];
                    
                    Uint32 color = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * tileID) + (textureSurf->w) * (int)texRow);
                    color = AlphaBlend(fogColor, color);
                    pixels[j + SCREEN_WIDTH * i] = (color>>1) & 0x7F7F7F7F;
                }
                xHit += xStep;
                yHit += yStep;
            }
        }
             
        //=============================================Wall Rendering======================================================== 
        for(Uint32 col = 0; col < SCREEN_WIDTH; col++){
            RayHit rayhit = {};

            //Casting the ray
            {
                float camPlaneNorm = col / (SCREEN_WIDTH / 2.0) - 1; //normalize screen columns into range [-1, 1]

                float xRay = player.xDir + player.xPlane * camPlaneNorm;
                float yRay = player.yDir + player.yPlane * camPlaneNorm;

                float slope = (xRay == 0) ? MAX_SLOPE : yRay / xRay;
                float inv_slope = (yRay == 0) ? MAX_SLOPE : xRay / yRay;

                float xStep = sqrt(1 + slope * slope); 
                float yStep = sqrt(1 + inv_slope * inv_slope);

                Uint32 xTile = (Uint32)player.xPos; 
                Uint32 yTile = (Uint32)player.yPos;

                float xOffset = xTile - player.xPos + (xRay > 0);
                float yOffset = yTile - player.yPos + (yRay > 0);

                float xExtend = ((xOffset < 0) ? -xOffset : xOffset) * xStep;
                float yExtend = ((yOffset < 0) ? -yOffset : yOffset) * yStep;

                Uint8 steppingInX = 0;

                //TODO: Would I rather use the boolean flag method to achieve one more DDA step, or just write a single step inside the door section
                Uint8 doorFlag = 0;
                RayHit lastHit;

                while(1){
                    steppingInX = (xExtend < yExtend);

                    if(steppingInX) xTile += (xRay > 0) ? 1 : -1;
                    else            yTile += (yRay > 0) ? 1 : -1;

                    if(Map[xTile + MAPSIZE * yTile].type != WALL_NULL || doorFlag){
                        float rayLength = (steppingInX) ? xExtend : yExtend;
                        float rayNorm = sqrt(xRay * xRay + yRay * yRay);

                        float xFinish = xRay/rayNorm * rayLength;
                        float yFinish = yRay/rayNorm * rayLength;

                        rayhit = (RayHit){xRay, yRay, xFinish, yFinish, xTile, yTile, col, steppingInX};
                        
                        if(Map[xTile + MAPSIZE * yTile].type == WALL_DOOR || doorFlag){
                            if(!doorFlag){
                                doorFlag = 1;
                                lastHit = rayhit;

                                if(steppingInX) xExtend += xStep;
                                else            yExtend += yStep;

                                continue;
                            }
                            doorFlag = 0;
                            
                            Door* door = Map[lastHit.xTile + MAPSIZE * lastHit.yTile].door;
                            //Intersection and Ray adjust for a door
                            {
                                float doorDepth = door->depth; 
                                float doorWidth = door->width; 
                                Uint8 doorDir = door->isXAligned;
                                int8_t doorStart = door->mapLeftAligned;

                                doorDepth = (doorDir) ? ((rayhit.yRay > 0) ? doorDepth : 1 - doorDepth) : ((rayhit.xRay > 0) ? doorDepth : 1 - doorDepth);

                                float nextDepth = (doorDir) ? rayhit.yRayLength : rayhit.xRayLength;
                                float wallStart = (doorDir) ? lastHit.yRayLength : lastHit.xRayLength;
                                int8_t rayDir = (doorDir) ? ((rayhit.yRay > 0) ? 1 : -1) : ((rayhit.xRay > 0) ? 1 : -1);
                                float rayDepthIntoWall = (nextDepth - wallStart) * rayDir;

                                //if we only hit the adjecent wall sides
                                if(rayDepthIntoWall < doorDepth) break;  
                                
                                //Handling the door portion
                                if(doorDir){
                                    if(lastHit.yTile == rayhit.yTile) rayhit.steppingInX = !rayhit.steppingInX;
                                }else{
                                    if(lastHit.xTile == rayhit.xTile) rayhit.steppingInX = !rayhit.steppingInX;
                                }
                                rayhit.xTile = lastHit.xTile;
                                rayhit.yTile = lastHit.yTile;

                                //Adjust ray to hit door
                                float xAdjust = doorDepth * rayDir; 
                                float yAdjust = doorDepth * rayDir;
                                
                                if(doorDir) xAdjust *= inv_slope;
                                else yAdjust *= slope;
                                
                                rayhit.xRayLength = lastHit.xRayLength + xAdjust;
                                rayhit.yRayLength = lastHit.yRayLength + yAdjust;
                                
                                //Door Width Checking
                                float widthCheck = (doorDir) ? (rayhit.xRayLength + player.xPos) : (rayhit.yRayLength + player.yPos);
                                widthCheck -= (int)widthCheck;
                                if(doorStart) widthCheck = 1 - widthCheck;
                                widthCheck -= doorWidth;
                                if(widthCheck >= 0){
                                    xTile = lastHit.xTile;
                                    yTile = lastHit.yTile;
                                    continue;
                                }
                            
                            }

                            Uint8 isTransparent = (Map[lastHit.xTile + MAPSIZE * lastHit.yTile].texID == 8); //TODO: Find a way to encode transparency info with texture data
                            if(isTransparent){
                                ListAppend(&renderList, rayhit);
                                xTile = lastHit.xTile;
                                yTile = lastHit.yTile;
                                continue;
                            }
                            
                            break;
                        }


                        Uint8 isTransparent = (Map[rayhit.xTile + MAPSIZE * rayhit.yTile].texID == 8); //TODO: Find a way to encode transparency info with texture data
                        if(isTransparent){
                            if(steppingInX) xExtend += xStep;
                            else            yExtend += yStep;
                            
                            ListAppend(&renderList, rayhit);
                            continue;
                        }
                        break;
                    }

                    if(steppingInX) xExtend += xStep;
                    else            yExtend += yStep;
                }
            }

            //Rendering walls/doors
            /*
            for(int i = 0; i < 3; i++){//TEMP: Go through and draw ever level offseting by wallheight * i
                if(rayhit.xTile < 0 && rayhit.yTile < 0) continue; //TODO: As of now we return a struct with negative tile position. Is there a better way to represent a "NULL Struct" without a null ptr
                
                Uint8 transparencyColFlag = 0;
                float perpDist = player.xDir * rayhit.xRayLength + player.yDir * rayhit.yRayLength;
                zBuffer[col] = perpDist;
                float wallHeight = (float)SCREEN_HEIGHT / perpDist;
                            
                int drawStart = (SCREEN_HEIGHT - wallHeight) / 2 + player.pitch;
                drawStart -= wallHeight * i;
                
                float texRow = 0;
                float texRowStep = TEX_SIZE;
                float wallEnd = drawStart + wallHeight;
                float wallHeightOld = wallHeight;
                //Clip Top
                if(drawStart < 0){
                    texRow = -drawStart / wallHeight * TEX_SIZE;
                    texRowStep -= texRow; 
                    wallHeight += drawStart;
                    drawStart = 0;
                }
                //Clip Bottom
                if(wallEnd >= SCREEN_HEIGHT){
                    texRowStep -= (wallEnd - SCREEN_HEIGHT) / wallHeightOld * TEX_SIZE;
                    wallHeight = SCREEN_HEIGHT - drawStart;
                }
                texRowStep /= wallHeight;

                Uint16 j = drawStart;
                float texCol = (rayhit.steppingInX) ? (player.yPos + rayhit.yRayLength) : (player.xPos + rayhit.xRayLength);
                texCol -= (int)texCol; //only get the decimal part
                texCol *= TEX_SIZE;

                
                for(; j < drawStart + wallHeight; j++){
                    Uint32 color = *((Uint32*)textureSurf->pixels + (int)texCol + (TEX_SIZE * Map[rayhit.xTile + MAPSIZE * rayhit.yTile].texID) + (textureSurf->w) * (int)texRow);
                    //TODO: Better way to organize/implement the fog? We can also cull anything far enough away where the fog the only color
                    float fogDist = perpDist / MAPSIZE * 255 * 2;
                    if(fogDist > 255) fogDist = 255;
                    Uint32 fogColor = (Uint8)fogDist << 24;

                    if(color >> 24 < 255)
                        pixels[col + SCREEN_WIDTH * j] = AlphaBlend(fogColor, AlphaBlend(color, pixels[col + SCREEN_WIDTH * j])); //TODO: Add directional shading
                    else
                        pixels[col + SCREEN_WIDTH * j] = AlphaBlend(fogColor, color);

                    texRow += texRowStep;
                }
            }
            */
            hits[col] = rayhit;
            ListAppend(&renderList, rayhit);
            //RenderWall(&player, &rayhit, Map);
        }

        //==========================================Sprite Transformations==============================================================
        Uint8 spriteZBuffer[spriteCount];
        Uint32 zBuffLen = renderList.size + spriteCount; 
        void* zBufferAll[zBuffLen];
        Uint8 zBufferAllType[zBuffLen];

        //Calculate all sprites in camera space
        for(Uint32 i = 0; i < spriteCount; i++){
            float xSprite = sprites[i].xPos;
            float ySprite = sprites[i].yPos;

            float xDiff = xSprite - player.xPos, yDiff = ySprite - player.yPos;
            //Transform sprite world coords into camera coords (direction and plane as basis vecs)
            float denom = player.xPlane * player.yDir - player.yPlane * player.xDir;
            float xSpriteCam = (player.yDir * xDiff - player.xDir * yDiff) / denom;
            float ySpriteCam = (-player.yPlane * xDiff + player.xPlane * yDiff) / denom;


            sprites[i].xCamPos = xSpriteCam;
            sprites[i].yCamPos = ySpriteCam;

            spriteZBuffer[i] = i;
        }
        
        //==========================================Rendering Pass==============================================================
        Uint32 ii = 0;
        for(; ii < renderList.size; ii++){
            zBufferAll[ii] = (void*)(renderList.list_array + ii);
            zBufferAllType[ii] = 0; 
        }
        for(; ii < zBuffLen; ii++){
            zBufferAll[ii] = (void*)(sprites + (ii - renderList.size));
            zBufferAllType[ii] = 1; 
        }
        
        //Sorting ZBuffer
        for(Uint32 i = 0; i < zBuffLen; i++){
            for(Uint32 j = 1; j < zBuffLen; j++){
                void* k1 = zBufferAll[j - 1];
                void* k2 = zBufferAll[j];
                float d1, d2;

                if(zBufferAllType[j - 1]){
                    d1 = ((Sprite*)k1)->yCamPos;
                }else{
                    RayHit* rayhit = (RayHit*)k1;
                    d1 = player.xDir * rayhit->xRayLength + player.yDir * rayhit->yRayLength;
                }
                
                if(zBufferAllType[j]){
                    d2 = ((Sprite*)k2)->yCamPos;
                }else{
                    RayHit* rayhit = (RayHit*)k2;
                    d2 = player.xDir * rayhit->xRayLength + player.yDir * rayhit->yRayLength;
                }
                
                if(d1 < d2){;
                    zBufferAll[j - 1] = zBufferAll[j];
                    zBufferAll[j] = k1;

                    Uint8 temp = zBufferAllType[j - 1];
                    zBufferAllType[j - 1] = zBufferAllType[j];
                    zBufferAllType[j] = temp;
                }
            }
        }

        for(Uint32 i = 0; i < zBuffLen; i++){
            if(zBufferAllType[i]){
                Sprite* sprite = (Sprite*)zBufferAll[i];
                RenderSprite(sprite, player.pitch);
            }else{
                RayHit* rayhit = (RayHit*)zBufferAll[i];
                RenderWall(&player, rayhit, Map);
            }
        }
    
        SDL_UpdateTexture(screenTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
        SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        ListReturnHome(&renderList);
    }

    //Exit Clean Up
    ListDestroy(&renderList);
    Map_Destroy(Map);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(screenTexture);
    SDL_FreeSurface(textureSurf);
    SDL_Quit();
    return 0;
}

