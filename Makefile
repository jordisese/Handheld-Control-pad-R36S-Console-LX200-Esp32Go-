# Makefile for R36S stuff
CFLAGS=-c -Wall -O2
LIBS = -lm
PROG = planets
INCLUDES = -I. -I/usr/include

Esptest: esp32go.c
	gcc esp32go.c kpad.c net.c query.c sdl2utils.c   -DK36S   -DLANG -g  -lm -lsqlite3  -lSDL2 -lSDL2_ttf -lGLEW -lGL -lnova -o  /roms/esp32go/esp32goESP.exec
	gcc esp32go.c kpad.c net.c query.c sdl2utils.c   -DK36S   -g  -lm -lsqlite3  -lSDL2 -lSDL2_ttf -lGLEW -lGL -lnova -o  /roms/esp32go/esp32go.exec
	
clean:
	rm -rf *o $(PROG)
