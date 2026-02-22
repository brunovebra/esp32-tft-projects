#ifndef COORDS_H
#define COORDS_H

#include <stdint.h>
#include <Arduino.h>

#define NOT_INIT -32768

class Coords {
    public:
        int16_t x, y;
        int16_t x0 = NOT_INIT, y0 = NOT_INIT;
        int16_t x1 = NOT_INIT, y1 = NOT_INIT;
        int16_t w  = NOT_INIT, h  = NOT_INIT;
    };

class SmartCoords : public Coords{
    private:
        int16_t calculate_width();
        int16_t calculate_height();

        // Validaciones de inicialización
        bool is_x0_initialized();
        bool is_x1_initialized();
        bool is_y0_initialized();
        bool is_y1_initialized();

        bool are_x0x1_initialized();
        bool are_y0y1_initialized();

    public:
        Coords zero_display, father, abs;

        void init(int16_t _x0, int16_t _y0);

        void set_x0(int16_t _x0);
        void set_y0(int16_t _y0);
        void set_x1(int16_t _x1);
        void set_y1(int16_t _y1);

        void set_x0y0(int16_t _x0, int16_t _y0);
        void set_x1y1(int16_t _x1, int16_t _y1);

        void set_width(int16_t width);
        void set_height(int16_t height);
        void set_width_n_height(int16_t width, int16_t height);
        void print(String msg);
};

#endif // COORDS_H
