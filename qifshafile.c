#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
   


//***General Notes***
// Size of maze has to be by more than 7.
// Width-Height have to match and be uneven or else maze will not be complete

//compile with: gcc mazeCreation.c -lncurses


// struct human_stats;
// struct human_pos;
// struct boss_stats;
// struct boss_pos;
// struct monster_pos;
// struct monster_stats



/////////Loot////////////
typedef struct 
{
   int points;

}loot_stats;

/////////Humans////////////

typedef struct 
{
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



/////////////Mmultiplayer Prototype///////////////
// typedef struct multiPlayer
// {

   
// }multiPlayer;


// gameStatsInit
human humanInit(human h){

   h.stats.health = 300;
   h.stats.armor = 10;
   h.stats.attack = 10;
   h.stats.accurasy = 0.9;
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
   m.stats.accurasy = 0.3;
   m.stats.level = 1;

   return m;
}

boss bossInit(boss b){

   b.stats.health = 30;
   b.stats.armor = 5;
   b.stats.attack = 15;
   b.stats.accurasy = 0.3;
   b.stats.level = 1;

   return b;
}





//Erase previous position of da smiley
void clearHistory(human h1, WINDOW *win){

	mvwprintw(win, h1.pos.y, h1.pos.x,"  ");
	wrefresh(win);

}


//Clear Output Window 
void clearWindow(WINDOW *win, int maxX){

	for ( int i = 1; i < 5; ++i)
	{
      for (int j = 0; j < maxX; ++j)
      {
         mvwprintw(win,i,j," ");
      }
	}
	wrefresh(win);   
}



int human_Attack_monster(WINDOW *win, human h,monster m, boss b){  
   int damage;
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   char health[256];      
   damage = h.stats.attack - m.stats.armor;
   if(hit <= h.stats.accurasy){

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


int monster_Attack_human(WINDOW *win, human h1,monster m, boss b){  
   int damage;   
   char health[256];                               
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   damage = m.stats.attack - h1.stats.armor;  //damage = 5
   if(hit <= m.stats.accurasy){
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



int human_Attack_boss(WINDOW *win, human h,monster m, boss b){  
   int damage;
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   char health[256];
      
   damage = h.stats.attack - b.stats.armor;

   
   

   if(hit <= h.stats.accurasy){

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


int boss_Attack_human(WINDOW *win, human h1,monster m, boss b){  
   int damage;   
   char health[256];                               
   int rnd = rand()%100;      
   float hit = rnd/100.0;
   damage = b.stats.attack - h1.stats.armor;  //damage = 5
   
   if(hit <= b.stats.accurasy){
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

int battleController(WINDOW *win, human h1, monster m, boss b, int swit){
   int winner;
   // char health[256];
   bool fighting = TRUE;

   switch(swit){
      case 1:
         do{
               time_t t;
               srand((unsigned) time(&t));
               wrefresh(win);
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


bool assignPoints(WINDOW *window, WINDOW *target, WINDOW *stats,WINDOW *Score, char **temp_maze, bool thinking, bool bossEncounter, human h1, monster m, boss b, int direction, int maxX)
{    int points=50;
    int i,winner, wins, loses,k;
    char ch,st;
    ch = wgetch(window);
    char health[256];    
   char armor[256];
   char attack[256];
   char accurasy[256];



   sprintf(health,"%d",h1.stats.health);
   sprintf(armor,"%d",h1.stats.armor);
   sprintf(attack,"%d",h1.stats.attack);
   sprintf(accurasy,"%g",h1.stats.accurasy);
    
 switch(ch)
 {  
    case 'y':
    curs_set(TRUE);
    echo();
    switch(direction){
       case KEY_UP:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y-1, h1.pos.x, "  ");  //Vasilw
                 
                  mvwprintw(target, 1, 1, "Oi! $)");
                  wrefresh(window);
                  wmove(target, 1, 1);
                  wrefresh(target);
                  temp_maze[h1.pos.y-2][h1.pos.x+1] = 0;
                  temp_maze[h1.pos.y-1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y-1][h1.pos.x+1] = 0;
                
                //curs_set(TRUE);
                while(points!=0)
                {  
                  
                  
                  switch(st)
                  { case 1 :
                  	   
                       h1.stats.health+=10;
                       points-=10;
                       mvwprintw(target, 2, 1,health);
                       wrefresh(target);
                       break;
                    case 2 :
                       
                       h1.stats.attack+=10;
                       points-=10;
                       mvwprintw(target, 3, 1, attack);
                       wrefresh(target);
                       break;
                    case 3 :
                        
                        h1.stats.armor+=10;
                        points-=10;
                        mvwprintw(target, 4, 1, armor);
                        wrefresh(target);
                        break;
                    case 4 :
                        
                        h1.stats.accurasy+=0.1;
                        points-=10;
                        mvwprintw(target, 5, 1, accurasy);
                        wrefresh(target);
                        break; 
                     thinking = FALSE;
                     break;


                  }
                  break;
              }
               case KEY_DOWN:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y+1, h1.pos.x, "  ");
                  //mvwprintw(window, h1.pos.y+2, h1.pos.x, "  ");
                  mvwprintw(target, 1, 1, "BLYN! Let's squat!");
                  wrefresh(window);
                  wmove(target, 1, 1);
                  wrefresh(target);
                  temp_maze[h1.pos.y+1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y+1][h1.pos.x+1] = 0;
                //  temp_maze[h1.pos.y+2][h1.pos.x] = 0;
                  temp_maze[h1.pos.y+2][h1.pos.x+1] = 0;
                  // curs_set(TRUE);
                   while(points!=0)
                {  
                  
                  switch(st)
                  { case 1 :
                  	   
                       h1.stats.health+=10;
                       points-=10;
                       mvwprintw(target, 2, 1,health);
                       wrefresh(target);
                       break;
                    case 2 :
                       
                       h1.stats.attack+=10;
                       points-=10;
                       mvwprintw(target, 3, 1,attack);
                       wrefresh(target);
                       break;
                    case 3 :
                        wmove(target, 4, 1);
                        h1.stats.armor+=10;
                        points-=10;
                        mvwprintw(target, 4, 1,armor);
                        wrefresh(target);
                        break;
                    case 4 :
                        
                        h1.stats.accurasy+=0.1;
                        points-=10;
                        mvwprintw(target, 5, 1,accurasy);
                        wrefresh(target);
                        break; 
                     thinking = FALSE;
                  
                    break;

                  }
                  break;
              }
                  thinking = FALSE;
                  break;
               case KEY_RIGHT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y, h1.pos.x+2, "  ");
                  mvwprintw(target, 1, 1, "BLYAT!These are some points!");
                  wrefresh(window);
                  wmove(target, 1, 1);
                  wrefresh(target);
                  for (i = 3; i < 5; ++i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x + i] = 0;
                  }
                  //curs_set(TRUE);
                   while(points!=0)
                {  
                  
                  
                  switch(st)
                  { case 1:
                  	   
                       h1.stats.health+=10;
                       points-=10;
                       mvwprintw(target, 2, 1,health);
                       wrefresh(target);
                       break;
                    case 2:
                       
                       h1.stats.attack+=10;
                       points-=10;
                       mvwprintw(target, 3, 1, attack);
                       wrefresh(target);
                       break;
                    case 3:
                        
                        h1.stats.armor+=10;
                        points-=10;
                        mvwprintw(target, 4, 1, armor);
                        wrefresh(target);
                        break;
                    case 4:
                        
                        h1.stats.accurasy+=0.1;
                        points-=10;
                        mvwprintw(target, 5, 1, accurasy);
                        wrefresh(target);
                        break; 
                     thinking = FALSE;
                  
                   break;

                  }
                  break;

                }
                  thinking = FALSE;
                  break;
               case KEY_LEFT:
                  clearWindow(target, maxX);
                  mvwprintw(window, h1.pos.y, h1.pos.x-5, "    ");
                  mvwprintw(target, 1, 1, "HA HA SUCK IT VADIM!");
                  wrefresh(window);
                  wmove(target, 1, 1);
                  wrefresh(target);
                  for (i = 5; i >=2 ; --i)
                  {
                     temp_maze[h1.pos.y][h1.pos.x - i] = 0;
                  }
                   while(points!=0)
                { //curs_set(TRUE); 
                  
                  
                  switch(st)
                  { case 1:
                  	   
                       h1.stats.health+=10;
                       points-=10;
                       mvwprintw(target, 2, 1,health);
                       wrefresh(target);
                       break;
                    case 2:
                       
                       h1.stats.attack+=10;
                       points-=10;
                       mvwprintw(target, 3, 1,attack);
                       wrefresh(target);
                       break;
                    case 3:
                        
                          h1.stats.armor+=10;
                        points-=10;
                        mvwprintw(target, 4, 1,armor);
                        wrefresh(target);
                        break;
                    case 4:
                        
                        h1.stats.accurasy+=0.1;
                        points-=10;
                        mvwprintw(target, 5, 1,accurasy);
                        wrefresh(target);
                        break; 
                     thinking = FALSE;
                   
                  break;

                  }
                  }//
                  thinking = FALSE;
                  break;               
               }
            break;
    case 'n':
    	clearWindow(target, 50);
    	mvwprintw(target, 1, 1, "Come back later or Vadim will take them!");
    	wrefresh(target);
          return FALSE ;
          break;
          }
       //return thinking;
}

bool userControl(WINDOW *window, WINDOW *target, WINDOW *stats,WINDOW *Score, char **temp_maze, bool thinking, bool bossEncounter, human h1, monster m, boss b, int direction, int maxX,bool money){
   int ch, i,winner, wins, loses,k;
   ch = wgetch(window);
   char health[256];
   char score[56];
   
   
   switch(ch)
   {
      case 'y':
         
         
         if(bossEncounter == FALSE){
             clearWindow(target, maxX);
             mvwprintw(target,1,1,"Fighting commenced");  
             wrefresh(target);
             wrefresh(window);
             sleep(1);
             winner =  battleController(target,h1, m, b, 1); 
             wrefresh(window);
             mvwprintw(target,1,1,"Fighting ended.");
             clearWindow(target, maxX);  
            if (winner == 1)
            {  
               mvwprintw(target,2,1,"Winner was declared the human pizdek."); 
               mvwprintw(target, 3, 1, "The path is free you western spy.");
               //clearWindow(target, maxX);
            
               switch(direction){
               case KEY_UP:
                  mvwprintw(window, h1.pos.y-1, h1.pos.x, "   ");  //Vasilw                 
                  wrefresh(window);
                  temp_maze[h1.pos.y-1][h1.pos.x] = 0;
                  temp_maze[h1.pos.y-1][h1.pos.x+1] = 0;
                  break;
               case KEY_DOWN:
                  mvwprintw(window, h1.pos.y+1, h1.pos.x, "   ");
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
               mvwprintw(stats, 3, 62, "0 ");      //Zero Vasilw health
               h1.stats.wins++;
               sprintf(score,"%d",h1.stats.wins);
               mvwprintw(Score, 3, 5, score);  //WINS
            
           }
          
            else{
               mvwprintw(target,2,1,"We lost bois. CY@");
               mvwprintw(window,h1.pos.y,h1.pos.x, "  ");   //Erase human
               mvwprintw(stats, 2, 9, "0 ");    //Zero humam health
               h1.stats.loses++;
               sprintf(score,"%d",h1.stats.loses);
               mvwprintw(Score, 3, 9, score);  //LOSES
            }
            wrefresh(target);
            wrefresh(stats);
            wrefresh(Score);
            thinking = FALSE;
            break;
         }
         else if(bossEncounter == TRUE){
            clearWindow(target, maxX);
            winner =  battleController(target,h1, m, b, 2);
            
            
            mvwprintw(target,1,1,"Fighting ended.");  
            if (winner == 1)
            {  clearWindow(target, maxX); 
               mvwprintw(target,2,1,"Winner was declared the human pizdek."); 
               mvwprintw(target, 3, 1, "The path is free you western spy.");
               mvwprintw(window,h1.pos.y+1,h1.pos.x, "  "); //Erase boss
               temp_maze[h1.pos.y+1][h1.pos.x] = 0;
               temp_maze[h1.pos.y+1][h1.pos.x+1] = 0;
               mvwprintw(stats, 6, 62, "0 ");      //Zero Big Boss health
               h1.stats.wins++;
               sprintf(score,"%d",h1.stats.wins);
               mvwprintw(Score, 3, 5, score);  //WINS
            }
            else{
               mvwprintw(target,2,1,"We lost bois. CY@");
               mvwprintw(window,h1.pos.y,h1.pos.x, "  ");   //Erase human
               mvwprintw(stats, 2, 9, "0 ");    //Zero humam health
               h1.stats.loses++;
               sprintf(score,"%d",h1.stats.loses);
               mvwprintw(Score, 3, 9, score);  //LOSES
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
void mainController(const char *maze, int width, int height,char **temp_maze, human h1,monster m, boss b, int file_num){
 
   // File stuff
   char buf[width*4];
   FILE *file;
   size_t nread;


   int notif_size = 7;
   int score_size = 8;
   int maxX, maxY, dx, dy, ch, response, line;
   int x = 0, y = 0;   

   char health[256];    
   char armor[256];
   char attack[256];
   char accurasy[256];
   char level[256];
   
   bool thinking, bossEncounter, start, end_game,money;

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
   sprintf(health,"%d",h1.stats.health);
   sprintf(armor,"%d",h1.stats.armor);
   sprintf(attack,"%d",h1.stats.attack);
   sprintf(accurasy,"%g",h1.stats.accurasy);
   sprintf(level,"%d",h1.stats.level);

   mvwprintw(Stats, 0, 0, "Player1:");
   mvwprintw(Stats, 2, 1, "Health:");
   mvwprintw(Stats, 2, 9, health);
   mvwprintw(Stats, 3, 1, "Armor:");
   mvwprintw(Stats, 3, 8, armor);
   mvwprintw(Stats, 4, 1, "Attack:");
   mvwprintw(Stats, 4, 9, attack);
   mvwprintw(Stats, 5, 1, "Accurasy:");
   mvwprintw(Stats, 5, 11, accurasy);
   mvwprintw(Stats, 6, 1, "Level:");
   mvwprintw(Stats, 6, 8, level);

   //Player2 stats
   // sprintf(health,"%d",p2.stats.health);
   // sprintf(armor,"%d",p2.stats.armor);
   // sprintf(attack,"%d",p2.stats.attack);
   // sprintf(accurasy,"%f",p2.stats.accurasy);
   
   // mvwprintw(Stats, 0, 16, "Player2:");
   // mvwprintw(Stats, 2, 17, "Health:");
   // mvwprintw(Stats, 2, 25, health);
   // mvwprintw(Stats, 3, 17, "Armor:");
   // mvwprintw(Stats, 3, 24, armor);
   // mvwprintw(Stats, 4, 17, "Attack:");
   // mvwprintw(Stats, 4, 25, attack);
   // mvwprintw(Stats, 5, 17, "Accurasy:");
   // mvwprintw(Stats, 5, 27, accurasy);
   // mvwprintw(Stats, 6, 17, "Level:");
   // mvwprintw(Stats, 6, 24, level);


   //Player3 stats
   // sprintf(health,"%d",p3.stats.health);
   // sprintf(armor,"%d",p3.stats.armor);
   // sprintf(attack,"%d",p3.stats.attack);
   // sprintf(accurasy,"%f",p3.stats.accurasy);
   
   // mvwprintw(Stats, 0, 33, "Player3:");
   // mvwprintw(Stats, 2, 34, "Health:");
   // mvwprintw(Stats, 2, 42, health);
   // mvwprintw(Stats, 3, 34, "Armor:");
   // mvwprintw(Stats, 3, 41, armor);
   // mvwprintw(Stats, 4, 34, "Attack:");
   // mvwprintw(Stats, 4, 42, attack);
   // mvwprintw(Stats, 5, 34, "Accurasy:");
   // mvwprintw(Stats, 5, 44, accurasy);
   // mvwprintw(Stats, 6, 34, "Level:");
   // mvwprintw(Stats, 6, 41, level);


   //Vasilw
   mvwprintw(Stats, 2, 53, "Vasilw:");
   sprintf(health,"%d",m.stats.health);
   mvwprintw(Stats, 3, 54, "Health:");
   mvwprintw(Stats, 3, 62, health);


   //Final Boss
   mvwprintw(Stats, 5, 53, "Big Boss:");
   sprintf(health,"%d",b.stats.health);
   mvwprintw(Stats, 6, 54, "Health:");
   mvwprintw(Stats, 6, 62, health);


   //Score
   mvwprintw(Score, 0, 4, "!Score!");
   
   mvwprintw(Score, 3, 7, "-");
   
   mvwprintw(Score, 5, 5, "W");
   mvwprintw(Score, 5, 9, "L");

   
   //Refresh Window          
   wrefresh(Output);               
   wrefresh(Stats); 
   wrefresh(Score); 
   sleep(1); 
   

   //Read file function
   file = fopen("level1.txt","r");
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
   start = TRUE;     //Player is at the starting posistion
   end_game = FALSE; 
  
   while(end_game == FALSE){
      thinking = TRUE;
      bossEncounter = FALSE;
   	keypad(Maze,TRUE);
	   nodelay(Maze, TRUE);
       
   	ch = wgetch(Maze);
	switch (ch){
	   case KEY_UP:
   	   if (h1.pos.y == 0){
   	   		mvwprintw(Output,1, 1, "Glupi kurwa, you can't go up, go down...BLYAT!");
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
                  thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX,FALSE);
               }
               break;
	   	}else if(temp_maze[h1.pos.y-1][h1.pos.x] == 4){
   	   		clearWindow(Output,maxX);
   	   		mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
   	   		mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
   	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");	
   	   		wrefresh(Output);
               while(thinking){
                  thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, TRUE, h1, m , b, ch, maxX,FALSE);
               }
   	   		break;
   	   }
       else if(temp_maze[h1.pos.y-1][h1.pos.x] == 5){
	   		mvwprintw(Output,1,1,"KURWA! You have found money");
	   		mvwprintw(Output,2,1,"Do you wish to take them and run off? or ");
	   		mvwprintw(Output,3,1,"Press 'y' to squat or 'r' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX);
            }
	   		break;
	   	}  
	   case KEY_DOWN:
         if(h1.pos.y == (height-1)){
            clearWindow(Output,maxX);
            mvwprintw(Output,1,1,"You beat the game...advansing level in 1 second.");
            wrefresh(Output);
            sleep(1);
            //Add new level function
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
	   		mvwprintw(Output,1,1,"BLYAT! you found rare monster.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX,FALSE);
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
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, TRUE, h1, m , b, ch, maxX, FALSE);
            }
	   		break;
	   	}
       else if(temp_maze[h1.pos.y+1][h1.pos.x] == 5){
	   		mvwprintw(Output,1,1,"KURWA! You have found money");
	   		mvwprintw(Output,2,1,"Do you wish to take them and run off? or ");
	   		mvwprintw(Output,3,1,"Press 'y' to squat or 'r' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX);
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
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX,FALSE);
            } 
	   		break;
	   	}
	   	else if(temp_maze[h1.pos.y][h1.pos.x + 2] == 4){
	   		mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, TRUE, h1, m , b, ch, maxX,FALSE);
            }
	   		break;
	   	}
        else if(temp_maze[h1.pos.y][h1.pos.x + 3] == 5){
	   		mvwprintw(Output,1,1,"KURWA! You have found money");
	   		mvwprintw(Output,2,1,"Do you wish to take them and run off? or ");
	   		mvwprintw(Output,3,1,"Press 'y' to squat or 'n' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX);
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
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX,FALSE);
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
               thinking = userControl(Maze, Output, Stats, Score, temp_maze, thinking, TRUE, h1, m , b, ch, maxX,FALSE);
            }
	   		break;
	   	}
        else if(temp_maze[h1.pos.y][h1.pos.x - 2] == 5){
	   		mvwprintw(Output,1,1,"KURWA! You have found money");
	   		mvwprintw(Output,2,1,"Do you wish to take them and run off? or ");
	   		mvwprintw(Output,3,1,"Press 'y' to squat or 'r' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = assignPoints(Maze, Output, Stats, Score, temp_maze, thinking, FALSE, h1, m , b, ch, maxX);
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




 // Display&save the maze.
void saveMaze(const char *maze, int width, int height,int file_num) {
   int i, x, y;


   FILE *files[10];
   char filename[20];

   sprintf(filename, "level%d.txt", file_num);
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
      maze[1 * width + 1] = 0;  //to amesws epomeno keli einai open, gia na exei lush 
      
      /* Seed the random number generator. */
      srand(time(0));


      /* Carve the maze. */
      for(y = 1; y < height; y += 2) {       //y+=2 gia na exei duades gia ton elegxo
         for(x = 1; x < width; x += 2) {    
           monsterRestriction = CarveMaze(maze, width, height, x, y, monsterRestriction);
           // printf("%d\n", monsterRestriction);
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


int main(int argc,char *argv[]) {

   human h1;
   human h2;
   human h3;

   int multiPlayer = 1;
   if (multiPlayer == 0)
   {
      h1 = humanInit(h1);
   }
   else{
      h1 = humanInit(h1);
      h2 = humanInit(h1);
      h3 = humanInit(h1);
   }
 
   monster m;
   m = monsterInit(m);

   boss b;
   b = bossInit(b);



   

   int width, height;
   int file_num=1;      
   char *maze;
   int monsters = 3;
   

   //Dynamic part
   // char **temp_maze = (char **)malloc((height*4)*sizeof(char*));
   
   // for (int i = 0; i < height; ++i)
   // {
   // 		temp_maze[i] = (char *)malloc((width*4)*sizeof(char));
   // }


   //Hardcoded part for j  -   legal values 7 9 11 13 15 17 19 21
   int j = 7; 
   char **temp_maze = (char **)malloc((j*4)*sizeof(char*));
   for (int i = 0; i < j; ++i)
   {
   		temp_maze[i] = (char *)malloc((j*4)*sizeof(char));    
   }

      
   // for (int i = 7; i < 15; i+=2)  
   // {  
      
      maze = (char*)malloc(j * j * sizeof(char));
      if(maze == NULL) {
         printf("error: not enough memory\n");
         exit(EXIT_FAILURE);
      }
      GenerateMaze(maze,temp_maze,j,j,h2,monsters);
      saveMaze(maze,j,j,file_num);
      printf("\n");
      mainController(maze, j, j, temp_maze, h1, m, b, file_num);

      free(maze);
      free(temp_maze);
      file_num++;
   // }
   return 0;
}