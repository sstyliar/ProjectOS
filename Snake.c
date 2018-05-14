#include<stdio.h>
#include<stdbool.h> //because c doesn't have boolean type 
#include<stdlib.h>  //just to use the rand function
#include<termios.h>
#include<time.h>//gia th xrhsh ths sunarthshs usleep()
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
int i,j,p,tlength=0;//tlength:to mege8os ths ouras
int tx[100],ty[100];//gia suntetagmenes ths ouras
int width=20;
int height=20;
int hx, hy,fx,fy;//suntetagmenes kefaliou, suntetagmenes trofhs
int score;     
bool gameOver;  
enum Direction //sunolo apo sta8eres, pou pairnoun times ksekinwntas apo 0(STOP=0,LEFT=1 ktl)
{
STOP,
LEFT,
RIGHT,
UP,
DOWN
} dir ;


int main()
{
 Setup();
 p=0;
 while(gameOver==false)
 {
    
	Draw();
    Input();
	Logic();
	usleep(100000); // me auth thn entolh kanei mia mikrh paush to susthma se microseconds(me th sleep htan polu argo)
    
 }

}

void Draw()
{
     bool pr;
     int o;
	 system("cls");
	  //clears the console window
     
	  
	  
	  for( i=0;i<width;i++)
      {
	     printf("-");
	     
      }
      printf("\n");
      for(i=0;i<height;i++)
      {
 	     for(j=0;j<width;j++)
 	     {
	       if(j==0|| j==width-1)
	       { 
	   	      printf("|");
	               }
	        else
	       {
		         if(i==hy && j==hx)
		         {
		           printf("@");
		           }
		         else if(i==fy && j==fx)
		         {
		   	       printf("X");
                  }
	              else
	             {
	               pr=false;
				   for(o=0;o<tlength;o++)
	               {
	               	  if(tx[o]==j&&ty[o]==i)
	               	  {
						 
						 printf("*");
						 pr=true;
				      }
	               }
				if(!pr)
				   printf(" ");
	                 }
	           }
	        }
	        printf("\n");
		}
	       for( i=0;i<width;i++)
           {
	         printf("-");
          }
          printf("\n");
            if(p==0)
            {
            	score=0;
                 printf("score:");
                 printf("%d", score);
				p++;
			}
			else
			{
			printf("score:");
            printf("%d", score);
	 	   //previously score was here
  }}
  

void Setup()
{
//score=0;
//printf("score:");
//printf("%d", &score);
gameOver=false;	
dir=STOP;
hx=width/2;
hy=height/2;
fx=rand()%width; //creates a random number with max being width-1
fy=rand()%height;
//score

}
void Input()
{   changemode(1);
	while( kbhit())
	{
		switch( getchar())
		{
			case 'a':
				dir=LEFT;
			break;
		    
			case 'd':
		       dir=RIGHT;
			break;
		      
			case 'w':
		        dir=UP;
			break;
		    
			case 's':
		      	dir=DOWN;
			break;
			   case 'x':
			   	gameOver=true;
			   	break;
		       default:
		       	break;
		}
	}
	
}
void Logic()
{
   int k,a;
   int prevX=tx[0];
   int prevY=ty[0];
   int prevX2,prevY2;
   tx[0]=hx;
   ty[0]=hy;
   for(k=1;k<tlength;k++)
   {
      prevX2=tx[k];
      prevY2=ty[k];
      tx[k]=prevX;
      ty[k]=prevY;
      prevX=prevX2;
      prevY=prevY2;
   
   
   }
   
   
   
   switch(dir)
   {
     case LEFT:
     	hx--;
     break;
     
	 case RIGHT:
        hx++;
     break;
     
	 case UP:
     	hy--;
     break;
     
	 case DOWN:
    	hy++;
     break;
   }
   if(hx>width||hx<0||hy>height||hy<0)
   {
   	  gameOver=true;
   	  printf("\n");
   	  printf("I'm sorry, you lost");
   }
     for(a=0;a<tlength;a++)
	   if(tx[a]==hx&&ty[a]==hy)
	     {
		 gameOver=true;
	     printf("\n");
   	     printf("I'm sorry, you lost");
        }
	 if(hx==fx&&hy==fy)
	          {
	              tlength++;
				   fx=rand()%width; //creates a random number with max being width-1
                   fy=rand()%height;
				    score=score+10;		 
                      
				   			  
			  }
			 printf("\n");
    
    
	   }

int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;
 
  tv.tv_sec = 0;
  tv.tv_usec = 0;
 
  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);
 
  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
 
}

void changemode(int dir)
{
  static struct termios oldt, newt;
 
  if ( dir == 1 )
  {
    tcgetattr( STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  }
  else
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

