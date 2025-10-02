#ifndef NET_H_INCLUDED
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include "conf.h"
int read_esp(int sockfd,char* buffer);
int read_eq(int sockfd,char* buffer);
int read_altaz(int sockfd,char* buffer);
int initsock();
int readcc(void);
char* ip(void);
#endif
