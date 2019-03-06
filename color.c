
#include "color.h"

COLOR color_rgb(uint8_t r, uint8_t g, uint8_t b)
{
    COLOR new_color;
    new_color.r = gamma[r];
    new_color.g = gamma[g];
    new_color.b = gamma[b];
    return new_color;
}

COLOR color_Hex(uint32_t hex)
{
    return color_rgb(((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), hex & 0xFF);
}

COLOR color_hsv(uint16_t hue, uint16_t brightness)
{
    COLOR color;
    
    uint8_t scaledHue = (hue * 6);
    
    uint8_t segment = scaledHue / 256; // segment 0 to 5 around the color wheel
    
    uint8_t segmentOffset = scaledHue - (segment * 256); // position within the segment

    uint8_t complement = 0;
    uint8_t prev = (brightness * ( 255 -  segmentOffset)) / 256;
    uint8_t next = (brightness *  segmentOffset) / 256;

    switch(segment) 
    {
    case 0:      // red
        return color_rgb(brightness, next, complement);
    case 1:     // yellow
        return color_rgb(prev, brightness, complement);
    case 2:     // green
        return color_rgb(complement, brightness, next);
    case 3:    // cyan
        return color_rgb(complement, prev, brightness);
    case 4:    // blue
        return color_rgb(next, complement, brightness);
    case 5:      // magenta
    default:
        return color_rgb(brightness, complement, prev);
    }
}

uint8_t dynamic_gamma(double gamma, uint8_t max_out, uint8_t color) // lookup table is: 2.9, 255
{
    return (uint8_t)(pow(((double)color / 255.f), gamma) * max_out + 0.5f);
}
