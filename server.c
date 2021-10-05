#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "miracl.h"   /* include MIRACL system */
#include <math.h>
#define MAX 1000
#define PORT 8081
#define SA struct sockaddr

int state = 0;

void strip(char *name) { 
    /* strip off filename extension */
    int i;
    for (i=0; name[i]!='\0'; i++)
    {
        if (name[i]!='.') continue;
        name[i]='\0';
        break;
    }
}

static void hashing(FILE *fp,big c_hash) {
	/* compute hash function */
    char h[20];
    int i,ch;
    sha sh;

    shs_init(&sh);

    while ((ch=fgetc(fp))!=EOF)
        shs_process(&sh,ch);
    
    shs_hash(&sh,h);
    bytes_to_big(20,h,c_hash);
}

miracl *mip;

int verify(char sig[])
{  
    
    /*  encode using public key, e = 3, N = 'taken from file'  */
    big N,e,s,c_hash,s_hash,t;

    FILE *ifile;

    mip=mirsys(100,0);

    N=mirvar(0);
    e=mirvar(0);
    s=mirvar(0);
    c_hash=mirvar(0);
    s_hash=mirvar(0);
    t=mirvar(0);


    /*Read Public Parameters from file public.key*/
    ifile=fopen("public.key","rt");
    mip->IOBASE=16;
    cinnum(N,ifile);
    cinnum(e,ifile);
    fclose(ifile);

    
    ifile=fopen("genkey.c","rt");

    mip->IOBASE=128;

    hashing(ifile,c_hash);
    fclose(ifile);

    copy(N,t);
    divide(c_hash,t,t);
    
    mip->IOBASE=16;

    cinstr(s,sig);
    powmod(s,e,N,s_hash);

    if (mr_compare(s_hash,c_hash) == 0) {
        return 1;
    } else {
		return 0;
	}
}

// Function designed for chat between client and server.
void func(int sockfd) {
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		read(sockfd, buff, sizeof(buff));

		
		if(state==0) {
			state = 1;
			bzero(buff, MAX);
			n = 0;
			strcpy(buff, "Please enter the signature:");
			write(sockfd, buff, sizeof(buff));
			
		} else {
			static char sig[1000];
			strcpy(sig, buff);
			if(verify(sig)==1)
				strcpy(buff, "Verification Success! Authenticated!");
			else 
				strcpy(buff, "Verification Failed! Rejected.");
			write(sockfd, buff, sizeof(buff));
			printf("%s", buff);
			int test;
			scanf("%d", &test);
		}

		// if msg contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server Exit...\n");
			break;
		}
	}
}

// Driver function
int main() {
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("Server accept failed...\n");
		exit(0);
	}
	else
		printf("Client joined...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sockfd);
}
