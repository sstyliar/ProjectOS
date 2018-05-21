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



int main(int argc, char *argv[])
{
	int sock,i;
	int pass_temp = 1234;
	int sock1;
	human_stats h1;
	int sphlia[50];
	char username[30]="Vasiliki";
	char password[30];
	sprintf(password, "%d", pass_temp);
	struct sockaddr_in addr;//Internet socket
	
	addr.sin_family = AF_INET;
	inet_aton(argv[1],&addr.sin_addr);
	addr.sin_port = htons(atoi(argv[2]));

	sock = socket(AF_INET,SOCK_STREAM,0);//Stream socket
	sock1 = socket(AF_INET,SOCK_STREAM,0);//Stream socket
	connect(sock,(struct sockaddr *)&addr,sizeof(addr));
	connect(sock1,(struct sockaddr *)&addr,sizeof(addr));
	sphlia[0]=0;
	

	for(i=0;i<50;i++){
	
		sphlia[i] = sphlia[0]++;
		printf("sending  : %d\n",sphlia[i]);
		send(sock,&sphlia,SIZE,0); 
		//sendto(sock,&msg,strlen(msg)+1,0,NULL,0);
	}
	
	send(sock1,&username,SIZE,0);
	send(sock1,&password,SIZE,0);
	
	printf("\nusername: %s\n",username);
	printf("\npassword: %s\n",password);
	
	h1.health=100;
	h1.armor=10;
	h1.attack=50;
	h1.accurasy=20;
	h1.level=1;	
	send(sock1,&h1.health,SIZE,0);
	send(sock1,&h1.armor,SIZE,0);
	send(sock1,&h1.attack,SIZE,0);
	send(sock1,&h1.accurasy,SIZE,0);
	send(sock1,&h1.level,SIZE,0);	

	printf("%d\n", h1.health);
	printf("%d\n", h1.armor);
	printf("%d\n", h1.attack);
	printf("%f\n", h1.accurasy);
	printf("%d\n", h1.level);

	close(sock1);
	close(sock);
	return 0;
}