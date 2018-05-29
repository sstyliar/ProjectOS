#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>



typedef struct 
{
   char username[50];
   int password;
   int health;
   int armor;
   int attack;
   float accurasy;
   int level;
   int wins;
   int loses;

}human_stats;

typedef struct 
{
	int y;
	int x;
}human_pos;

typedef struct 
{  
   human_stats stats;
   human_pos pos;
        
}human;

void registerf(human* h1);

/////////Monsters////////////

typedef struct
{
   int health;
   int armor;
   int attack;
   float accurasy;
   int level;

}monster_stats;

typedef struct 
{  
   monster_stats stats;
}monster;

/////////////Boss//////////////

typedef struct 
{
   int health;
   int armor;
   int attack;
   float accurasy;
   int level;
   
}boss_stats;

typedef struct 
{
   // boss_pos pos;
   boss_stats stats;
}boss;



// gameStatsInit
human humanInit(human h){

   strcpy(h.stats.username, "");
   h.stats.password = 0;
   h.stats.health = 100;
   h.stats.armor = 5;
   h.stats.attack = 10;
   h.stats.accurasy = 0.1;
   h.stats.level = 1;
   h.stats.wins = 0;
   h.stats.loses = 0;
   h.pos.x = 5;
   h.pos.y = 0;
   return h;
}

monster monsterInit(monster m){

   m.stats.health = 15;
   m.stats.armor = 2;
   m.stats.attack = 5;
   m.stats.accurasy = 0.1;
   m.stats.level = 1;

   return m;
}

boss bossInit(boss b){

   b.stats.health = 21;
   b.stats.armor = 5;
   b.stats.attack = 10;
   b.stats.accurasy = 0.1;
   b.stats.level = 1;

   return b;
}


int main(int argc,char *argv[]) {
   int o,k=0;
   human h1;

   //init monster - boss

   h1 = humanInit(h1);
   char username[60];
   printf("Welcome to DCSMASHER\n");
   printf("-------------------------------\n");
   sleep(3);
   system("clear");
   printf("BLIN! You western spy chose between register,login, or exit:\n");
   printf("1.Register\n");
   printf("2.Log in\n");
   printf("3.Exit\n");
   scanf("%d",&o);
   while(o!=1 && o!=2 && o!=3)
  {
    printf("You spy had 1 job!\n");
    
    printf("------------------------------------------------------\n");
    
    printf("Please let someone who is not a total retard type for you!:\n");
    printf("1.Register\n");
    printf("2.Log in\n");
    printf("3.Exit");
    printf("------------------------------------------------------\n");
    scanf("%d",&o);
  }
  printf("You a clever man!\n");
  if(o==1)
  {  
  	 registerf(&h1);
     printf("Would you like to log in right now? 1 for yes 2 for no:\n");
     scanf("%d",&k);
     while(k!=1 && k!=2)
     {
       printf("Your choice was invalid...\n");
       printf("Would you like to log in right now? 1 for yes 2 for no:\n");
       scanf("%d",&k);
     

     }
    if(k==2)
    {
     exit(0);

    }
  }

  if(o==2 || k==1)
  {  
     if(k==1)
      {
       printf("logging in...\n");
       //Start game with the current settins for human
       sleep(5);
      }
      else
      { 
        h1 = login();
        //Start game...

      }
    
  }
  else
  {
     printf("exiting the game...\n");
     sleep(5);
     exit(0);
  }
  save_stats(h1);

   return 0;
}




 void registerf(human* h) //edw kateu8unetai o xrhsths gia na eggrafei
{ system("clear");
  FILE *fp;
  char username[60];
  char password[60];
  char u1[60];
  int k=0;
    
    printf("Please enter a username:");
    scanf("%s",(char*)&username);
    strcpy(h->stats.username, username);
    printf("Please enter a password:");
    // scanf("%s",(char*)&password);
    scanf("%d",&h->stats.password);
    // h->stats.password = password;

    //Send stuff to server.


    if(!(fp=fopen("users","r")))
    {                              //an den uparxei to arxeio to dhmiourgei(me to "r" de dhmiourgeitai arxeio)
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
         strcpy(h->stats.username, username);
       }
    }
     fclose(fp);
     fp=fopen("users","a");
    //me to "a" grafei sto telos tou arxeiou
    // fprintf(fp,"%s\n",username);
    fprintf(fp,"%s\n", h->stats.username);
    // fprintf(fp,"%s\n\n",password);
    fprintf(fp,"%d\n\n",h->stats.password);
    printf("Your account has been created\n");
    fclose(fp);

    
   
}




human login()
{
  system("clear");
  human h2;
  h2 = humanInit(h2);
  int i=0,k=0;
  char username[60];
  char password[60];
  char c[60],b[60];
  FILE *fp;
  printf("Please enter your username:");
  scanf("%s",(char *)&username);
  printf("Please enter your password:");
  scanf("%s",(char *)&password);
  if(!(fp=fopen("users","r")))
  {                              //an den uparxei to arxeio to dhmiourgei(me to "r" de dhmiourgeitai arxeio)
      fp=fopen("users","w+");
      fclose(fp);
      printf("Your account doesn't exist, please try registering first, would you like to do it now?\n");
      printf("Type 1 to register, or 2 to exit\n");
      scanf("%d",&i);
      while(i!=1 && i!=2)
      {
         printf("How about giving a valid choice instead? :)");
         printf("Type 1 to register, or 2 to exit\n");
         scanf("%d",&i);
      }

      if(i==1)
      {
        registerf(h2);
      }
      else
      {
         printf("You may now exit the game...\n");
         sleep(2);
         exit(0);
      }

  }

  
  	fp = fopen("users", "r");
    size_t len = 0;
    ssize_t read;
    char * line = NULL;
    fp = fopen("users", "r");
    
    while(fscanf(fp,"%s",(char *)c)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
    {  
      if(strcmp(c,username)==0)
        {
          k++;
          printf("Your username is correct\n");
         fclose(fp);
                
         }
    }
      
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
    	}
	}

  return h2;
}




void save_stats(char username, human h1)
{  
   char u[256];
   FILE *fp;
    if(!(fp=fopen("stats","r")))
    {  

      fp=fopen("stats","w+");
      fclose(fp);

    }

    else
    {
       
       fp=fopen("stats","a+");
       fprintf(fp,"%s\n",username);       
       fprintf(fp,"%d\n",h1.stats.health);
       fprintf(fp,"%d\n",h1.stats.attack);
       fprintf(fp,"%d\n",h1.stats.armor);
       fprintf(fp,"%g\n",h1.stats.accurasy);
       fprintf(fp,"%d\n",h1.stats.level);
       fprintf(fp,"%d\n",h1.stats.wins);
       fprintf(fp,"%d\n\n",h1.stats.loses);
       fclose(fp);

    }



}


human load_stats(char username, human h1)
{
     FILE *fp;
     char c[256];
     fp=fopen("stats","r+");
      while(fscanf(fp,"%s",(char *)c)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
    {  
      if(strcmp(c,username)==0)
        {
              
       fscanf(fp,"%d\n",h1.stats.health);
       fscanf(fp,"%d\n",h1.stats.attack);
       fscanf(fp,"%d\n",h1.stats.armor);
       fscanf(fp,"%g\n",h1.stats.accurasy);
       fscanf(fp,"%d\n",h1.stats.level);
       fscanf(fp,"%d\n",h1.stats.wins);
       fscanf(fp,"%d\n",h1.stats.loses);
          
          
       fclose(fp);
                
         }} 


  
    return h1;
}
