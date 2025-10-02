#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "kpad.h"
#include "conf.h"
#include "query.h"
#include "sdl2utils.h"
#include "net.h"
#include <math.h>

TTF_Font* font1;
TTF_Font* font2;
TTF_Font* font;
//int sel_row = 0, sel_col = 0;
char buffp[200] =" ";
char buff_alt[200] =" ";
char buftarget[1024]=" ";
extern MenuItem items[ROWS*COLS];
uintptr_t sockfd;
char buf[1024]=" ";
char buf3[1024]=" ";
char inputbuffer[200]="NGC1980";
//Prototypes not declared on Headers
void drawMainScreen(SDL_Renderer *renderer,int sel_row, int sel_col) ;
void draw_pad(SDL_Renderer *renderer,int sel_row,int sel_col,TTF_Font *font,MenuItem *items);
//--------------------------------------------
void calcular_posicion(const char *nombre)
{

    struct ln_equ_posn pos;

    struct lnh_equ_posn hequ;

    struct ln_lnlat_posn observer;
    struct ln_hrz_posn hrz;
    char temp[100]="   ";

    observer.lat = 36.72; /* 55.92 N */
    observer.lng = -4.12; /* 3.18 W rintf(out);*/
    double JD = ln_get_julian_from_sys();

    if (strcmp(nombre, "Sol") == 0)
        ln_get_solar_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Mercurio") == 0)
        ln_get_mercury_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Venus") == 0)
        ln_get_venus_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Luna") == 0)
        ln_get_lunar_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Marte") == 0)
        ln_get_mars_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Jupiter") == 0)
        ln_get_jupiter_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Saturno") == 0)
        ln_get_saturn_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Urano") == 0)
        ln_get_uranus_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Neptuno") == 0)
        ln_get_neptune_equ_coords(JD, &pos);
    else if (strcmp(nombre, "Pluton") == 0)
        ln_get_pluto_equ_coords(JD, &pos);
    else if (strcmp(nombre, "<-") == 0)
    {
        inputbuffer[strlen(inputbuffer)-1]=0;
        return;
    }
    else if (strcmp(nombre, "Clr") == 0)
    {
        inputbuffer[0]=0;
        return;
    }
    else if (strcmp(nombre, "Search") == 0)
    {
        printf("BUSQUEDA\n");
        squery(inputbuffer,buffp,&pos,inputbuffer[0]);
        //return;
    }
    else if (strcmp(nombre, "SearCh") == 0)
    {
        printf("BUSQUEDA\n");

        squery(inputbuffer,buffp,&pos,'s');
        //return;
    }
    else if (strcmp(nombre, "GoTo") == 0)
    {
        //  strcat(buffp,":MS#");(SDL_Color) {0xFF,0, 0, 255}
        write(sockfd,buffp,sizeof(buffp));
        write(sockfd,":MS#",4);
        read(sockfd,temp,4);
        return;
    }
     else if (strcmp(nombre, "Sync") == 0)
    {
        //  strcat(buffp,":MS#");(SDL_Color) {0xFF,0, 0, 255}
        write(sockfd,buffp,sizeof(buffp));
        write(sockfd,":CM#",4);
        read(sockfd,temp,4);
        return;
    }
    else if (strcmp(nombre, "NGC") == 0)
    {
        strcpy(inputbuffer,nombre);
        return;
    }
    else if (strcmp(nombre, "Messier") == 0)
    {
        strcpy(inputbuffer,"M");
        return;
    }
    else if (strcmp(nombre, "IC") == 0)
        strcpy(inputbuffer,nombre);
    else if ((strlen(nombre)==1) )
    {
        strcat(inputbuffer,nombre);
        return;
    }
    else if (strcmp(nombre, "Home") == 0)
    {
        write(sockfd, ":pH#", 4);
        return;

    }
    else if (strcmp(nombre, "Park") == 0)
    {
        write(sockfd, ":hP#", 4);
        return;
    }
    else if (strcmp(nombre, "Track") == 0)
    {
        write(sockfd, ":Qw#:Mt#", 8);
        return;
    }
    else if (strcmp(nombre, "Untrack") == 0)
    {
        write(sockfd, ":Mh#", 4);
        return;
    }
    else if (strcmp(nombre, "Mode") == 0)
    {
        changemat(0,items);
        return;
    }
    else if (strcmp(nombre, "Planets") == 0)
    {
        changemat(1,items);
        return;
    }
    else if (strcmp(nombre, "Keyb") == 0)
    {
        changemat(2,items);
        return;
    }
    else if (strcmp(nombre, "Shift") == 0)
    {
        changemat(3,items);
        return;
    }
    else if (strcmp(nombre, "Ceres") == 0)
    {
        // int sel_row = 0, sel_col = 0;
        printf("Ceres no está en libnova directamente.\n");
        return;
    }
    else
    {
        printf("No calculable: %s\n", nombre);
        return;
    }

    printf("%s - RA: %.4f h, DEC: %.4f°\n", nombre, pos.ra, pos.dec);
    ln_get_equ_prec(&pos,2451545.0+9000.0,&pos);

    ln_equ_to_hequ (&pos, &hequ);

    int sig=(hequ.dec.neg==1)? -1:1;
    ln_get_hrz_from_equ (&pos, &observer, JD, &hrz);
    sprintf(buftarget,"%s RA %02d:%02d:%02.0f Dec %+03d°%02d\'%02.0f  Az  %.4f  Alt %.4f    ",inputbuffer,
            hequ.ra.hours, hequ.ra.minutes, hequ.ra.seconds, sig * hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds,fmod (hrz.az+180.0,360.0),hrz.alt);

    sprintf(buffp,":Sr%02d:%02d:%02.0f#:Sd%+03d:%02d:%02.0f#",hequ.ra.hours,
            hequ.ra.minutes, hequ.ra.seconds, sig *hequ.dec.degrees, hequ.dec.minutes, hequ.dec.seconds);


    //  sprintf (buff_alt,"Az  %.4f  Alt %.4f°    ",fmod (hrz.az+180.0,360.0),hrz.alt);
    printf ("Alt %f\n", hrz.alt);

}
//Thread function  for read esp32go  position and statusdata
int readsock(void *point)
{
    SDL_Event user_event;
    SDL_zero(user_event);  /* Create custom event to updare screen on socket receive */
    user_event.type = SDL_USEREVENT;
    user_event.user.code = 2;
    user_event.user.data1 = NULL;
    user_event.user.data2 = NULL;
    SDL_PushEvent(&user_event);
    int *sockf= point;
    int sockfd=(uintptr_t) sockf;
    int count;

    // int n=0;
    int len=0;
    while (true)
    {
        ioctl(sockfd, FIONREAD, &count);
        read(sockfd,buf,count);
        // read(sockfd,buf,100);
        len = read_esp(sockfd,buf3);
        if (len>0)
        {
            //  printf("esp %d read %d bytes from socket %d  %d\n",n++,len,sockfd,count);
            /*  len=read_altaz(sockfd,bufde);
               printf("alt %d read %d bytes from socket %d  %d\n",n++,sockfd,len,count);
              len=read_eq(sockfd,buf);
              printf("esp %d read %d bytes from socket %d  %d\n",n++,sockfd,len,count);*/

        }
        SDL_PushEvent(&user_event);
        SDL_Delay(300);
    }
    return 0;
}

