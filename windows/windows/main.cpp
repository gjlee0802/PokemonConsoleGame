#define _CRT_SECURE_NO_WARNINGS

#include <atlstr.h>   // use Cstring type
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //use srand() function
#include <conio.h>   //for Windows OS
#include <windows.h>

#define LAB_VERTICAL_FIELD_SIZE 21
#define FIELD_SIZE 50

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SPACE 4
#define QUIT 9

#define 파워휩	438
#define 물의파동 352
#define 역린		200
#define 솔라빔	76
#define 잎날가르기 75
#define 거품광선	61
#define 화염방사 53
#define 불꽃세례 52
#define 몸통박치기 33
#define 덩굴채찍	22
#define 할퀴기	10

struct Scene {
	int sceneNum;				// 맵의 번호
	char Coor[FIELD_SIZE][FIELD_SIZE];
	
	int HeroX;
	int HeroY;
	int myPokeNum[6] = { 0 };   // 갖고 있는 포켓몬의 도감번호
	int myPokeLevel[6];			// 갖고 있는 포켓몬의 레벨
	int myPokeHealth[6];		// 갖고 있는 포켓몬의 체력
	int myPokeExp[6];			//갖고 있는 포켓몬의 경험치
	int LevelUpExp[6];			//레벨업 위해 얻어야 할 경험치
	int leftedExp[6];			//레벨업하고 남은 경험치 버퍼
	int currPokeIndex = 0;		// 가장 먼저 나올 포켓몬 or 현재 배틀중인 포켓몬의 인덱스
	char currPokeName[30];

	int enemyPokeNum;
	int enemyPokeLevel;
	int enemyPokeHealth;
	char enemyPokeName[30];

	int story1;
	char yesorno;
};
typedef struct Scene scene;


// ** Functions
void pokeNameFind(int, char *);	// Param1: pokeNum, Param2: pokeName(추출한 포켓몬 이름이 저장될 문자열 공간의 주소)
void pokemonPrint(int, int);	// Param1: pokeNum, Param2: onlyHead(머리부분만 출력할 것이면 1 아니면 0)
//
void battleInit(scene*);
void battleMenu(scene*);
int fightMenu(scene*, int);
//
void skillPrint(const char *);	// Param1: 파일(경로) 이름
int pokeVal(int, const char*);
float typeRel(int , int);		// typeRelativity(타입 관계)의 약자, Param1: 공격기술의 번호(techNum), Param2: 공격을 받는 포켓몬 번호(pokeNum)
int useSkill(scene*, int, int);	// Param2: skillNum
//
void checkEvent(scene *);
void teleportMap(scene *, int, int);	// Param 2: 이동하기 전의 sceneNum, Param 3: 이동할 sceneNum
void sceneMap(scene*);
void scenePrint(scene*);
void move(scene*, int);
int isColi(scene*, int, int);
//
int keyControl();
void setColor(int, int);
void titleDraw();


/* MAIN Function */
int main()
{
	//pokemonPrint(25,false);   // 포켓몬 그림 테스트용 코드 테스트 안하면 주석처리.
	
	system("mode con cols=120 lines=120");
	//titleDraw();

	scene* Sptr;
	Sptr = (scene*)malloc(sizeof(scene));




	Sptr->sceneNum = 1;

	Sptr->myPokeNum[0] = 4;   // 향후 연구소에서 지정하도록 설정.
	Sptr->myPokeLevel[0] = 5;
	Sptr->myPokeHealth[0] = Sptr->myPokeLevel[0] * (pokeVal(Sptr->myPokeNum[0], "HP") + 200) / 50; //160) / 50;
	Sptr->myPokeExp[0] = 0;
	Sptr->LevelUpExp[0] = (Sptr->myPokeLevel[0] + 1) * (Sptr->myPokeLevel[0] + 1) * (Sptr->myPokeLevel[0] + 1);
	Sptr->story1 = 0;
	Sptr->HeroX = 11;
	Sptr->HeroY = 12;
	//Sptr->HeroX = 8;
	//Sptr->HeroY = 24;

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
		case SPACE:
			checkEvent(Sptr);
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
	wildPoke = 10;
	itoa(wildPoke, file_name, 10);
	Sptr->enemyPokeNum = wildPoke;
	srand((unsigned int)time(NULL));
	Sptr->enemyPokeLevel = rand() % 15 + 1;   // 레벨 1부터 15까지의 야생 포켓몬이 출현함.
	//Sptr->enemyPokeHealth = Sptr->enemyPokeLevel * 30;   // 체력은 레벨의 30배로 설정.
	Sptr->enemyPokeHealth = Sptr->enemyPokeLevel * (pokeVal(Sptr->enemyPokeNum, "HP") + 200) / 50;	// 체력은 (레벨 * (종족값 + 200) / 50)으로 설정
	pokemonPrint(Sptr->enemyPokeNum, false); 
	battleMenu(Sptr);
}

