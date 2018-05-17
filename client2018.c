#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024

int main(int argc, char *argv[])
{
	int sock,i;
	char msg[SIZE];
	struct sockaddr_in addr;//Internet socket
	
	addr.sin_family = AF_INET;
	inet_aton(argv[1],&addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));
	sock = socket(AF_INET,SOCK_STREAM,0);//Stream socket
	connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	
	strcpy(msg,"");
	for(i=3;i<argc;i++){
		strcat(msg,argv[i]);
		strcat(msg," ");
	}
	msg[strlen(msg)-1]='\0'; //Remove the last space from string
	
	printf("sending command : %s\n",msg);
	
	send(sock,msg,strlen(msg)+1,0);
	//sendto(sock,msg,strlen(msg)+1,0,NULL,0);
	close(sock);
	return 0;
}
















