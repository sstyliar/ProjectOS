#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// #include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
#include <pthread.h>

#define SIZE 1024
pthread_mutex_t mutex;
/////////Loot////////////
typedef struct 
{
   int points;

}loot_stats;

/////////Humans//////////

typedef struct 
{
   char username[50];
   int password;
   int health;
   int armor;
   int attack;
   float accuracy;
   int level;
   int wins;
   int loses;
   char msg[SIZE];

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
   int sock;
        
}human;


/////////Monsters////////

typedef struct
{
   int health;
   int armor;
   int attack;
   float accuracy;
   int level;

}monster_stats;

typedef struct 
{  
   monster_stats stats;
}monster;

/////////////Boss////////

typedef struct 
{
   int health;
   int armor;
   int attack;
   float accuracy;
   int level;
   
}boss_stats;

typedef struct 
{
   // boss_pos pos;
   boss_stats stats;
}boss;

// gameStatsInit
human humanInit(human h){
   
   strcpy(h.stats.username, "Test Dummy");
   h.stats.password = 0;
   h.stats.health = 0;
   h.stats.armor = 0;
   h.stats.attack = 0;
   h.stats.accuracy = 0;
   h.stats.level = 1;
   h.stats.wins = 0;
   h.stats.loses = 0;
   h.pos.x = 5;
   h.pos.y = 0;
   return h;
}

monster monsterInit(monster m){

   m.stats.health = 25;
   m.stats.armor = 5;
   m.stats.attack = 15;
   m.stats.accuracy = 0.2;
   m.stats.level = 1;

   return m;
}

boss bossInit(boss b){

   b.stats.health = 30;
   b.stats.armor = 5;
   b.stats.attack = 15;
   b.stats.accuracy = 0.3;
   b.stats.level = 1;

   return b;
}


//Save Player Stats - SERVER SIDE
void save_stats(human *h1)
{  
   char u[256];
   FILE *fp;
   char filename[20];
   sprintf(filename, "%s_stats", h1->stats.username);
   
   if(!(fp=fopen(filename,"r")))
   {  
      fp=fopen(filename,"w+");
      fclose(fp);
   }

   fp=fopen(filename,"r+");    
   fprintf(fp,"%d\n",h1->stats.health);
   fprintf(fp,"%d\n",h1->stats.armor);
   fprintf(fp,"%d\n",h1->stats.attack);
   fprintf(fp,"%g\n",h1->stats.accuracy);
   fprintf(fp,"%d\n",h1->stats.level);
   fprintf(fp,"%d\n",h1->stats.wins);
   fprintf(fp,"%d\n\n",h1->stats.loses);
   fclose(fp);

}

void saveMaze(const char *maze, int width, int height) {
   int i, x, y;


   FILE *files[10];
   char filename[20];

   sprintf(filename, "maze");
   files[i] = fopen(filename, "w");
   if (files[i] == NULL)
   {
      printf("Error opening file!\n");
      exit(1);
   }

   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
         switch(maze[y * width + x]) {
         case 1: fprintf(files[i],"{||}"); break;   //Wall
         case 3: fprintf(files[i]," :/ "); break;   //Monster
         case 4: fprintf(files[i]," :# "); break;   //Boss
         case 5: fprintf(files[i]," $$ "); break;   //Loot
         default: fprintf(files[i],"    "); break;  //Empty space
         }
      }
      fprintf(files[i], "\n");
   }
   fclose(files[i]);
}

  // Anoigei to monopati
int CarveMaze(char *maze, int width, int height, int x, int y, int monsterRestriction) {

   int x1, y1;    
   int x2, y2;
   int dx, dy;
   int dir, count;
   int flag = 0;
   int phantom_pos = 0, loot_pos = 0;


   dir = rand() % 4;
   count = 0;
   while(count < 4) {
      dx = dy = 0;
      switch(dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
      }
      x1 = x + dx;      
      y1 = y + dy;      
      x2 = x1 + dx;     
      y2 = y1 + dy;     
      
      if(x2 > 0 && x2 < width && y2 > 0 && y2 < height && maze[y1 * width + x1] == 1 && maze[y2 * width + x2] == 1) {
         maze[y1 * width + x1] = 0;
         maze[y2 * width + x2] = 0;
         x = x2; y = y2;
         dir = rand() % 4;
         count = 0;
         flag = 0;

         if (flag == 0 && monsterRestriction > 0)
         {
            phantom_pos = y1 * width + x1;
            loot_pos = y2 * width + x2;
         }
      }
      else {
        dir = (dir + 1) % 4;  //auksanei to dir alla to krataei panta <4
        count += 1;
        flag++;
      }
   }
   if (phantom_pos != 0 && loot_pos != 0 )
   {
      maze[phantom_pos] = 3;
      maze[loot_pos] = 5;
   }
   return monsterRestriction-1;
}

 // Generate maze in matrix maze with size width * height.