void battleMenu(scene* Sptr)
{
	Sptr->currPokeIndex = 0;
	int myturn = 1;   // 0: 상대 턴 , 1: 나의 턴
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
			system("cls");
			setColor(10, 0);
			printf(">> 정신을 잃었다... 게임을 종료한다..\n");
			printf("<<<<<  GAME OVER!!  >>>>>\n");
			Sleep(3000);
			break;
		}
		if (Sptr->enemyPokeHealth <= 0)
		{
			setColor(10, 0);
			printf(">> 상대 포켓몬이 쓰러졌다!\n");
			Sleep(1500);
			Sptr->myPokeExp[Sptr->currPokeIndex] = Sptr->myPokeExp[Sptr->currPokeIndex] + (Sptr->enemyPokeLevel) * (Sptr->enemyPokeLevel) * (Sptr->enemyPokeLevel);
			//얻는 경험치는 상대포켓몬 레벨**3으로 설정
			printf(">> 경험치가 %d만큼 증가했다!\n", (Sptr->enemyPokeLevel) * (Sptr->enemyPokeLevel) * (Sptr->enemyPokeLevel));
			Sleep(1500);
			if (Sptr->myPokeExp[0] >= Sptr->LevelUpExp[0]) {
				while (Sptr->myPokeExp[0] >= Sptr->LevelUpExp[0]){
					Sptr->myPokeExp[0] = Sptr->myPokeExp[0] - Sptr->LevelUpExp[0];
					Sptr->myPokeLevel[0] += 1;
					Sptr->LevelUpExp[0] = (Sptr->myPokeLevel[0] + 1) * (Sptr->myPokeLevel[0] + 1) * (Sptr->myPokeLevel[0] + 1);
					printf(">> 레벨이 1 증가했다!\n");
					Sleep(1500);
				}
				printf(">> 레벨이 %d(이)가 되었다!\n", Sptr->myPokeLevel[0]);
				Sleep(2000);

			}
			
			break;
		}

		pokeNameFind(Sptr->myPokeNum[Sptr->currPokeIndex], Sptr->currPokeName);
		pokeNameFind(Sptr->enemyPokeNum, Sptr->enemyPokeName);

		if (myturn == true)
		{
			// 포켓몬 상태 출력
			setColor(10,0);
			printf("\n=====#=====#=====#=====#=====#=====#=====#=====#=====#\n");
			printf("상대 포켓몬%s)) LEVEL: %d | HP: %d\n", Sptr->enemyPokeName, Sptr->enemyPokeLevel, Sptr->enemyPokeHealth);
			for (int h = 0; h < Sptr->enemyPokeHealth / 5; h++)	// HP 바 출력
			{
				setColor(9,10);
				printf("]]");
			} setColor(10, 0); printf("\n");
			printf("  내 포켓몬:%s)) LEVEL: %d | HP: %d | EXP : %d/%d\n", Sptr->currPokeName, Sptr->myPokeLevel[Sptr->currPokeIndex], Sptr->myPokeHealth[Sptr->currPokeIndex], Sptr->myPokeExp[Sptr->currPokeIndex], Sptr->LevelUpExp[Sptr->currPokeIndex]);
			for (int h = 0; h < Sptr->myPokeHealth[Sptr->currPokeIndex] / 5; h++)	// HP 바 출력 
			{
				setColor(9, 10);
				printf("]]");
			}
			setColor(10, 0); printf("\n=====#=====#=====#=====#=====#=====#=====#=====#=====#\n");
			// 선택 메뉴 출력
			printf(">> 무엇을 할까?\n");
			setColor(12, 0);
			printf("============M=E=N=U============>\n");
			setColor(14, 0);
			printf("O---------------------------===>\n");
			printf("| (1)싸운다     (2)교체한다   ==>\n");
			printf("| (3)몬스터볼   (4)도망간다   ==>\n");
			printf("L---------------------------===>\n");
			setColor(15, 0);
			char temp = getch();
			skillNum = 0;
			damage = 0;
			switch (temp)
			{
			case '1':   //싸운다.
				system("cls");
				while (skillNum <= 0)
				{
					skillNum = fightMenu(Sptr, myturn);
					if (skillNum <= 0)
					{
						setColor(10,0);
						printf(">> 메뉴로 돌아갑니다.\n"); break;
					}
				}
				damage = useSkill(Sptr, skillNum, myturn);
				Sptr->enemyPokeHealth = Sptr->enemyPokeHealth - damage;
				setColor(10, 0);
				printf(">> %s가 상대 포켓몬(%s)에게 데미지를 %d만큼 입혔다!\n", Sptr->currPokeName, Sptr->enemyPokeName, damage);
				Sleep(2000);
				break;
			case '2':   //교체한다.
				break;
			case '4':   //도망간다.
				setColor(10,0);
				printf(">> 도망쳤다.!\n");
				Sleep(1000);
				return;
			default:
				setColor(10,0);
				printf(">> 그러한 선택지가 없습니다.\n");
			}
		}
		else if (myturn == false)
		{
			skillNum = 0;
			damage = 0;
			srand((unsigned int)time(NULL));
			skillNum = fightMenu(Sptr, myturn);
			damage = useSkill(Sptr, skillNum, myturn);
			Sptr->myPokeHealth[Sptr->currPokeIndex] = Sptr->myPokeHealth[Sptr->currPokeIndex] - damage;
			setColor(10, 0);
			printf(">> 상대 포켓몬(%s)이 공격하여 데미지를 %d만큼 입었다!\n", Sptr->enemyPokeName, damage);
			Sleep(3000);
		}

		// 턴 교체
		if (myturn == true)
			myturn--;
		else if (!myturn)
			myturn++;
	}
	setColor(10,0);
	printf(">> 배틀 메뉴 종료. 맵으로 돌아갑니다.\n");
}

