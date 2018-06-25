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
#define PORT 44444
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
   loot_stats loot;
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
   printf("Saving player stats....\n");
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
   fprintf(fp,"%d\n",h1->stats.loses);
   fprintf(fp,"%d\n\n",h1->loot.points);
   fclose(fp);

}

void saveMaze(const char *maze, int width, int height, int num) {
   int i, x, y;
   FILE *file;
   char filename[20];

   sprintf(filename, "maze%d",num);
   file = fopen(filename, "w");
   if (file == NULL)
   {
      printf("Error creating file!\n");
      exit(1);
   }

   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
         switch(maze[y * width + x]) {
         case 1: fprintf(file,"{||}"); break;   //Wall
         case 3: fprintf(file," :/ "); break;   //Monster
         case 4: fprintf(file," :# "); break;   //Boss
         case 5: fprintf(file," $$ "); break;   //Loot
         default: fprintf(file,"    "); break;  //Empty space
         }
      }
      fprintf(file, "\n");
   }
   fclose(file);
}

  // Carves maze to free space.
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
        dir = (dir + 1) % 4;  //dir++ but dir < 4 at all times( 4 directions ).
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
void GenerateMaze(human *h1) {

   int x,y,i,j,width;
   char *maze;
   int monsterRestriction;

   j = 7;   
   for (i = 1; i < h1->stats.level; ++i)
   {
      j += 2;
   }
   width  = j;
   maze = (char*)malloc(j * j * sizeof(char));              
   /* Initialize the maze. */
   for(x = 0; x < width * width; x++) {
      maze[x] = 1;
   }
   maze[1 * width + 1] = 0;  //to amesws epomeno keli einai open, gia na exei starting point h carveMAze()

   // Seed the random number generator. 
   srand(time(0));
   monsterRestriction = h1->stats.level;   
   // Carve the maze. 
   for(y = 1; y < width; y += 2) {       //y+=2 gia na exei duades gia ton elegxo
      for(x = 1; x < width; x += 2) {    
         monsterRestriction = CarveMaze(maze, width, width, x, y, monsterRestriction);
      }
   }
   

   //default eisodos k eksodos apo to lavurintho, maze[0][1] kai maze[..][..]
   maze[1] = 2;
   maze[(width - 1) * width + (width - 2)] = 4;

   saveMaze(maze,width,width,h1->stats.level);  
   send(h1->sock, maze, j*j, 0);
   free(maze);
}


//Retrieve player stats from file - SERVER SIDE
void load_stats(human *h1)
{
   FILE *fp;
   human h2;
   char c[256], filename[256];
   printf("Loading player stats...\n");
   sleep(1);
   sprintf(filename, "%s_stats", h1->stats.username);
   fp=fopen(filename,"r");    //r+ => Opens a file to update both reading and writing. The file must exist.
   if (fp == NULL)
   {
      printf("Error: File doens't exist.\n");
   }

   fscanf(fp,"%d\n",&h2.stats.health);
   fscanf(fp,"%d\n",&h2.stats.armor);
   fscanf(fp,"%d\n",&h2.stats.attack);
   fscanf(fp,"%g\n",&h2.stats.accuracy);
   fscanf(fp,"%d\n",&h2.stats.level);
   fscanf(fp,"%d\n",&h2.stats.wins);
   fscanf(fp,"%d\n",&h2.stats.loses);
   fscanf(fp,"%d\n",&h2.loot.points);
   fclose(fp);
   strcpy(h2.stats.username, h1->stats.username);
   send(h1->sock, &h2.stats, sizeof(human_stats), 0);
   send(h1->sock, &h2.loot.points, sizeof(int), 0);
}


//User Login Function
void userLogin(human *h1){


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
      load_stats(h1);
   }
   else if (strcmp(h1->stats.msg, "create_maze") == 0)
   {
      GenerateMaze(h1);
   }
   else if (strcmp(h1->stats.msg, "save_stats") == 0)
   {
      save_stats(h1);
   }

}




int main(int argc, char *argv []){

   human h1;
   h1 = humanInit(h1);
   loot_stats loot;
   loot.points = 0;
   monster m;
   m = monsterInit(m);
   boss b;
   b = bossInit(b);
  
   int monsters = h1.stats.level -1;;
   char *maze;
   void *status;

   int psock, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	
	pid_t childpid;

	psock = socket(AF_INET, SOCK_STREAM, 0);
	if(psock < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[1]));
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	ret = bind(psock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	

	if(listen(psock, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1){
		h1.sock = accept(psock, (struct sockaddr*)&newAddr, &addr_size);
		if(h1.sock < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(psock);

		strcpy(h1.stats.msg,"");

		while(1){
			recv(h1.sock,&h1,sizeof(human), 0);
			if(strcmp(h1.stats.msg, "exit") == 0){
				printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
				break;
			}else{
				printf("Server received:%s\n",h1.stats.msg);
				socketThread((void *)&h1);
			}
		}
	}

   }

   close(newSocket);
   printf ("Connection closed...\n") ;
   sleep(1);
   unlink(argv [1]);

   return 0;
}
