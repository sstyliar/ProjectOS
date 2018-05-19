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
   int accurasy;
   int level;

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
   int accurasy;
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
   int accurasy;
   int level;
   
}boss_stats;

// typedef struct 
// {
// 	int x;
// 	int y;
// }boss_pos;

typedef struct 
{
   // boss_pos pos;
   boss_stats stats;
}boss;



// ///////////Mmultiplayer Prototype///////////////
// typedef struct multiPlayer
// {

   
// }multiPlayer;




//Erase previous position of da smiley
void clearHistory(human h1, WINDOW *win){

	mvwprintw(win, h1.pos.y, h1.pos.x,"  ");
	wrefresh(win);

}

//Clear Output Window 
void clearWindow(WINDOW *win, int maxX){
	//temp solution
	char *empty_string = "                                                             ";
	for ( int i = 1; i < 5; ++i)
	{
		mvwprintw(win,i,1,empty_string);
	}
	wrefresh(win);
}


bool userControl(WINDOW *win, WINDOW *target){
   int ch;
   bool thinking = TRUE;
   ch = wgetch(win);
   switch(ch)
   {
      case 'y':
         clearWindow(target, 3);
         mvwprintw(target, 1, 1, "Under Construction...");
         wrefresh(target);
         thinking = FALSE;
         break;
      case 'n':
         clearWindow(target, 3);
         mvwprintw(target, 1, 1, "im sleepyyyyyyyyy see you tomorrow");
         wrefresh(target);
         thinking = FALSE;
         break;
      case 'r':
         clearWindow(target, 3);
         mvwprintw(target, 1, 1, "You are free to go for now.");
         wrefresh(target);
         thinking = FALSE;
         break;
   }

   return thinking;
}

   