void GenerateMaze(int width, human h1, int monsterRestriction) {

      int x, y,i,j;
      int step = 0;
      char *maze;

      j = 7;   
      for (i = 1; i < h1.stats.level; ++i)
      {
         j += 2;
      }
                                       
      char **temp_maze = (char **)malloc((j*4)*sizeof(char*));                               
      for (i = 0; i < j; ++i)                                 
      {                                
            temp_maze[i] = (char *)malloc((j*4)*sizeof(char));                                     
      }
                                    
      maze = (char*)malloc(j * j * sizeof(char));


      /* Initialize the maze. */
      for(x = 0; x < width * width; x++) {
         maze[x] = 1;
      }
      maze[1 * width + 1] = 0;  //to amesws epomeno keli einai open, gia na exei starting point h carveMAze()
      
      /* Seed the random number generator. */
      srand(time(0));


      /* Carve the maze. */
      for(y = 1; y < width; y += 2) {       //y+=2 gia na exei duades gia ton elegxo
         for(x = 1; x < width; x += 2) {    
           monsterRestriction = CarveMaze(maze, width, width, x, y, monsterRestriction);
         }
      }

   
      //default eisodos k eksodos apo to lavurintho, maze[0][1] kai maze[..][..]
      maze[1] = 2;
      maze[(width - 1) * width + (width - 2)] = 4;

      // send(h1->sock, maze, j*sizeof(char *), 0);

      for (int y = 0; y < width; y++)
      {
         for (int x = 0; x < width*4; x+=4)
         {
            temp_maze[y][x] = maze[y*width+step];
            temp_maze[y][x+1] = maze[y*width+step];
            temp_maze[y][x+2] = maze[y*width+step];
            temp_maze[y][x+3] = maze[y*width+step];
            step++;
            printf("%c\n",*(char *)temp_maze);
         }
         step = 0;   
      }
}


//Retrieve player stats from file - SERVER SIDE
void load_stats(human *h1)
{
   FILE *fp;
   human h2;
   char c[256], filename[256];
   printf("Loading player stats...\n");
   sprintf(filename, "%s_stats", h1->stats.username);
   fp=fopen(filename,"r");    //r+ => Opens a file to update both reading and writing. The file must exist.

   fscanf(fp,"%d\n",&h2.stats.health);
   fscanf(fp,"%d\n",&h2.stats.armor);
   fscanf(fp,"%d\n",&h2.stats.attack);
   fscanf(fp,"%g\n",&h2.stats.accuracy);
   fscanf(fp,"%d\n",&h2.stats.level);
   fscanf(fp,"%d\n",&h2.stats.wins);
   fscanf(fp,"%d\n",&h2.stats.loses);
   fclose(fp);
   send(h1->sock, &h2, sizeof(human), 0);
}


