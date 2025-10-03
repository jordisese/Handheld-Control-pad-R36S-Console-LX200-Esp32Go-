#ifndef SDL2UTILS_H_INCLUDED
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_ttf.h>
//#include "kpad.h"
#define  RED (SDL_Color) {0xFF,0, 0, 255}
#define  REDW (SDL_Color) {0x00,100, 100, 255}
#define  ORANGE (SDL_Color) {0xFF,0x60, 0, 255}
#define  D_AMBER (SDL_Color) {0xFF,0xBF, 0, 255}
#define  WHITEL (SDL_Color) {0x60,0x60, 0x60, 0}
#define  BLACK (SDL_Color) {0x0,0x0, 0, 255}
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font,SDL_Color textColor);
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect rect);
//void draw_pad(SDL_Renderer *renderer,int sel_row,int sel_col,TTF_Font *font,MenuItem *items);
#endif
