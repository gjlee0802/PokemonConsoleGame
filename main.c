#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define FIELD_SIZE 50

struct Scene{
	int sceneNum;
	char Coor[FIELD_SIZE][FIELD_SIZE];
};
typedef struct Scene scene;

void sceneMap(scene *);

void scenePrint(scene *Sptr)
{
	for(int i=0; i<FIELD_SIZE; i++)
	{
		for(int j=0; j<FIELD_SIZE; j++)
		{printf("%c ", Sptr->Coor[i][j]);}
		printf("\n");
	}
}

int main()
{	
	scene *Sptr;
	Sptr = (scene *)malloc(sizeof(scene));

	Sptr->sceneNum = 0;

	sceneMap(Sptr);
	scenePrint(Sptr);
}

void sceneMap(scene *Sptr)
{
	switch(Sptr->sceneNum)
	{
	int i=0, j=0, k=0;
	case 0:
		
		for(i=0; i<FIELD_SIZE; i++)
			Sptr->Coor[0][i] = '^';
		for(i=1; i<FIELD_SIZE-1; i++)
		{	Sptr->Coor[i][0] = '^'; Sptr->Coor[i][FIELD_SIZE-1]='^';}
		for(i=0; i<FIELD_SIZE; i++)
			Sptr->Coor[FIELD_SIZE-1][i] = '^';
		for(i=1; i<FIELD_SIZE-1; i++)
			for(j=1; j<FIELD_SIZE-1; j++)
				Sptr->Coor[i][j]=' ';

		for(i=1+5; i<(1+5)+5; i++)
			for(j=1+5; j<(1+5)+5; j++)
				Sptr->Coor[i][j]='M';
		for(j=1+5; j<(1+5)+5; j++)
			Sptr->Coor[j][(1+5+5)+1]='^';

		for(i=0; i<1+5; i++)
		{
			Sptr->Coor[5+1+i][20+1-i]='/';
			Sptr->Coor[5+1+i][20+1+i]='\\';
		}
		break;
	default:
		for(i=0;i<FIELD_SIZE;i++)
			for(j=0;j<FIELD_SIZE;j++)
				Sptr->Coor[i][j]='^';
	}

}
