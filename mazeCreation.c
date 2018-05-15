#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>



// typedef struct {     //template
//     int date;     
//     int month;    
//     int year;     
// } OfferDate;      
                     
                     
// typedef struct {     
// char* type;          
// char* destination;    
// float price;      
// OfferDate date;      
// } Offer;             //template


//***General Notes***
// Size of maze has to be by more than 7.
// Width-Height have to match and be uneven or else maze will 
// not be complete


// struct human_stats;
// struct human_pos;
// struct boss_stats;
// struct boss_pos;
// struct monster_pos;
// struct monster_stats



/////////Humans////////////

typedef struct 
{
   int health;
   int armor;
   int attack;
   int accurasy;

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

// typedef struct
// {
   
// }monster_stats;

// typedef struct 
// {  
   
// }monster;

// ///////////Boss//////////////

typedef struct 
{
   int health;
   int armor;
   int attack;
   int accurasy;
   
}boss_stats;

typedef struct 
{
	int x;
	int y;
}boss_pos;

typedef struct 
{
   boss_pos pos;
   boss_stats stats;
}boss;



// ///////////Mmultiplayer Prototype///////////////
// typedef struct multiPlayer
// {

   
// }multiPlayer;

//Function to move human
void moveHuman(const char *maze, int width, int height, human h1,int file_num){
 
   // File stuff
   char buf[width*4];
   FILE *file;
   size_t nread;

   int score_size = 7;
   int maxX, maxY, dx, dy, ch;
   int x = 0, y = 0;

   initscr();  //initialize and create global vars
   noecho();   //stop echoing of typed chars
   curs_set(FALSE);
   getmaxyx(stdscr, maxY, maxX); //get command window size

   //New window
   WINDOW *Maze = newwin(maxY - score_size, maxX, 0, 0); 
   WINDOW *Game = newwin(maxY - score_size, maxX, 0, 0);
   WINDOW *Stats = newwin(score_size, maxX, maxY - score_size, 0); 
   WINDOW *Score = newwin(score_size, maxX-15, maxY - score_size, maxX-15); 
   
   //print on window
   mvwprintw(Maze,0, 0, "Maze:");

   mvwprintw(Stats, 0, 0, "Player1:");
   mvwprintw(Stats, 2, 1, "Health:");
   mvwprintw(Stats, 3, 1, "Armor:");
   mvwprintw(Stats, 4, 1, "Attack:");
   mvwprintw(Stats, 5, 1, "Accurasy:");

   mvwprintw(Score, 0, 4, "!Score!");
   mvwprintw(Score, 3, 4, "13");  //WINS
   mvwprintw(Score, 3, 7, "-");
   mvwprintw(Score, 3, 9, "65");  //LOSSES
   mvwprintw(Score, 5, 5, "W");
   mvwprintw(Score, 5, 9, "L");

   
   //Refresh Window
   // wrefresh(Field);               
   wrefresh(Maze);               
   wrefresh(Stats); 
   wrefresh(Score); 
   sleep(1); 
   

  
   
   file = fopen("level1.txt","r");
   if (file) {
      int line = 2;
      while (fgets(buf,1024,file)){
         mvwprintw(Maze,line,0,buf);
         wrefresh(Maze);
         line++;
      }
       fclose(file);
   }


   //Place human and Final Boss
   mvwprintw(Maze,h1.pos.y,h1.pos.x, ":)");
   mvwprintw(Maze,height+1,(width*4)-7 , ":#");
   wrefresh(Maze);
   sleep(1);

	
	
   while(TRUE){
   	keypad(Maze,TRUE);
	nodelay(Maze, TRUE);
   	ch = wgetch(Maze);
	switch (ch) {
	   case KEY_UP:
	   	if (maze)
	   	{
	   		/* code */
	   	}
	   		mvwprintw(Maze,h1.pos.y - 1 ,h1.pos.x,":)");
	   		wrefresh(Maze);
	   		h1.pos.y = h1.pos.y - 1;
	   		break;
	   case KEY_DOWN: 
	   		mvwprintw(Maze,h1.pos.y + 1,h1.pos.x,":)");
	   		wrefresh(Maze);
	   		h1.pos.y = h1.pos.y + 1;
	   		break;
	   case KEY_RIGHT:
	   		mvwprintw(Maze,h1.pos.y ,h1.pos.x + 2,":)");
	   		wrefresh(Maze);
	   		h1.pos.x = h1.pos.x + 2;
	   		break;
	   case KEY_LEFT: 
	   		mvwprintw(Maze,h1.pos.y,h1.pos.x - 2,":)"); 
	   		wrefresh(Maze);
	   		h1.pos.x = h1.pos.x - 2;
	   		break;
	}

	if (false)
	{
	   //Clean up-delete window
	   delwin(Maze); 
	   delwin(Game);                           
	   delwin(Stats);  
	   delwin(Score);  
	   endwin();
	   break;
	}
   }          
     
	// delwin(Maze);              
	//    delwin(Stats);  
	//    delwin(Score);  
	//    endwin();

   
}

// void loadMaze(){

// }


 // Display-save the maze. we want this one......VGALE TO HUMAN APO EKEI......
void Show_saveMaze(const char *maze, int width, int height, human h1, int file_num) {
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
         case 1:  printf("{||}"); fprintf(files[i],"{||}"); break;
         // case 2:  printf(" 👲  "); fprintf(files[i]," 👲   "); break;
         // case 3:  printf(" 👻  "); fprintf(files[i]," 👻   "); break;
         // case 4:  printf(" 👹  "); fprintf(files[i]," 👹   "); break;
         // case 5:  printf(" 💰  "); fprintf(files[i]," 💰   "); break;
         default: printf("    "); fprintf(files[i],"    "); break; 
         }
      }
      printf("\n"); fprintf(files[i], "\n");
   }
   printf("\n");
}

  // Anoigei to monopati
void CarveMaze(char *maze, int width, int height, int x, int y) {

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
         if (flag ==0 )
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
}

 // Generate maze in matrix maze with size width, height.
void GenerateMaze(char *maze, int width, int height,human h1) {
   
      int x, y;
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
            CarveMaze(maze, width, height, x, y);
         }
      }

   
      //default eisodos k eksodos apo to lavurintho, maze[0][1] kai maze[..][..]
      // maze[h1.pos.position] = 2;
      // maze[(height - 1) * width + (width - 2)] = 4;

}


int main(int argc,char *argv[]) {

   human h1;
   h1.stats.health = 90;
   h1.stats.armor = 45;
   h1.stats.attack = 60;
   h1.stats.accurasy = 100;
   h1.pos.y = 2;
   h1.pos.x = 5;

   int width, height;
   int file_num=1;
   char *maze;
   

   // if(argc != 3 && argc != 4) {
   //    printf("usage: maze <width> <height> [s]\n");
   //    exit(EXIT_FAILURE);
   // }  

   // /* Get and validate the size. */
   // width = atoi(argv[1]);
   // height = atoi(argv[2]);
   // if(width < 7 || height < 7) {
   //    printf("error: illegal maze size\n");
   //    exit(EXIT_FAILURE);
   // }
   
   // for (int i = 7; i < 15; i+=2)  // 7 9 11 13 15 
   // {  
      int i = 7;
      maze = (char*)malloc(i * i * sizeof(char));
      if(maze == NULL) {
      printf("error: not enough memory\n");
      exit(EXIT_FAILURE);
      }
      GenerateMaze(maze,i,i,h1);
      // Show_saveMaze(maze,i,i,h1,file_num);
      moveHuman(maze,i,i,h1,file_num);
      free(maze);
      file_num++;
   // }


   return 0;

}