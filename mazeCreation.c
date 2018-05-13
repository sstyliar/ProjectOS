#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define human_face  "👲"
#define monster_face  "👻"
#define boss_face  "👹"

typedef struct {
    int date;
    int month;
    int year;
} OfferDate;


typedef struct {
char* type;
char* destination;
float price;
OfferDate date; 
} Offer;


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



//////////Humans////////////

// typedef struct 
// {
//    int health;
//    int armor;
//    int attack;
//    int accurasy;

// }human_stats;

// typedef struct 
// {
//    int x;
//    int y;
      
// }human_pos;

// typedef struct 
// {  
//    human_stats stats;
//    human_pos pos;
        
// }human;


///////////Monsters//////////////

// typedef struct monster
// {  
   
// }monster;


// typedef struct monster_pos
// {
   
// }monster_pos;

// typedef struct monster_stats
// {
   
// }monster_pos;

// ///////////Bosses//////////////

// typedef struct boss
// {
   
// }boss;

// typedef struct boss_stats
// {
   
// }boss_stats;

// typedef struct boss_pos
// {
   
// }boss_pos;


// ///////////Mmultiplayer Prototype///////////////
// typedef struct multiPlayer
// {

   
// }multiPlayer;




 // Display the maze. we want this one
void ShowMaze(const char *maze, int width, int height) {
   int x, y;

   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {

         printf("maze[%d][%d] = %d  ",y,x,maze[y*width+x]);
      }
      printf("\n");
   }

   for(y = 0; y < height; y++) {
      for(x = 0; x < width; x++) {
         switch(maze[y * width + x]) {
         case 1:  printf("{||}");  break;
         case 2:  printf("%c",*human_face);  break;
         default: printf("    ");  break; 
         }
         
      }
      printf("\n");
   }
}

  // Carve the maze starting at x, y.  we want this one 
void CarveMaze(char *maze, int width, int height, int x, int y) {

   int x1, y1;    
   int x2, y2;
   int dx, dy;
   int dir, count;

   dir = rand() % 4;
   // printf("%d", dir);
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
         printf("\tx1 = %d, x2 = %d, y1 = %d, y2 = %d\n", x1, x2, y1, y2);
         maze[y1 * width + x1] = 0;
         printf("maze cell number = maze[%d] is equal to 0\n", y1 * width + x1);
         maze[y2 * width + x2] = 0;
         printf("maze cell number = maze[%d] is equal to 0\n", y2 * width + x2);
         x = x2; y = y2;
         dir = rand() % 4;
         count = 0;
      } else {
         dir = (dir + 1) % 4;  //auksanei to dir alla to krataei panta <4
         count += 1;
      }
   }

}

 // Generate maze in matrix maze with size width, height.
  // MOST DEF WE WANT THIS ONE 
void GenerateMaze(char *maze, int width, int height) {

   int x, y;

   /* Initialize the maze. */
   for(x = 0; x < width * height; x++) {
      maze[x] = 1;
   }
   maze[1 * width + 1] = 0;
   
   /* Seed the random number generator. */
   srand(time(0));

   /* Carve the maze. */
   for(y = 1; y < height; y += 2) {
      for(x = 1; x < width; x += 2) {
         CarveMaze(maze, width, height, x, y);
      }
   }

   /* Set up the entry and exit. */
   maze[0 * width + 1] = 0;
   maze[(height - 1) * width + (width - 2)] = 0;
   
}


int main(int argc,char *argv[]) {

   // human h1;
   // h1.pos.x = 0;
   // h1.pos.x = 2;
   

   int width, height;
   char *maze;

   if(argc != 3 && argc != 4) {
      // printf("usage: maze <width> <height> [s]\n");
      exit(EXIT_FAILURE);
   }

   /* Get and validate the size. */
   width = atoi(argv[1]) * 2 + 3;
   height = atoi(argv[2]) * 2 + 3;
   if(width < 7 || height < 7) {
      // printf("error: illegal maze size\n");
      exit(EXIT_FAILURE);
   }
   
   /* Allocate the maze array. */
   maze = (char*)malloc(width * height * sizeof(char));
   if(maze == NULL) {
      // printf("error: not enough memory\n");
      exit(EXIT_FAILURE);
   }

   /* Generate and display the maze. */
   // GenerateMaze(maze, width, height);
   // ShowMaze(maze, width, height, h1);

   GenerateMaze(maze, 9, 9);
   ShowMaze(maze, 9, 9);
   
   free(maze);
   return 0;
}