int fightMenu(scene* Sptr, int myturn)
{
	int skillNum = 0;
	int pokeNum = 0;

	system("cls");
	if (myturn)
	{
		pokeNum = Sptr->myPokeNum[Sptr->currPokeIndex]; setColor(10, 0);
		printf(">> 스킬을 선택하세요.(되돌아가려면 1~4를 제외한 키 입력) \n");
	}
	else if (!myturn)
	{
		pokeNum = Sptr->enemyPokeNum; setColor(10, 0);
		printf(">> 상대 포켓몬(%s) 차례이다...\n\n", Sptr->enemyPokeName);
	}

	char temp;
	switch (pokeNum)   // 포켓몬 도감번호 
	{
	case 1:   // 이상해씨
		printf("1.몸통박치기 2.덩굴채찍 3.잎날가르기 4.솔라빔\n\n");
		if (myturn)	
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			setColor(10,0);
			printf(">> 상대 포켓몬(%s)이 (%c)를 선택했다!", Sptr->enemyPokeName,temp);
			pokemonPrint(Sptr->enemyPokeNum, 1);
			Sleep(2000);
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
			skillNum = 파워휩;
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
			setColor(10,0);
			printf(">> 상대 포켓몬(%s)이 (%c)를 선택했다!\n", Sptr->enemyPokeName, temp);
			pokemonPrint(Sptr->enemyPokeNum, 1);
			Sleep(2000);
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
		printf("1.할퀴기 2.물의파동 3.거품광선 4.몸통박치기\n\n");
		if (myturn)
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			setColor(10, 0);
			printf(">> 상대 포켓몬(%s)이 (%c)를 선택했다!\n", Sptr->enemyPokeName, temp);
			setColor(9, 0);
			pokemonPrint(Sptr->enemyPokeNum, 1);
			Sleep(2000);
		}
		switch (temp)
		{
		case '1':
			skillNum = 할퀴기;
			break;
		case '2':
			skillNum = 물의파동;
			break;
		case '3':
			skillNum = 거품광선;
			break;
		case '4':
			skillNum = 몸통박치기;	// 추후 변경
			break;
		default:
			skillNum = 0;
			break;
		}
		break;
	case 10: // 캐터피
		printf("1.할퀴기 2.물의파동 3.거품광선 4.몸통박치기\n\n");
		if (myturn)
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			setColor(10, 0);
			printf(">> 상대 포켓몬(%s)이 (%c)를 선택했다!\n", Sptr->enemyPokeName, temp);
			setColor(9, 0);
			pokemonPrint(Sptr->enemyPokeNum, 1);
			Sleep(2000);
		}
		switch (temp)
		{
		case '1':
			skillNum = 할퀴기;
			break;
		case '2':
			skillNum = 물의파동;
			break;
		case '3':
			skillNum = 거품광선;
			break;
		case '4':
			skillNum = 몸통박치기;	// 추후 변경
			break;
		default:
			skillNum = 0;
			break;
		}
		break;
	case 25: //피카츄
		printf("1.할퀴기 2.물의파동 3.거품광선 4.몸통박치기\n\n");
		if (myturn)
			temp = getch();
		else if (!myturn)
		{
			srand((unsigned int)time(NULL));
			temp = (char)(rand() % 4 + 1 + 48);
			setColor(10, 0);
			printf(">> 상대 포켓몬(%s)이 (%c)를 선택했다!\n", Sptr->enemyPokeName, temp);
			setColor(9, 0);
			pokemonPrint(Sptr->enemyPokeNum, 1);
			Sleep(2000);
		}
		switch (temp)
		{
		case '1':
			skillNum = 할퀴기;
			break;
		case '2':
			skillNum = 물의파동;
			break;
		case '3':
			skillNum = 거품광선;
			break;
		case '4':
			skillNum = 몸통박치기;	// 추후 변경
			break;
		default:
			skillNum = 0;
			break;
		}
		break;
	}
	return skillNum;
}

