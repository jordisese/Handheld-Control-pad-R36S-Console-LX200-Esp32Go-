#include "kpad.h"

const  char *labels[ROWS * COLS*4] =
{
    "Planets","Messier", "NGC","IC","Keyb",
    "GoTo","1","2","3","<-",
    "Search", "4", "5","6","Clr",
    "Sync", "7", "8","9","+",
    "Track", ".", "0",":","-",
    "Untrack","Home"," "," "," ",

    "Mode","Keyb", "Park","UnPark","Keyb",
    "GoTo","Sol","Mercurio","Venus","Luna",
    "Sync", "Marte", "Jupiter","Saturno","Urano",
    "Halt", "Neptuno","Pluton","+"," ",
    "Track", "East","West","NCP","South",
    "UnTrack","Zenith","MW","ME"," ",

    "Mode","a","b","c","d",
    "SearCh","e","f","g","h",
    "Clr","i","j","k","l",
    "Shift","m","n","o","p",
    "<-","q","r","s","t",
    "u","v","x","y","z",

    "Mode","A","B","C","D",
    "SearCh","E","F","G","h",
    "Keyb","I","J","K","L",
    "Clr","M","N","0","P",
    "<-","Q","R","S","T",
    "U","V","X","Y","Z",






};

MenuItem items[ROWS*COLS];

void init_mat(MenuItem *items)
{
    int idx = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int index=r*COLS+c;
            items[index].label = labels[idx++];
            items[index].rect.x = MARGIN_X + c * (CELL_W + 5);
            items[index].rect.y = MARGIN_Y + r * (CELL_H + 5);
            items[index].rect.w = CELL_W;
            items[index].rect.h = CELL_H;
        }
    }
}
/*
void changemat(int a)
{
    int idx = 0+a*25;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            items[r][c].label = labels[idx++];

        }
    }
}
*/
void changemat(int a,MenuItem *items)
{
    int idx = 0+a*ROWS*COLS;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            int index=r*COLS+c;
            items[index].label = labels[idx++];

        }
    }
}


void readpad(SDL_Event e,int *last_axis, int *sel,int bsize)
{

    if (e.jaxis.value < -28000 && *last_axis != -1)
    {
        *sel = (*sel - 1 + bsize) % bsize;
        *last_axis = -1;
    }
    else if (e.jaxis.value > 28000 && *last_axis != 1)
    {
        *sel = (*sel + 1) % bsize;
        *last_axis = 0;
    }
    else if (e.jaxis.value > -18000 && e.jaxis.value < 18000)
    {
        *last_axis = 0;
    }


}



