#ifndef NET_H_INCLUDED
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <libnova/ln_types.h>
#include "conf.h"
int read_esp(int sockfd,char* buffer);
int read_eq(int sockfd,char* buffer);
int read_altaz(int sockfd,char* buffer);
int sendCmd(int sockfd, const  char* cmd);
int initsock();
int readcc(void);
char* ip(void);
int read_geo(int sockfd);
#endif
