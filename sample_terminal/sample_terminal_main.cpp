#define TERMINAL_IMPLEMENTATION
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <chrono>
#include <thread>

#include "terminal.h"

int main(int argc, char *argv[]) {
    terminal_color_t r;
    r.r = 255, r.g = 0, r.b = 0;
    terminal_color_t g;
    g.r = 0, g.g = 255, g.b = 0;
    terminal_color_t b;
    b.r = 0, b.g = 0, b.b = 255;
    terminal_color_t c1[2] = {r, g};
    terminal_color_t c2[2] = {g, b};
    int w = 150;
    int h = 100;
    bool create_test_pattern = false;

    terminal_clear();
    canvas_handler_t canvas = terminal_create_canvas(w, h);
    if (create_test_pattern) {
        for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
                if (j % 2 == 0) {
                    terminal_set_pixel(canvas, i, j, c1[i % 2]);
                } else {
                    terminal_set_pixel(canvas, i, j, c2[i % 2]);
                }
            }
        }
        terminal_update(canvas);
    } else {
        auto t0 = std::chrono::high_resolution_clock::now();
        auto tc = std::chrono::high_resolution_clock::now();
        int counter = 0;
        while (tc-t0<std::chrono::seconds(10)) {
            double t = (tc-t0).count();
            for (int i = 0; i < w; i++) {
                for (int j = 0; j < h; j++) {
                    terminal_color_t c;
                    c.r = (uint8_t)((std::cos(t+i / 10.0) * std::cos(j / 10.0) + 1) / 2 * 255);
                    c.g = (uint8_t)((std::cos(t+i / 5.0) * std::sin(j / 5.0) + 1) / 2 * 255);
                    c.b = (uint8_t)((std::sin(t+i / 10.0) * std::cos(j / 5.0) + 1) / 2 * 255);
                    terminal_set_pixel(canvas, i, j, c);
                }
            }
            tc = std::chrono::high_resolution_clock::now();
            terminal_update(canvas);
            counter++;
            // std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        printf("number of updates: %d\n", counter);
    }
    printf("done\n");
    return 0;
}
// void terminal_clear();
// void terminal_destroy_canvas(canvas_handler_t c);
// /// set a 'pixel' in the canvas
// void terminal_set_pixel(canvas_handler_t c, uint16_t row, uint16_t col,
// terminal_color_t color);
// /// update the canvas in the terminal
// void terminal_update(canvas_handler_t c);
