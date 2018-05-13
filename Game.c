#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<curses.h>
#include<string.h>
#include<sys/stat.h>

void registerf();
void login();
void Setup();
void mazeGenerator(int width,int height);

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
   mazeGenerator(20,20);
}



void registerf()
{
  FILE *fp;
  char username[60];
  char password[60];
  char c[60];
  int k=0;
  
    printf("Please enter your username:");
    scanf("%s",&username);
    printf("Please enter your password:");
    scanf("%s",&password);
    
  //sprintf(username,"%s",username); //mallon 8etei to onoma sto telos sthn
                                       //arxikh metavlhth kai dhmiourgei to arxeio  
    fp=fopen("users","w");
   
     fprintf(fp,"username:%s\n",username);
     fprintf(fp,"password:%s\n",password);
      //grafei to password mesa sto arxeio me to onoma username
     
     printf("Your account has been created\n");
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
  if ((fp = fopen("users", "r")))
    {   
      
      while(fscanf(fp,"%s",c)!=EOF)
      {  
        if(strcmp(c,"username:%susername")==0)   //sugkrinei 2 sumvoloseires
        { printf("your username is correct\n");
          close(fp);
          while(fscanf(fp,"%s",b))
          {
           if(strcmp(b,"password:%spassword")==0)
           {
             fclose(fp);
             printf("your password is correct\n");
             sleep(1);
             k=1;
       
           }}
         }
      } }
     if(k==1)
     {
      printf("Logging in...\n");
      sleep(5);
      }
     else
     {
      printf("It seems that the username, or password is incorrect\n");
      printf("You may now exit the game...\n");
      sleep(7);
      exit(0);
     }
   
  
}



void Setup()
{
}


void mazeGenerator(int width,int height)
{   int i,j;
   char maze[width][height];
   for( i=0;i<width;i++)
      {
	     printf("-");
	     
      }
      printf("\n");
      for(i=0;i<height;i++)
      {
 	     for(j=0;j<width;j++)
 	     {
	       if(j==0|| j==width-1)
	       { 
	   	      printf("#");
	               }

            else if(i==0||i==height-1)
            {
              printf("#");
                }
              }
        }
 


}



