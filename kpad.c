#include "kpad.h"
#include "labels.h"
const  char *labels[ROWS * COLS*5] =
{
    PLANETS,"Messier", "NGC","IC",STARS,
    GOTO,"1","2","3",B_DEL,
    SEARCH, "4", "5","6",CLEAR,
    SYNC, "7", "8","9","+",
    TRACK, ".", "0",":","-",
    UNTRACK,HOME,CONNECT,CLOSE_CONN,ALING,

    MODE,STARS, PARK,UNPARK,ALINGC,
    GOTO,SUN,MERCURY,VENUS,MOON,
    SYNC, MARS, JUPITER,SATURN,URANUS,
    "Halt",NEPTUNE,PLUTO," "," ",
    TRACK, EAST,WEST,N_POLE,SOUTH,
    UNTRACK,ZENITH,HOME,SETHOME,ALING,

    MODE,"a","b","c","d",
    SEARCHST,"e","f","g","h",
    CLEAR,"i","j","k","l",
    " ","m","n","o","p",
    "q","r","s","t","u",
    "w","v","x","y","z",

    MODE,"Alp Umi","Alp Tau","Bet Ori","Alp AUR",
    STAR1,"Alp Ori","Alp Cmi","Alp Gem","Alp CMI",
    STAR2,"Bet Gem","Alp Leo","Alp Vir","Alp BOO",
    NORMAL_SYNC,"Alp Sco","Alp Lyr","Alp Aql","Alp Cyg",
    RESET,"Alp Psa"," "," "," ",
    SYNC,GOTO,ALINGC," "," ",

    MODE,"Polaris","Aldebara","Rigel","Capella",
    STAR1,"Betelg","Procion","Castor","Procyon",
    STAR2,"Pollux","Regulus","Spica","Arcturus",
    SYNC,"Antares","Vega","Altair","Deneb",
    RESET,"FomalH","Achenar","Mizar","Alnilam",
    NORMAL_SYNC,GOTO,ALING," "," "
};

/*   "Mode","A","B","C","D",
   "SearCh","E","F","G","h",
   "Keyb","I","J","K","L",
   "Clr","M","N","0","P",
   "Q","R","S","T","W",
   "U","V","X","Y","Z",

"A UMI","A TAU","B ORI","A AUR",
"A ORI","A CMA","A GEM","A CMI",
"B GEM","A LEO","A VIR","A BOO",
"A SCO","A LYR","A AQL","A CYG",
"A PSA




};*/

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