//User Login Function
void userLogin(human *h1){

   system("clear");
   human h2;
   int i=0,k=0;
   char temp;
   char c[60],b[60],pass[60];
   FILE *fp;
   sprintf(pass,"%d",h1->stats.password);
   if((fp=fopen("users","r")) != NULL)     //r => Read from file, file must exist else returns NULL.
   {                              
      while(fscanf(fp,"%s",(char *)c)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
      {
         if(strcmp(c,h1->stats.username)==0)     //strcmp(str1,str2) => Compare str1 to str2, if equal then returns 0.
         {
            k++;
            fscanf(fp,"%s",(char *)b);
            if (strcmp(b, pass)==0)
            { 
               k++;
               fclose(fp);
            }
            break;
         }
      }
   }
   else{
      strcpy(h2.stats.msg, "non_existent_file");
      send(h1->sock, &h2.stats.msg, 17, 0);
   }
   if(k<2){
      strcpy(h2.stats.msg, "wrong_credentials");
      send(h1->sock, &h2.stats.msg, 17, 0);
   }
   else{
      strcpy(h2.stats.msg, "done");
      send(h1->sock, &h2.stats.msg, 20, 0);
   }


}//End of userLogin Function.



void userRegistration(human *h1){

   FILE *fp;
   char u1[60];
   human h2;
   strcpy(h2.stats.username, h1->stats.username);
   int success = 0;
   int count;

   if(!(fp=fopen("users","r")))     //r => Read from file, file must exist
   {                              
       fp=fopen("users","w+");
       fclose(fp);
   }
   fp=fopen("users","r");
   strcpy(h2.stats.msg, "");

   while(fscanf(fp,"%s",(char *)u1)!=EOF)      //Loop reads file line by line and saves each line in u1 until end of file( eof ).
   {  
      if(strstr(u1,h1->stats.username)!=0)    //Elegxei an sthn string u1 uparxei h substring username, epistrefei 0 an den uparxei.
      {
         strcpy(h2.stats.msg, "failed");
      }
   }

   if (strcmp(h2.stats.msg, "failed") == 0)
   {
      send(h1->sock, &h2.stats.msg, 10, 0); 
   }
   else
   {  
      fclose(fp);
      fp=fopen("users","a");                     //a => Append( Write at the end of the file ).  
      fprintf(fp,"%s\n", h1->stats.username);     
      fprintf(fp,"%d\n\n",h1->stats.password); 
      strcpy(h2.stats.msg, "done");
      send(h1->sock, &h2.stats.msg, 10, 0);
   }

   fclose(fp);
}




void *socketThread(void *args){

   human *h1;
   h1 = (human *)args;
   system("clear");
   pthread_mutex_lock(&mutex);
   if (strcmp(h1->stats.msg, "register") == 0)
   {
      printf("Recognized call for new user registration\n");
      userRegistration(h1);
   }
   else if (strcmp(h1->stats.msg, "login") == 0)
   {
      printf("Recognized login request.\n");
      userLogin(h1);
   }
   else if (strcmp(h1->stats.msg, "load_stats") == 0)
   {
      printf("Loading player stats...\n");
      sleep(1);
      load_stats(h1);
   }
    pthread_mutex_unlock(&mutex);
 /*  else if (strcmp(h1->stats.msg, "create_maze") == 0)
   {
       send(sock, msg, sizeof(int)* + 1, 0) ;
   }*/
   // else if (strcmp(h1->stats.msg, "create_temp_maze") == 0)
   // {
   //    /* code */
   // }

}




int main(int argc, char *argv []){



   human h1;
   h1 = humanInit(h1);
GenerateMaze(7,h1,20);
   loot_stats loot;
   loot.points = 0;

   human h2;
   h2 = humanInit(h2);
GenerateMaze(7,h2,20);
   

   human h3;
   h3 = humanInit(h3);
GenerateMaze(7,h3,20);
   

   monster m;
   m = monsterInit(m);

   boss b;
   b = bossInit(b);
//dhmiourgei ta 3 threads gia tous 3 clients se pinaka 
   pthread_t thread[3];   
   
   int monsters = h1.stats.level -1;;
   char *maze;
   void *status;

   int psock, add, i, j, ret;
   struct sockaddr_in addr;                           // Internet Socket

   addr.sin_family = AF_INET;                         // Internet socket
   addr.sin_addr.s_addr = htonl(INADDR_ANY);          // All interfaces
   addr.sin_port = htons(atoi(argv[1]));

   psock = socket(AF_INET, SOCK_STREAM, 0);           // Stream socket
   bind(psock, ( struct sockaddr *)&addr, sizeof(addr));
   listen (psock, 3) ;                                //How many to expect to listen to.
   system("clear");  
   printf ("Waiting for new connection (port : %s) ...\n", argv [1]) ;
   h1.sock = accept(psock, NULL, NULL);
   /*h2.sock = accept(psock, NULL, NULL);
   h3.sock = accept(psock, NULL, NULL);*/
   printf ("Connection established...\n") ;

   strcpy(h1.stats.msg, "") ;


   // if (recv(h1.sock, &h1, sizeof(human), 0)  < 0)
   // {
   //    printf("Error: SERVER didn't recv anything.\n");
   // }
   //pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *restrict attr); //dhmiourgoume to mutex
   printf("ekana to mutex");
   pthread_mutex_init(&mutex, NULL);

   while (strcmp(h1.stats.msg, "exit") != 0) {

      recv(h1.sock, &h1, sizeof(human), 0);
      printf("Server received: %s\n",h1.stats.msg);
      sleep(2);
      
      
      ret = pthread_create(&thread[0], NULL, socketThread, (void *)&h1);

                        


      if (ret!=0)
      {
         printf("thread_create() error\n");
      }
      pthread_join(thread[0], NULL);

   }






   // while (strcmp(h2.stats.msg, "exit") != 0) {

   //    recv(h2.sock, &h2, sizeof(human), 0);
   //    printf("Server received: %s\n",h2.stats.msg);
   //    sleep(2);
   //    ret = pthread_create(&thread[1], NULL, socketThread, (void *)&h2);
   //    if (ret!=0)
   //    {
   //       printf("thread_create() error\n");
   //    }
   //    pthread_join(thread[1], NULL);

   // }
   // while (strcmp(h1.stats.msg, "exit") != 0) {

   //    recv(h3.sock, &h3, sizeof(human), 0);
   //    printf("Server received: %s\n",h3.stats.msg);
   //    sleep(2);
   //    ret = pthread_create(&thread[2], NULL, socketThread, (void *)&h3);
   //    if (ret!=0)
   //    {
   //       printf("thread_create() error\n");
   //    }
   //    pthread_join(thread[2], NULL);

   // }

   close(h1.sock);
   printf ("Connection closed...\n") ;
   close(psock);
   unlink(argv [1]);

   return 0;
}