void skillPrint(const char * path)
{
	char ch, * name_tmp = (char*)malloc(sizeof(char) * 10);
	strcpy(name_tmp, path);
	FILE* fp = fopen(name_tmp, "r");
	if (fp == NULL) {
		printf("파일 오픈 실패 !\n");
	}
	while (1) {
		ch = fgetc(fp);
		if (ch == EOF)
			break;
		printf("%c", ch);
	}
	printf("\n");
	fclose(fp);
	setColor(15, 0);
}

int techVal(int techNum, const char* keyword)
{
	if (!strcmp(keyword, "TYPE"))
	{
		switch (techNum)
		{
		case 할퀴기:
			return 0;
		case 덩굴채찍:
			return 3;
		case 몸통박치기:
			return 0;
		case 불꽃세례:
			return 1;
		case 화염방사:
			return 1;
		case 거품광선:
			return 2;
		case 잎날가르기:
			return 3;
		case 솔라빔:
			return 3;
		case 역린:
			return 14;
		case 물의파동:
			return 2;
		case 파워휩:
			return 3;
		default:
			printf("techVal() : Can't find such tech!\n");
			return 0;
		}
	}
}

int pokeVal(int pokeNum, const char* keyword)
{
	// strcmp(char *, const char *) 함수는 같은 문자열이면 false를 반환한다. 수정하지 마셈.
	if (!strcmp(keyword, "HP"))
	{
		switch (pokeNum)
		{
		case 1:
			return 45;
		case 4:
			return 39;
		case 7:
			return 44;
		default:
			printf("pokeVal() : Can't find such pokemon!\n");
			return 0;
		}
	}
	if (!strcmp(keyword, "ATK"))
	{
		switch (pokeNum)
		{
		case 1:
			return 49;
		case 4:
			return 52;
		case 7:
			return 48;
		default:
			printf("pokeVal() : Can't find such pokemon!\n");
			return 0;
		}
	}
	if (!strcmp(keyword, "DEF"))
	{
		switch (pokeNum)
		{
		case 1:
			return 49;
		case 4:
			return 43;
		case 7:
			return 65;
		default:
			printf("pokeVal() : Can't find such pokemon!\n");
			return 0;
		}
	}
	if (!strcmp(keyword, "TYPE"))
	{
		switch (pokeNum)
		{
		case 1:
			return 3;
		case 4:
			return 1;
		case 7:
			return 2;
		default:
			printf("pokeVal() : Can't find such pokemon!\n");
			return 0;
		}
	}
}

