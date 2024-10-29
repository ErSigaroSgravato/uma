#include<iostream>
#include<SDL2/SDL.h>

#define HEIGHT 480
#define WIDTH 640
int main(){
    SDL_Window* window = NULL;
    SDL_Surface* screen_surface = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Hello, World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL){
        printf("SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    screen_surface = SDL_GetWindowSurface(window);


    SDL_Event e;
    bool quit = false;

    char r = 255, g = 255, b = 255;
    while(!quit){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_KEYDOWN){
                switch(e.key.keysym.sym){
                case SDLK_r: r -= 2.01; break;
                case SDLK_g: g -= 2.01; break;
                case SDLK_b: b -= 2:.01; break;
                }
            }
        }
        
        SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, r , g , b));
        SDL_UpdateWindowSurface(window);
 
    }
}
