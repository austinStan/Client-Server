/*
*UDP SERVER
*
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void error (char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char * argv[])
{
    int sock, length, fromlen, n;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];
    char arrayname[3];
    int answer;
    int a,b;
    int k;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
    {
        error("0pening socket");
    }
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family       = AF_INET;
    server.sin_addr.s_addr  = INADDR_ANY;
    server.sin_port         = htons(atoi(argv[1]));
    if (bind(sock, (struct sockaddr *)&server,length)<0)
    {
        error("binding");
    }
    fromlen = sizeof(struct sockaddr_in);

    /* The server keeps listening indefinitely until a packet arrives */
    while(1)
    {
        bzero(buf, 1024); //clear buffer
        n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0)
        {
         error("recvfrom");
        }

        write(1,"Received a datagram: ",21);
        write(1,buf,n); //Display received datagram


        printf("Responding to Client... \n");
        
        //Loop through and check if its a number or a string
        for(k=0;k<3;k++){

            arrayname[k]=buf[k];

        }
        if(arrayname[1]=='+'){

            int a=arrayname[0]-'0';
            int b=arrayname[2]-'0';

            answer =a+b;
            snprintf(buf,sizeof(buf),"%d \n",answer);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&from,fromlen);
        }

        else if(arrayname[1]=='-')
        {

            int a=arrayname[0]-'0';
            int b=arrayname[2]-'0';

            answer=a-b;
            snprintf(buf,sizeof(buf),"%d \n",answer);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&from,fromlen);
        }
        else if(arrayname[1]=='/'){

            int a=arrayname[0]-'0';
            int b=arrayname[2]-'0';

            answer=a/b;

            snprintf(buf,sizeof(buf),"%d \n",answer);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&from,fromlen);
        }
    
        else if(arrayname[1]=='*'){

            int a=arrayname[0]-'0';
            int b=arrayname[2]-'0';

            answer=a*b;
            snprintf(buf,sizeof(buf),"%d \n",answer);
            sendto(sock,buf,strlen(buf),0,(struct sockaddr*)&from,fromlen);
        } 


        else{        

        
        n = sendto(sock, buf, strlen(buf), 0, (struct sockaddr *)&from, fromlen); // send back message to client
            if (n < 0)
            {
                error("sendto");
            }
        }
    }
} 


