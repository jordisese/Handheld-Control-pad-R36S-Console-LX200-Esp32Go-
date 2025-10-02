#ifndef KPAD_H_INCLUDED
#include <SDL2/SDL.h>
#define COLS 5
#define ROWS 6
#define CELL_W 120
#define CELL_H 30
#define MARGIN_X 10
#define MARGIN_Y 270
typedef struct
{
    const char *label;
    SDL_Rect rect;
} MenuItem;

  void init_mat(MenuItem *items);
  void changemat(int a,MenuItem *items);
 void readpad(SDL_Event e,int *last_axis, int *sel,int bsize);
#endif
