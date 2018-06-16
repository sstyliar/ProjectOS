#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>   
// #include <pthread.h>


#define SIZE 1024

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


void next_level(human h1, monster m, boss b, char **temp_maze, int width, bool defeated, const char *maze, int sock);

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
   h.loot.points = 0;
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

//SERVER SIDE
void upgradeEnemies(monster* m, boss* b, int lvl)
{  
  
   int i,monsterRestriction;
   
   //fur the vasilw
   m->stats.health = 25;
   m->stats.armor = 5;
   m->stats.attack = 15;
   m->stats.accuracy = 0.2;
   m->stats.level = 1; 

   //fur the boss
   b->stats.health = 30;
   b->stats.armor = 5;
   b->stats.attack = 15;
   b->stats.accuracy = 0.3;
   b->stats.level = 1;
        
  for(i=1;i<=lvl;i++)
  {
      //fur the vasilw   
      m->stats.health += 5;
      m->stats.armor += 5;
      m->stats.attack += 5;
      m->stats.accuracy += 0.1;

      //fur the boss
      b->stats.health += 10;
      b->stats.armor += 10;
      b->stats.attack += 10;
      b->stats.accuracy += 0.1;

 }
}

//Send reguest to the server to save players stats
void save_stats(human *h1, int sock){
   human h2;
   h2.stats = h1->stats;
   h2.loot = h1->loot;
   strcpy(h2.stats.msg,"save_stats");
   send(sock, &h2, sizeof(human),0);
}


void createTemp_maze(char *maze, char **temp_maze, int width){
   int x, y, step = 0;
   for (int y = 0; y < width; y++)
   {
      for (int x = 0; x < width*4; x+=4)
      {
         temp_maze[y][x] = maze[y*width+step];
         temp_maze[y][x+1] = maze[y*width+step];
         temp_maze[y][x+2] = maze[y*width+step];
         temp_maze[y][x+3] = maze[y*width+step];
         step++;
      }
      step = 0;   
   }

}

//Get maze from server.
void getMaze(human h1, char *maze, int j, int sock){

      strcpy(h1.stats.msg,"create_maze");
      send(sock,&h1, sizeof(human),0);
      recv(sock,maze, j*j,0);
      if (maze[0] == -1 || maze[0] == 0)
      {
         printf("Error: Client failed to receive array from server.\n");
         strcpy(h1.stats.msg,"exit");
         send(sock,&h1, sizeof(human),0);
         exit(0);
      }
}


//Erase previous position of da smiley - CLIENT SIDE
void clearHistory(human h1, WINDOW *win){

   mvwprintw(win, h1.pos.y, h1.pos.x,"  ");
   wrefresh(win);

}


//Clear Output Window -CLIENT SIDE
void clearWindow(WINDOW *win, int maxX){

   for ( int i = 1; i < 7; ++i)
   {
      for (int j = 0; j < maxX; ++j)
      {
         mvwprintw(win,i,j," ");
      }
   }
   wrefresh(win);   
}


//CLIENT SIDE
int human_Attack_monster(WINDOW *win, human h,monster m, boss b){  
   int damage;
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   char health[256];      
   damage = h.stats.attack - m.stats.armor;
   if(hit <= h.stats.accuracy){

      if(damage >=0){  
          
         m.stats.health = m.stats.health - damage;
         mvwprintw(win, 2, 14, "  ");
         sprintf(health,"%d",m.stats.health);
         mvwprintw(win, 2, 1, "Monster health: ");
         mvwprintw(win, 2, 18, "  ");
         mvwprintw(win, 2, 18, health);
         wrefresh(win);
         
         if(m.stats.health<=0){
            m.stats.health=0;
         }
      }
   }
   return m.stats.health;
}

//CLIENT SIDE
int monster_Attack_human(WINDOW *win, human h1,monster m, boss b){  
   int damage;   
   char health[256];                               
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   damage = m.stats.attack - h1.stats.armor;  
   if(hit <= m.stats.accuracy){
      if(damage >=0){
         h1.stats.health = h1.stats.health  - damage;
         sprintf(health,"%d",h1.stats.health);
         mvwprintw(win, 3, 1, "human health: ");
         mvwprintw(win, 3, 15, "  ");
         mvwprintw(win, 3, 15, health);
         wrefresh(win);
         if(h1.stats.health<=0){
            h1.stats.health=0;
         }
      }
   }   
   return h1.stats.health;
}


//CLIENT SIDE
int human_Attack_boss(WINDOW *win, human h,monster m, boss b){  
   int damage;
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   char health[256];
      
   damage = h.stats.attack - b.stats.armor;

   
   

   if(hit <= h.stats.accuracy){

      if(h.stats.attack - b.stats.armor >=0){  
          
         b.stats.health = b.stats.health - damage;
         mvwprintw(win, 2, 14, "  ");
         sprintf(health,"%d",b.stats.health);
         mvwprintw(win, 2, 1, "Monster health: ");
         mvwprintw(win, 2, 17, "  ");
         mvwprintw(win, 2, 17, health);
         wrefresh(win);
         
         if(b.stats.health<=0){
            b.stats.health=0;

         }
      }
   }

   return b.stats.health;
}


