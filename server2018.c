#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024


typedef struct 
{
   int health;
   int armor;
   int attack;
   float accurasy;
   int level;

}human_stats;

int main(int argc,char *argv[])
{
	int psock,sock,i;
	int sock1;
	human_stats h1;	
	int sphlia[50];
	char username[30];
	char password[30];
	struct sockaddr_in addr; //internet socket
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr=htonl(INADDR_ANY);// All interfaces
	addr.sin_port = htons(atoi(argv[1]));
	psock = socket(AF_INET,SOCK_STREAM,0);//Stream socket
	bind(psock,(struct sockaddr *)&addr,sizeof(addr));
	listen(psock,3);//dexetai 3 paixtes client 

	
	printf("\n waiting for new connection (port: %s)...\n",argv[1]);
	sock = accept(psock,NULL,NULL);
	sock1 = accept(psock,NULL,NULL);
		
	printf("connection established...\n");
	//for(i=0;i<50;i++){
	//	sphlia[i]=0;
	//}
	
	for(i=0;i<50;i++){
		
		recv(sock,sphlia,SIZE,0);
		//recfrom(sock,msg,SIZE,0,NULL,NULL);
		printf("received:%d\n",sphlia[i]);
		
	}
	close(sock);

	recv(sock1,username,SIZE,0);
	recv(sock1,password,SIZE,0);
	
	printf("\nusername: %s\n",username);
	printf("\npassword: %s\n",password);

	recv(sock1,&h1.health,SIZE,0);
	recv(sock1,&h1.armor,SIZE,0);
	recv(sock1,&h1.attack,SIZE,0);
	recv(sock1,&h1.accurasy,SIZE,0);
	recv(sock1,&h1.level,SIZE,0);

	printf("%d\n", h1.health);
	printf("%d\n", h1.armor);
	printf("%d\n", h1.attack);
	printf("%f\n", h1.accurasy);
	printf("%d\n", h1.level);

	printf("\n connection closed...\n");
	close(sock1);
	close(psock);
	unlink(argv[1]);
	
	
	
	return 0;
}



