#include <stdint.h>
#include "../include/coords/coords.h"

/**
 * 
 * SmartCoords class
 * 
 */

/* Private methods*/
bool SmartCoords::is_x0_initialized() { return rel.x0 != NOT_INIT; }
bool SmartCoords::is_x1_initialized() { return rel.x1 != NOT_INIT; }
bool SmartCoords::is_y0_initialized() { return rel.y0 != NOT_INIT; }
bool SmartCoords::is_y1_initialized() { return rel.y1 != NOT_INIT; }

// bool SmartCoords::are_x0x1_initialized(){ return is_x0_initialized() && is_x1_initialized(); }
// bool SmartCoords::are_y0y1_initialized(){ return is_y0_initialized() && is_y1_initialized(); }

/* Public methods*/

void SmartCoords::init(int16_t x0, int16_t y0){
    zero_display.x0 = 0;
    zero_display.y0 = 0;

    father.x = x0;
    father.y = y0;
}


void SmartCoords::set_x0(int16_t x0) {
    rel.x0 = x0;
    abs.x0 = father.x + rel.x0;

    if (is_x1_initialized()) {
        rel.w = calculate_width();
    }
}
void SmartCoords::set_y0(int16_t y0) {
    rel.y0 = y0;
    abs.y0 = father.y + rel.y0;

    if (is_y1_initialized()) {
        rel.h = calculate_height();
    }
}
void SmartCoords::set_x1(int16_t x1) {
    rel.x1 = x1;
    abs.x1 = father.x + rel.x1;

    if (is_x0_initialized()) {
        rel.w = calculate_width();
    }
}
void SmartCoords::set_y1(int16_t y1) {
    rel.y1 = y1;
    abs.y1 = father.y + rel.y1;

    if (is_y0_initialized()) {
        rel.h = calculate_height();
    }
}


void SmartCoords::set_x0y0(int16_t x0, int16_t y0) {
    set_x0(x0);
    set_y0(y0);
}

void SmartCoords::set_x1y1(int16_t x1, int16_t y1) {
    set_x1(x1);
    set_y1(y1);
}

int16_t SmartCoords::calculate_width(){ return rel.x1 - rel.x0; }
int16_t SmartCoords::calculate_height(){ return rel.y1 - rel.y0; }

void SmartCoords::set_width(int16_t width){
    if (is_x0_initialized()) {
        rel.x1 = rel.x0 + width;
        rel.w = calculate_width();
        abs.x1 = father.x + rel.x1;
    }
}
void SmartCoords::set_height(int16_t height){
    if (is_y0_initialized()) {
        rel.y1 = rel.y0 + height;
        rel.h = calculate_height();
        abs.y1 = father.y + rel.y1;
    }
}
void SmartCoords::set_width_n_height(int16_t width, int16_t height){
    set_width(width);
    set_height(height);
}
