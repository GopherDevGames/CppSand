
#include "SDL_error.h"
#include "SDL_events.h"
#include "SDL_mouse.h"
#include "SDL_render.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <iostream>
#include <string>
#include <cstdint>
#include <chrono>

const uint16_t WINDOW_DEFAULT_W = 1280;
const uint16_t WINDOW_DEFAULT_H = 720;

const uint8_t P_VOID = 0;
const uint8_t P_SAND = 1;
const uint8_t P_WATER = 2;
const uint8_t P_AMOUNT = 3;

uint8_t temp_pixels[WINDOW_DEFAULT_H*WINDOW_DEFAULT_W];
uint8_t pixels[WINDOW_DEFAULT_H*WINDOW_DEFAULT_W];

const auto framerate = 1.0/144.0;

uint8_t get_pixel(uint16_t x,uint16_t y){
    return pixels[(x*WINDOW_DEFAULT_H)+y];
}
uint8_t get_temp_pixel(uint16_t x,uint16_t y){
    return temp_pixels[(x*WINDOW_DEFAULT_H)+y];
}
void set_temp_pixel(uint16_t x,uint16_t y,uint8_t pixel){
    temp_pixels[(x*WINDOW_DEFAULT_H)+y] = pixel;
}

void set_pixel(uint16_t x,uint16_t y,uint8_t pixel){
    pixels[(x*WINDOW_DEFAULT_H)+y] = pixel;
}

#define P_DOWN get_temp_pixel(x, std::max(y-1,1))
#define P_DOWN_LEFT get_temp_pixel(std::max(x-1,1), std::max(y-1,1))
#define P_DOWN_RIGHT get_temp_pixel(std::min(x+1,WINDOW_DEFAULT_W-1), std::max(y-1,1))

// GLobals
bool is_drawing = false;

void run_logic(){

    if (is_drawing){
        int x, y;
        SDL_GetMouseState(&x, &y);
        set_pixel(x, WINDOW_DEFAULT_H-y, P_SAND);
        set_temp_pixel(x,WINDOW_DEFAULT_H-y,P_SAND);
    }

    for (uint16_t x = 0; x<WINDOW_DEFAULT_W; x++){
    for (uint16_t y = 0; y<WINDOW_DEFAULT_H; y++){
        switch (get_pixel(x,y)){
        case P_SAND:
            if (P_DOWN == P_VOID){
                set_temp_pixel(x,y,P_VOID);
                set_temp_pixel(x,y-1,P_SAND);
            }
            else if (y>1){
                if (P_DOWN_LEFT == P_VOID){
                    set_temp_pixel(x,y,P_VOID);
                    set_temp_pixel(x-1,y-1,P_SAND);
                }
                else if (P_DOWN_RIGHT == P_VOID){
                    set_temp_pixel(x,y,P_VOID);
                    set_temp_pixel(x+1,y-1,P_SAND);
                }
            }
            break;
        }
    }
    }
    // Copy temp pixels into real ones and reset them
    memcpy(&pixels, &temp_pixels, sizeof(temp_pixels));
}

//                         void   sand     water   invalid
Uint8 colors_r[P_AMOUNT] = {0,200, 30};
Uint8 colors_g[P_AMOUNT] = {0,150, 20};
Uint8 colors_b[P_AMOUNT] = {0,50,255};

void render(SDL_Renderer *rend){
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    for (int x = 0; x<WINDOW_DEFAULT_W; x++){
    for (int y = 0; y<WINDOW_DEFAULT_H; y++){
        auto pixel = get_pixel(x, y);
        if (pixel == P_VOID) continue;
        SDL_SetRenderDrawColor(rend, colors_r[pixel], colors_g[pixel], colors_b[pixel], 255);
        SDL_RenderDrawPoint(rend, x, WINDOW_DEFAULT_H-y);
    }
    }
    SDL_RenderPresent(rend);
}

int main() {
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
        temp_pixels[i] = P_VOID;
        pixels[i] = P_VOID;
    }

    printf("Values Initizalized!");

    set_pixel(300, 500, P_SAND);
    set_pixel(300, 510, P_SAND);
    set_pixel(300, 515, P_SAND);

    int frames_passed = 0;

    while (!close) {
        frames_passed += 1;
        SDL_Event event;

        auto current_tick = std::chrono::high_resolution_clock::now();
        auto time_between_ticks = std::chrono::duration_cast<std::chrono::microseconds>(current_tick-last_tick).count();
        auto delta = time_between_ticks/(1000.0*1000.0);
        if (frames_passed%10==0) std::cout<<"FPS: "<<1.0/delta<<std::endl;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                close = true;
                break;

            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                        is_drawing = true;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                        is_drawing = false;
                    break;
                }
            break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    break;
                }
            }
        }
        run_logic();
        render(rend);

        printf("%s", SDL_GetError());

        //SDL_Delay(framerate-delta);
        last_tick = current_tick;
    }

    // Cleanup
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
  }
