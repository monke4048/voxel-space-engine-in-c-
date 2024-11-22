#include <SDL2/SDL.h>
#include <iostream>
using namespace std;
 


int main()
{
    int width= 320,height = 200;
    
    float scalefactor = 100.0;
    float x = 0.0,y = 0.0;
    float Cheight = 100.0;
    float theta = 0.0;
    float horizon = 50.0;
    float zfar = 600.0;
    int mapsize = 1024;
    int Pheight = 10;
    SDL_Window* win = SDL_CreateWindow(NULL,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);
    SDL_Init(SDL_INIT_EVERYTHING);

    bool isrunning = true;
    SDL_Event e;

    SDL_Surface* colormapIM = SDL_LoadBMP("C1W.bmp");
    SDL_Surface* heightmapIM= SDL_LoadBMP("D1.bmp");

    Uint8* mapoffset = (Uint8*)heightmapIM->pixels;
    int bpp = heightmapIM->format->BytesPerPixel;

    SDL_Color *palette = colormapIM->format->palette->colors;
    int paletteSize = colormapIM->format->palette->ncolors;
    Uint8 *coloroffset = (Uint8*)colormapIM->pixels;
    
    
    while(isrunning)
    {
        while(SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                isrunning = false;
            }
            const Uint8 *keystate = SDL_GetKeyboardState(NULL);
            if(keystate[SDL_SCANCODE_W])
            {
                x += cos(theta);
                y += sin(theta);
            }
            if(keystate[SDL_SCANCODE_S])
            {
                x -= cos(theta);
                y -= sin(theta);
            }
            if(keystate[SDL_SCANCODE_A])
            {
                theta -=  0.05;
            }
            if(keystate[SDL_SCANCODE_D])
            {
                theta +=  0.05;
            }
            if(keystate[SDL_SCANCODE_E])
            {
                Cheight += 1;
            }
            if(keystate[SDL_SCANCODE_R])
            {
                Cheight -= 1;
            }
            if(keystate[SDL_SCANCODE_G])
            {
                horizon += 1;
            }
            if(keystate[SDL_SCANCODE_H])
            {
                horizon -= 1;
            }
            
        } 
        float sinangle = sin(theta);
        float cosangle = cos(theta);

        float plx = cosangle*zfar +sinangle*zfar;
        float ply = sinangle*zfar -cosangle*zfar;

        float prx = cosangle*zfar -sinangle*zfar;
        float pry = sinangle*zfar +cosangle*zfar;

        int collisionPos = (mapsize * ((int)(y) & 1023) + ((int)(x) & 1023)) * bpp;
        Uint8 collisionValue = mapoffset[collisionPos];
    
        if(Cheight < (collisionValue+Pheight))
        {
            Cheight = collisionValue + Pheight;
        }
        for(int i = 0; i < width; i++)
        {
            float del_x = (plx + (prx-plx)/width*i)/zfar;
            float del_y = (ply + (pry-ply)/width*i)/zfar;
            float rx = x;
            float ry = y;
            float max_height = height;

            for (int z = 1; z < zfar; z++)
            {
                rx += del_x;
                ry += del_y;

                int C_rx =((int)(rx) & 1023);
                int C_ry =((int)(ry) & 1023);
                int pixelPos = (mapsize*C_ry + C_rx) * bpp;

                Uint8 heightValue = mapoffset[pixelPos];
                int heightonscreen = int((Cheight - heightValue)/z*scalefactor + horizon);
                
                Uint8 pixelIndex = coloroffset[mapsize*C_ry + C_rx];
                SDL_Color color = palette[pixelIndex];
                
                if (heightonscreen < 0)
                {
                    heightonscreen = 0;
                }
                if (heightonscreen > height)
                {
                    heightonscreen = height -1;
                }
                
                if (heightonscreen < max_height)
                {
                    for(int jg = heightonscreen; jg < max_height;jg++)
                    {

                        SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,255);
                        SDL_RenderDrawPoint(renderer,i,jg);
                    }
                    max_height = heightonscreen;
                }
            }
        }
        SDL_SetRenderDrawColor(renderer,108, 160, 220,255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}
