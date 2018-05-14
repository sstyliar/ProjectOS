#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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




 // Display-save the maze. we want this one
void Show_saveMaze(const char *maze, int width, int height, int file_num) {
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
         case 2:  printf(" 👲  "); fprintf(files[i]," 👲   "); break;
         case 3:  printf(" 👻  "); fprintf(files[i]," 👻   "); break;
         case 4:  printf(" 👹  "); fprintf(files[i]," 👹   "); break;
         case 5:  printf(" 💰  "); fprintf(files[i]," 💰   "); break;
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
      x1 = x + dx;      //
      y1 = y + dy;      //
      x2 = x1 + dx;     //
      y2 = y1 + dy;     //
      
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
void GenerateMaze(char *maze, int width, int height) {
   
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
      maze[0 * width + 1] = 2;
      maze[(height - 1) * width + (width - 2)] = 4;

}


int main(int argc,char *argv[]) {

   // human h1;
   // h1.pos.x = 0;
   // h1.pos.x = 2;
   

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
   
   for (int i = 7; i < 15; i+=2)
   {  
      maze = (char*)malloc(i * i * sizeof(char));
      if(maze == NULL) {
      printf("error: not enough memory\n");
      exit(EXIT_FAILURE);
      }
      GenerateMaze(maze,i,i);
      Show_saveMaze(maze,i,i,file_num);
      free(maze);
      file_num++;
   }
   return 0;
}