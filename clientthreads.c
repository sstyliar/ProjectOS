#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SIZE 1024

typedef struct 
{
	int x;
}bitch;

void * clientThread(void *arg)
{
	bitch b1;
  printf("In thread\n");
  /*char message[1000];
  char buffer[1024];*/
  int clientSocket;
  struct sockaddr_in addr;
  socklen_t addr_size;

  // Create the socket. 
  	clientSocket = socket(AF_INET, SOCK_STREAM, 0);

  //Configure settings of the server address
 // Address family is Internet 
  	addr.sin_family = AF_INET;


  //Set port number, using htons function 
  	addr.sin_port = htons(1234);


 //Set IP address to localhost
  	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

 //Set the padding to 0
  	//memset(addr.sin_zero, '\0', sizeof addr.sin_zero);
  	printf("\negine to connection\n");
//Connect the socket to the server using the address
	addr_size = sizeof addr;
	connect(clientSocket, (struct sockaddr *) &addr, addr_size);
	//strcpy(message,"Hello");
	b1.x=1;
	if( send(clientSocket , &b1 , sizeof(bitch), 0) < 0)
	{
	        printf("Send failed\n");
	}
	printf("\nserver esteile:%d\n",b1.x);
	//Read the message from the server into the buffer
	if(recv(clientSocket, &b1, sizeof(bitch), 0) < 0)
	{
	   printf("Receive failed\n");
	}
	printf("\nclient parelave%d\n",b1.x);
	//Print the received message
	
	close(clientSocket);
	pthread_exit(NULL);
	//printf("ekane exit to thread");
}


int main(){
  int i = 0;
  int ret;
  pthread_t tid[3];
  bitch b1;
  b1.x = 6;
  while(i<3)
  {
  	ret = pthread_create(&tid[i], NULL, clientThread, (void *)&b1);
    if( ret != 0 )
           printf("Failed to create thread\n");
    i++;
  }
  sleep(3);
  i = 0;
  while(i < 3)
  {
     pthread_join(tid[i++],NULL);
     printf("%d:\n",i);
  }
  return 0;

}
