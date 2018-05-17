#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SIZE 1024

int main(int argc,char *argv[])
{
	int psock,sock;
	char msg[SIZE];
	struct sockaddr_in addr; //internet socket
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);// All interfaces
	addr.sin_port = htons(atoi(argv[1]));
	psock = socket(AF_INET,SOCK_STREAM,0);//Stream socket
	bind(psock,(struct sockaddr *)&addr,sizeof(addr));
	listen(psock,3);//dexetai 3 paixtes client 

	strcpy(msg,"");
	while (strcmp(msg,"exit") !=0) {		//an to msg einai exit kleinei thn syndesh o server
		printf("\nwaiting for new connection (port: %s)...\n",argv[1]);
		
		sock = accept(psock,NULL,NULL);
		
		printf("connection established...\n");
		
		recv(sock, msg,SIZE,0);
		//recfrom(sock,msg,SIZE,0,NULL,NULL);

		if (strcmp(msg,"exit") !=0){
			printf("running command:[%s]\n",msg);
			system(msg);
		}
		close(sock);
		printf("\nconnect closed...\n");
	}
	close(psock);
	unlink(argv[1]);
	return 0;
}
























