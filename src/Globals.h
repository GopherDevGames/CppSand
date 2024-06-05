#include "SDL_stdinc.h"
#include <cstdint>
#include <cstdlib>

#ifndef CPPSAND_GLOBALS_H
#define CPPSAND_GLOBALS_H

const uint16_t static WINDOW_DEFAULT_W = 320;
const uint16_t static WINDOW_DEFAULT_H = 200;
const uint16_t static SIZE_MULTIPLIER = 3;
const int static constexpr P_COUNT = WINDOW_DEFAULT_W * WINDOW_DEFAULT_H;

const static uint8_t P_VOID = 0;
const static uint8_t P_SAND = 1;
const static uint8_t P_WATER = 2;
const static uint8_t P_TYPES_AMOUNT = 3;
                                        //void  sand    water
const static uint8_t gases[P_TYPES_AMOUNT] =   {true,  false,  false};
const static uint8_t liquids[P_TYPES_AMOUNT] = {true,  false,  true};
const static uint8_t solids[P_TYPES_AMOUNT] =  {false, true,   false};

//                                       void   sand     water
    Uint8 static colors_r[P_TYPES_AMOUNT] = {0,    200,    30};
    Uint8 static colors_g[P_TYPES_AMOUNT] = {0,    150,    20};
    Uint8 static colors_b[P_TYPES_AMOUNT] = {0,    50,     255};

inline bool random_50_50() {
   return rand() > (RAND_MAX / 2);
}
#endif