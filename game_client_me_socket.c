#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <curse.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIZE 1024
void registerf();
void login();
void Setup();


enum ma {W,A,S,D,X,Z};

int health,armor,attack,accuracy;

struct Character
{
  char name[60];
  int level;
  double health;
  double armor;
  double attack;
  double accuracy;
};


void main()
{ 
//-------------------ARXIKOPOIHSH SOCKET---------------------------------------------------
	int sock, x,y;
   	struct sockaddr_in addr; // Internet socket

    	addr.sin_family = AF_INET; // Internet socket
    	inet_aton(argv[1], &addr.sin_addr); 
    	addr.sin_port = htons(atoi(argv[2])); 

    	sock = socket(AF_INET, SOCK_STREAM, 0); // Stream socket
    	connect(sock, (struct sockaddr *)&addr, sizeof(addr));
//-----------------------------------------------------------------------------------------	



  int o;
  printf("Welcome to the Game\n");
  
  printf("------------------------------------------------------\n");
  
  printf("Please chose one of the options below:\n");
  
  printf("1.register\n");
  printf("2.log in\n");
  printf("3.exit\n");  
  
  printf("------------------------------------------------------\n");


   
  scanf("%d",&o);
  
  
  while(o!=1 && o!=2 && o!=3)
  {
    printf("Please type a valid option\n");
    
    printf("------------------------------------------------------\n");
    
    printf("Please chose one of the options below:\n");
    printf("1.register\n");
    printf("2.log in\n");
    
    printf("------------------------------------------------------\n");
    scanf("%d",&o);
  }
  
  if(o==1)
  {
   registerf ();
  }
  else if(o==2)
  {
    login();

   }
   else
   {
     printf("exiting the game...\n");
     sleep(5);
     exit(0);
     }
  
}



void registerf() //edw kateu8unetai o xrhsths gia na eggrafei
{
  FILE *fp;
  char username[60];
  char password[60];
  char u1[60];
  int k=0;
  
    printf("Please enter a username:");
    scanf("%s",&username);
    printf("Please enter a password:");
    scanf("%s",&password);
    fp=fopen("users","r");
    while(fscanf(fp,"%s",u1)!=EOF) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
    {  
       if(strstr(u1,username)!=0) //elegxei an sthn string u1 uparxei h substring username epistrefei 0 an den uparxei.
           
       { printf("Your username is already taken\n");
         printf("Please enter another username:");
         scanf("%s",&username);
       }
    }
     fclose(fp);
     fp=fopen("users","a");
    //me to "a" grafei sto telos tou arxeiou
    fprintf(fp," username:%s\n",username);
    fprintf(fp," password:%s\n\n",password);
    printf("Your account has been created\n");
//-------------------------------------Send username and password to Server-------------------------------
	send(sock, &username, sizeof(char)*60, 0);
	send(sock, &password, sizeof(char)*60, 0);
//--------------------------------------------------------------------------------------------------------
    fclose(fp);
    }




void login()
{
  int k=0;
  char username[60];
  char password[60];
  char c[60],b[60];
  printf("Please enter your username:");
  scanf("%s",&username);
  printf("Please enter your password:");
  scanf("%s",&password);
  FILE *fp;
  fp = fopen("users", "r");
  while(fscanf(fp,"%s",c)!=EOF)
      { 
        
     
        if(strstr(c,username)!=0)   //sugkrinei 2 sumvoloseires
        { 
          printf("your username is correct\n");
          fclose(fp);
          fp=fopen("users","r");
          while(fscanf(fp,"%s",b)!=EOF)
          {
            if(strstr(b,password)!=0)
            {
               printf("your password is correct\n");
               fclose(fp);
            
               }
            else
            {
                printf("Your username or password is incorrect\n");  
                printf("Please enter your username:");
                scanf("%s",&username);
                printf("Please enter your password:");
                scanf("%s",&password);
             
             }

          }
        }
        else
        {
          printf("Your username or password is incorrect\n");  
          printf("Please enter your username:");
          scanf("%s",&username);
          printf("Please enter your password:");
          scanf("%s",&password);
         } 
      }
  
      printf("Logging in...\n");
      sleep(5);




}
