/*
*UDP CLIENT
*
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>




void error (char *);

int main(int argc, char * argv[])
{

    /******** DECLARATIONS ***********/
    int sock, length, n, m;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256]; //for the sent message
    char msg[256]; //for the return message

    m = 0;
    

    if (argc != 3)
    {
        printf("Usage: server port\n");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        error("socket");
    }

    server.sin_family       = AF_INET;
    hp = gethostbyname(argv[1]);

    if (hp == 0)
    {
        error("Unknown host");
    }

    bcopy((char *)hp -> h_addr, (char *)&server.sin_addr, hp -> h_length);
    server.sin_port = htons(atoi(argv[2]));
    length = sizeof(struct sockaddr_in);
 
    printf("Please enter the message: ");

    //read data for sending to the server
    bzero(buffer, 256);
    fgets(buffer, 256, stdin);

 
    printf("Sending message to server...\n");

    n = sendto(sock, buffer, strlen(buffer), 0, &server, length); //send to server
    if (n < 0)
    {
       error("Sendto");
    }
    

    if(anyThingThere(sock)>0){
    m = recvfrom(sock, msg, 256, 0, &from, &length);  
    //display message from server
    write(1, msg, m);

    }else if(anyThingThere(sock)==0){   
    /****** TIMING *********/
        for(int x=0; x<3; x++)
        {
            printf("Resending message to server...\n");     
            n = sendto(sock, buffer, strlen(buffer), 0, &server, length); //resend to server once
            usleep(2000000);
        }              
                 
    }else { 
            error("Sendto"); 
    }
            
              

 
}


/* use select to test whether there is any input on descriptor s*/
int anyThingThere(int s)
{
	unsigned long read_mask;
	struct timeval timeout;
	int n;

	timeout.tv_sec = 3; /*seconds wait*/
	timeout.tv_usec = 0;        /* micro seconds*/
	read_mask = (1<<s);
	if((n = select(32, (fd_set *)&read_mask, 0, 0, &timeout))<0)
		perror("Select fail:\n");
	return n;
}

