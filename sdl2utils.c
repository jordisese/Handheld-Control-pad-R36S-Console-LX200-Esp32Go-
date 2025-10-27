#include "sdl2utils.h"
#include "kpad.h"
#include "conf.h"
extern TTF_Font* font1;
extern TTF_Font* font2;
extern TTF_Font* font;

void render_text(SDL_Renderer *renderer, int x, int y, const char *text, TTF_Font *font,SDL_Color textColor)
{
    if (strlen(text)==0) return;
    SDL_Color bgColor   = {0, 0, 0, 0};    // Black background

    SDL_Surface *surface = TTF_RenderUTF8_Shaded(font, text, textColor, bgColor);
    if (!surface)
    {
        fprintf(stderr, "TTF_RenderText_Shaded Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_FreeSurface(surface);  // no longer needed

    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);  // free immediately since we don’t reuse it
}
void draw_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect rect)
{
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    int tw, th;
    SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
    SDL_Rect dst = { rect.x + (rect.w - tw)/2, rect.y + (rect.h - th)/2, tw, th };
    SDL_RenderCopy(renderer, texture, NULL, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void draw_pad(SDL_Renderer *renderer,int sel_row,int sel_col,TTF_Font *font,MenuItem *items)
{
    //MenuItem* items[ROWS][COLS];
//items[0][0]=itemsp;
    SDL_Color white = {255,0,0,255};
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int index=r*COLS+c;
            if (index==sel_row*COLS+sel_col)
            {
                white=BLACK;
                SDL_SetRenderDrawColor(renderer, 255, 0,0, 255);
            }
            else
            {
                white=RED;
                SDL_SetRenderDrawColor(renderer, 50, 40, 40, 255);
            }
            SDL_RenderFillRect(renderer, &items[index].rect);
            draw_text(renderer, font, items[index].label, white, items[index].rect);

        }
    }
}



void init_fonts(void)
{

    font = TTF_OpenFont(DVSANSBOLD, 24);
    if (!font)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        return ;
    }

    font1 = TTF_OpenFont(DVSANSBOLD, 48);

    if (!font)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        return ;

    }
    font2 = TTF_OpenFont(DVSANSBOLD, 16);

    if (!font2)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        return ;

    }
}

int read_battery(void)
{
    int percent=100;
    SDL_GetPowerInfo(NULL, &percent);
    return percent;
}
