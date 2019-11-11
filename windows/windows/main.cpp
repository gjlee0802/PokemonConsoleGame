#define _CRT_SECURE_NO_WARNINGS

#include <atlstr.h>   // use Cstring type
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //use srand() function
#include <conio.h>   //for Windows OS
#include <windows.h>

#define FIELD_SIZE 50

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SPACE 4
#define QUIT 9

#define 물의파동 352
#define 역린 200
#define 솔라빔 76
#define 잎날가르기 75
#define 거품광선 61
#define 화염방사 53
#define 불꽃세례 52
#define 몸통박치기 33
#define 덩굴채찍 22
#define 할퀴기 10

struct Scene {
	int sceneNum;
	char Coor[FIELD_SIZE][FIELD_SIZE];
	int HeroX;
	int HeroY;
	int myPoke[6] = { 0 };   // 갖고 있는 포켓몬의 도감번호
	int myPokeLevel[6];   // 갖고 있는 포켓몬의 레벨
	int myPokeHealth[6];   // 갖고 있는 포켓몬의 체력
	int currPokemonIndex = 0;   // 가장 먼저 나올 포켓몬 or 현재 배틀중인 포켓몬의 인덱스
	int enemyPoke;
	int enemyPokeLevel;
	int enemyPokeHealth;
};
typedef struct Scene scene;


// ** Functions
void sceneMap(scene*);
void scenePrint(scene*);
void move(scene*, int);
int isColi(scene*, int, int);
int keyControl();
void setColor(int, int);
void pokemonPrint(int);
void battleInit(scene*);
void battleMenu(scene*);
int fightMenu(scene*, int);
int useSkill(scene*, int);

/* MAIN Function */
int main()
{
	//pokemonPrint((char *)"4");   // 포켓몬 그림 테스트용 코드 테스트 안하면 주석처리.

	scene* Sptr;
	Sptr = (scene*)malloc(sizeof(scene));

	Sptr->sceneNum = 0;

	Sptr->myPoke[0] = 4;   // 향후 연구소에서 지정하도록 설정.
	Sptr->myPokeLevel[0] = 5;
	Sptr->myPokeHealth[0] = Sptr->myPokeLevel[0] * 30;
	Sptr->HeroX = 11;
	Sptr->HeroY = 12;
	/*Sptr->HeroX = 8;
	Sptr->HeroY = 24;*/

	sceneMap(Sptr);
	scenePrint(Sptr);
	while (1)
	{
		int n = keyControl();
		switch (n)
		{
		case UP:
			move(Sptr, n);
			break;
		case DOWN:
			move(Sptr, n);
			break;
		case LEFT:
			move(Sptr, n);
			break;
		case RIGHT:
			move(Sptr, n);
			break;
		case QUIT:
			free(Sptr);
			return 0;
		default:
			break;
		}
		sceneMap(Sptr);
		scenePrint(Sptr);
	}

	free(Sptr);
	return 0;
}


void battleInit(scene* Sptr)
{
	char file_name[10];
	int wildPoke = 0;
	srand((unsigned int)time(NULL));
	//wildPoke = rand() % 116 + 1;
	wildPoke = 4;
	itoa(wildPoke, file_name, 10);
	Sptr->enemyPoke = wildPoke;
	srand((unsigned int)time(NULL));
	Sptr->enemyPokeLevel = rand() % 15 + 1;   // 레벨 1부터 15까지의 야생 포켓몬이 출현함.
	Sptr->enemyPokeHealth = Sptr->enemyPokeLevel * 30;   // 상대의 체력은 레벨의 30배로 설정.

	pokemonPrint(4);   //pokemonPrint(Sptr->enemyPoke);   //으로 변경 
	battleMenu(Sptr);
}

