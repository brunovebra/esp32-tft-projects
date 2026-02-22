#include <stdint.h>
#include "../include/coords/coords.h"

/**
 * 
 * SmartCoords class
 * 
 */

/* Private methods*/
bool SmartCoords::is_x0_initialized() { return x0 != NOT_INIT; }
bool SmartCoords::is_x1_initialized() { return x1 != NOT_INIT; }
bool SmartCoords::is_y0_initialized() { return y0 != NOT_INIT; }
bool SmartCoords::is_y1_initialized() { return y1 != NOT_INIT; }

// bool SmartCoords::are_x0x1_initialized(){ return is_x0_initialized() && is_x1_initialized(); }
// bool SmartCoords::are_y0y1_initialized(){ return is_y0_initialized() && is_y1_initialized(); }

/* Public methods*/

void SmartCoords::init(int16_t _x0, int16_t _y0){
    zero_display.x0 = 0;
    zero_display.y0 = 0;

    father.x = _x0;
    father.y = _y0;
}


void SmartCoords::set_x0(int16_t _x0) {
    x0 = _x0;
    abs.x0 = father.x + x0;

    if (is_x1_initialized()) {
        w = calculate_width();
    }
}
void SmartCoords::set_y0(int16_t _y0) {
    y0 = _y0;
    abs.y0 = father.y + y0;

    if (is_y1_initialized()) {
        h = calculate_height();
    }
}
void SmartCoords::set_x1(int16_t _x1) {
    x1 = _x1;
    abs.x1 = father.x + x1;

    if (is_x0_initialized()) {
        w = calculate_width();
    }
}
void SmartCoords::set_y1(int16_t _y1) {
    y1 = _y1;
    abs.y1 = father.y + y1;

    if (is_y0_initialized()) {
        h = calculate_height();
    }
}


void SmartCoords::set_x0y0(int16_t _x0, int16_t _y0) {
    set_x0(_x0);
    set_y0(_y0);
}

void SmartCoords::set_x1y1(int16_t _x1, int16_t _y1) {
    set_x1(_x1);
    set_y1(_y1);
}

int16_t SmartCoords::calculate_width(){ return x1 - x0; }
int16_t SmartCoords::calculate_height(){ return y1 - y0; }

void SmartCoords::set_width(int16_t width){
    if (is_x0_initialized()) {
        x1 = x0 + width;
        w = calculate_width();
        abs.x1 = father.x + x1;
    }
}
void SmartCoords::set_height(int16_t height){
    if (is_y0_initialized()) {
        y1 = y0 + height;
        h = calculate_height();
        abs.y1 = father.y + y1;
    }
}
void SmartCoords::set_width_n_height(int16_t width, int16_t height){
    set_width(width);
    set_height(height);
}

void SmartCoords::print(String msg){
    Serial.printf("\n-%s-\nx0y0: %d, %d\nx1y1: %d, %d\nwh: %d, %d\n", msg.c_str(), x0,x0,y0,y1,w,h);
}