//Default Screen update  only on SDL event  Updates

void drawMainScreen(SDL_Renderer *renderer,int sel_row, int sel_col)
{ int status =buf3[47]-48+buf3[48]-48;
   const char state[50];
    //SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   // sprintf(state,"%s %s %s" ,(status&1)?"Tracking":"Stop",(status&2)?"Parked":" ",(status&8)?"Slewing":" ");
  if (status&8) strcpy(state,"Slewing") ;
   else if (status&2) strcpy(state,"Parked");
   else  strcpy(state,(status&1)? "Tracking":"Stop") ;
  // sprintf(state,"%s " ,(status&1)?"Tracking":"Stop",(status&2)?"Parked":" ",(status&8)?"Slewing":" ");
    SDL_RenderClear(renderer);
    draw_pad(renderer,sel_row,sel_col,font,items);
    render_text( renderer,10,240,(char* )buftarget, font2,ORANGE);//lx200 target
    // render_text( renderer,10,240,(char* )buff_alt, font2,ORANGE);//lx200 target
    render_text(renderer,20,20,(char*) buf3,font1,RED); //ra
    render_text(renderer,20,90,(char*) buf3+11,font1,RED);//dec
    //render_text(renderer,550,150,(char*) buf3+47,font2,RED);
    render_text(renderer,500,150,(char*) state,font2,RED);
    render_text(renderer,340,20,(char*) (buf3+23),font1,REDW);//az
    render_text(renderer,325,90,(char*) (buf3+35),font1,REDW);//alt
    SDL_Rect rect= {5,5,630,140};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    render_text( renderer, 15,7,(char*) "Right ascension ",font2,RED);
    render_text( renderer, 340,7,(char*) "Azimuth ",font2,RED);
    render_text( renderer, 15,75,(char*) "Declination",font2,RED);
    render_text( renderer, 340,75,(char*) "Altitude ",font2,RED);
    render_text( renderer, 10,140,"                 ",font1,RED);
    if (strlen(inputbuffer)>0) render_text( renderer, 10,140,inputbuffer,font1,ORANGE);
    // Draw the rectangle (filled)
    SDL_RenderDrawRect(renderer, &rect);
    rect.w = 630/2;
    SDL_RenderDrawRect(renderer, &rect);
    rect= (SDL_Rect)
    {
        5,144,630,120
    };
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderPresent(renderer);

}
int main(int argc, char *argv[])
{
    int i;
    printf("argc is %d\n",argc == 2);
    for(i=0; i<argc; i++)
        printf("argv[%d] is %s\n",i,argv[i]);
//printf("argv[%d] is %s\n",i,getcwd());

    initDB();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        printf("Error SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0)
    {
        printf("Error TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() > 0)
        joystick = SDL_JoystickOpen(0);

    SDL_Window *window = SDL_CreateWindow("ESP32Go r36s",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 26);
    if (!font)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        return 1;
    }

    font1 = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 48);

    if (!font)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        exit(2);
    }
    font2 = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16);

    if (!font2)
    {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        exit(2);
    }
    //  init_mat();
    init_mat(items);

    int sel_row = 0, sel_col = 0;
    int last_haxis = 0, last_vaxis = 0;
    bool running = true;
    SDL_Event e;
    readcc();
    sockfd=initsock();
    if (sockfd>=0)
    {
//        render_text( renderer, 20,400,(char*)ESP_ADDRESS,font2);
        //  render_text( renderer, 20,400,(char*)line,font2);
        SDL_Thread *thread = SDL_CreateThread(readsock, "thread",(void *)sockfd);
        if (thread == NULL)
        {
            printf("Could not create thread! %s\n", SDL_GetError());
            return 1;
        }
    }
    else
    {
        render_text( renderer, 20,200,(char*) "Disconnected",font2,RED);
    }
    //int h,v,n;



    //unsigned int cn=0;

    while (running)

    {
        //  while (SDL_PollEvent(&e))
        if (SDL_WaitEvent(&e))
        {
            if  (e.type ==SDL_JOYHATMOTION)
            {
                switch(e.jhat.value)
                {
                    {
                    case NORTH_HAT: // Fn

                        write(sockfd, ":Mn#", 4);
                        break;
                    case SOUTH_HAT: // Start
                        write(sockfd, ":Ms#", 4);

                        break;
                    case WEST_HAT : // Fn
                        write(sockfd, ":Mw#", 4);
                        break;
                    case EAST_HAT: // Start
                        write(sockfd, ":Me#", 4);

                        break;
                    case 0: // Start
                        write(sockfd, ":Qw#:Qn#", 8);

                        break;
                    }

                }
                printf("The hat with index %d was moved to position %d.\n", e.jhat.hat, e.jhat.value);
            }
            if (e.type == SDL_QUIT) running = false;

            else if (e.type == SDL_JOYAXISMOTION)
            {
                if (e.jaxis.axis == 0)
                {
                    readpad( e,&last_haxis,&sel_col,COLS);
                }
                else if (e.jaxis.axis == 1)
                {
                    readpad( e,&last_vaxis,&sel_row,ROWS);
                }


            }
            else if (e.type == SDL_JOYBUTTONDOWN)
            {
                switch(e.jbutton.button)
                {
                case NORTH_BTN: // Fn


                    write(sockfd, ":Mn#", 4);
                    break;
                case SOUTH_BTN: // Start
                    write(sockfd, ":Ms#", 4);
                    break;
                case WEST_BTN: // Fn
                    write(sockfd, ":Mw#", 4);
                    break;
                case EAST_BTN: // Start
                    write(sockfd, ":Me#", 4);
                    break;
                case GUIDE_BTN: // Start
                    write(sockfd, ":RG#", 4);
                    render_text( renderer,530,180,(char*) "GUIDE    ",font2,RED);
                    break;
                case SLEW_BTN: // Start
                    write(sockfd, ":RS#", 4);
                    render_text( renderer, 530,180,(char*) "SLEW    ",font2,RED);
                    break;
                case CENTER_BTN: // Start
                    write(sockfd, ":RC#", 4);
                    render_text( renderer, 530,180,(char*) "CENTER ",font2,RED);
                    break;
                case FIND_BTN: // Start
                    write(sockfd, ":RM#", 4);
                    render_text( renderer, 530,180,(char*) "FIND      ",font2,RED);
                    break;
                case FN_BTN:
                    close(sockfd);
                    sockfd=initsock();
                    if (sockfd>=0)
                    {
                        render_text( renderer, 20,240,(char*) ip(),font2,RED);
                    }
                    else
                    {
                        render_text( renderer, 20,240,(char*) "Disconnected   ",font2,RED);
                    }


                    break;
                default:

                    break;
                }

                printf("Button down. Button=%d\n", e.jbutton.button);
                /*  drawText( screenSurface, (char*) buf, 48,  0,0, fgC1, bgC1);
                  SDL_UpdateWindowSurface(window);*/
                if ((e.jbutton.button == JOY1_BTN)  || (e.jbutton.button == A_BTN)) // Botón A
                    //  if (e.jbutton.button == B_BTN)   // Botón A
                {
                    const char *seleccion = items[sel_row*COLS+sel_col].label;
                    calcular_posicion(seleccion);


                }
                if (e.jbutton.button == B_BTN)   // Botón B
                {
                    running = false;
                }
            }
            else if (e.type == SDL_JOYBUTTONUP)
            {
                switch(e.jbutton.button)
                {
                case NORTH_BTN: // Fn

                    write(sockfd, ":Qn#", 4);
                    break;
                case SOUTH_BTN: // Start
                    write(sockfd, ":Qs#", 4);

                    break;
                case WEST_BTN: // Fn
                    write(sockfd, ":Qw#", 4);
                    break;
                case EAST_BTN: // Start
                    write(sockfd, ":Qe#", 4);
                    break;
                case 13: // Start
                    // quit=SDL_TRUE;
                    break;

                default:
                    break;
                }
                printf("Button up. Button=%d\n", e.jbutton.button);
            }
            else if (e.type == SDL_USEREVENT) ;//printf("User event %d\r\n",e.user.code);

        }
        drawMainScreen(renderer,sel_row,sel_col); //SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);

    }

    if (joystick) SDL_JoystickClose(joystick);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
