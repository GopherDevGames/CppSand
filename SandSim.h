#include "Globals.h"
#include <algorithm>
#include <cstring>

class SandSim {
private:
    uint8_t pixels[P_COUNT];
    bool placed_mask[P_COUNT];

public:
    bool is_drawing = false;
    double framerate = 60.0;
    uint8_t picked_type = P_SAND;
    uint16_t mouse_pos_x;
    uint16_t mouse_pos_y;

    SandSim() { // Initialize arrays
        for (int i = 0; i < P_COUNT; i++) {
            pixels[i] = P_VOID;
        }
    }

    uint8_t get_pixel(uint16_t x, uint16_t y) {
        return pixels[(x * WINDOW_DEFAULT_H) + y];
    }
    
    void set_pixel(uint16_t x, uint16_t y, uint8_t pixel, bool set_placed_mask = true){
    pixels[(x * WINDOW_DEFAULT_H) + y] = pixel;
    placed_mask[(x * WINDOW_DEFAULT_H) + y] = set_placed_mask;
    }

    bool get_placed_mask(uint16_t x, uint16_t y) {
        return placed_mask[(x * WINDOW_DEFAULT_H) + y];
    }

#define P_DOWN x, static_cast<uint16_t>(std::max(y - 1, 1))
#define P_LEFT static_cast<uint16_t>(std::max(x - 1, 1)), y
#define P_RIGHT static_cast<uint16_t>(std::min(x + 1, WINDOW_DEFAULT_W - 1)), y
#define P_DOWN_LEFT static_cast<uint16_t>(std::max(x - 1, 1)),static_cast<uint16_t>(std::max(y - 1, 1))
#define P_DOWN_RIGHT static_cast<uint16_t>(std::min(x + 1, WINDOW_DEFAULT_W - 1)),static_cast<uint16_t>(std::max(y - 1, 1))
    void run_logic() {
        // Reset Placed Mask
        memset(placed_mask, false, sizeof(placed_mask));
        if (is_drawing) set_pixel(mouse_pos_x, mouse_pos_y, picked_type, false);

        for (uint16_t x = 0; x < WINDOW_DEFAULT_W; x++) {
            for (uint16_t y = 0; y < WINDOW_DEFAULT_H; y++) {
                if (get_placed_mask(x, y)) continue;
                switch (get_pixel(x, y)) {
                    case P_SAND:
                        if (liquids[get_pixel(P_DOWN)] && !get_placed_mask(P_DOWN)) {
                            set_pixel(x, y, get_pixel(P_DOWN));
                            set_pixel(x, y - 1, P_SAND);
                        } else if (y > 1) {
                            if (liquids[get_pixel(P_DOWN_LEFT)] && x > 0 && !get_placed_mask(P_DOWN_LEFT)) {
                                set_pixel(x, y, get_pixel(P_DOWN_LEFT));
                                set_pixel(x - 1, y - 1, P_SAND);
                            } else if (liquids[get_pixel(P_DOWN_RIGHT)] && x < WINDOW_DEFAULT_W && !get_placed_mask(P_DOWN_RIGHT)) {
                                set_pixel(x, y, get_pixel(P_DOWN_RIGHT));
                                set_pixel(x + 1, y - 1, P_SAND);
                            }
                        }
                    break;
                    case P_WATER:
                        if (gases[get_pixel(P_DOWN)] && !get_placed_mask(P_DOWN)) {
                            set_pixel(x, y, get_pixel(P_DOWN));
                            set_pixel(x, y - 1, P_WATER);
                        } else if (y > 1) {
                            if (gases[get_pixel(P_LEFT)] && x > 0 && !get_placed_mask(P_LEFT) && random_50_50()) {
                                set_pixel(x, y, get_pixel(P_LEFT));
                                set_pixel(x - 1, y, P_WATER);
                            } else if (gases[get_pixel(P_RIGHT)] && x < WINDOW_DEFAULT_W && !get_placed_mask(P_LEFT) && random_50_50()) {
                                set_pixel(x, y, get_pixel(P_RIGHT));
                                set_pixel(x + 1, y, P_WATER);
                            }
                        }
                }
            }
        }
    }
};