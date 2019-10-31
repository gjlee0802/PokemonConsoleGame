#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <conio.h>	//for Windows OS

#define FIELD_SIZE 50

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SPACE 4

struct Scene{
	int sceneNum;
	char Coor[FIELD_SIZE][FIELD_SIZE];
	int HeroX;
	int HeroY;
};
typedef struct Scene scene;


// ** Functions
void sceneMap(scene *);
void scenePrint(scene *);

void move(scene *, int);
int isColi(scene *, int, int);
int keyControl();


/* MAIN Function */
int main()
{	
	scene *Sptr;
	Sptr = (scene *)malloc(sizeof(scene));
	Sptr->sceneNum = 0;
	Sptr->HeroX = 8;
	Sptr->HeroY = 24;
	while(1)
	{
		int n=keyControl();
		switch(n)
		{
		case UP:
			move(Sptr, n);
		case DOWN:
			move(Sptr, n);
		case LEFT:
			move(Sptr, n);
		case RIGHT:
			move(Sptr, n);
		}
		sceneMap(Sptr);
		scenePrint(Sptr);
	}
	return 0;
}

/* Draw/Print Functions */
void sceneMap(scene *Sptr)
{
	switch(Sptr->sceneNum)
	{
	int i=0, j=0, k=0;
	case 0:
		// Square Colider
		for(i=0; i<FIELD_SIZE; i++)
			Sptr->Coor[0][i] = '^';
		for(i=1; i<FIELD_SIZE-1; i++)
		{	Sptr->Coor[i][0] = '^'; Sptr->Coor[i][FIELD_SIZE-1]='^';}
		for(i=0; i<FIELD_SIZE; i++)
			Sptr->Coor[FIELD_SIZE-1][i] = '^';
		for(i=1; i<FIELD_SIZE-1; i++)
			for(j=1; j<FIELD_SIZE-1; j++)
				Sptr->Coor[i][j]=' ';

		// 
		for(i=1+5; i<(1+5)+5; i++)
			for(j=1+5; j<(1+5)+5; j++)
				Sptr->Coor[i][j]='M';
		for(j=1+5; j<(1+5)+5; j++)
			Sptr->Coor[j][(1+5+5)+1]='^';

		// House
		for(i=0; i<1+5; i++)
		{
			Sptr->Coor[15+1+i][5+1-i]='/';
			Sptr->Coor[15+1+i][5+1+i]='\\';
		}
		for(i=21; i<21+3; i++)
			Sptr->Coor[i][2]='|';
		for(i=21; i<21+3; i++)
			Sptr->Coor[i][10]='|';
		for(j=3; j<10; j++)
			Sptr->Coor[20][j]='=';
		Sptr->Coor[19][4]='H';Sptr->Coor[19][5]='O';Sptr->Coor[19][6]='U';Sptr->Coor[19][7]='S';Sptr->Coor[19][8]='E';
		Sptr->Coor[22][7]='[';Sptr->Coor[22][8]=']';
		for(j=3; j<10;j++)
			Sptr->Coor[23][j]='=';

		// LAB
		//for(i=0; )

		break;
	default:
		for(i=0;i<FIELD_SIZE;i++)
			for(j=0;j<FIELD_SIZE;j++)
				Sptr->Coor[i][j]='^';
	}

}


void scenePrint(scene *Sptr)
{
	int i=0;
	printf("   ");
	for(i=0; i<FIELD_SIZE; i++)
	{
		if(i<9)
			printf("%d ", i);
		else
			printf("%d", i);
	}
	printf("\n");
	for(i=0; i<FIELD_SIZE; i++)
	{
		if(i<=9)
			printf("%1d: ", i);
		else
			printf("%1d:", i);
		for(int j=0; j<FIELD_SIZE; j++)
		{printf("%c ", Sptr->Coor[i][j]);}
		printf("\n");
	}
}

void move(scene *Sptr, int keyInput)
{

	switch(keyInput)
	{
	case UP:
		if(!isColi(Sptr, (Sptr->HeroX), (Sptr->HeroY)-1))
			Sptr->HeroY-=1;	
		else
			printf("Colider detected!\n");
		break;
	case DOWN:
		if(!isColi(Sptr, (Sptr->HeroX), (Sptr->HeroY)+1))
			Sptr->HeroY+=1;
		else
			printf("Colider detected!\n");
		break;
	case LEFT:
		if(!isColi(Sptr, (Sptr->HeroX)-1, (Sptr->HeroY)))
			Sptr->HeroX-=1;
		else
			printf("Colider detected!\n");
		break;
	case RIGHT:
		if(!isColi(Sptr, (Sptr->HeroX)+1, (Sptr->HeroY)))
			Sptr->HeroX+=1;
		else
			printf("Colider detected!\n");
		break;
	default:
		printf("move() err : cant understand keyinput : %d\n", keyInput);
	}
}

int isColi(scene *Sptr, int x, int y)
{
	if(Sptr->Coor[x][y]=='^' || 
	Sptr->Coor[x][y]=='|' || 
	Sptr->Coor[x][y]=='/' || 
	Sptr->Coor[x][y]=='\\'||
	Sptr->Coor[x][y]=='='
	)
		return 1;
	else
		return 0;
} 

int keyControl()
{
	char temp = getch();
	
	if(temp == 'w' || temp == 'W'){
		return UP;
	} else if(temp == 'a' || temp == 'A'){
		return LEFT;
	} else if(temp == 's' || temp == 'S'){
		return DOWN;
	} else if(temp == 'd' || temp == 'D'){
		return RIGHT;
	} else if(temp == ' '){ // 스페이스바(공백)이 선택 버튼  
		return SPACE;
	}
}



