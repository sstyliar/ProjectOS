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


void * socketThread(void *arg)
{

	bitch b1;
	int newSocket = *((int *)arg);
  	recv(newSocket , &b1 , sizeof(bitch), 0);
  	printf("\nparelave:%d\n",b1.x);
  	b1.x=3;
  	send(newSocket,&b1,sizeof(bitch),0);
  	printf("\nserver esteile:%d\n",b1.x);
  	
 	close(newSocket);
 	pthread_exit(NULL);
 	printf("Exit socketThread \n");

}

int main()
{

	bitch b1;
	int psock,sock;

	struct sockaddr_in addr; //internet socket	
	// struct sockaddr_storage serverStorage;	//..Set the size of address 
	// socklen_t addr_size;
	

	addr.sin_family = AF_INET;	//as it is

	// addr.sin_addr.s_addr = htonl(INADDR_ANY);// All interfaces
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// addr.sin_port = htons(atoi(argv[1]));
	addr.sin_port = htons(1234);

	// Create Socket
	psock = socket(AF_INET,SOCK_STREAM,0);//Stream socket

	//Set the padding to 0
	memset(addr.sin_zero, '\0', sizeof addr.sin_zero);

	//Bind the address struct to the socket 
	bind(psock,(struct sockaddr *)&addr,sizeof(addr));
	

	// listen(psock,3);//dexetai 3 paixtes client 


	listen(psock,3);
    printf("Listening\n");
	
	pthread_t tid[60];
	int i = 0;
	while(1){

		//Accept call creates a new socket for the incoming connection
        // addr_size = sizeof serverStorage;
        // newSocket = accept(psock, (struct sockaddr *) &serverStorage, &addr_size);
        sock = accept(psock, NULL,NULL);

        //for each client request creates a thread and assign the client request to it to process
       	//so the main thread can entertain next request
        if( pthread_create(&tid[i], NULL, socketThread, &sock) != 0 )
           printf("Failed to create thread\n");
        
        printf("\nftiaxnontai ta threads\n");

        if( i >= 3)
        {
          i = 0;
          while(i < 3)
          {
            pthread_join(tid[i++],NULL);
            printf("\nmphka sthn while 92\n");
          }
          i = 0;

        }



    }

	
	
	return 0;
}
