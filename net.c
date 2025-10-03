#include "net.h"
#define PORT 10001
char  line[30] = ESP_ADDRESS;

int readcc(void)
{
    FILE * fp;
    //  char * line = NULL;
    //  size_t len = 0;
    //  ssize_t read;
#ifdef K36S
    fp = fopen("/roms/esp32go/conf.cfg", "r");
#else
    fp = fopen("conf.cfg", "r");
#endif
    if (fp == NULL)
        if (fp == NULL)
            return -1;


    fgets(line,sizeof(line),fp);
    printf("%s", line);
    fclose(fp);
    //  if (line)
//       free(line);
    // exit(EXIT_SUCCESS);
    return 0;
}
int initsock()
{
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int sockfd;
    struct sockaddr_in servaddr;
    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        printf("socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr =inet_addr(ESP_ADDRESS);// htonl(INADDR_LOOPBACK);
    servaddr.sin_addr.s_addr =inet_addr(line);
    printf("%s", line);

    servaddr.sin_port = htons(PORT);
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);


    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
    {
        printf("connection with the server failed...\n");
        //exit(0);
    }
    else
    {
        printf("connected to the server..\n");
    }

    return sockfd;
}

int read_esp(int sockfd,char* buffer)
{
    char  buffx[100]=":Gx#";

    write(sockfd, buffx, sizeof(buffx));
    int len = read(sockfd,buffx,99);
    if (len==49)

    {
        //printf("%s\n",buffx);



        buffx[10]=buffx[30]=0;
        buffx[20]= buffx[40]=buffx[14]=0;

        buffx[24]=buffx[34]=0;
        buffx[17]=buffx[27]=buffx[37]='\'';
        buffx[48]=0;
        //render_text(renderer,10,200,(char*) buf3,font2);
        sprintf(buffer,"%s %s°%s\" %s°%s\" %s°%s\" %c%c %s",buffx,buffx+11,buffx+15,buffx+21,buffx+25,buffx+31,buffx+35,buffx[41],buffx[42],buffx+43);
        // printf("%s\n",buffer);
        buffer[22]= buffer[46]=buffer[10]=buffer[34]=0;
        // printf("%s°%s\" %s°%s°%s\"",buffx,buffx+15,buffx+21,buffx+25,buffx+35);
    }
    else len=-1;
    return len;

}

int read_eq(int sockfd,char* buffer)
{
    char  buffx[]=":GR#:GD#";
    write(sockfd, buffx, sizeof(buffx));

    int len = read(sockfd,buffer,70);
    if (len>0)
    {
        // len = read(sockfd,buffer,1024);
        buffer[10]=buffer[20]=' ';
        buffer[14]=':';
        buffer[17]='"';
        buffer[20]=0;

    }
    else len=-1;
    return len;

}


int read_altaz(int sockfd,char* buffer)
{
    char  buffx[]= ":GZ#:GA#";
    write(sockfd, buffx, sizeof(buffx));

    int len = read(sockfd,buffer,70);
    if (len>0)
    {

        buffer[9]=buffer[19]=' ';
        buffer[3]=buffer[13]=':';
        buffer[19]=0;
    }
    else len=-1;
    return len;

}
char* ip(void){
return line;};

int sendCmd(int sockfd, const  char* cmd)
{
int len=strlen(cmd);
if (len>0)
 write(sockfd, cmd,len );
 }
