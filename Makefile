# Makefile for R36S stuff
CFLAGS=-c -Wall -O2 
LIBS = -lm
PROG = planets
INCLUDES = -I. -I/usr/include

Esptest: planetasl.c
	gcc planetasl.c  -g -lSDL2 -lSDL2_ttf -lGLEW -lGL -lnova -o joy
	
clean:
	rm -rf *o $(PROG)
