#ifndef DISPLAY_SDL_H
#define DISPLAY_SDL_H

#include <SDL2/SDL.h>

void display_init();
void display_exit();
void display_clear();
void display_event_loop(void (*keypress_callback)(int key, int shift));
void display_refresh();
void display_put_char(char c, int x, int y, int r, int g, int b);
void display_put_string(const char *s, int x, int y, int r, int g, int b);

#endif