//CLIENT SIDE
int boss_Attack_human(WINDOW *win, human h1,monster m, boss b){  
   int damage;   
   char health[256];                               
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   damage = b.stats.attack - h1.stats.armor;  //damage = 5
   
   if(hit <= b.stats.accuracy){
      if(damage >=0){
         h1.stats.health = h1.stats.health  - damage;
         sprintf(health,"%d",h1.stats.health);
         mvwprintw(win, 3, 1, "human health: ");
         mvwprintw(win, 3, 15, "  ");
         mvwprintw(win, 3, 15, health);
         wrefresh(win);
         if(h1.stats.health<=0){
            h1.stats.health=0;
         }
      }
   }   
   
   return h1.stats.health;
}



//CLIENT SIDE
int battleController(WINDOW *win, human h1, monster m, boss b, int swit){
   int winner;
   bool fighting = TRUE;
   switch(swit){
      case 1:
         do{
               time_t t;
               srand((unsigned) time(&t));
               m.stats.health = human_Attack_monster(win,h1,m,b);     
               h1.stats.health = monster_Attack_human(win, h1,m,b);
               sleep(1);
               if(h1.stats.health==0 || m.stats.health==0){
                  if(m.stats.health == 0){
                     winner = 1;
                     fighting = FALSE;       
                  }
                  else if (h1.stats.health == 0){
                     winner = 2;
                     fighting = FALSE;       
                  }
               }
            }
            while(fighting == TRUE);
            break;
      case 2:
         do{
            time_t t;
            srand((unsigned) time(&t));
            b.stats.health = human_Attack_boss(win,h1,m,b);     
            h1.stats.health = boss_Attack_human(win,h1,m,b);
            sleep(1);
            if(h1.stats.health==0 || b.stats.health==0){
               if(b.stats.health == 0){
                  winner = 1;
                  fighting = FALSE;       
               }
               else if (h1.stats.health == 0){
                  winner = 2;
                  fighting = FALSE;       
               }
            }
         }
         while(fighting == TRUE);
         break;
   }
   return winner;
}

//Points Assigment Function 
bool assignPoints(WINDOW *window, WINDOW *target, WINDOW *stats, char **temp_maze, human* h1, int direction, int maxX, int width, bool start_game, bool nextLevel, int sock){
   
   human h2;
   char ch;
   float val;
   int i;
   bool thinking;
   thinking = TRUE;
   nodelay(window, TRUE);
   ch = wgetch(window);

   if (start_game == TRUE)
   {
      mvwprintw(target,1,1,"Let's begin...please press 'y'");   
      wrefresh(target);
      h1->loot.points = 50;
   }
   else{
      ch = 'y';
   }
   
   mvwprintw(stats, 7, 7, "  ");
   mvwprintw(stats, 7, 7, "%d",h1->loot.points);
   wrefresh(stats);
   
   switch(ch)
   {
      case 'y':  
            if (start_game == FALSE && nextLevel == FALSE)
            {
               h1->loot.points += 20;               
            }
            
            while(thinking && h1->loot.points !=0){
               clearWindow(target, maxX);
               mvwprintw(target,1,1,"Remaining points : %d", h1->loot.points);   
               mvwprintw(target,2,1,"Press '1' to upgrade Health:");  
               mvwprintw(target,3,1,"Press '2' to upgrade Armor:");  
               mvwprintw(target,4,1,"Press '3' to upgrade Attack:");  
               mvwprintw(target,5,1,"Press '4' to upgrade Accuracy:");
               mvwprintw(target,6,1,"Press '5' to stop assigning points.");
               mvwprintw(stats, 7, 7, "  ");
               mvwprintw(stats, 7, 7, "%d",h1->loot.points);
               wrefresh(target);
               wrefresh(stats);
            
               ch = wgetch(target);
               switch(ch){
                  case '1':

                     echo();
                     curs_set(1);
                     wmove(target,2,30);  
                     wrefresh(target);
                     wscanw(target,"%f",&val);
                     if (val <= h1->loot.points)
                     {
                        h1->stats.health += val;
                        h1->loot.points -= val;
                        mvwprintw(stats,2,9,"%d",h1->stats.health);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",h1->loot.points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > h1->loot.points) && (h1->loot.points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(h1->loot.points == 0)
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"You run out of points...");   
                        wrefresh(target);
                        thinking = FALSE;
                     }
                     break;

                  case '2':

                     echo();
                     curs_set(1);
                     wmove(target,3,29);  
                     wrefresh(target);
                     wscanw(target,"%f",&val);
                     if (val <= h1->loot.points)
                     {
                        h1->stats.armor += val;
                        h1->loot.points -= val;
                        mvwprintw(stats,3,8,"%d",h1->stats.armor);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",h1->loot.points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > h1->loot.points) && (h1->loot.points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(h1->loot.points == 0)
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"You run out of points...");   
                        wrefresh(target);
                        thinking = FALSE;
                     }
                     break;

                  case '3':

                     echo();
                     curs_set(1);
                     wmove(target,4,30);  
                     wrefresh(target);
                     wscanw(target,"%f",&val);
                     if (val <= h1->loot.points)
                     {
                        h1->stats.attack += val;
                        h1->loot.points -= val;
                        mvwprintw(stats,4,9,"%d",h1->stats.attack);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",h1->loot.points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > h1->loot.points) && (h1->loot.points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(h1->loot.points == 0)
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"You run out of points...");   
                        wrefresh(target);
                        thinking = FALSE;
                     }
                     break;

                  case '4':

                     echo();
                     curs_set(1);
                     wmove(target,5,32);  
                     wrefresh(target);
                     wscanw(target,"%f",&val);
                     if (val <= h1->loot.points)
                     {
                        if ((h1->stats.accuracy + val) <= 1.0)
                        {
                           h1->stats.accuracy += val;
                           h1->loot.points -= val*10;
                           mvwprintw(stats,5,11,"   ");
                           mvwprintw(stats,5,11,"%g",h1->stats.accuracy);
                           mvwprintw(stats, 7, 7, "  ");
                           mvwprintw(stats, 7, 7, "%d",h1->loot.points);
                           wrefresh(stats);
                        }
                        else{

                           clearWindow(target, maxX);
                           mvwprintw(target,1,1,"Exceeding maximun allowed accuracy.");
                           wrefresh(target);
                           sleep(3);
                        }
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > h1->loot.points) && (h1->loot.points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(h1->loot.points == 0)
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"You run out of points...");   
                        wrefresh(target);
                        thinking = FALSE;
                     }
                     break;

                  case '5':
                     
                     clearWindow(target, maxX);
                     mvwprintw(target,1,1,"You can go now...");
                     wrefresh(target);
                     thinking = FALSE;
                     break;
                  
               }
            }
            save_stats(h1, sock);
            thinking = FALSE;             //Maybe unnecessary.
            clearWindow(target,maxX);
            switch(direction){
               case KEY_UP:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1->pos.y-1, h1->pos.x, "  ");  //Loot, Same for KEY_DOWN.
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  temp_maze[h1->pos.y-1][h1->pos.x] = 0;
                  temp_maze[h1->pos.y-1][h1->pos.x+1] = 0;
                  thinking = FALSE;
                  break;
               case KEY_DOWN:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1->pos.y+1, h1->pos.x, "  ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  temp_maze[h1->pos.y+1][h1->pos.x] = 0;
                  temp_maze[h1->pos.y+1][h1->pos.x+1] = 0;
                  thinking = FALSE;
                  break;
               case KEY_RIGHT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1->pos.y, h1->pos.x+2, "  ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  for (i = 2; i < 5; ++i)
                  {
                     temp_maze[h1->pos.y][h1->pos.x + i] = 0;
                  }
                  thinking = FALSE;
                  break;
               case KEY_LEFT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1->pos.y, h1->pos.x-5, "     ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  for (i = 5; i >=2 ; --i)
                  {
                     temp_maze[h1->pos.y][h1->pos.x - i] = 0;
                  }
                  thinking = FALSE;
                  break;               
               }

         break;
      case 'r':
         clearWindow(target, maxX);
         mvwprintw(target,1,1,"Why you run from money Blin.");  
         wrefresh(target);
         thinking = FALSE;
         break;
   }


   noecho();
   curs_set(FALSE);
   return thinking;  
}

