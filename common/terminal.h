#pragma once

#include <stdlib.h>
#include <stdint.h>

// this library contains functions to work with a text terminal. 
//
// types:
// canvas_t

/// color
typedef struct {
    uint8_t r; ///< red component (0..255)
    uint8_t g; ///< green component (0..255)
    uint8_t b; ///< blue component (0..255)
    uint8_t a; ///< alpha component (0..255) - UNUSED
} terminal_color_t;

// opaque type representing the buffer used to set colors
typedef void* canvas_handler_t;


void terminal_clear();
canvas_handler_t terminal_create_canvas(uint16_t width, uint16_t height);
void terminal_destroy_canvas(canvas_handler_t c);
/// set a 'pixel' in the canvas
void terminal_set_pixel(canvas_handler_t c, uint16_t row, uint16_t col, terminal_color_t color);
/// update the canvas in the terminal
void terminal_update(canvas_handler_t c);

#define TERMINAL_IMPLEMENTATION
#ifdef TERMINAL_IMPLEMENTATION

#include <string.h>
#include <stdio.h>
#include <unistd.h>

// we have to build this string for each vertical pair of pixels: "\033[38;2;128;100;255;48;2;255;255;010m▀"
// that has the following format: "ESC[ 38;2;⟨r⟩;⟨g⟩;⟨b⟩;48;2;⟨r⟩;⟨g⟩;⟨b⟩m▀"
//                                      |______________| |______________| ^_____________
//                                       foreground clr   background clr   top half char
#define TERMINAL_TEMPLATE_STR         "\033[38;2;rrr;ggg;bbb;48;2;rrr;ggg;bbbm▀\033[0m"
#define TERMINAL_TEMPLATE_STR_SPRINTF "\033[38;2;%03u;%03u;%03u;48;2;%03u;%03u;%03um▀\033[0m"
#define PIXEL_CHAR "▀"
#define TERMINAL_CLEAR    "\033c"
#define TERMINAL_RESET  "\033[0m"           // reset terminal settings
#define TERMINAL_CURSOR_UP_FORMAT "\033[%dA"  // Move cursor up given lines.
#define TERMINAL_CURSOR_POS_FORMAT "\033[%d;%dH"  // Move cursor up given lines.


typedef struct {
    uint16_t width;
    uint16_t height;
    terminal_color_t *pixels;
    char *term_string;
} canvas_t;

void terminal_clear() {
    ssize_t ret = write(STDOUT_FILENO, TERMINAL_CLEAR, strlen(TERMINAL_CLEAR));
    (void)ret; // ignore the return value, but suppress the compiler warning
}


canvas_handler_t terminal_create_canvas(uint16_t width, uint16_t height) {
    canvas_t *c = (canvas_t*)malloc(sizeof(canvas_t));
    // since every char represents 2 vertical pixels, make sure the height is an even number
    uint16_t height_padded = 2*((height+1)/2);
    c->width = width;
    c->height = height_padded;
    // allocate space for the pixels
    c->pixels = (terminal_color_t*)malloc(width*height_padded*sizeof(terminal_color_t));
    memset(c->pixels, 0, width*height_padded*sizeof(terminal_color_t));
    // allocate space for the string
    const uint16_t tmpl_len = strlen(TERMINAL_TEMPLATE_STR);
    c->term_string = (char*)malloc(width*height_padded/2*tmpl_len*sizeof(char) + height_padded/2 + 1);
    char *pos = c->term_string;
    for (int j=0; j<height_padded/2; j++) {
        // printf("%ld\n", pos-c->term_string);
        for (int i=0; i<width; i++) {
            sprintf(pos, TERMINAL_TEMPLATE_STR_SPRINTF, 0, 0, 0, 0, 0, 0);
            pos+=tmpl_len;
        }
        // printf(" %ld\n", pos-c->term_string);
        sprintf(pos, "%c", '\n');
        ++pos;
        // printf("  %ld\n", pos-c->term_string);
    }
    *pos = 0;
    // printf("%d %d %d\n", tmpl_len, (int)strlen(TERMINAL_TEMPLATE_STR), (int)strlen(PIXEL_CHAR));
    return (canvas_handler_t)c;
}

void terminal_destroy_canvas(canvas_handler_t c_) {
    canvas_t *c = (canvas_t*)c_;
    free(c->pixels);
    free(c->term_string);
    free(c);
}

void terminal_set_pixel(canvas_handler_t c_, uint16_t i, uint16_t j, terminal_color_t color) {
    canvas_t *c = (canvas_t*)c_;
    c->pixels[j*c->width+i] = color;
}

void terminal_update(canvas_handler_t c_) {
    canvas_t *c = (canvas_t*)c_;
    const uint16_t tmpl_len = strlen(TERMINAL_TEMPLATE_STR);
    char tmp[128];
    for (int k=0; k<c->width*c->height/2; k++) {
        uint16_t i = k%c->width;
        uint16_t j1 = 2*(k/(c->width));
        uint16_t j2 = j1+1;
        // char *pos = c->term_string + k*tmpl_len;
        char *pos = c->term_string + j1/2*(c->width*tmpl_len+1) + i*tmpl_len;
        terminal_color_t c1 = c->pixels[j1*c->width+i];
        terminal_color_t c2 = c->pixels[j2*c->width+i];
        sprintf(tmp, TERMINAL_TEMPLATE_STR_SPRINTF, c1.r, c1.g, c1.b, c2.r, c2.g, c2.b); // optimization: instead of the entire string we could overwrite only the numbers
        memcpy(pos, tmp, strlen(tmp));
    }
    // move the cursor to a fixed pos
    // sprintf(tmp, TERMINAL_CURSOR_UP_FORMAT, c->height/2);
    sprintf(tmp, TERMINAL_CURSOR_POS_FORMAT, 0,0);
    ssize_t ret = write(STDOUT_FILENO, tmp, strlen(tmp));
    ret = write(STDOUT_FILENO, c->term_string, strlen(c->term_string));//c->width*c->height/2*tmpl_len);
    (void)ret;
    // printf("\n");
}
#endif