void battleMenu(scene* Sptr)
{
	Sptr->currPokemonIndex = 0;
	int myturn = 1;   // 0: 나의 턴 , 1: 상대 턴
	int skillNum = 0;
	int damage = 0;

	while (1)
	{
		// 배틀 메뉴 종료 조건
		int deadPoke = 0;
		for (int i = 0; i < 6; i++)
		{
			if (Sptr->myPokeHealth[i] <= 0)
				deadPoke++;
		}
		if (deadPoke == 6)
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("정신을 잃었다... 게임을 종료한다..\n");
			printf("<<<<<  GAME OVER!!  >>>>>\n");
			//free(Sptr);
			break;
		}
		if (Sptr->enemyPokeHealth <= 0)
		{
			printf(">> 야생 포켓몬이 쓰러졌다!\n");
			//쓰러뜨린 상대 포켓몬의 레벨에 따라 경험치 부여 예정
			break;
		}

		if (myturn == true)
		{
			printf("\n\n\n\n\n\n\n\n\n\n\n");
			printf("상대 포켓몬)) LEVEL: %d | HP: %d\n", Sptr->enemyPokeLevel, Sptr->enemyPokeHealth);
			printf("  내 포켓몬)) LEVEL: %d | HP: %d\n", Sptr->myPokeLevel[Sptr->currPokemonIndex], Sptr->myPokeHealth[Sptr->currPokemonIndex]);
			printf(">> 무엇을 할까?\n");
			printf("-----------------------------\n");
			printf("| (1)싸운다     (2)교체한다  |\n");
			printf("| (3)몬스터볼   (4)도망간다  |\n");
			printf("-----------------------------\n");
			char temp = getch();
			skillNum = 0;
			damage = 0;
			switch (temp)
			{
			case '1':   //싸운다.
				while (skillNum <= 0)
				{
					skillNum = fightMenu(Sptr, myturn);
					if (skillNum <= 0)
					{
						printf(">> 메뉴로 돌아갑니다.\n"); break;
					}
				}
				damage = useSkill(Sptr, skillNum);
				Sptr->enemyPokeHealth = Sptr->enemyPokeHealth - damage;
				printf(">> 상대 포켓몬에게 데미지를 %d만큼 입혔다!\n", damage);
				break;
			case '2':   //교체한다.
				break;
			case '4':   //도망간다.
				printf(">> 도망쳤다.!\n");
				return;
			default:
				printf(">> 그러한 선택지가 없습니다.\n");
			}
		}
		else if (myturn == false)
		{
			skillNum = 0;
			damage = 0;
			srand((unsigned int)time(NULL));
			skillNum = fightMenu(Sptr, myturn);
			damage = useSkill(Sptr, skillNum);
			Sptr->myPokeHealth[Sptr->currPokemonIndex] = Sptr->myPokeHealth[Sptr->currPokemonIndex] - damage;
			printf(">> 상대 포켓몬이 공격하여 데미지를 %d만큼 입었다!\n", damage);
		}

		// 턴 교체
		if (myturn == true)
			myturn--;
		else if (!myturn)
			myturn++;
	}
	printf(">> 배틀 메뉴 종료. 맵으로 돌아갑니다.\n");
}

