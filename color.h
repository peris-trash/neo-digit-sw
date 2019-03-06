/* 
 * File:   color.h
 * Author: rafael
 *
 * Created on 4 de Março de 2019, 22:52
 */

#ifndef COLOR_H
#define	COLOR_H

#include <stdint.h>
#include <math.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define navy    0x001F3F
#define blue    0x0074D9
#define aqua    0x7FDBFF
#define teal    0x39CCCC
#define olive   0x3D9970
#define green   0x2ECC40
#define lime    0x01FF70
#define yellow  0xFFDC00
#define orange  0xFF851B
#define red     0xFF4136
#define marron  0x85144b
#define fushcia 0xF012BE
#define purple  0xB10DC9
#define black   0x111111
#define grey    0xAAAAAA
#define silver  0xDDDDDD
    
const uint8_t gamma[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
    10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
    25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
    37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
    51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
    69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
    90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
    115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
    144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
    177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
    215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
};
    
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} COLOR;

inline COLOR color_rgb(uint8_t r, uint8_t g, uint8_t b);

inline COLOR color_Hex(uint32_t hex);

COLOR color_hsv(uint16_t hue, uint16_t brightness);

uint8_t dynamic_gamma(double gamma, uint8_t max_out, uint8_t color);

#ifdef	__cplusplus
}
#endif

#endif	/* COLOR_H */

