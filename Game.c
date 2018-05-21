#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<curses.h>
#include<string.h>
#include<sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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


void main(int argc, char *argv[])
{ 
  int o,k;
  
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
   printf("Would you like to log in right now? 1 for yes 2 for no:\n");
   scanf("%d",&k);
   while(k!=1 && k!=2)
   {
     printf("Your choice was invalid...\n");
     printf("Would you like to log in right now? 1 for yes 2 for no:\n");
     scanf("%d",&k);
     

    }
    if(k==1)
    {
      login();
    }
   else
   {
     exit(0);

   }
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
    scanf("%s",(char*)&username);
    printf("Please enter a password:");
    scanf("%s",(char*)&password);
    if(!(fp=fopen("users","r")))
    {
      fp=fopen("users","w+");
      fclose(fp);

    }
    fp=fopen("users","r");
    
    while(fscanf(fp,"%s",(char *)u1)!=EOF) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
    {  
       if(strstr(u1,username)!=0) //elegxei an sthn string u1 uparxei h substring username epistrefei 0 an den uparxei.
           
       { printf("Your username is already taken\n");
         printf("Please enter another username:");
         scanf("%s",(char *)&username);
       }
    }
     fclose(fp);
     fp=fopen("users","a");
    //me to "a" grafei sto telos tou arxeiou
    fprintf(fp," %s\n",username);
    fprintf(fp," %s\n\n",password);
    printf("Your account has been created\n");
    fclose(fp);
   
}


void login()
{ 
  int i=0,k=0;
  char username[60];
  char password[60];
  char c[60],b[60];
  printf("Please enter your username:");
  scanf("%s",(char *)&username);
  printf("Please enter your password:");
  scanf("%s",(char *)&password);
  
  FILE *fp;
  fp = fopen("users", "r");
    size_t len = 0;
    ssize_t read;
     char * line = NULL;
    fp = fopen("users", "r");
    //if (fp == NULL)
    //{   printf("failure\n");
      //  exit(EXIT_FAILURE);
     //}
    //while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);
        //sleep(2);
    while(fscanf(fp,"%s",(char *)c)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
    {  
      if(strcmp(c,username)==0)
        {
          k++;
          printf("Your username is correct\n");
         fclose(fp);
                
         }}
      
  fp=fopen("users","r");
  while(fscanf(fp,"%s",(char *)b)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sth c mexri na ftasei sto telos tou arxeiou
    {  
      if(strcmp(b,password)==0 && k>=1)
      {    
        printf("Your password is correct\n");
        printf("Logging in...\n");
        k++;
            sleep(5);
            fclose(fp);
            break;
            
            
          }
        }
          
           
      
       if(k<2 )
       { 
        printf("Your username or password was incorrect\n");
        printf("Would you like to try again?\n");
        printf("1.Retry/2.Exit\n");
        scanf("%d",&i);
        if(i==1)
        {
          login();


        }    
        else
        {
           printf("You may now exit the game...");
           printf("exiting...");
           sleep(5);
           exit(0);

        }}
    
    
}
   





