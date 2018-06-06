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
#include <pthread.h>


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


void next_level(human h1, monster m, boss b, loot_stats* loot, char **temp_maze, int width, bool defeated, const char *maze);

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


//SERVER SIDE
void upgradeEnemies(monster* m,boss* b,int lvl)
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
   damage = m.stats.attack - h1.stats.armor;  //damage = 5
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
   // char health[256];
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

            }while(fighting == TRUE);
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

         }while(fighting == TRUE);
         break;
   }
   
   return winner;
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




//Points Assigment Function - CLIENT SIDE
bool assignPoints(WINDOW *window, WINDOW *target, WINDOW *stats, char **temp_maze, human* h1, loot_stats* loot
      ,int direction, int maxX, int width, bool start_game, bool nextLevel){
   
   char ch;
   float val;
   int i;
   bool thinking;
   thinking = TRUE;
   nodelay(window, TRUE);
   ch = wgetch(window);

   if (start_game == TRUE)
   {
      mvwprintw(target,1,1,"Welcome to the game we lost one semester worth of time developing");   
      mvwprintw(target,2,1,"Press 'y' if you agree that diplomas are overestimated and piracy pays.");   
      wrefresh(target);
      loot->points = 50;
   }
   else{
      ch = 'y';
   }
   
   mvwprintw(stats, 7, 7, "  ");
   mvwprintw(stats, 7, 7, "%d",loot->points);
   wrefresh(stats);
   
   switch(ch)
   {
      case 'y':  
            if (start_game == FALSE && nextLevel == FALSE)
            {
               loot->points += 20;               
            }
            
            while(thinking && loot->points !=0){
               clearWindow(target, maxX);
               mvwprintw(target,1,1,"Remaining points : %d", loot->points);   
               mvwprintw(target,2,1,"Press '1' to upgrade Health:");  
               mvwprintw(target,3,1,"Press '2' to upgrade Armor:");  
               mvwprintw(target,4,1,"Press '3' to upgrade Attack:");  
               mvwprintw(target,5,1,"Press '4' to upgrade Accuracy:");
               mvwprintw(target,6,1,"Press '5' to stop assigning points.");
               mvwprintw(stats, 7, 7, "  ");
               mvwprintw(stats, 7, 7, "%d",loot->points);
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
                     if (val <= loot->points)
                     {
                        h1->stats.health += val;
                        loot->points -= val;
                        mvwprintw(stats,2,9,"%d",h1->stats.health);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",loot->points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > loot->points) && (loot->points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(loot->points == 0)
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
                     if (val <= loot->points)
                     {
                        h1->stats.armor += val;
                        loot->points -= val;
                        mvwprintw(stats,3,8,"%d",h1->stats.armor);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",loot->points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > loot->points) && (loot->points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(loot->points == 0)
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
                     if (val <= loot->points)
                     {
                        h1->stats.attack += val;
                        loot->points -= val;
                        mvwprintw(stats,4,9,"%d",h1->stats.attack);
                        mvwprintw(stats, 7, 7, "  ");
                        mvwprintw(stats, 7, 7, "%d",loot->points);
                        wrefresh(stats);
                        noecho();
                        curs_set(0);
                     }
                     else if ((val > loot->points) && (loot->points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(loot->points == 0)
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
                     if (val <= loot->points)
                     {
                        if ((h1->stats.accuracy + val) <= 1.0)
                        {
                           h1->stats.accuracy += val;
                           loot->points -= val*10;
                           mvwprintw(stats,5,11,"   ");
                           mvwprintw(stats,5,11,"%g",h1->stats.accuracy);
                           mvwprintw(stats, 7, 7, "  ");
                           mvwprintw(stats, 7, 7, "%d",loot->points);
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
                     else if ((val > loot->points) && (loot->points != 0))
                     {
                        clearWindow(target,maxX);
                        mvwprintw(target,1,1,"Now now, lets not exaggerate...try again.");   
                        wrefresh(target);
                        noecho();
                        curs_set(0);
                        sleep(3);
                     }
                     else if(loot->points == 0)
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
            save_stats(h1);      //SEND h1 TO THE SERVER TO BE SAVED BY saveStats()
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
    bool thinking, bool bossEncounter, human h1, monster m, boss b, loot_stats* loot, int direction, int maxX, int width){
   
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
               loot->points += 10;
               mvwprintw(stats, 7, 7, "  "); 
               mvwprintw(stats, 7, 7, "%d",loot->points); 
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
               next_level(h1, m, b, loot, temp_maze, width, TRUE, maze);
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
               loot->points += 20;
               mvwprintw(stats, 7, 7, "  "); 
               mvwprintw(stats, 7, 7, "%d",loot->points); 
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
               next_level(h1, m, b, loot, temp_maze, width, TRUE, maze);
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




//Main Game Controller  - CLIENT SIDE
void mainController(const char *maze, int width, int height, char **temp_maze, human h1, monster m, boss b, loot_stats* loot,
                     bool start_game, bool loaded){

   // File stuff
   char buf[width*4];
   FILE *file;
   // size_t nread;

   int score_size = 9;
   int maxX, maxY, dx, dy, ch, response, line;
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
   
   //New window
   WINDOW *Maze = newwin(maxY - score_size*2, maxX, 0, 0);  
   WINDOW *Output = newwin(maxY - score_size, maxX, maxY - score_size*2, 0); 
   WINDOW *Stats = newwin(score_size, maxX, maxY - score_size, 0); 
   WINDOW *Score = newwin(score_size, maxX-13, maxY - score_size, maxX-13); 
   
   mvwprintw(Output,0, 0, "Game says:");
   

 //Player1 stats
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
   mvwprintw(Stats, 7, 7, "%d",loot->points);

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
         thinking = assignPoints(Maze,Output,Stats, temp_maze, &h1, loot, ch, maxX, width, start_game,TRUE);
      }
   }
   
   
   //Read file function
   file = fopen("maze","r");
   if (file) {
      line = 0;
      while (fgets(buf,1024,file)){
         mvwprintw(Maze,line,0,buf);
         wrefresh(Maze);
         line++;
      }
       fclose(file);
   }  

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
               mvwprintw(Output,1,1,"BLYAT! you found rare monster.");
               mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
               mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
               mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
               wrefresh(Output);
               while(thinking){
                  thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, loot, ch, maxX, width);
               }
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 4){
               clearWindow(Output,maxX);
               mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
               mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
               mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");  
               wrefresh(Output);
               while(thinking){
                  thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, loot, ch, maxX, width);
               }
               break;
         }else if(temp_maze[h1.pos.y-1][h1.pos.x] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, loot, ch, maxX, width, FALSE, FALSE);
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
            next_level(h1, m, b, loot, temp_maze, width, FALSE, maze);
            break;
         } 
         else if (temp_maze[h1.pos.y+1][h1.pos.x] == 0 && start == TRUE){
            clearHistory(h1, Maze);
            clearWindow(Output,maxX);
            mvwprintw(Maze,0,4,"{||}");
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
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, loot, ch, maxX, width);
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
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, loot, ch, maxX, width);
            }
            break;
         }else if(temp_maze[h1.pos.y+1][h1.pos.x] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, loot, ch, maxX, width, FALSE, FALSE);
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
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, loot, ch, maxX, width);
            } 
            break;
         }
         else if(temp_maze[h1.pos.y][h1.pos.x + 2] == 4){
            mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
            mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
            wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, loot, ch, maxX, width);
            }
            break;
         }else if(temp_maze[h1.pos.y][h1.pos.x+3] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to tale and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, loot, ch, maxX, width, FALSE, FALSE);
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
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, FALSE, h1, m , b, loot, ch, maxX, width);
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
               thinking = userControl(Maze, Output, Stats, Score, maze, temp_maze, thinking, TRUE, h1, m , b, loot, ch, maxX, width);
            }
            break;
         }else if(temp_maze[h1.pos.y][h1.pos.x-2] == 5){                                                 //Points Assignment
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"Found Loot Box.");
            mvwprintw(Output,2,1,"Vadim must be jealous");
            mvwprintw(Output,3,1,"Press 'y' to take and assign the points or 'r' to walk away.");  
            wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, temp_maze, &h1, loot, ch, maxX, width, FALSE, FALSE);
            }
            break;
         }
      case KEY_END:
         end_game = TRUE;
         break;      
     }
   }   
   delwin(Maze); 
   delwin(Output);                           
   delwin(Stats);  
   delwin(Score);  
   endwin();       
}




 // save the maze.
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
void GenerateMaze(char *maze,char **temp_maze, int width, int height,human h1, int monsterRestriction) {
   
      int x, y;
      int step = 0;
      /* Initialize the maze. */
      for(x = 0; x < width * height; x++) {
         maze[x] = 1;
      }
      maze[1 * width + 1] = 0;  //to amesws epomeno keli einai open, gia na exei starting point h carveMAze()
      
      /* Seed the random number generator. */
      srand(time(0));


      /* Carve the maze. */
      for(y = 1; y < height; y += 2) {       //y+=2 gia na exei duades gia ton elegxo
         for(x = 1; x < width; x += 2) {    
           monsterRestriction = CarveMaze(maze, width, height, x, y, monsterRestriction);
         }
      }

   
      //default eisodos k eksodos apo to lavurintho, maze[0][1] kai maze[..][..]
      maze[1] = 2;
      maze[(height - 1) * width + (width - 2)] = 4;

      for (int y = 0; y < height; y++)
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



void next_level(human h1, monster m, boss b, loot_stats* loot, char **temp_maze, int width, bool defeated,const char *maze){

   int monsters;
   h1.pos.x = 5;
   h1.pos.y = 0;
   if (defeated == TRUE)
   {
      h1.stats.loses++;
      mainController(maze, width, width, temp_maze, h1, m, b, loot, FALSE,FALSE);

   }
   else{
      h1.stats.wins++;
      h1.stats.level++;
      char *maze;
      char **temp_maze = (char **)malloc((width*4)*sizeof(char*));
      for (int i = 0; i < width; ++i)
      {
            temp_maze[i] = (char *)malloc((width*4)*sizeof(char));    
      }
    
      monsters = h1.stats.level;

      upgradeEnemies(&m, &b, monsters-1);

      maze = (char*)malloc(width * width * sizeof(char));

      GenerateMaze(maze,temp_maze,width,width,h1,monsters);
      saveMaze(maze,width,width);
      mainController(maze, width, width, temp_maze, h1, m, b, loot, FALSE,FALSE);
   }
}



//User Login Function
int userLogin(human *h1){

   system("clear");
   int i=0,k=0;
   char temp;
   char username[60];
   char password[60];
   char c[60],b[60];
   FILE *fp;
   printf("Please enter your username: ");
   scanf("%s",(char *)&username);
   printf("Please enter your password: ");
   scanf("%s",(char *)&password);

   if((fp=fopen("users","r")) != NULL)     //r => Read from file, file must exist else returns NULL.
   {                              
      size_t len = 0;
      ssize_t read;
      char * line = NULL;

      while(fscanf(fp,"%s",(char *)c)!=EOF ) //diavazei ta strings tou arxeiou grammh pros grammh kai apo8hkeuei to ka8e ena sthn u1 mexri na ftasei sto telos tou arxeiou
      {
         if(strcmp(c,username)==0)     //strcmp(str1,str2) => Compare str1 to str2, if equal then returns 0.
         {
            k++;
            printf("Your username is correct.\n");
            strcpy(h1->stats.username, username);
            sleep(2);
            fscanf(fp,"%s",(char *)b);
            if (strcmp(b,password)==0)
            {
               printf("Your password is correct.\nLogging in...\n");
               k++;
               sleep(3);
               fclose(fp);
               return 1;
               // break;   
            }
            break;
         }
      }
   }
   else{
      printf("\nError: File doesn't exist.\n");
      printf("If this persists consider making a new registration.\nPlease try again.\n");
      printf("Would you like to make a new registration?(y/n): ");
      scanf(" %c",&temp);
      if (temp == 'y'){
         return 0;
      }
      else{
         printf("\nRefreshing...\n");
         sleep(3);
         userLogin(h1);
      }
   }
   if(k<2){
      printf("\nError: Credentials don't match.\n");
      printf("If this persists consider making a new registration.\nPlease try again.\n");
      printf("Would you like to make a new registration?(y/n): ");
      scanf(" %c",&temp);
      if (temp == 'y'){
         return 0;
      }
      else{
         printf("\nRefreshing...\n");
         sleep(3);
         userLogin(h1);
      }
   }


}//End of userLogin Function.



//Register Function
int userRegistration(human *h1){
   system("clear");
   FILE *fp;
   char username[60];
   char password[60];
   char u1[60];
   int k=0;

   printf("Please enter desired username: ");
   scanf("%s",(char*)&username);
   strcpy(h1->stats.username, username);
   printf("Please enter desired password: ");
   scanf("%d",&h1->stats.password);

   //send(h1);
   //Send the rest of the script to server side script
   

      if(!(fp=fopen("users","r")))     //r => Read from file, file must exist
       {                              
         fp=fopen("users","w+");
         fclose(fp);
       }
       fp=fopen("users","r");

       while(fscanf(fp,"%s",(char *)u1)!=EOF)      //Loop reads file line by line and save in each line until end of file( eof ).
       {  
         if(strstr(u1,username)!=0)                //Elegxei an sthn string u1 uparxei h substring username epistrefei 0 an den uparxei.
         {
           printf("\nYour username is already taken\nYour password will remain the same.\n");
           printf("Please input another username: ");
           scanf("%s",(char *)&username);
           strcpy(h1->stats.username, username);

         }
       }

      fclose(fp);
      fp=fopen("users","a");      //a => Append( Write at the end of the file ) - SERVER SIDE.  
      fprintf(fp,"%s\n", h1->stats.username);      //SERVER SIDE      
      fprintf(fp,"%d\n\n",h1->stats.password);     //SERVER SIDE
      printf("Your account has been created\n");
      system("clear");
      fclose(fp);

   return 1;
}//End of userRegistration Function.



//Retrieve player stats from file - SERVER SIDE
void load_stats(human *h1)
{
   FILE *fp;
   char c[256], filename[256];
   sprintf(filename, "%s_stats", h1->stats.username);
   fp=fopen(filename,"r");    //r+ => Opens a file to update both reading and writing. The file must exist.

   fscanf(fp,"%d\n",&h1->stats.health);
   fscanf(fp,"%d\n",&h1->stats.armor);
   fscanf(fp,"%d\n",&h1->stats.attack);
   fscanf(fp,"%g\n",&h1->stats.accuracy);
   fscanf(fp,"%d\n",&h1->stats.level);
   fscanf(fp,"%d\n",&h1->stats.wins);
   fscanf(fp,"%d\n",&h1->stats.loses);
   fclose(fp);
   
}



//1st Function to be called - Displays Welcoming Bullshit.
bool gameInitiation(human *h1){
   int userChoice, flag;
   system("clear");
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   printf("                Welcome to ProjectOS.c\n");
   printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
   sleep(4);
   system("clear");
   while(1){
      printf("Please chose between register,login, or exit:\n");
      printf("Press '1' to Register.\nPress '2' to Log in.\nPress '3' to Exit.\nUser's Choice: ");
      scanf("%d",&userChoice);
      if (userChoice < 4 && userChoice > 0)
      {
         flag = 0;
         while(flag == 0){
            if (userChoice == 1)
            {
               flag = userRegistration(h1);
               return FALSE;
            }
            else if (userChoice == 2)
            {
               flag = userLogin(h1);         //An h userLogin() kanei return 0, tha kalesei thn userRegistration().
               if (flag == 1)                //Alliws de mas noiazei to userChoice. 
               {
                  load_stats(h1);            //Send flag to server and recieve human stats from file.
                  return TRUE;
               }                      
               userChoice = 1;                           
            }
            else if (userChoice == 3)
            {
               printf("Exiting...\n");
               sleep(2);
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


int main(void) {

   human h1;
   h1 = humanInit(h1);

   bool loaded;
   loaded = gameInitiation(&h1);

   loot_stats loot;
   loot.points = 0;
 
   monster m;
   m = monsterInit(m);

   boss b;
   b = bossInit(b);

   int monsters = h1.stats.level -1;;
   char *maze;
   int i,j;


   //THE REST GO TO SERVER EXCEPT THE MAIN CONTROLLER
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
   if(maze == NULL) {                                 
      printf("error: not enough memory\n");                                
      exit(0);                               
   }
   GenerateMaze(maze,temp_maze,j,j,h1,monsters);                                 
   saveMaze(maze,j,j);                                
   mainController(maze, j, j, temp_maze, h1, m, b, &loot, TRUE,loaded);                                

return 0;
}  