float typeRel(int techNum, int victimNum)
{
	/*
	0:노말  1:불꽃    2:물      3:풀    4:전기    5:얼음 
	6:격투  7:독      8:땅      9:비행 10:에스퍼 11:벌레 
	12:바위 13:고스트 14:드래곤 15:악   16:강철  17:페어리
	*/
	float type_matrix[18][18] = {
	{1,1,1,1,1,1,1,1,1,1,1,0.5,0,1,1,0.5,1},			// 노말
	{1,0.5,0.5,2,1,2,1,1,1,1,1,2,0.5,1,0.5,1,2,1},		// 불꽃
	{1,2,0.5,0.5,1,1,1,1,2,1,1,1,2,1,0.5,1,1,1},		// 물
	{1,0.5,2,0.5,1,1,1,0.5,2,0.5,1,0.5,2,1,0.5,1,0.5,1},// 풀
	{1,1,2,0.5,0.5,1,1,1,0,2,1,1,1,1,0.5,1,1,1},		// 전기
	{1,0.5,0.5,2,1,0.5,1,1,2,2,1,1,1,1,2,1,0.5,1},		// 얼음
	{2,1,1,1,1,2,1,0.5,1,0.5,0.5,0.5,2,0,1,2,2,0.5},	// 격투
	{1,1,1,2,1,1,1,0.5,0.5,1,1,1,0.5,0.5,1,1,0,2},		// 독
	{1,2,1,0.5,2,1,1,2,1,0,1,0.5,2,1,1,1,2,1},			// 땅
	{1,1,1,2,0.5,1,2,1,1,1,1,2,0.5,1,1,1,0.5,1},		// 비행
	{1,1,1,1,1,1,2,2,1,1,0.5,1,1,1,1,0,0.5,1},			// 에스퍼
	{1,0.5,1,2,1,1,0.5,0.5,1,0.5,2,1,1,0.5,1,2,0.5,0.5},// 벌레
	{1,2,1,1,1,2,0.5,1,0.5,2,1,2,1,1,1,1,0.5,1},		// 바위
	{0,1,1,1,1,1,1,1,1,1,2,1,1,2,1,0.5,1,1},			// 고스트
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,0.5,0},			// 드래곤
	{1,1,1,1,1,1,0.5,1,1,1,2,1,1,2,1,0.5,1,0.5},		// 악
	{1,0.5,0.5,1,0.5,2,1,1,1,1,1,1,2,1,1,1,0.5,2},		// 강철
	{1,0.5,1,1,1,1,2,0.5,1,1,1,1,1,1,2,2,0.5,1}			// 페어리
	};

	int attack_type=0, victim_type=0;
	attack_type = techVal(techNum, "TYPE");
	victim_type = pokeVal(victimNum, "TYPE");

	
	if(type_matrix[attack_type][victim_type] == 1)
		printf("효과가 평범했다.\n");
	if (type_matrix[attack_type][victim_type] == 2)
		printf("효과가 굉장했다!\n");
	if (type_matrix[attack_type][victim_type] == 0.5)
		printf("효과가 별로였다...\n");
	if(type_matrix[attack_type][victim_type] == 0)
		printf("효과가 없었다...\n");

	return type_matrix[attack_type][victim_type];
}

int useSkill(scene* Sptr, int skillNum, int myturn)   // 레벨에 따라 데미지를 리턴한다.
{
	double dmg;
	switch (skillNum)
	{
	case 파워휩:
		dmg = 120;
		setColor(10, 0);
		skillPrint("attack_hit_Grass_3.txt");
	case 물의파동:
		dmg = 60;
		setColor(9,0);
		skillPrint("water_WhirlPool.txt");
		break;
	case 역린:
		dmg = 120;
		setColor(12,0);
		skillPrint("fire_Sacred_Fire.txt");
		break;
	case 솔라빔:
		dmg = 120;
		skillPrint("attack_hit_Grass_3.txt");
		break;
	case 잎날가르기:
		dmg = 55;
		setColor(10,0);
		skillPrint("attack_hit_Grass_1.txt");
		break;
	case 거품광선:
		dmg = 65;
		setColor(9,0);
		skillPrint("water_Water_Pledge.txt");
		break;
	case 화염방사:
		dmg = 90;
		setColor(12,0);
		skillPrint("attack_hit_Fire_3.txt");
		break;
	case 불꽃세례:
		dmg = 40;
		setColor(12,0);
		skillPrint("fire_Ember.txt");
		break;
	case 몸통박치기:
		dmg = 50;
		setColor(8, 0);
		skillPrint("attack_hit_Normal.txt");
		break;
	case 덩굴채찍:
		dmg = 45;
		setColor(10,0);
		skillPrint("attack_hit_Grass_1.txt");
		break;
	case 할퀴기:
		dmg = 40;
		setColor(8, 0);
		skillPrint("attack_hit_Normal.txt");
		break;
	default:
		dmg = 0;
	}

	// 데미지 계산 식 구현
	if (myturn==true)
	{
		/*
		dmg = dmg * 
			pokeVal(Sptr->myPokeNum[Sptr->currPokeIndex], "ATK")
			* ((Sptr->myPokeLevel[Sptr->currPokeIndex] * 2/5 + 2)/pokeVal(Sptr->enemyPokeNum, "DEF")/50+2)
			* 1;*/
		dmg = dmg *
			pokeVal(Sptr->myPokeNum[Sptr->currPokeIndex], "ATK") / pokeVal(Sptr->myPokeNum[Sptr->currPokeIndex], "DEF") *
			(Sptr->myPokeLevel[Sptr->currPokeIndex]+2) / 50 * typeRel(skillNum, Sptr->enemyPokeNum);
		printf("damage : %d\n", int(dmg));
	}
	else
	{
		dmg = dmg *
			pokeVal(Sptr->enemyPokeNum, "ATK") / pokeVal(Sptr->enemyPokeNum, "DEF") *
			(Sptr->enemyPokeLevel + 2) / 50 * typeRel(skillNum, Sptr->myPokeNum[Sptr->currPokeIndex]);
		printf("damage : %d\n", int(dmg));
	}

	return int(dmg);

}