int fightMenu(scene* Sptr, int myturn)
{
	int skillNum = 0;
	int pokeNum = 0;

	printf("\n\n\n\n\n\n\n\n\n\n\n");
	if (myturn)
	{
		pokeNum = Sptr->myPoke[Sptr->currPokemonIndex];
		printf(">> 스킬을 선택하세요.(되돌아가려면 1~4를 제외한 키 입력) \n");
	}
	else if (!myturn)
	{
		pokeNum = Sptr->enemyPoke;
		printf(">> 상대 차례이다...\n");
	}

	char temp;
	switch (pokeNum)   // 포켓몬 도감번호 
	{
	case 1:   // 이상해씨
		printf("1.몸통박치기 2.덩굴채찍 3.잎날가르기 4. 솔라빔\n\n");
		if (myturn)
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			printf(">> 상대 포켓몬이 (%c)를 선택했다!", temp);
		}
		switch (temp)
		{
		case '1':
			skillNum = 몸통박치기;
			break;
		case '2':
			skillNum = 덩굴채찍;
			break;
		case '3':
			skillNum = 잎날가르기;
			break;
		case '4':
			skillNum = 솔라빔;
			break;
		default:
			skillNum = 0;
		}
		break;
	case 4:   // 파이리
		printf("1.할퀴기 2.불꽃세례 3.화염방사 4.역린\n\n");
		if (myturn)
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			printf(">> 상대 포켓몬이 (%c)를 선택했다!\n", temp);
		}
		switch (temp)
		{
		case '1':
			skillNum = 할퀴기;
			break;
		case '2':
			skillNum = 불꽃세례;
			break;
		case '3':
			skillNum = 화염방사;
			break;
		case '4':
			skillNum = 역린;
			break;
		default:
			skillNum = 0;
			break;
		}
		break;
	case 7:   // 꼬부기
		break;
	case 10: // 캐터피
		break;
	}
	return skillNum;
}

int useSkill(scene* Sptr, int skillNum)   // 레벨에 따라 데미지를 리턴한다.
{
	int dmg;
	switch (skillNum)
	{
	case 물의파동:
		dmg = 60;
		break;
	case 역린:
		dmg = 120;
		break;
	case 솔라빔:
		dmg = 120;
		break;
	case 잎날가르기:
		dmg = 55;
		break;
	case 거품광선:
		dmg = 65;
		break;
	case 화염방사:
		dmg = 90;
		break;
	case 불꽃세례:
		dmg = 40;
		break;
	case 몸통박치기:
		dmg = 50;
		break;
	case 덩굴채찍:
		dmg = 45;
		break;
	case 할퀴기:
		dmg = 40;
		break;
	default:
		dmg = 0;
	}
	return dmg;

}

