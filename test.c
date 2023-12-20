#include <math.h>
#include <SDL2/SDL.h>

#define WIDTH 600
#define HEIGHT 400

#define squared(a) (a*a)

#define MAX_SLOPE 1000000000000;

Uint32 lastTime;

uint32_t pixels[WIDTH * HEIGHT];
SDL_Texture* texture;
SDL_Renderer* renderer;

#define MAPSIZE 24
int map[MAPSIZE * MAPSIZE]=
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

float playerX = 2.5, playerY = 2.5;
float dirX = 0.707, dirY = 0.707;
float xPlane = -0.707, yPlane = 0.707;

u_int8_t keys[6] = {0,0,0,0,0,0};

unsigned short max(short a, short b){
    if(a>b) return a;
    return b;
}

int main(int argc, char *argv[])
{
 
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if(!win){
        printf("error creating window\n");
        return 1;
    }

    renderer = SDL_CreateRenderer(win, -1, 0);
    if(!renderer){
        printf("error creating renderer\n");
        return 1;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    if(!texture){
        printf("error creating texture\n");
        return 1;
    }

    unsigned char running = 1;
    SDL_Event event;
    lastTime = SDL_GetTicks();
    
    while (running){

        //EVENT HANDLING
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            printf("Quitting the window\n");
            running = 0;
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
                case SDLK_w:
                    keys[0] = 1;
                break;
                case SDLK_a:
                    keys[1] = 1;
                break;
                case SDLK_s:
                    keys[2] = 1;
                break;
                case SDLK_d:
                    keys[3] = 1;
                break;
                case SDLK_q:
                    keys[4] = 1;
                break;
                case SDLK_e:
                    keys[5] = 1;
                break;
            }
        }else if(event.type == SDL_KEYUP){
            switch(event.key.keysym.sym){
                case SDLK_w:
                    keys[0] = 0;
                break;
                case SDLK_a:
                    keys[1] = 0;
                break;
                case SDLK_s:
                    keys[2] = 0;
                break;
                case SDLK_d:
                    keys[3] = 0;
                break;
                case SDLK_q:
                    keys[4] = 0;
                break;
                case SDLK_e:
                    keys[5] = 0;
                break;
            }
        }

        //Update Call
        
        if(SDL_GetTicks() - lastTime >= (int)(1/60.0)){
            lastTime = SDL_GetTicks();
            
            float oldDirX = dirX, oldPlaneX = xPlane;
            float angVel = 0.03 * (keys[5] - keys[4]);
            dirX = cos(angVel) * dirX - sin(angVel) * dirY;
            dirY = sin(angVel) * oldDirX + cos(angVel) * dirY;

            xPlane = cos(angVel) * xPlane - sin(angVel) * yPlane;
            yPlane = sin(angVel) * oldPlaneX + cos(angVel) * yPlane;

            playerX += dirX/15 * (keys[0] - keys[2]); 
            playerY += dirY/15 * (keys[0] - keys[2]); 

            playerX -= dirY/15 * (keys[3] - keys[1]); 
            playerY += dirX/15 * (keys[3] - keys[1]);
        }
        

        //RENDERING
        //SDL_SetRenderTarget(renderer, NULL);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //For each column of pixles, cast a ray to get distance
        float xFin = 0, yFin = 0;
        
        float norm = sqrt(2) * dirX;
        //xPlane /= norm;
        //yPlane /= norm;

        //for(uint32_t c = 0; c < WIDTH; c++){  
        for(uint32_t c = 0; c < WIDTH; c++){ 
            float camX = c / (WIDTH / 2.0) - 1; //transform screen col to range [-1,1]
            float xRay = dirX + xPlane * camX;
            float yRay = dirY + yPlane * camX;
            
            norm = sqrt(squared(xRay) + squared(yRay));
            xRay /= norm;
            yRay /= norm;

            //DDA Ray Tracing Algorithm
            float xSlope = (float)yRay / xRay;
            if(xRay == 0) xSlope = MAX_SLOPE;
            float ySlope = (float)xRay / yRay;
            if(yRay == 0) ySlope = MAX_SLOPE;
            float sideStepX = sqrt(1 + squared(xSlope));
            float sideStepY = sqrt(1 + squared(ySlope));

            unsigned char stepX = 0;
            float xExtend = 0, yExtend = 0;
            
            //Finding inital offsets
            float xOffset = 0, yOffset = 0;
            unsigned int xTile = (int)playerX, yTile = (int)playerY;
            if(xRay > 0)
                xOffset = xTile + 1 - playerX; 
            else
                xOffset = xTile - playerX;    
            
            if(yRay > 0)
                yOffset = yTile + 1 - playerY;
            else
                yOffset = yTile - playerY;


            //Inital Ray Lengths using offsets
            xExtend = sqrt(squared(xOffset) + squared(xOffset * xSlope));
            yExtend = sqrt(squared(yOffset) + squared(yOffset * ySlope));
            
            int r = 0, g = 0, b = 0;
            //LOOP UNTIL WALL HIT
            while(1){
                //We step in the X direction if the Xray is shorter than the Yray 
                stepX = (xExtend < yExtend);
                
                xTile += stepX * ((xRay > 0) - (xRay <= 0));
                yTile += !stepX * ((yRay > 0) - (yRay <= 0));
                if(map[xTile + MAPSIZE * yTile] != 0){
                    float distToWall = stepX * xExtend + !stepX * yExtend;
                    xFin = xRay * distToWall;
                    yFin = yRay * distToWall;

                    switch(map[xTile + MAPSIZE * yTile]){
                        case 1:
                            r = 0;
                            g = 0;
                            b = 255;
                        break;

                        case 2:
                            r = 0;
                            g = 255;
                            b = 0;
                        break;

                        case 3:
                            r = 255;
                            g = 0;
                            b = 0;
                        break;

                        case 4:
                            r = 255;
                            g = 255;
                            b = 0;
                        break;

                        case 5:
                            r = 0;
                            g = 255;
                            b = 255;
                        break;

                    }

                    break;
                }


                //Extend ray in direction with shortest ray
                xExtend += sideStepX * stepX;    
                yExtend += sideStepY * !stepX; 
     
            }

            //FISHEYE PROBLEM FIX HERE
            float perpDist = dirX * xFin + dirY * yFin;
            int wallHeight = (float)HEIGHT / perpDist;
            int start = (HEIGHT - wallHeight ) / 2;

            if(stepX)
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            else
                SDL_SetRenderDrawColor(renderer, max(r - 50, 0), max(g - 50, 0), max(b - 50, 0), 255);

            SDL_RenderDrawLine(renderer, c, start, c, start + wallHeight);

            SDL_SetRenderDrawColor(renderer, 27, 65, 0, 255);
            SDL_RenderDrawLine(renderer, c, start + wallHeight, c, HEIGHT);
            
            SDL_SetRenderDrawColor(renderer, 142, 174, 196, 255);
            SDL_RenderDrawLine(renderer, c, 0, c, start);

            //SDL_SetRenderTarget(renderer, texture);
            //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            //SDL_RenderDrawLine(renderer, (int)(playerX * 8 + 5), (int)(playerY * 8 + 5), (int)((playerX + xFin) * 8 + 5), (int)((playerY + yFin) * 8 + 5));
        }
        

        for(int x = 5; x < MAPSIZE*8 + 5; x++){
            for(int y = 5; y < MAPSIZE*8 + 5; y++){
                unsigned short indexX = (x - 5)/8;
                unsigned short indexY = (y - 5)/8;
                unsigned short tileCol = map[indexX + MAPSIZE * indexY];
                if((x - 5) % 8 == 0) tileCol = 0;
                if((y - 5) % 8 == 0) tileCol = 0;
                pixels[x + WIDTH * y] = tileCol * 0xFFFF00;
            }
        }
        //SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(uint32_t));
        //SDL_Rect bounds = {0, 0, 5+MAPSIZE*8, 5+MAPSIZE*8};
        //SDL_RenderCopy(renderer, texture, &bounds, &bounds);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_RenderDrawLine(renderer, (int)(playerX * 8 + 5), (int)(playerY * 8 + 5),(int)(playerX * 8 + 6), (int)(playerY * 8 + 6));
        SDL_Rect playerRect = {(int)(playerX * 8 + 5), (int)(playerY * 8 + 5), 2, 2};
        SDL_RenderDrawRect(renderer, &playerRect);
        SDL_RenderPresent(renderer);
    }
 

    //Clean Up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_DestroyTexture(texture);
    SDL_Quit();

    return 0;
}