//CLIENT SIDE
bool userControl(WINDOW *window, WINDOW *target, WINDOW *stats,WINDOW *Score,const char *maze, char **temp_maze,
    bool thinking, bool bossEncounter, human h1, monster m, boss b, int direction, int maxX, int width, int sock){
   
   int ch, i,winner, wins, loses;
   ch = wgetch(window);
   char health[256];
   char score[56];
   
   
   switch(ch)
   {
      case 'y':
         clearWindow(target, maxX);
         mvwprintw(target,1,1,"Fighting commenced");  
         wrefresh(target);
         sleep(1);
         if(bossEncounter == FALSE){
            winner =  battleController(target,h1, m, b, 1);
            clearWindow(target, maxX);
            mvwprintw(target,1,1,"Fighting ended.");  
            if (winner == 1)
            {
               mvwprintw(target,2,1,"Winner was declared the human pizdek."); 
               mvwprintw(target, 3, 1, "The path is free you western spy.");
               h1.loot.points += 10;
               mvwprintw(stats, 7, 7, "  "); 
               mvwprintw(stats, 7, 7, "%d",h1.loot.points); 
               switch(direction){
               case KEY_UP:
                  mvwprintw(window, h1.pos.y-1, h1.pos.x, "  ");  //Vasilw                 
                  wrefresh(window);
                  temp_maze[h1.pos.y-1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y-1][h1.pos.x+1] = 0;
                  break;
               case KEY_DOWN:
                  mvwprintw(window, h1.pos.y+1, h1.pos.x, "  ");
                  wrefresh(window);
                  temp_maze[h1.pos.y+1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y+1][h1.pos.x+1] = 0;
                  break;
               case KEY_RIGHT:
                  mvwprintw(window, h1.pos.y, h1.pos.x+4, "    ");
                  wrefresh(window);
                  for (i = 3; i < 8; ++i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x + i] = 0;
                  }
                  break;
               case KEY_LEFT:
                  mvwprintw(window, h1.pos.y, h1.pos.x-5, "    ");
                  wrefresh(window);
                  for (i = 5; i >=2 ; --i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x - i] = 0;
                  }
                  break;               
               }
            }
            else{
               mvwprintw(target,2,1,"We lost bois. CY@");
               mvwprintw(target,3,1,"Refreshing level in 3 seconds.");
               mvwprintw(window,h1.pos.y,h1.pos.x, "  ");   //Erase human
               mvwprintw(stats, 2, 9, "0 ");    //Zero humam health
               sleep(3);
               next_level(h1, m, b, temp_maze, width, TRUE, maze, sock);
            }
            wrefresh(target);
            wrefresh(stats);
            wrefresh(Score);
            thinking = FALSE;
            break;
         }
         else if(bossEncounter == TRUE){
            winner =  battleController(target,h1, m, b, 2);
            clearWindow(target, maxX);
            mvwprintw(target,1,1,"Fighting ended.");  
            if (winner == 1)
            {
               mvwprintw(target,2,1,"Winner was declared the human pizdek."); 
               mvwprintw(target, 3, 1, "The path is free you western spy.");
               h1.loot.points += 20;
               mvwprintw(stats, 7, 7, "  "); 
               mvwprintw(stats, 7, 7, "%d",h1.loot.points); 
               mvwprintw(window,h1.pos.y+1,h1.pos.x, "  "); //Erase boss
               temp_maze[h1.pos.y+1][h1.pos.x] = 0;
               temp_maze[h1.pos.y+1][h1.pos.x+1] = 0;
            }
            else{
               mvwprintw(target,2,1,"We lost bois. CY@");
               mvwprintw(target,3,1,"Refreshing level in 3 seconds.");
               mvwprintw(window,h1.pos.y,h1.pos.x, "  ");   //Erase human
               mvwprintw(stats, 2, 9, "0 ");    //Zero humam health
               wrefresh(target);
               sleep(3);
               next_level(h1, m, b, temp_maze, width, TRUE, maze, sock);
            }
            wrefresh(target);
            wrefresh(stats);
            wrefresh(Score);
            thinking = FALSE;
            break;
         }
      case 'n':
         if (bossEncounter == FALSE){
            switch(direction){
               case KEY_UP:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y-1, h1.pos.x, "  ");  //Vasilw
                  mvwprintw(window, h1.pos.y-2, h1.pos.x, "  ");  //Loot   Same for KEY_DOWN.
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  temp_maze[h1.pos.y-1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y-1][h1.pos.x+1] = 0;
                  temp_maze[h1.pos.y-2][h1.pos.x] = 0;
                  temp_maze[h1.pos.y-2][h1.pos.x+1] = 0;
                  thinking = FALSE;
                  break;
               case KEY_DOWN:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y+1, h1.pos.x, "  ");
                  mvwprintw(window, h1.pos.y+2, h1.pos.x, "  ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  temp_maze[h1.pos.y+1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y+1][h1.pos.x+1] = 0;
                  temp_maze[h1.pos.y+2][h1.pos.x] = 0;
                  temp_maze[h1.pos.y+2][h1.pos.x+1] = 0;
                  thinking = FALSE;
                  break;
               case KEY_RIGHT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y, h1.pos.x+4, "      ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  for (i = 3; i < 10; ++i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x + i] = 0;
                  }
                  thinking = FALSE;
                  break;
               case KEY_LEFT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y, h1.pos.x-9, "         ");
                  mvwprintw(target, 1, 1, "The path is free you western spy.");
                  wrefresh(window);
                  wrefresh(target);
                  for (i = 9; i >=2 ; --i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x - i] = 0;
                  }
                  thinking = FALSE;
                  break;               
               }
            break;
         }
         else{
            clearWindow(target, maxX);
            mvwprintw(target, 1, 1, "Oi pizdec, can't send boss away.");
            wrefresh(target);
            thinking = FALSE;
            break;
         }
         
      case 'r':
         clearWindow(target, maxX);
         mvwprintw(target, 1, 1, "You are free to go for now.");
         wrefresh(target);
         thinking = FALSE;
         break;
   }

   return thinking;
}

