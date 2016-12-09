\*
Abhishek Madiraju
Parthdevsinh Rathod
Jay Desai
Revathi Panneer Selvam
*/
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
        int sockfd, newsockfd, port,clilen;
        char buff[100000];
        char *buffptr=buff;
        char res[200];
        char user[20]="USER anonymous\r\n";
        char password[17]="PASS blahblah\r\n";
//        char retr[100]="RETR ";
        char userresp[999999];
	char passresp[10000];
	char pasvresp[10000];
	char size[20]="SIZE ";
	char path[100];
	char dres[200];
	char *httpresp="HTTP/1.1 200 OK\r\n"
	"Date: Tue, 25 Oct 2016 2:4:25 GMT\r\n"
	"Server: Apache/2.2.14 (Win32)\r\n"
	"Last-Modified: Tue, 24 May 2016 20:15:59 GMT\r\n" 
	"Content-Type: application/octet-stream; charset=binary\r\n"
	"Connection: Closed\r\n"
	"\r\n";
        char pasv[10]="PASV\r\n";
        char buffer[1000];
        int hostsockfd, newhostsockfd,cmdsockfd,datasockfd;
        struct servent *portptr;
        struct in_addr **addr_ip;
        struct sockaddr_in serv_addr, cli_addr;
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
                else { printf("\nbuffer %s\n",buff); }
                bzero(res,200);
                int i=0,j=0;
		int k=0,l=0,m=0,n=0;
                char proto[10];
		int p=0;
                int slashcnt=0;
                /*Parsing HTTP request to get hostname */
                while(slashcnt<2)
                {
                    if(buffptr[i]==' ')
                    {
			i++;
			while(buffptr[i]!=':')
			{
                           proto[p] = buffptr[i];
			   i++;
			   p++;
			}
                    }
                    if(buffptr[i]=='/')
                    {
                         slashcnt=slashcnt+1;
                    }
                                           
                    i=i+1;
                 }
		char portu[6];
		bzero(path,99);
                while(buffptr[i]!='/')
                {    


			res[j]=buffptr[i];
                        i=i+1;
                        j=j+1;
                }
                res[j]='\0';
		if(buffptr[i]=='/')
		{	path[k]=buffptr[i];
			i=i+1;
			k=k+1;
			while(buffptr[i]!=' ')
			{
				path[k]=buffptr[i];
				i=i+1;
				k=k+1;
			}
		}
		
		n=0;
		while(n<(strlen(res)) && res[n]!=':')
		{
			dres[m]=res[n];
			n=n+1;
			m=m+1;
		}
		
		while(n<strlen(res))
		{
			n=n+1;
			portu[l]=res[n];
			l=l+1;
		}
		p=p+1;	
		path[k]='\0';
		portu[l]='\0';
		proto[p]='\0';
                printf("Protocol: %s\n",proto);
                printf("\nThe host name is: %s\n",dres);
		printf("The path is: %s\n",path);
		printf("The port is: %s\n",portu);
		char retr[100]="RETR ";
		char size[20]="SIZE ";
		strcat(retr,path);
                struct hostent *hname;
                /*Get the ip address of the host */
                if((hname=gethostbyname(dres))==NULL)
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
                //struct sockaddr_in cmd_addr;
                //bzero((char*)&cmd_addr,sizeof(host_addr));
                if((strstr(proto,"ftp")))
                {
                       int comma=0;
		       char port1[4],port2[4];
		       int dataport;
		       int p1=0,p2=0;
			char dest[5];
		       int x=0,y=0,z=0,userread=0,passread=0;
			int a=0,b=0;
	               portptr=getservbyname("ftp","tcp");
                       printf("Port: %d\n",ntohs(portptr->s_port));
		       printf("RETR: %s\n",retr);
                       struct sockaddr_in cmd_addr;
                       bzero((char*)&cmd_addr,sizeof(cmd_addr));
                       cmd_addr.sin_port=htons(21);
                       cmd_addr.sin_family=AF_INET;
                       cmd_addr.sin_addr.s_addr=inet_addr(ip);
                       cmdsockfd=socket(AF_INET,SOCK_STREAM,0);
                       connect(cmdsockfd,(struct sockaddr*)&cmd_addr,sizeof(struct sockaddr));
		       bzero(userresp,999999);
			while(1)
                       {
                               read(cmdsockfd,userresp,sizeof(userresp));
                                printf(userresp);
                               if(strstr(userresp,"220"))
                                {
                                        printf("Abhi");
                                        break;
                                }
                        }

		       //read(cmdsockfd,userresp,999999);
		       //printf(userresp);
                       send(cmdsockfd,user,strlen(user),0);
		       bzero(userresp,999999);
		       while(1)
		       {         
		       	       read(cmdsockfd,userresp,sizeof(userresp));
				printf(userresp);
			       if((strstr(userresp,"331")) || (strstr(userresp,"230")))
				{
					break;
				}
			}
		       bzero(userresp,999999);
		       send(cmdsockfd,password,strlen(password),0);
	               while(1)
                       {
                               read(cmdsockfd,userresp,sizeof(userresp));
				printf(userresp);
                               if(strstr(userresp,"230"))
                                {
                                        
                                        break;
                                }
                        }
		       //read(cmdsockfd,userresp,999999);
		       send(cmdsockfd,pasv,strlen(pasv),0);
		       bzero(userresp,999999);
			 while(1)
                       {
                               read(cmdsockfd,userresp,sizeof(userresp));
				printf(userresp);
                               if(strstr(userresp,"227"))
                                {
                                        break;
                                }
                        }

		       //read(cmdsockfd,userresp,999999);
                       while(x!=strlen(userresp))
 		       {
				if(userresp[x]==',')
				{
					comma=comma+1;
				}
				if(comma==4)
				{
					x++;
					while(userresp[x]!=',')
					{
						port1[y]=userresp[x];
						x++;
						y++;
					}
					comma++;
					x++;
					while(userresp[x]!=')')
					{
						port2[z]=userresp[x];
						x++;
						z++;
					}
				}
				x++;
			}			
			printf("Port1: %s\n",port1);
			printf("Port2: %s\n",port2);
			p1=atoi(port1);
			p2=atoi(port2);
			printf("P2: %d\n",p1);
			printf("P2: %d\n",p2);
			dataport=p1*256+p2;
			printf("Datapoprt is: %d\n",dataport);
			struct sockaddr_in data_addr;
			bzero((char*)&data_addr,sizeof(data_addr));
			data_addr.sin_port=htons(dataport);
			data_addr.sin_family=AF_INET;
			data_addr.sin_addr.s_addr=inet_addr(ip);
			datasockfd=socket(AF_INET,SOCK_STREAM,0);
			connect(datasockfd,(struct sockaddr*)&data_addr,sizeof(struct sockaddr));
			strcat(size,path);
			strcat(size,"\r\n");
                        printf(size);
                        send(cmdsockfd,size,strlen(size),0);
			bzero(userresp,999999);
                        read(cmdsockfd,userresp,999999);
                        printf(userresp);
			strcat(retr,"\r\n");
			printf("retrieve cmd: %s",retr);
			send(cmdsockfd,retr,strlen(retr),0);
			printf("retrieve cmd: %s",retr);
			bzero(userresp,4999);
			read(cmdsockfd,userresp,999999);
			printf("RETR: %s\n",userresp);
			bzero(userresp,999999);
			printf("%s\n",userresp);
			bzero(userresp,999999);
			n=0;
			i=0;
			send(newsockfd,httpresp,strlen(httpresp),0);
			while((n=read(datasockfd,userresp,999999))>0)
			{
				send(newsockfd,userresp,n,0);
				i=i+n;
			}
			printf("THE SIZE RECIEVED is: %d\n",i);
                        bzero(userresp,999999); 
			send(cmdsockfd,"QUIT\r\n",10,0);
			read(cmdsockfd,userresp,999999);
			printf(userresp);
			bzero(retr,strlen(retr));
			bzero(port1,strlen(port1));
			bzero(port2,strlen(port2));
			bzero(size,strlen(size));
			close(newsockfd);
                }

                /*Get the port number on which the proxy has to send the request */
		else if((strstr(proto,"http"))){
			
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
               		printf(buff); 
			send(hostsockfd,buff,strlen(buff),0);
			//printf("HELLOOOOO\n");
              		  /* Send the browser request to host */
               		 bzero(buff,strlen(buff));
               		 /* Recieve host response at proxy server */
               		 read(hostsockfd,buff,1000000);
               		 /* Send the response from proxy to browser */
				send(newsockfd,buff,strlen(buff),0);
				
			
			 
		}
        }
        close(newhostsockfd);
        close(hostsockfd);
     	close(newsockfd);
        close(sockfd);
}
