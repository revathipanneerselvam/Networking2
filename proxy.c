#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
void error(char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	int sockfd, newsockfd,port, clilen;
	char buff[100000];
	char *buffptr=buff;
	int i=0,slashcnt=0,j=0;
	char res[200];
	char buffer[1000];
	int hostsockfd, newhostsockfd;
	struct servent *portptr;
	struct in_addr **addr_ip;
	struct sockaddr_in serv_addr, cli_addr;
	;
	if(argc < 2)
	{
		printf("Please enter the port number\n");
	}
	if(argc < 2)
	{
		printf("Please enter the port number\n");
	}
	if(argc < 2)
	{
		printf("Please enter the port number\n");
		exit(1);
	}

	/* Create new socket to connect client to proxy */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		error("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	port = atoi(argv[1]);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=INADDR_ANY;
	serv_addr.sin_port=htons(port);
	
	/* Bind the socket to a local address */	

	if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
	{
		error("Error on binding\n");
	}
	printf("Binding done on port %d\n", port);
	listen(sockfd,5);
	printf("Listening\n"); fflush(stdout);
	for( ; ; )
	{
		clilen=sizeof(cli_addr);
		newsockfd=accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if(newsockfd<0)
		{
			error("ERROR on accept");
		}
		printf("New Socket\n");
		bzero(buff,100000);	
		if(read(newsockfd,buff,1025)<0)
		{
			printf("Error reading from socket\n");
		}
		bzero(res,200);

		/*Parsing HTTP request to get hostname */
		
		for(i=0;i<strlen(buff);i++)
		{
			if(buffptr[i]=='\/')			
			{
				slashcnt=slashcnt+1;
			}
			if(slashcnt==2)
			{
				while(buffptr[i]!=' ')
				{
					i=i+1;
					if(buffptr[i]=='/')
					{
						res[j]='\0';
						goto printslash;
					}
					else
					{
						res[j]=buffptr[i];
					//	printf("%c",res[j]);
						j=j+1;
				}	}	
			}
		}
		printslash:
		printf("\n");
		printf("The host name is: %s\n",res);
		struct hostent *hname;
		/*Get the ip address of the host */	
		if((hname=gethostbyname(res))==NULL)
		{
			herror("gethostbyname");
			return 2;
		} 
		addr_ip=(struct in_addr **)hname->h_addr_list;
		char ip[20];
		strcpy(ip,inet_ntoa(*addr_ip[0]));
		printf("The ip address is: %s\n",ip);
		bzero((char*)buffer,1000);					
		strcpy(buffer,buff);

		/*Get the port number on which the proxy has to send the request */

		portptr=getservbyname("http","tcp");
		printf("Port: %d\n",ntohs(portptr->s_port));

		/* Create a new socket to connect proxy to host */

		struct sockaddr_in host_addr;
		bzero((char*)&host_addr,sizeof(host_addr));
		host_addr.sin_port=htons(80);
		host_addr.sin_family=AF_INET;
		host_addr.sin_addr.s_addr=inet_addr(ip);
		hostsockfd=socket(AF_INET,SOCK_STREAM,0);
		newhostsockfd=connect(hostsockfd,(struct sockaddr*)&host_addr,sizeof(struct sockaddr));
		send(hostsockfd,buffer,strlen(buffer),0);

		/* Send the browser request to host */

		bzero((char*)buff,100000);
		/* Recieve host response at proxy server */
		recv(hostsockfd,buff,100000,0);
				
		/* Send the response from proxy to browser */		

		send(newsockfd,buff,100000,0);		
	}
	close(newhostsockfd);
	close(hostsockfd);
	close(newsockfd);
	close(sockfd);
}	
