#include "SDL_scancode.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <chrono>

const uint16_t size_multiplier = 4;
const uint16_t WINDOW_DEFAULT_W = (1280)/size_multiplier;
const uint16_t WINDOW_DEFAULT_H = (720)/size_multiplier;
const uint32_t P_COUNT = (WINDOW_DEFAULT_H*WINDOW_DEFAULT_W);

const uint8_t P_VOID = 0;
const uint8_t P_SAND = 1;
const uint8_t P_WATER = 2;
const uint8_t P_TYPES_AMOUNT = 3;
                                        //void       sand      water
const uint8_t gases[P_TYPES_AMOUNT] = {true,false,false};
const uint8_t liquids[P_TYPES_AMOUNT] = {true,false,true};
const uint8_t solids[P_TYPES_AMOUNT] = {false,true,false};

uint8_t pixels[P_COUNT];
bool placed_mask[P_COUNT];

uint8_t get_pixel(uint16_t x,uint16_t y){
    return pixels[(x*WINDOW_DEFAULT_H)+y];
}

void set_pixel(uint16_t x,uint16_t y,uint8_t pixel, bool set_placed_mask = true){
    pixels[(x*WINDOW_DEFAULT_H)+y] = pixel;
    placed_mask[(x*WINDOW_DEFAULT_H)+y] = set_placed_mask;
}

bool get_placed_mask(uint16_t x,uint16_t y){
    return placed_mask[(x*WINDOW_DEFAULT_H)+y];
}

#define P_DOWN x, static_cast<uint16_t>(std::max(y-1,1))
#define P_LEFT static_cast<uint16_t>(std::max(x-1,1)), y
#define P_RIGHT static_cast<uint16_t>(std::min(x+1,WINDOW_DEFAULT_W-1)), y
#define P_DOWN_LEFT static_cast<uint16_t>(std::max(x-1,1)), static_cast<uint16_t>(std::max(y-1,1))
#define P_DOWN_RIGHT static_cast<uint16_t>(std::min(x+1,WINDOW_DEFAULT_W-1)), static_cast<uint16_t>(std::max(y-1,1))

// GLobals
bool is_drawing = false;
auto framerate = 60.0;
uint8_t picked_type = P_SAND;

bool random_50_50() {
   return rand() > (RAND_MAX / 2);
}


void run_logic(){

    if (is_drawing){
        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        mouse_y = WINDOW_DEFAULT_H-(mouse_y/size_multiplier);
        mouse_x/=size_multiplier;
        uint16_t pos_x = std::min(static_cast<uint16_t>(mouse_x),WINDOW_DEFAULT_W);
        uint16_t pos_y = std::min(static_cast<uint16_t>(mouse_y),WINDOW_DEFAULT_H);
        set_pixel(pos_x,pos_y, picked_type,false);
    }

    for (uint16_t x = 0; x<WINDOW_DEFAULT_W; x++){
    for (uint16_t y = 0; y<WINDOW_DEFAULT_H; y++){
        if (get_placed_mask(x,y)) continue;
        switch (get_pixel(x,y)){
        case P_SAND:
            if (liquids[get_pixel(P_DOWN)] && !get_placed_mask(P_DOWN)){
                set_pixel(x,y,get_pixel(P_DOWN));
                set_pixel(x,y-1,P_SAND);
            }
            else if (y>1){
                if (liquids[get_pixel(P_DOWN_LEFT)] && x>0 && !get_placed_mask(P_DOWN_LEFT)){
                    set_pixel(x,y,get_pixel(P_DOWN_LEFT));
                    set_pixel(x-1,y-1,P_SAND);
                }
                else if (liquids[get_pixel(P_DOWN_RIGHT)] && x < WINDOW_DEFAULT_W && !get_placed_mask(P_DOWN_RIGHT)){
                    set_pixel(x,y,get_pixel(P_DOWN_RIGHT));
                    set_pixel(x+1,y-1,P_SAND);
                }
            }
            break;
        case P_WATER:
            if (gases[get_pixel(P_DOWN)]&& !get_placed_mask(P_DOWN)){
                set_pixel(x,y,get_pixel(P_DOWN));
                set_pixel(x,y-1,P_WATER);
            }
            else if (y>1){
                if (gases[get_pixel(P_LEFT)] && x>0 && !get_placed_mask(P_LEFT) && random_50_50()){
                    set_pixel(x,y,get_pixel(P_LEFT));
                    set_pixel(x-1,y,P_WATER);
                }
                else if (gases[get_pixel(P_RIGHT)] && x < WINDOW_DEFAULT_W && !get_placed_mask(P_LEFT) && random_50_50()){
                    set_pixel(x,y,get_pixel(P_RIGHT));
                    set_pixel(x+1,y,P_WATER);
                }
            }
            
        }
    }
    }
    // Reset Placed Mask
    memset(placed_mask,false,sizeof(placed_mask));
}

