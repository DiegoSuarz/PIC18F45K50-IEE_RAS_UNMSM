#include "display_7seg.h"


uint8_t Display_anodo(uint8_t literal){
    uint8_t dato;
    dato=SevenSegmentASCII[literal];
    return (uint8_t)~dato;
}

uint8_t Display_catodo(uint8_t literal){
    uint8_t dato;
    dato=SevenSegmentASCII[literal];
   return (uint8_t)dato;
}
