
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>

#define WINDOW_DEFAULT_W 1280
#define WINDOW_DEFAULT_H 720

#define P_NULL 0
#define P_SAND 1
#define P_WATER 2
#define P_INVALID 255

uint8_t temp_pixels[WINDOW_DEFAULT_H*WINDOW_DEFAULT_W];
uint8_t pixels[WINDOW_DEFAULT_H*WINDOW_DEFAULT_W];

const auto framerate = 1.0/144.0;

uint8_t get_pixel(uint16_t x,uint16_t y){
    return pixels[(x*WINDOW_DEFAULT_H)+y];
}

void run_logic(){
    for (uint16_t x = 0; x<WINDOW_DEFAULT_W; x++){
    for (uint16_t y = 0; y<WINDOW_DEFAULT_H; y++){
        uint8_t p_down,p_up,p_left,p_right,p_up_left,p_up_right,p_down_left,p_down_right = P_INVALID;
        if (x>0) p_left = get_pixel(x-1, y);
        if (x<WINDOW_DEFAULT_W-1) p_right = get_pixel(x+1, y);
        if (y>0) p_down = get_pixel(x, y-1);
        if (y<WINDOW_DEFAULT_H-1) p_up = get_pixel(x, y+1);
        if (p_left!=P_INVALID && p_down != P_INVALID) p_down_left = get_pixel(x-1, y-1);
        if (p_right!=P_INVALID && p_down != P_INVALID) p_down_right = get_pixel(x+1, y-1);
        if (p_left!=P_INVALID && p_up != P_INVALID) p_up_left = get_pixel(x-1, y+1);
        if (p_right!=P_INVALID && p_up != P_INVALID) p_up_right = get_pixel(x+1, y+1);
        switch (get_pixel(x,y)){
        case P_SAND:

            break;
        }
    }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        printf("Game will now self destruct.");
    }

    SDL_Window *win =
        SDL_CreateWindow("CppSand", // creates a window
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_DEFAULT_W, WINDOW_DEFAULT_H, 0);

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetVSync(rend, 0);

    bool close = false;
    auto last_tick = std::chrono::high_resolution_clock::now();
    
    // Initialize arrays
    for (int i = 0; i<WINDOW_DEFAULT_H*WINDOW_DEFAULT_W; i++){
        temp_pixels[i] = P_NULL;
        pixels[i] = P_NULL;
    }

    printf("Values Initizalized!");

    while (!close) {
        SDL_Event event;

        auto current_tick = std::chrono::high_resolution_clock::now();
        auto time_between_ticks = std::chrono::duration_cast<std::chrono::microseconds>(current_tick-last_tick).count();
        auto delta = time_between_ticks/(1000.0*1000.0);
        std::cout<<"FPS: "<<1.0/delta<<std::endl;;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                close = true;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    break;
                }
            }

        run_logic();

        SDL_Delay(framerate-delta);
        last_tick = current_tick;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
  }
