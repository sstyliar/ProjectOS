#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>


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



//Attack monster when the player wants to
int human_Attack_monster(human_stats h2, boss_stats b2){	//h logikh einai oti prwta epitithetai o paikths gt to teras koimatai :) 
	int damage;
	int n;
	int Hit=rand()%100;		
	float Hit2=Hit/100.0;
	
	if(Hit2 > (h2.accurasy)){
		if(h2.attack - b2.armor > 0){
			printf("hooman attacked\n");	
			b2.health = b2.health - (h2.attack - b2.armor);
			if(b2.health<=0){
				b2.health=0;
			}
		}
	}
	else{printf("misssed\n");}

	

	return b2.health;
}
int monster_Attack_human(human_stats h2,boss_stats b2){	//to teras antepitithetai kai opoios exei perissotero health kai pososto armor epiviwnei
	int damage;							//afou o paikths pairnei exp kai kalytera stats apo alla teratakia kathe fora tha einai etoimos gia na antimetwpisei to megalo boss
	double Hit= rand()%2 ;
	Hit=Hit/100;
	if(Hit >(b2.accurasy/100) ){
		printf("monstar attacked\n");
		h2.health = h2.health + h2.armor - b2.attack;
	}
	else{printf("missed\n");}	
	
	if(h2.health<=0){
		h2.health=0;
	}
	return h2.health;
}


int main(){

	human_stats h1;
	monster_stats m1;
	boss_stats b1;
	int x,y;

	
	h1.health=50;
	h1.attack=20;
	h1.armor=10;
	h1.accurasy=0.5;
	printf("human\n%d\n",h1.health);
	printf("%d\n",h1.attack);
	printf("%d\n",h1.armor);
	printf("%f\n\n",h1.accurasy);
	
	m1.health=30;
	m1.attack=10;
	m1.armor=10;
	m1.accurasy=0.2;
	printf("monster\n%d\n",m1.health);
	printf("%d\n",m1.attack);
	printf("%d\n",m1.armor);
	printf("%f\n\n",m1.accurasy);

	b1.health=50;
	b1.attack=20;
	b1.armor=10;
	b1.accurasy=0.3;
	printf("boss\n%d\n",b1.health);
	printf("%d\n",b1.attack);
	printf("%d\n",b1.armor);
	printf("%f\n\n",b1.accurasy);

	

	int win=0;
	int fores=0;
	do{	
	
	
	b1.health = human_Attack_monster(h1,b1);
	//sleep(1);
	
	h1.health=monster_Attack_human(h1,b1);
	//sleep(1);

	printf("boss\n%d\n",b1.health);
	printf("%d\n",b1.attack);
	printf("%d\n",b1.armor);
	printf("%f\n\n",b1.accurasy);

	printf("human\n%d\n",h1.health);
	printf("%d\n",h1.attack);
	printf("%d\n",h1.armor);
	printf("%f\n",h1.accurasy);
	fores++;
	if(h1.health==0 || b1.health==0){
		printf("fores%d",fores);
		if(b1.health==0){
			printf("\nHooman Win !!!1\n");
			win=1;	
		}
		else if (h1.health==0){
			printf("\nYou lost loser \n");
			win=1;
		}
	}

	}while(win!=1);
return 0;
}