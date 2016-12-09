#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define BUFSIZE 1024

void error(char *s)
{
  extern int errno;
  printf(" FATAL ERROR (%s) (errno=%d)\n", s);
  exit(errno);
}
int main(int argc, char **argv) {
    int sockfd, portno;
    ssize_t n;
    //int serverlen, nr;
    struct sockaddr_in serveraddr;
    //struct hostent *server;
    char *hostname;
    int fd;
    useconds_t delay;
    char buf[BUFSIZE];

    /* check command line arguments */
    if (argc != 5) {
       fprintf(stderr,"usage: %s <hostname> <port> <delay in ms> <filename>\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    hostname = argv[1];
    delay = atoi(argv[3]);
    printf("The port number and hostname is %d %s\n",portno,hostname);
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("sockfd created!!!!!!!!!!!!!\n");
    if (sockfd < 0){
        error("ERROR opening socket");
    }    
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family=AF_INET;
	      serveraddr.sin_addr.s_addr=inet_addr(hostname);
	      serveraddr.sin_port=htons(portno);
        printf(" server %x:%d (%d)\n",serveraddr.sin_addr.s_addr,serveraddr.sin_port,portno);
    if ((fd = open(argv[4],O_RDONLY)) < 0) 
        error(" ERROR can't open file!\n");
    for(;;)
    {   
       n = read(fd,buf,BUFSIZE);
       //printf("Reading the file!!!!!!!!!!\n");
       //printf("Size of file!!!! %d\n",n);
      // if(n>=0)
     //  {
       sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr));
       
       printf("Sent Packet \n");
       usleep(delay * 1000);
       bzero(buf,1024);
        
       //}
       if(n<=0)
       break;
    }
     printf(" done\n");
    return 0;
}