//Main Game Controller
void mainController(const char *maze, int width, int height, char **temp_maze, human h1, monster m, boss b,
                        bool start_game, bool loaded, int sock){

   // File stuff
   char buf[width*4];
   FILE *file;
   // size_t nread;

   human h2;
   int score_size = 9;
   int maxX, maxY, step, ch, response, line, i, j;
   int x = 0, y = 0;   

   char health[256];    
   char armor[256];
   char attack[256];
   char accuracy[256];
   char level[256];
   char score[10];

   bool bossEncounter, start, end_game;
   bool thinking = TRUE;

   initscr();  //initialize and create global vars
   noecho();   //stop echoing of typed chars
   curs_set(FALSE);  //Makes cursor not visible
   getmaxyx(stdscr, maxY, maxX); //get command window size
   
   //New windows Declarations
   WINDOW *Maze = newwin(maxY - score_size*2, maxX, 0, 0);  
   WINDOW *Output = newwin(maxY - score_size, maxX, maxY - score_size*2, 0); 
   WINDOW *Stats = newwin(score_size, maxX, maxY - score_size, 0); 
   WINDOW *Score = newwin(score_size, maxX-13, maxY - score_size, maxX-13); 

   mvwprintw(Output,0, 0, "Game says:");
   
   //Player stats
   mvwprintw(Stats, 0, 0, h1.stats.username);
   mvwprintw(Stats, 2, 1, "Health:");
   mvwprintw(Stats, 2, 9,"%d",h1.stats.health);
   mvwprintw(Stats, 3, 1, "Armor:");
   mvwprintw(Stats, 3, 8,"%d",h1.stats.armor);
   mvwprintw(Stats, 4, 1, "Attack:");
   mvwprintw(Stats, 4, 9,"%d",h1.stats.attack);
   mvwprintw(Stats, 5, 1, "Accuracy:");
   mvwprintw(Stats, 5, 11,"%g",h1.stats.accuracy);
   mvwprintw(Stats, 6, 1, "Level:");
   mvwprintw(Stats, 6, 8, "%d",h1.stats.level);
   mvwprintw(Stats, 7, 1, "Loot:");
   mvwprintw(Stats, 7, 7, "  ");
   mvwprintw(Stats, 7, 7, "%d",h1.loot.points);
  
   //Vasilw
   mvwprintw(Stats, 1, 53, "Vasilw:");
   mvwprintw(Stats, 2, 54, "H: %d  At: %d", m.stats.health, m.stats.attack);
   mvwprintw(Stats, 3, 54, "Ar: %d  Ac: %g", m.stats.armor, m.stats.accuracy);

   //Final Boss
   mvwprintw(Stats, 5, 53, "Big Boss:");
   mvwprintw(Stats, 6, 54, "H: %d  At: %d", b.stats.health, b.stats.attack);
   mvwprintw(Stats, 7, 54, "Ar: %d  Ac: %g", b.stats.armor, b.stats.accuracy);

   //Score
   mvwprintw(Score, 0, 4, "!Score!");
   sprintf(score,"%d",h1.stats.wins);
   mvwprintw(Score, 3, 5,score );
   mvwprintw(Score, 3, 7, "-");
   sprintf(score,"%d",h1.stats.loses);
   mvwprintw(Score, 3, 9,score );
   mvwprintw(Score, 5, 5, "W");
   mvwprintw(Score, 5, 9, "L");

   
   //Refresh Window          
   wrefresh(Output);               
   wrefresh(Stats); 
   wrefresh(Score); 
 

   if (!loaded)
   {
      while(thinking){
         thinking = assignPoints(Maze,Output,Stats, temp_maze, &h1, ch, maxX, width, start_game,TRUE,sock);
      }
   }


   // maze -> array, Maze -> window
   step = 0;
   for (i = 0; i < width; ++i)
   {
      for (j = 0; j < width; j++)
      {
         switch(maze[i * width + j]) {
            case 1: mvwprintw(Maze,i,j+step,"{||}"); break;   //Wall
            case 3: mvwprintw(Maze,i,j+step," :/ "); break;   //Monster
            case 4: mvwprintw(Maze,i,j+step," :# "); break;   //Boss
            case 5: mvwprintw(Maze,i,j+step," $$ "); break;   //Loot
            default: mvwprintw(Maze,i,j+step,"    "); break;  //Empty space
         }
         step +=3;
      }
      step = 0;
   }
   wrefresh(Maze);


   //Initiallize players position
   mvwprintw(Maze,h1.pos.y,h1.pos.x,":)");
   wrefresh(Maze);
   end_game = FALSE; 
   start = TRUE;     //Beginning of the game.
   while(end_game == FALSE){
      thinking = TRUE;
      bossEncounter = FALSE;
      keypad(Maze,TRUE);
      nodelay(Maze, TRUE);
      ch = wgetch(Maze);
   switch (ch){
      case KEY_UP:
         if (h1.pos.y == 0){
               mvwprintw(Output,1, 1, "You can't go up, go down...BLYAT!");
               wrefresh(Output);
               break;
         }else if ((temp_maze[h1.pos.y-1][h1.pos.x] == 0) && (temp_maze[h1.pos.y-1][h1.pos.x +1] != 1)){ 
               clearHistory(h1, Maze);
               clearWindow(Output,maxX);
               mvwprintw(Maze,h1.pos.y - 1 ,h1.pos.x,":)");
               wrefresh(Maze);
               h1.pos.y = h1.pos.y - 1;
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 1){
               clearWindow(Output,maxX);
               mvwprintw(Output,1, 1, "BLYAT! you Press wall");
               wrefresh(Output);
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 3){
               clearWindow(Output,maxX);
               mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
               mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
               mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
               mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
               wrefresh(Output);
               while(thinking){
                  thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, ch, maxX, width,sock);
               }
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 4){
               clearWindow(Output,maxX);
               mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
               mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
               mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");  
               wrefresh(Output);
               while(thinking){
                  thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, ch, maxX, width,sock);
               }
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, ch, maxX, width, FALSE, FALSE,sock);
            }
            break;
         }
      case KEY_DOWN:
         if(h1.pos.y == (height-1)){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"You beat the game...advansing level in 3 seconds.");
            wrefresh(Output);
            sleep(3);
            width+=2;
            next_level(h1, m, b, temp_maze, width, FALSE, maze, sock);
            break;
         } 
         else if (temp_maze[h1.pos.y+1][h1.pos.x] == 0 && start == TRUE){
            clearHistory(h1, Maze);
            clearWindow(Output,maxX);
            mvwprintw(Maze,0,4,"{||}");
            temp_maze[h1.pos.y][h1.pos.x] = 1;
            mvwprintw(Maze,h1.pos.y + 1,h1.pos.x,":)");
            wrefresh(Maze);
            h1.pos.y = h1.pos.y + 1;
            start = FALSE;
            break;
          }
          else if(temp_maze[h1.pos.y+1][h1.pos.x] == 1 || temp_maze[h1.pos.y+1][h1.pos.x +1] == 1){
            clearWindow(Output,maxX);
            mvwprintw(Output,1, 1, "BLYAT! you Press wall");
            wrefresh(Output);
            break;
         }
          else if (temp_maze[h1.pos.y+1][h1.pos.x] == 0 && temp_maze[h1.pos.y+1][h1.pos.x +1] == 0){
            clearHistory(h1, Maze);
            clearWindow(Output,maxX);
            mvwprintw(Maze,h1.pos.y + 1,h1.pos.x,":)");
            wrefresh(Maze);
            h1.pos.y = h1.pos.y + 1;
            break;
          }
         else if(temp_maze[h1.pos.y+1][h1.pos.x] == 3 && temp_maze[h1.pos.y+1][h1.pos.x +1] == 3){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
            mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, ch, maxX, width,sock);
            }
            break;
         }
         else if(temp_maze[h1.pos.y+1][h1.pos.x] == 4){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
            mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, ch, maxX, width,sock);
            }
            break;
         }else if(temp_maze[h1.pos.y+1][h1.pos.x] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, ch, maxX, width, FALSE, FALSE,sock);
            }
            break;                                                 
         }                                                        
      case KEY_RIGHT:
         if (temp_maze[h1.pos.y][h1.pos.x + 3] == 0)
         {
               clearHistory(h1, Maze);
               clearWindow(Output,maxX);
               mvwprintw(Maze,h1.pos.y ,h1.pos.x + 2,":)");
               wrefresh(Maze);
               h1.pos.x = h1.pos.x + 2;
               break;
         }else if(temp_maze[h1.pos.y][h1.pos.x + 3] == 1){
               clearWindow(Output,maxX);
               mvwprintw(Output,1, 1, "BLYAT! you Press wall");
               wrefresh(Output);
               break;
         }
         else if(temp_maze[h1.pos.y][h1.pos.x + 3] == 3){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
            mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, ch, maxX, width,sock);
            } 
            break;
         }
         else if(temp_maze[h1.pos.y][h1.pos.x + 2] == 4){
            mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
            mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, ch, maxX, width,sock);
            }
            break;
         }else if(temp_maze[h1.pos.y][h1.pos.x+3] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to tale and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, ch, maxX, width, FALSE, FALSE,sock);
            }
            break;
         }  
      case KEY_LEFT: 
         if (temp_maze[h1.pos.y][h1.pos.x - 2] == 0)
         {
               clearHistory(h1, Maze);
               clearWindow(Output,maxX);
               mvwprintw(Maze,h1.pos.y,h1.pos.x - 2,":)"); 
               wrefresh(Maze);
               h1.pos.x = h1.pos.x - 2;
               break;
         }else if(temp_maze[h1.pos.y][h1.pos.x - 2] == 1){
            clearWindow(Output,maxX);
               mvwprintw(Output,1, 1, "BLYAT! you Press wall");
               wrefresh(Output);
               break;
         }
         else if(temp_maze[h1.pos.y][h1.pos.x - 2] == 3){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"BLYAT! you found rare monster.");
            mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, ch, maxX, width, sock);
            }
            break;
         }
         else if(temp_maze[h1.pos.y][h1.pos.x - 2] == 4){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
            mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, ch, maxX, width, sock);
            }
            break;
         }else if(temp_maze[h1.pos.y][h1.pos.x-2] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, ch, maxX, width, FALSE, FALSE, sock);
            }
            break;
         }
      case KEY_END:
         clearWindow(Output, maxX);
         mvwprintw(Output,1,1,"Exiting the game...");
         wrefresh(Output);
         sleep(3);
         end_game = TRUE;
         break;      
     }
   }   
   delwin(Maze); 
   delwin(Output);                           
   delwin(Stats);  
   delwin(Score);  
   endwin();
   system("clear");
   strcpy(h2.stats.msg,"exit");
   send(sock, &h2, sizeof(human), 0);
   exit(0); 

}