/* Draw/Print Functions */
void sceneMap(scene* Sptr)
{
	printf(">> Map : %d\n", Sptr->sceneNum);
	int i = 0, j = 0, k = 0;
	switch (Sptr->sceneNum)
	{
	case 0: {
		// Square Colider
		for (i = 0; i < FIELD_SIZE; i++)
			Sptr->Coor[0][i] = '^';
		for (i = 1; i < FIELD_SIZE - 1; i++)
		{
			Sptr->Coor[i][0] = '^'; Sptr->Coor[i][FIELD_SIZE - 1] = '^';
		}
		for (i = 0; i < FIELD_SIZE; i++)
			Sptr->Coor[FIELD_SIZE - 1][i] = '^';
		for (i = 1; i < FIELD_SIZE - 1; i++)
			for (j = 1; j < FIELD_SIZE - 1; j++)
				Sptr->Coor[i][j] = ' ';


		// WOODS & BUSHES
		for (i = 1 + 5; i < (1 + 5) + 5; i++)
			for (j = 1 + 5; j < (1 + 5) + 5; j++)
				Sptr->Coor[i][j] = 'M';
		for (i = 12; i <= 15; i++)
			for (j = 1; j <= 10; j++)
				Sptr->Coor[i][j] = 'M';

		for (i = 1 + 5; i < (1 + 5) + 5; i++)
			Sptr->Coor[i][(1 + 5 + 5) + 1] = '^';
		for (i = 15; i <= 22; i++)
			Sptr->Coor[i][12] = '^';

		for (j = 1; j <= 14; j++)
			Sptr->Coor[31][j] = '=';
		for (i = 31; i <= 37; i++)
			Sptr->Coor[i][14] = '=';
		for (j = 15; j <= 30; j++)
			Sptr->Coor[37][j] = '=';
		for (i = 34; i <= 37; i++)
			Sptr->Coor[i][31] = '=';
		for (j = 32; j <= 40; j++)
			Sptr->Coor[34][j] = '=';
		for (i = 32; i <= 34; i++)
			Sptr->Coor[i][41] = '=';
		for (j = 42; j <= 48; j++)
			Sptr->Coor[32][j] = '=';

		for (j = 1; j < 49; j++)
		{
			int water_cnt = 0;
			for (i = 31; i < 49; i++)
				if (Sptr->Coor[i][j] == '=')
				{
					++water_cnt;
				}
				else if (water_cnt >= 1 && Sptr->Coor[i][j] == ' ')
				{
					Sptr->Coor[i][j] = '~';
				}
		}


		// House
		for (i = 0; i < 1 + 5; i++)
		{
			Sptr->Coor[15 + 1 + i][5 + 1 - i] = '/';
			Sptr->Coor[15 + 1 + i][5 + 1 + i] = '\\';
		}
		for (i = 21; i < 21 + 3; i++)
			Sptr->Coor[i][2] = '|';
		for (i = 21; i < 21 + 3; i++)
			Sptr->Coor[i][10] = '|';
		for (j = 3; j < 10; j++)
			Sptr->Coor[20][j] = '=';
		Sptr->Coor[19][4] = 'H'; Sptr->Coor[19][5] = 'O'; Sptr->Coor[19][6] = 'U'; Sptr->Coor[19][7] = 'S'; Sptr->Coor[19][8] = 'E';
		Sptr->Coor[22][7] = '['; Sptr->Coor[22][8] = ']';
		for (j = 3; j < 10; j++)
			Sptr->Coor[23][j] = '=';

		// LAB                               
		for (j = 21; j < 21 + 7; j++)
			Sptr->Coor[16][j] = '=';
		for (j = 33; j < 33 + 7; j++)
			Sptr->Coor[16][j] = '=';
		Sptr->Coor[16][29] = '='; Sptr->Coor[16][31] = '=';
		Sptr->Coor[15][30] = '='; Sptr->Coor[17][30] = '='; Sptr->Coor[16][30] = 'P';
		Sptr->Coor[16][28] = '|'; Sptr->Coor[16][32] = '|';
		Sptr->Coor[15][29] = '/'; Sptr->Coor[15][31] = '\\';
		Sptr->Coor[17][29] = '\\'; Sptr->Coor[17][31] = '/';
		for (i = 17; i < 24; i++)
		{
			Sptr->Coor[i][21] = '|'; Sptr->Coor[i][39] = '|';
		}
		for (j = 22; j < 39; j++)
			Sptr->Coor[23][j] = '=';
		for (i = 0; i < 3; i++)
		{
			Sptr->Coor[15 - i][21 + i] = '/'; Sptr->Coor[15 - i][39 - i] = '\\';
		}
		for (j = 24; j < 37; j++)
			Sptr->Coor[13][j] = '=';
		for (i = 20; i <= 22; i++)
		{
			Sptr->Coor[i][29] = '['; Sptr->Coor[i][31] = ']';
		}
		for (j = 22; j <= 26; j = j + 2)
		{
			Sptr->Coor[20][j] = '('; Sptr->Coor[20][j + 1] = ')';
			Sptr->Coor[22][j] = '|'; Sptr->Coor[22][j + 1] = '|';
		}
		for (j = 33; j <= 37; j = j + 2)
		{
			Sptr->Coor[20][j] = '('; Sptr->Coor[20][j + 1] = ')';
			Sptr->Coor[22][j] = '|'; Sptr->Coor[22][j + 1] = '|';
		}

		// HERO
		Sptr->Coor[Sptr->HeroY][Sptr->HeroX] = 'H';
		break;
	}

	case 1: {
		//Edge
		for (i = 0; i < FIELD_SIZE; i++)
			for (j = 0; j < FIELD_SIZE; j++)
				Sptr->Coor[i][j] = '^';
		for (i = 1; i < FIELD_SIZE - 1; i++)
			for (j = 1; j < FIELD_SIZE - 1; j++)
				Sptr->Coor[i][j] = ' ';

		//Door open
		Sptr->Coor[49][24] = ' ';
		Sptr->Coor[49][23] = ' ';

		//Professor
		Sptr->Coor[8][24] = 'P';

		//table
		for (i = 35; i < 46; i++)
			Sptr->Coor[13][i] = 'T';
		for (i = 35; i < 46; i++)
			Sptr->Coor[14][i] = 'T';

		for (j = 36; j <= 44; j += 2)
			Sptr->Coor[15][j] = '|';

		//pokemon ball
		for (j = 36; j <= 44; j = j + 4)
		{
			Sptr->Coor[12][j] = 'p'; Sptr->Coor[11][j] = 'b';
		}

		//Machine1
		Sptr->Coor[2][4] = 'm';
		Sptr->Coor[2][5] = 'm';
		for (j = 3; j <= 6; j++)
			Sptr->Coor[3][j] = 'm';
		for (i = 4; i <= 6; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

		//Machine2
		Sptr->Coor[8][4] = 'm';
		Sptr->Coor[8][5] = 'm';
		for (i = 3; i <= 6; i++)
			Sptr->Coor[9][i] = 'm';
		for (i = 10; i <= 12; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

		//Machine3
		Sptr->Coor[14][4] = 'm';
		Sptr->Coor[14][5] = 'm';
		for (i = 3; i <= 6; i++)
			Sptr->Coor[15][i] = 'm';
		for (i = 16; i <= 18; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

	}
		  Sptr->Coor[Sptr->HeroY][Sptr->HeroX] = 'H';
	}//Switch end

}


void scenePrint(scene* Sptr)
{

	int i = 0;
	printf("   ");
	for (i = 0; i < FIELD_SIZE; i++)
	{
		if (i < 9)
			printf("%d ", i);
		else
			printf("%d", i);
	}
	printf("\n");
	for (i = 0; i < FIELD_SIZE; i++)
	{
		if (i <= 9)
			printf("%1d: ", i);
		else
			printf("%1d:", i);
		for (int j = 0; j < FIELD_SIZE; j++)
		{
			switch (Sptr->Coor[i][j])
			{
			case '~':
				setColor(9, 11);
				break;
			case 'M':
				setColor(0, 10);
				break;
			case 'T':
				setColor(14, 14);
				break;
			case '|':
				setColor(14, 0);
				break;
			case 'b':
				setColor(4, 4);
				break;
			case 'p':
				setColor(15, 15);
				break;
			case 'm':
				setColor(67, 67);
				break;
			case 'c':
				setColor(23, 23);
				break;
			default:
				setColor(15, 0);
				break;
			}

			printf("%c ", Sptr->Coor[i][j]);
		}
		printf("\n");
	}
}

void move(scene* Sptr, int keyInput)
{

	switch (keyInput)
	{
	case UP:
		if (!(isColi(Sptr, (Sptr->HeroX), (Sptr->HeroY) - 1)))
			Sptr->HeroY = Sptr->HeroY - 1;
		else
			printf("Colision detected!\n");
		break;
	case DOWN:
		if (!(isColi(Sptr, (Sptr->HeroX), (Sptr->HeroY) + 1)))
			Sptr->HeroY = Sptr->HeroY + 1;
		else
			printf("Colision detected!\n");
		break;
	case LEFT:
		if (!(isColi(Sptr, (Sptr->HeroX) - 1, (Sptr->HeroY))))
			Sptr->HeroX = (Sptr->HeroX) - 1;
		else
			printf("Colision detected!\n");
		break;
	case RIGHT:
		if (!(isColi(Sptr, (Sptr->HeroX) + 1, (Sptr->HeroY))))
			Sptr->HeroX = (Sptr->HeroX) + 1;
		else
			printf("Colision detected!\n");
		break;
	default:
		printf("move() err : can't understand keyinput : %d\n", keyInput);
	}
	printf("Hero : (%d, %d)\n", Sptr->HeroX, Sptr->HeroY);
}

int isColi(scene* Sptr, int x, int y)
{
	if (Sptr->Coor[y][x] == '^' ||
		Sptr->Coor[y][x] == '|' ||
		Sptr->Coor[y][x] == '/' ||
		Sptr->Coor[y][x] == '\\' ||
		Sptr->Coor[y][x] == '=' ||
		Sptr->Coor[y][x] == 'T' ||
		Sptr->Coor[y][x] == 'b' ||
		Sptr->Coor[y][x] == 'p' ||
		Sptr->Coor[y][x] == 'm' ||
		Sptr->Coor[y][x] == 'c'
		)
	{
		return 1;
	}
	else if (Sptr->Coor[y][x] == 'M')
	{
		srand((unsigned int)time(NULL));
		int val = 0;
		val = rand() % 100 + 1;
		if (val <= 45)
		{
			battleInit(Sptr);
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

void pokemonPrint(int pokeNum)
{
	HANDLE fileSearch;
	WIN32_FIND_DATA wfd;
	CString musiccount;
	CString findFirstFileName;
	CString findSecondFileName;

	char find_path[10] = "";
	char found_path[40] = "";

	find_path[2] = (char)(pokeNum % 10 + 48);
	find_path[1] = (char)((pokeNum / 10) % 10 + 48);
	find_path[0] = (char)(pokeNum / 100 + 48);
	//find_path[0] = '0';
	//find_path[1] = '0';
	//find_path[2] = '4';
	find_path[3] = '_';
	find_path[4] = '*';
	find_path[5] = '\0';

	//musiccount.Format(_T("C:\\Program Files\\TEST\\1_*.*"));
	musiccount.Format(_T((const char*)find_path));

	fileSearch = FindFirstFile(musiccount, &wfd);
	if (fileSearch != INVALID_HANDLE_VALUE)
	{
		findFirstFileName.Format(_T("%s"), wfd.cFileName);
		strcpy(found_path, wfd.cFileName);
		FindClose(fileSearch);
		printf("%s!\n", found_path);
	}

	char pokemon_name[30];

	int i = 0;
	while (1)
	{
		if (i > 3)
		{
			if (found_path[i] == '.')
			{
				pokemon_name[i - 4] = '\0';
				break;
			}
			else
			{
				pokemon_name[i - 4] = found_path[i];
			}
		}
		i++;
	}
	printf("pokemon name : %s\n", pokemon_name);

	char ch, * name_tmp = (char*)malloc(sizeof(char) * 10);
	name_tmp = found_path;
	FILE* fp = fopen(name_tmp, "rt");
	if (fp == NULL) {
		printf("파일 오픈 실패 !\n");
	}
	while (1) {
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		// 파이리일 경우 문자별 색상 설정
		switch (ch)
		{
		case '.':
			setColor(15, 15);
			break;
		case '@':
			setColor(0, 0);
			break;
		case '%':
			setColor(2, 2);
			break;
		case '?':
			setColor(4, 4);
			break;
		case '*':
			setColor(4, 4);
			break;
		case ',':
			setColor(14, 14);
			break;
		case '+':
			setColor(12, 12);
			break;
		default:
			setColor(15, 0);
		}
		printf("%c", ch);
	}
	fclose(fp);
	setColor(15, 0);

}

int keyControl()
{
	char temp = getch();
	if (temp == 'w' || temp == 'W') {
		return UP;
	}
	else if (temp == 'a' || temp == 'A') {
		return LEFT;
	}
	else if (temp == 's' || temp == 'S') {
		return DOWN;
	}
	else if (temp == 'd' || temp == 'D') {
		return RIGHT;
	}
	else if (temp == ' ') { // 스페이스바(공백)이 선택 버튼  
		return SPACE;
	}
	else if (temp == 'q') {
		return QUIT;
	}
}

void setColor(int color, int bgcolor)
{
	color &= 0xf;
	bgcolor &= 0xf;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bgcolor << 4) | color);
}