#define SDL_MAIN_HANDLED
#include "SandSim.h"
#include <SDL.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include "Globals.h"

class SandRenderer {
private:
    SandSim sand_sim;
    SDL_Window* win;
    SDL_Renderer* rend;
public:
    double framerate;
    SandRenderer(double framerate){
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("error initializing SDL: %s\n", SDL_GetError());
            printf("Game will now self destruct.");
        }

        win = SDL_CreateWindow(
            "CppSand", // creates a window
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            WINDOW_DEFAULT_W * SIZE_MULTIPLIER, WINDOW_DEFAULT_H * SIZE_MULTIPLIER, 0);

        rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        SDL_RenderSetVSync(rend, 0);
        this->framerate=framerate;
        sand_sim.framerate = framerate;
    }

    ~SandRenderer(){
        SDL_DestroyRenderer(rend);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

    std::chrono::time_point<std::chrono::high_resolution_clock> last_tick = std::chrono::high_resolution_clock::now();
    bool tick(){
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
            return false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                    sand_sim.is_drawing = true;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button){
                    case SDL_BUTTON_LEFT:
                    sand_sim.is_drawing = false;
                    break;
                }
            break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_1:
                case SDL_SCANCODE_KP_1:
                sand_sim.picked_type = P_SAND;
                break;
                case SDL_SCANCODE_2:
                case SDL_SCANCODE_KP_2:
                sand_sim.picked_type = P_WATER;
                break;
                }
            }
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        mouse_y = WINDOW_DEFAULT_H-(mouse_y/SIZE_MULTIPLIER);
        mouse_x/=SIZE_MULTIPLIER;
        // Make sure the mouse isn't outside of the screen
        mouse_x = std::min(mouse_x,static_cast<int>(WINDOW_DEFAULT_W-1));
        mouse_y = std::min(mouse_y,static_cast<int>(WINDOW_DEFAULT_H-1));
        mouse_x = std::max(mouse_x,2);
        mouse_y = std::max(mouse_y,2);
        sand_sim.mouse_pos_x = static_cast<uint16_t>(mouse_x);
        sand_sim.mouse_pos_y = static_cast<uint16_t>(mouse_y);
        std::cout<<static_cast<uint16_t>(mouse_y)<<std::endl;

        sand_sim.run_logic();
        return true;
    }
    int drawn_frames = 0;
    void render() {
        drawn_frames += 1;
        auto current_tick = std::chrono::high_resolution_clock::now();
        auto time_between_ticks = std::chrono::duration_cast<std::chrono::microseconds>(current_tick-last_tick).count();
        auto delta = static_cast<double>(time_between_ticks)/(1000.0*1000.0);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        for (int x = 0; x < WINDOW_DEFAULT_W; x++) {
        for (int y = 0; y < WINDOW_DEFAULT_H; y++) {
            auto pixel =
                sand_sim.get_pixel(static_cast<uint16_t>(x), static_cast<uint16_t>(y));
            if (!sand_sim.get_placed_mask(x, y)) continue; // Don't render pixels that didn't change
            SDL_Point points[SIZE_MULTIPLIER * SIZE_MULTIPLIER];
            for (int size_w = 0; size_w < SIZE_MULTIPLIER; size_w++) {
            for (int size_h = 0; size_h < SIZE_MULTIPLIER; size_h++) {
                points[(size_w * SIZE_MULTIPLIER) + size_h] =
                    SDL_Point{(x * SIZE_MULTIPLIER) + size_w,
                            ((WINDOW_DEFAULT_H - y) * SIZE_MULTIPLIER) + size_h};
            }
            }
            SDL_SetRenderDrawColor(rend, colors_r[pixel], colors_g[pixel],
                                colors_b[pixel], 255);
            SDL_RenderDrawPoints(rend, points, SIZE_MULTIPLIER * SIZE_MULTIPLIER);
        }
        }
        SDL_RenderPresent(rend);
        printf("%s", SDL_GetError());
            double delay = ((1.0/framerate)-delta)*1000.0;
            if (delay < 0) delay = 0.0;
            if (delay > 1000.0) delay = 1000.0;
            SDL_Delay(static_cast<uint32_t>(delay));
            last_tick = current_tick;

    }
};