//Function to move human
void mainController(const char *maze, int width, int height,char **temp_maze, human h1,monster m1,boss b1,int file_num){
 
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
   
   bool thinking;

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
   sprintf(accurasy,"%d",h1.stats.accurasy);
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
   // sprintf(accurasy,"%d",p2.stats.accurasy);
   
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
   // sprintf(accurasy,"%d",p3.stats.accurasy);
   
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
   mvwprintw(Stats, 3, 54, "Health:");
   mvwprintw(Stats, 3, 62, health);


   //Final Boss
   mvwprintw(Stats, 5, 53, "Big Boss:");
   mvwprintw(Stats, 6, 54, "Health:");
   mvwprintw(Stats, 6, 62, health);


   //Score
   mvwprintw(Score, 0, 4, "!Score!");
   mvwprintw(Score, 3, 4, "13");  //WINS
   mvwprintw(Score, 3, 7, "-");
   mvwprintw(Score, 3, 9, "65");  //LOSSES
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
   mvwprintw(Maze,h1.pos.y,h1.pos.x,":)");
   wrefresh(Maze);

   while(TRUE){
      thinking = TRUE;
   	keypad(Maze,TRUE);
	   nodelay(Maze, TRUE);
   	ch = wgetch(Maze);
	switch (ch){
	   case KEY_UP:
	   if (h1.pos.y == 0)
	   {
	   		mvwprintw(Output,1, 1, "Glupi kurwa, you can't go up, go down...BLYAT!");
	   		wrefresh(Output);
	   		break;
	   }
	   else if ((temp_maze[h1.pos.y-1][h1.pos.x] == 0) && (temp_maze[h1.pos.y-1][h1.pos.x +1] != 1))
	   {	
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
	   	}
	   else if(temp_maze[h1.pos.y-1][h1.pos.x] == 3){
	   		clearWindow(Output,maxX);
	   		mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output);
            }
            break;
	   	}
	   else if(temp_maze[h1.pos.y-1][h1.pos.x] == 4){
	   		clearWindow(Output,maxX);
	   		mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");	
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output);
            }
	   		break;
	   	}
	   		
	   case KEY_DOWN:
	   if (temp_maze[h1.pos.y+1][h1.pos.x] == 0 && temp_maze[h1.pos.y+1][h1.pos.x +1] != 1)   
	    {
	    	clearHistory(h1, Maze);
	    	clearWindow(Output,maxX);
	    	mvwprintw(Maze,0,4,"{||}");
	    	mvwprintw(Maze,h1.pos.y + 1,h1.pos.x,":)");
	   		wrefresh(Maze);
	   		h1.pos.y = h1.pos.y + 1;
	   		break;
	    }else if(temp_maze[h1.pos.y+1][h1.pos.x] == 1){
	    	clearWindow(Output,maxX);
	   		mvwprintw(Output,1, 1, "BLYAT! you Press wall");
	   		wrefresh(Output);
	   		break;
	   	}
	   	else if(temp_maze[h1.pos.y+1][h1.pos.x] == 3){
	   		clearWindow(Output,maxX);
	   		mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
            mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output);
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
               thinking = userControl(Maze, Output);
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
               thinking = userControl(Maze, Output);
            }
	   		break;
	   	}
	   	else if(temp_maze[h1.pos.y][h1.pos.x + 2] == 4){
	   		mvwprintw(Output,1,1,"BLYAT! BLYAT! BLYAT! vrikes Final Boss.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and win your freedom?");
	   		mvwprintw(Output,3,1,"Press 'y' to start fighting or 'r' to walk away.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output);
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
	   		mvwprintw(Output,1,1,"BLYAT! you found rare monster Vasilw.");
	   		mvwprintw(Output,2,1,"Do you wish to fight and be rewarded upon victory?");
            mvwprintw(Output,3,1,"Press 'y' to start fighting or 'n' to decline or 'r' to run.");
	   		mvwprintw(Output,4,1,"If you decline Vasilw will leave with the money.");
	   		wrefresh(Output);
            while(thinking){
               thinking = userControl(Maze, Output);
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
               thinking = userControl(Maze, Output);
            }
	   		break;
	   	}
	   	case KEY_END:
	   	   delwin(Maze); 
   		   delwin(Output);                           
   		   delwin(Stats);  
   		   delwin(Score);  
   		   endwin();
   		   break;
	   		
	  }
   }          
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
         // case 2: fprintf(files[i]," :) "); break;   //Human
         case 3: fprintf(files[i]," :/ "); break;   //Monster
         case 4: fprintf(files[i]," :# "); break;   //Boss
         case 5: fprintf(files[i]," $$ "); break;   //Loot
         default: fprintf(files[i],"    "); break;  //Free space
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
   int phantom_pos, loot_pos;


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
      } else {
        dir = (dir + 1) % 4;  //auksanei to dir alla to krataei panta <4
        count += 1;
        flag++;
      }
   }
   maze[phantom_pos] = 3;
   maze[loot_pos] = 5;
   return monsterRestriction-1;
}

 // Generate maze in matrix maze with size width, height.
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
      for(y = 1; y < height; y += 2) {       //y+=2 gia na exei treiades gia ton elegxo
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


int main(int argc,char *argv[]) {

   human h1;
   h1.stats.health = 90;
   h1.stats.armor = 45;
   h1.stats.attack = 60;
   h1.stats.accurasy = 100;
   h1.stats.level = 1;
   h1.pos.y = 0;
   h1.pos.x = 5;

   monster m1;
   m1.stats.health = 90;
   m1.stats.armor = 45;
   m1.stats.attack = 60;
   m1.stats.accurasy = 100;
   m1.stats.level = 1;

   boss b1;
   b1.stats.health = 90;
   b1.stats.armor = 45;
   b1.stats.attack = 60;
   b1.stats.accurasy = 3;
   b1.stats.level = 1;

   int width, height;
   int file_num=1;      
   char *maze;
   int monsters = 1;
   

   //Dynamic part
   // char **temp_maze = (char **)malloc((height*4)*sizeof(char*));
   
   // for (int i = 0; i < height; ++i)
   // {
   // 		temp_maze[i] = (char *)malloc((width*4)*sizeof(char));
   // }


   //Hardcoded part for i=7
   char **temp_maze = (char **)malloc((7*4)*sizeof(char*));
   for (int i = 0; i < 28; ++i)
   {
   		temp_maze[i] = (char *)malloc((7*4)*sizeof(char));
   }

   
   
   // for (int i = 7; i < 15; i+=2)  // 7 9 11 13 15 anything more give segmentation fault
   // {  
      int i = 7;
      maze = (char*)malloc(i * i * sizeof(char));
      if(maze == NULL) {
      printf("error: not enough memory\n");
      exit(EXIT_FAILURE);
      }
      GenerateMaze(maze,temp_maze,i,i,h1,monsters);
      saveMaze(maze,i,i,file_num);
      printf("\n");
      mainController(maze,i,i,temp_maze,h1,m1,b1,file_num);
      free(maze);
      free(temp_maze);
      file_num++;
   // }
   return 0;
}