void next_level(human h1, monster m, boss b, char **temp_maze, int width, bool defeated,const char *maze, int sock){

   int monsters;

   h1.pos.x = 5;
   h1.pos.y = 0;
   if (defeated == TRUE)
   {
      h1.stats.loses++;
      save_stats(&h1, sock);
      mainController(maze, width, width, temp_maze, h1, m, b, FALSE,FALSE,sock);

   }
   else{
   
      h1.stats.wins++;
      h1.stats.level++;
      char *maze;
      maze = (char*)realloc(maze, width * width * sizeof(char));
      char **temp_maze = (char **)realloc(temp_maze, (width*4)*sizeof(char*));
      for (int i = 0; i < width; ++i)
      {
            temp_maze[i] = (char *)realloc(temp_maze[i], (width*4)*sizeof(char));    
      }
    
      monsters = h1.stats.level;
      upgradeEnemies(&m, &b, monsters-1);
      getMaze(h1, maze, width, sock);
      createTemp_maze(maze,temp_maze,width);
      mainController(maze, width, width, temp_maze, h1, m, b,  FALSE,FALSE,sock);

   }
}



//User Login Function
int userLogin(human *h1, int sock){

   system("clear");
   human h2;
   int i=0,k=0;
   char temp;
   char username[60];
   char password[60];
   char c[60],b[60];
   bool success = FALSE;

   printf("Please enter your username: ");
   scanf("%s",(char *)&username);
   strcpy(h2.stats.username, username);

   printf("Please enter your password: ");
   scanf("%d",&h2.stats.password);

   strcpy(h2.stats.msg,"login");
   send(sock, &h2, sizeof(human),0);

   if (recv(sock, &h2.stats.msg, 20, 0)  < 0)
   {
      printf("Error: Client didn't recv in logIn fun.\n");
   }
   else if (strcmp(h2.stats.msg, "done") == 0)
   {
      printf("Your username is correct.\n");
      strcpy(h1->stats.username, h2.stats.username);
      printf("Your password is correct.\nLogging in...\n");
      sleep(3);
      return 1;                              //Well Done
   }
   else if(strcmp(h2.stats.msg, "wrong_credentials") == 0)    //Don't forget to set on the send of the server buff_size = 20.
   {
      printf("\nError: Credentials don't match.\n");
      printf("If this persists consider making a new registration.\nPlease try again.\n");
      printf("Would you like to make a new registration?(y/n): ");
      scanf(" %c",&temp);
      if (temp == 'y'){                                         //Wrong username or pass
         printf("User chose to register.\n");
         sleep(3);
         return 2;
      }
      else{
         printf("\nRefreshing...\n");
         sleep(3);
         return 0;
         // userLogin(h1,sock);
      }
   }
   else if (strcmp(h1->stats.msg, "non_existent_file") == 0)
   {
      printf("\nError: File doesn't exist.\n");
      printf("If this persists consider making a new registration.\nPlease try again.\n");
      printf("Would you like to make a new registration?(y/n): ");
      scanf(" %c",&temp);
      if (temp == 'y'){
         return 2;                           //No 'users' file found.
      }
      else{
         printf("\nRefreshing...\n");
         sleep(3);
         return 0;
         // userLogin(h1,sock);
      }
   }


}//End of userLog Function.         -     Return 1 for successful login, 2 for wrong credentials or for file not existing error.
//                                                                         -> Queue the register function.