void checkEvent(scene* Sptr)	// This function would be executed when SPACE key pushed.
{
	switch (Sptr->sceneNum)
	{
	case 0:	// 마을0
		// Check entrance
		if (Sptr->HeroX == 30 && Sptr->HeroY == 24)
			teleportMap(Sptr, 0, 1);
		break;
	case 1:	// 연구소
		// Check entrance
		if ((Sptr->HeroX == 24 && Sptr->HeroY == 19)||
			(Sptr->HeroX == 23 && Sptr->HeroY == 19))
			teleportMap(Sptr, 1, 0);
		
		// Talk with Professor
		if (Sptr->story1 == 0) {
			if (((24 - Sptr->HeroX == -1 || 24 - Sptr->HeroX == 1) && Sptr->HeroY == 8) ||
				((8 - Sptr->HeroY == -1 || 8 - Sptr->HeroY == 1) && Sptr->HeroX == 24)) {
				printf("안녕! 난 오박사란다. \n");
				getch();
				printf("너 포켓몬이 없구나. 포켓몬을 키워보겠니?\n");
				getch();
				printf("옆에 있는 포켓몬 세마리 중 하나를 고르렴.\n");
				getch();
			}
		}

		//choice pokemon
		if (Sptr->HeroX == 36 && Sptr->HeroY == 10) {
			pokemonPrint(1, false);
			printf("풀의 포켓몬 이상해씨를 고르시겠습니까?(y, n)");
			scanf("%c", &Sptr->yesorno);
			if (Sptr->yesorno == 'y' || Sptr->yesorno == 'Y') {
				Sptr->myPokeNum[0] = 1;
				printf("이상해씨를 손에 넣었다!");
				getch();
			}
		}
		if (Sptr->HeroX == 40 && Sptr->HeroY == 10) {
			pokemonPrint(4, false);
			printf("불의 포켓몬 파이리를 고르시겠습니까?(y, n)");
			scanf("%c", &Sptr->yesorno);
			if (Sptr->yesorno == 'y' || Sptr->yesorno == 'Y') {
				Sptr->myPokeNum[0] = 4;
				printf("파이리를 손에 넣었다!");
				getch();
			}
		}
		if (Sptr->HeroX == 44 && Sptr->HeroY == 10) {
			pokemonPrint(7, false);
			printf("물의 포켓몬 꼬부기를 고르시겠습니까?(y, n)");
			scanf("%c", &Sptr->yesorno);
			if (Sptr->yesorno == 'y' || Sptr->yesorno == 'Y') {
				Sptr->myPokeNum[0] = 7;
				printf("꼬부기를 손에 넣었다!");
				getch();
			}
		}
		break;
	}
}