//                         void   sand     water   invalid
Uint8 colors_r[P_TYPES_AMOUNT] = {0,200, 30};
Uint8 colors_g[P_TYPES_AMOUNT] = {0,150, 20};
Uint8 colors_b[P_TYPES_AMOUNT] = {0,50,255};

void render(SDL_Renderer *rend){
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);
    for (int x = 0; x<WINDOW_DEFAULT_W; x++){
    for (int y = 0; y<WINDOW_DEFAULT_H; y++){
        auto pixel = get_pixel(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
        if (pixel == P_VOID) continue; // Don't render empty pixels, bonus optimization
        SDL_Point points[size_multiplier*size_multiplier];
        for (int size_w = 0; size_w < size_multiplier; size_w++){
        for (int size_h = 0; size_h < size_multiplier; size_h++){
            points[(size_w*size_multiplier)+size_h] = SDL_Point{(x*size_multiplier)+size_w,((WINDOW_DEFAULT_H-y)*size_multiplier)+size_h};
        }
        }
        SDL_SetRenderDrawColor(rend, colors_r[pixel], colors_g[pixel], colors_b[pixel], 255);
        SDL_RenderDrawPoints(rend, points, size_multiplier*size_multiplier);
    }
    }
    SDL_RenderPresent(rend);
}

int main() {

    std::cout<<"Enter simulation Framerate (default:60): "<<std::endl;
    if (std::cin.peek() == '\n' || !(std::cin>>framerate)){
        framerate = 60.0;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
        printf("Game will now self destruct.");
    }

    SDL_Window *win =
        SDL_CreateWindow("CppSand", // creates a window
                       SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       WINDOW_DEFAULT_W*size_multiplier, WINDOW_DEFAULT_H*size_multiplier, 0);

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetVSync(rend, 0);

    bool close = false;
    auto last_tick = std::chrono::high_resolution_clock::now();
    
    // Initialize arrays
    for (int i = 0; i<P_COUNT; i++){
        pixels[i] = P_VOID;
    }

    printf("Values Initizalized!");

    int frames_passed = 0;

    while (!close) {
        frames_passed += 1;
        SDL_Event event;

        auto current_tick = std::chrono::high_resolution_clock::now();
        auto time_between_ticks = std::chrono::duration_cast<std::chrono::microseconds>(current_tick-last_tick).count();
        auto delta = static_cast<double>(time_between_ticks)/(1000.0*1000.0);
        if (frames_passed%100==0) std::cout<<"FPS: "<<1.0/delta<<std::endl;

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
                case SDL_SCANCODE_1:
                case SDL_SCANCODE_KP_1:
                picked_type = P_SAND;
                break;
                case SDL_SCANCODE_2:
                case SDL_SCANCODE_KP_2:
                picked_type = P_WATER;
                break;
                }
            }
        }
        run_logic();
        render(rend);

        printf("%s", SDL_GetError());
        double delay = ((1.0/framerate)-delta)*1000.0;
        if (delay < 0) delay = 0.0;
        if (delay > 1000.0) delay = 1000.0;
        SDL_Delay(static_cast<uint32_t>(delay));
        last_tick = current_tick;
    }

    // Cleanup
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
  }