//Register Function
int userRegistration(human *h1, int sock){

	system("clear");
	char username[60];
	char password[60];
	bool success = FALSE;
   human h2;

	printf("Please enter desired username: ");
	scanf("%s",(char*)&username);
	strcpy(h2.stats.username, username);

	printf("Please enter desired password: ");
   scanf("%d",&h2.stats.password);

   strcpy(h2.stats.msg, "register");
	send(sock, &h2, sizeof(human), 0);
	   
	if (recv(sock, &h2.stats.msg, 10, 0)  < 0) 
	{
		printf("\nError: Client didn't receive in register fun.\n");
	}
	else if(strcmp(h2.stats.msg, "failed") == 0)
	{
		printf("\n\nYour username is already taken\n");
		printf("Please try again in 3 seconds...\n");
      sleep(3);
      system("clear");
      return 0;
	}
   else if (strcmp(h2.stats.msg, "done") == 0)
   {
      printf("\nYour account has been created\nLoading game in 3 seconds...\n");
      sleep(3);
      strcpy(h1->stats.username, h2.stats.username);
      strcpy(h2.stats.msg, "");
      return 1;
   }

   
}



//1st Function to be called - Displays Welcoming Bullshit.
bool gameInitiation(human *h1, int sock){
   int userChoice, flag, reg;
   char usr[10];
   reg = 0;
   human h2;
   system("clear");
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   printf("                Welcome to ProjectOS.c\n");
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   sleep(2);
   system("clear");
   while(1){
      printf("Please chose between register,login, or exit:\n");
      printf("Press '1' to Register.\nPress '2' to Log in.\nPress '3' to Exit.\nUser's Choice: ");
      scanf("%d",&userChoice);
      if (userChoice < 4 && userChoice > 0)
      {
         flag = 0;
         while(1){
            if (userChoice == 1)
            {
               while(reg == 0){
                  reg = userRegistration(h1,sock);
               }
               return FALSE;
            }
            else if (userChoice == 2)
            {
               while(reg == 0){
                  reg = userLogin(h1, sock);         
               }
               if (reg == 1)                
               {
                  strcpy(h2.stats.msg,"load_stats");
                  strcpy(h2.stats.username,h1->stats.username);
                  strcpy(usr,h1->stats.username);
                  send(sock, &h2, sizeof(human), 0);
                  recv(sock, &h2, sizeof(human), 0);
                  h1->stats = h2.stats;
                  h1->loot = h2.loot;  
                  strcpy(h1->stats.username,usr);                 
                  return TRUE;                           
               }
               else
               {
                  userChoice = 1;
                  reg = 0;
               }                      
                                          
            }
            else if (userChoice == 3)
            {
               printf("Exiting...\n");
               sleep(2);
               strcpy(h2.stats.msg,"exit");
               send(sock, &h2, sizeof(human), 0);
               exit(0);
            }
         }
         break;
      }
      else{
         printf("Please try again...Refreshing in 2 seconds.\n");
         sleep(2);
         system("clear");
      }
   }
}