void teleportMap(scene* Sptr, int sceneNum0, int sceneNum1)
{
	if (sceneNum0 == 0 && sceneNum1 == 1)
	{
		Sptr->HeroX = 24;
		Sptr->HeroY = 18;
		Sptr->sceneNum = 1;
	}
	else if (sceneNum0 == 1 && sceneNum1 == 0)
	{
		Sptr->HeroX = 30;
		Sptr->HeroY = 24;
		Sptr->sceneNum = 0;
	}
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
		for (j = 42; j < FIELD_SIZE-1; j++)
			Sptr->Coor[32][j] = '=';

		for (j = 1; j < FIELD_SIZE-1; j++)
		{
			int water_cnt = 0;
			for (i = 31; i < FIELD_SIZE-1; i++)
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
		for (i = 1; i < LAB_VERTICAL_FIELD_SIZE - 2; i++)
			for (j = 1; j < FIELD_SIZE - 1; j++)
				Sptr->Coor[i][j] = ' ';

		//Door open
		Sptr->Coor[19][24] = ' ';
		Sptr->Coor[19][23] = ' ';

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
		Sptr->Coor[1][4] = 'm';
		Sptr->Coor[1][5] = 'm';
		for (j = 3; j <= 6; j++)
			Sptr->Coor[2][j] = 'm';
		for (i = 3; i <= 5; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

		//Machine2
		Sptr->Coor[7][4] = 'm';
		Sptr->Coor[7][5] = 'm';
		for (i = 3; i <= 6; i++)
			Sptr->Coor[8][i] = 'm';
		for (i = 9; i <= 11; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

		//Machine3
		Sptr->Coor[13][4] = 'm';
		Sptr->Coor[13][5] = 'm';
		for (i = 3; i <= 6; i++)
			Sptr->Coor[14][i] = 'm';
		for (i = 15; i <= 17; i++)
			for (j = 3; j <= 6; j++)
				Sptr->Coor[i][j] = 'c';

		//book
		for (int i = 1; i <= 3; i++) {
			for (int j = 35; j <= 45; j++)
				Sptr->Coor[i][j] = 'E';
		}

	}
		  Sptr->Coor[Sptr->HeroY][Sptr->HeroX] = 'H';
	}//Switch end

}


void scenePrint(scene* Sptr)
{
	if (Sptr->sceneNum == 0) {
		system("cls");
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
				case '^':
					setColor(2, 15);
					break;
				case '/':
					setColor(6, 0);
					break;
				case '\\':
					setColor(6, 0);
					break;
				default:
					setColor(15, 0);
					break;
				}

				printf("%c ", Sptr->Coor[i][j]);
			}
			printf("\n");
			setColor(15, 0);
		}
	}
	else if(Sptr->sceneNum == 1) {
			system("cls");
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
			for (i = 0; i < LAB_VERTICAL_FIELD_SIZE; i++)
			{
				if (i <= 9)
					printf("%1d: ", i);
				else
					printf("%1d:", i);
				for (int j = 0; j < FIELD_SIZE; j++)
				{
					switch (Sptr->Coor[i][j])
					{
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
						setColor(1,1);
						break;
					case 'c':
						setColor(23, 23);
						break;
					case 'E':
						setColor(0,8);
						break;
					case 'A':
						setColor(2, 2);
						break;
					case '^':
						setColor(7,7);
						break;
					default:
						setColor(15, 0);
						break;
					}

					printf("%c ", Sptr->Coor[i][j]);
				}
				printf("\n");
				setColor(15,0);
			}
		
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
		Sptr->Coor[y][x] == 'c' ||
		Sptr->Coor[y][x] == 'P'	// Professor
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

void pokeNameFind(int pokeNum, char *pokeName)
{
	HANDLE fileSearch;
	WIN32_FIND_DATA wfd;
	CString musiccount;
	CString findFirstFileName;
	CString findSecondFileName;

	char find_path[20] = "";
	char found_path[40] = "";
	
	find_path[0] = (char)(pokeNum / 100 + 48);
	find_path[1] = (char)((pokeNum / 10) % 10 + 48);
	find_path[2] = (char)(pokeNum % 10 + 48);
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
	strcpy(pokeName, (const char*)pokemon_name);
}

void pokemonPrint(int pokeNum, int onlyHead)
{
	HANDLE fileSearch;
	WIN32_FIND_DATA wfd;
	CString musiccount;
	CString findFirstFileName;
	CString findSecondFileName;
	char find_path[20] = "";
	char found_path[40] = "";

	find_path[0] = (char)(pokeNum / 100 + 48);
	find_path[1] = (char)((pokeNum / 10) % 10 + 48);
	find_path[2] = (char)(pokeNum % 10 + 48);
	find_path[3] = '_';
	find_path[4] = '*';
	if (onlyHead == false)
	{
		find_path[5] = '\0';
	}
	else if (onlyHead == true)
	{
		find_path[5] = '_'; find_path[6] = 'F'; find_path[7] = 'a'; find_path[8] = 'c'; find_path[9] = 'e';
		find_path[10] = '*'; find_path[11] = '\0';
	}

	//musiccount.Format(_T("C:\\Program Files\\TEST\\1_*.*"));
	musiccount.Format(_T((const char*)find_path));
	fileSearch = FindFirstFile(musiccount, &wfd);
	if (fileSearch != INVALID_HANDLE_VALUE)
	{
		findFirstFileName.Format(_T("%s"), wfd.cFileName);
		strcpy(found_path, wfd.cFileName);
		FindClose(fileSearch);
		//printf("%s!\n", found_path);
	}
	char pokemon_name[30];
	int i = 0;
	while (1)
	{
		if (i > 3)
		{
			if (found_path[i] == '.'|| found_path[i] == '_')
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
	printf("pokemon_name : %s\n", pokemon_name);

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
		
		if(onlyHead==false)					// 전체를 출력할 경우(머리X)
		{
			switch (pokeNum)
			{
			case 1:  //이상해씨
				switch (ch)
				{
				case '@':case '#':case '%':case ',':case ';':case ':':
					setColor(0, 0);
					break;
				case '.':
					setColor(15, 15);
					break;
				case 'S':case '?':
					setColor(2, 2);
					break;
				case '+':
					setColor(11, 11);
					break;
				case '*':
					setColor(10, 10);
					break;
				default:
					setColor(15, 0);
					break;
				}
				break;
			case 4:	//파이리
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
					break;
				}
				break;
			case 7:  //꼬부기
				switch (ch)
				{
				case '@':case '?':case '#':case '%':
					setColor(0, 0);
					break;
				case ';':case ':':
					setColor(11, 11);
					break;
				case '/':case 'S':
					setColor(4, 4);
					break;
				case '+':
					setColor(12, 12);
					break;
				case '*':
					setColor(3, 3);
					break;
				case '!':
					setColor(7, 7);
					break;
				case '.':case ',':
					setColor(15, 15);
					break;
				case '^':
					setColor(6, 6);
					break;
				case '-':
					setColor(5, 5);
					break;
				case '=':
					setColor(14, 14);
					break;
					
				default:
					setColor(15, 0);
					break;
				}
				break;
			case 10:  //캐터피
				switch (ch)
				{
				case '@':case '#':case '%':
					setColor(0, 0);
					break;
				case ';':case ':':
					setColor(11, 11);
					break;
				case 'S':
					setColor(4, 4);
					break;
				case '/':
					setColor(12, 12);
					break;
				case '+':
					setColor(10, 10);
					break;
				case '*':
					setColor(2, 2);
					break;
				case '?':case '-':
					setColor(7, 7);
					break;
				case ',':
					setColor(14, 14);
					break;
				case '.':
					setColor(15, 15);
					break;
				case '^':
					setColor(6, 6);
					break;
				case '=':
					setColor(14, 14);
					break;

				default:
					setColor(15, 0);
					break;
				}
				break;
			case 25:  //피카츄
				switch (ch)
				{
				case '@':case '#':case ',':case '?':case '*':case ';':
					setColor(0, 0);
					break;
				case '.':
					setColor(15, 15);
					break;
				case ':':
					setColor(14, 14);
					break;
				case '+':
					setColor(6, 6);
					break;
				case 'S':
					setColor(12, 12);
					break;
				case '%':
					setColor(4, 4);
					break;
				default:
					setColor(15, 0);
					break;
				}
				break;
			default:
				setColor(15,0);
			}
		}
		else if (onlyHead == true)
		{
			switch (pokeNum)
			{
			case 1:
				setColor(10, 0);
				break;
			case 4:
				setColor(12, 0);
				break;
			case 7:
				setColor(9, 0);
				break;
			}
		}
		printf("%c", ch);
	}
	printf("\n");
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

void titleDraw() {
	printf("\n\n\n\n");
	printf("  ###################    #######   ## ##      ##########              ##              #########   ###\n");
	printf("  ##################    ########   ## ##      ###    ###            ######            ########    ###\n");
	printf("       ##     ##            ###  #### ##      ###    ###           ########           ##          ###\n");
	printf("        #     #          #####   #### ##      ##########          ##########          ######   ######\n");
	printf("  ###################     ###      ## ##         ###             #####   #####        ######   ######\n");
	printf("  ##################      ##       #  #     ###############     #####     ######      ##          ###\n");
	printf("          ###                  ####         ##############     #####       ######     #######     ###\n");
	printf("          ###               ####  ####         ##                                     ########    ###\n");
	printf("######################     ####    ####        ##       #     ####################               ####\n");
	printf("  ###################     ####       ###       #########      ###################               ####\n");
	printf("                                                                                               ####\n");
	printf("\n");
	printf("\n");
}