int main(int argc, char *argv [])
{

	int sock, i,j;
	char msg[SIZE], *maze;
	struct sockaddr_in addr; 								

	addr.sin_family = AF_INET; 								
	inet_aton(argv [1], &addr.sin_addr);
	addr.sin_port = htons(atoi(argv [2]));

	sock = socket(AF_INET, SOCK_STREAM, 0); 				
	connect(sock,(struct sockaddr *)&addr,sizeof(addr));

	human h1, h2;
	h1 = humanInit(h1);

	bool loaded;
	loaded = gameInitiation(&h1, sock);

	loot_stats loot;
	loot.points = 0;

	monster m;
	m = monsterInit(m);

	boss b;
	b = bossInit(b);

	int monsters = h1.stats.level;     //ADD -1 so that the first level doesnt have any monsters in it
   j = 7;   
   for (i = 1; i < h1.stats.level; ++i)
   {
      j += 2;
      upgradeEnemies(&m, &b, monsters);
   }

   char **temp_maze = (char **)malloc((j*4)*sizeof(char*));                               
   for (i = 0; i < j; ++i)                                 
   {                                
         temp_maze[i] = (char *)malloc((j*4)*sizeof(char));                               
   }
                                 
   maze = (char*)malloc(j * j * sizeof(char));
   if(maze == NULL){                                 
      printf("Error: not enough memory\n");                                
      exit(0);                                   
   }

   getMaze(h1, maze, j, sock);

printf("%d\n",h1.stats.health );
printf("%d\n",h1.stats.armor );
printf("%d\n",h1.stats.attack );
printf("%g\n",h1.stats.accuracy);
printf("%d\n",h1.stats.level );
printf("%d\n",h1.stats.wins );
printf("%d\n\n",h1.stats.loses );



   int x,y;
   for(y = 0; y < j; y++) {
      for(x = 0; x < j; x++) {
         printf("%d",maze[y * j + x] );
      }
      printf("\n");
   }
   printf("\n");
   sleep(5);





   createTemp_maze(maze,temp_maze,j);






   for (int y = 0; y < j; y++)
   {
      for (int x = 0; x < j*4; x++)
      {
         printf("%d",temp_maze[y][x] );
      }
      printf("\n");
   }


   sleep(5);








   mainController(maze, j, j, temp_maze, h1, m, b, TRUE,loaded, sock);










//=============TESTING=============
   // // print maze array
   // int width = j;
   // int x,y;
   // for(y = 0; y < width; y++) {
   //    for(x = 0; x < width; x++) {
   //       printf("%d",maze[y * width + x] );
   //    }
   //    printf("\n");
   // }
   // printf("\n");


   // //print temp_maze array
   // for (int y = 0; y < width; y++)
   // {
   //    for (int x = 0; x < width*4; x++)
   //    {
   //       printf("%d",temp_maze[y][x] );
   //    }
   //    printf("\n");
   // }


   // int x,y;
   //    for(y = 0; y < j; y++) {
   //       for(x = 0; x < j; x++) {
   //          printf("%d",maze[y * j + x] );
   //       }
   //       printf("\n");
   //    }
   //    printf("\n");
   //    sleep(1);



   // //Get temp_maze array
   // strcpy(h2.stats.msg,"create_temp_maze");
   // send(sock, &h2, sizeof(human), 0);
   // recv(sock, , sizeof(human), 0);




         // if (loaded == TRUE)
         // {
         //    printf("True, the user just loaded.\n");
         //    printf("And his stats are: \n");
         // }
         // printf("%d\n",h1.stats.health );
         // printf("%d\n",h1.stats.armor );
         // printf("%d\n",h1.stats.attack );
         // printf("%g\n",h1.stats.accuracy);
         // printf("%d\n",h1.stats.level );
         // printf("%d\n",h1.stats.wins );
         // printf("%d\n",h1.stats.loses );

         // printf("Exiting...\n");
         // sleep(2);
         // human h2;
         // strcpy(h2.stats.msg,"exit");
         // send(sock, &h2, sizeof(human), 0);




	// printf ("What say you?\n");
	// scanf("%s",msg);
	// send(sock, msg, strlen (msg) + 1, 0) ;
	// recv(sock, msg, SIZE, 0) ;

	// while (strcmp(msg, "exit") != 0) {
	// 	printf ("Dude 2 said: %s\n", msg);
	// 	printf("What does the Dude 1 say about it?\n");
	// 	strcpy(msg, "") ;
	// 	scanf("%s",msg);
	// 	send(sock, msg, strlen(msg)+1,0);
	// 	recv(sock, msg, SIZE, 0) ;
	// }


	close(sock);
	return 0;
}