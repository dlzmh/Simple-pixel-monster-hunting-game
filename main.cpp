#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>
#include<windows.h>
#include<conio.h>
#include<math.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define MAX_FIREBALL 10		//火球队列最大容量
#define FIRE_COOLDOWN 3000	//发射间隔 单位毫秒
#define ATTACK_COOLDOWN 9000//骷髅龙攻击间隔

DWORD last_fire_time = 0;	//上一次发射的时间戳
DWORD last_attack_time = 0;	//上一次骷髅龙攻击时间戳

IMAGE MAINMENU;	
IMAGE MAINMENU2;
IMAGE MAINMENU3;
IMAGE MAP;
IMAGE PLAYERR[31];			//player right
IMAGE PLAYERL[31];			//player left
IMAGE PLAYERRUNR[31];		//player run right
IMAGE PLAYERRUNL[31];		//player run left 
IMAGE PLAYERJUMPR[21];		//player jump right
IMAGE PLAYERJUMPL[21];		//player jump left
IMAGE PLAYERATTACKR[16];	//player attack right
IMAGE PLAYERATTACKL[16];	//player attack left
IMAGE MAGMADRAGON[16];		//magma dragon
IMAGE SKELETONDRAGON[15];	//skeleton dragon
IMAGE SKELETONDRAGONRUNL[16];
IMAGE SKELETONDRAGONRUNR[16];
IMAGE SKELETONDRAGONATTACKL[31];
IMAGE SKELETONDRAGONATTACKR[31];
IMAGE SKELETONDRAGONATTACKPAWL[16];		//skeleton dragon attack left
IMAGE SKELETONDRAGONATTACKPAWR[16];		//skeleton dragon attack right
IMAGE FIREBALL[21];						//fire ball
IMAGE PLAYERAVATAR;						//player avatar头像
IMAGE SKELETONDRAGONAVATAR;				//skeleton dragon avator
IMAGE MAGMADRAGONAVATAR;				//magma dragon avator
IMAGE CheckoutScreen[3];				//1失败2胜利

int frameCounter = 0;		//帧计数器，控制动画速度player
int frameCounter2 = 0;		//帧计数器，控制动画速度
int animInterval = 5;		//每三帧换一次图
int mainMenu3X = 300;

int frameStand = 1;			//角色待机帧
int frameRunRight = 1;		//角色跑步右帧
int frameRunLeft = 1;		//角色跑步左帧
int frameJump = 1;			//角色跳跃帧
int frameAttackRight = 1;	//角色攻击右帧
int frameAttackLeft = 1;	//角色攻击左帧
int playerRunSpeed = 8;		//角色跑步速度
int direction = 0;			//角色朝向左0右1
int playerVy = 0;			//角色竖直速度
int gravity = 5;			//重力
int isJump = 0;				//0在地面1在空中
int isAttack = 0;			//0没攻击，1在攻击
int playerHPx = 100;		//角色血条长度
int playerHert = 10;		//角色伤害
int playerLive = 0;			//0存活1死亡
bool playerMusic = FALSE;	

int frameMagmaDragon = 1;	//岩浆龙待机帧
int frameFireBall = 1;		//火球帧
int fireBallSpeed = 25;		//火球速度
int magmaDragonHPx = 200;	//岩浆龙血条长度
int fireBallHert = 15;		//火球伤害
int bounceFireBallHert = 20;//反弹火球伤害
int magmaDragonLive = 0;	//0存活1死亡
bool magmaDragonMusic = FALSE;
bool fireBallMusic = FALSE;

int frameSkeletonDragon = 1;			//骷髅龙待机帧
int frameSkeletonDragonRunLeft = 1;		//骷髅龙行走左帧
int frameSkeletonDragonRunRight = 1;	//骷髅龙行走右帧
int frameSkeletonDragonTaliAttack = 1;	//骷髅龙甩尾攻击帧
int frameSkeletonDragonPawAttack = 1;	//骷髅龙利爪攻击帧
int skeletonDragonRunSpeed = 10;		//骷髅龙移动速度
int isSkeletonDragonAttack = 0;			//骷髅龙甩尾攻击 0未攻击 1攻击中
int skeletonDragonAttackKind = 0;		//0甩尾攻击，1利爪攻击
int skeletonDragonHPx = 200;			//骷髅龙血条长度
int skeletonDragonTailHart = 1;			//甩尾伤害
int skeletonDragonPawHart = 10;			//利爪伤害
int skeletonDragonLive = 0;				//0存活1阵亡
bool skeletonDragonMusic = FALSE;

typedef struct PlayerPlace				//角色坐标
{
	int x;
	int y;
}PlayerPlace;

typedef struct SkeletonDragonPlace		//骷髅龙坐标
{
	int x;
	int y;
}SkeletonDragonPlace;

typedef struct MagmaDragonPlace			//岩浆龙坐标
{
	int x;
	int y;
}MagmaDragonPlace;

struct FireBallPlace					//火球坐标
{
	int x;
	int y;
	float dx;	//方向向量x
	float dy;	//方向向量y
};

typedef struct							//火球队列
{
	FireBallPlace data[MAX_FIREBALL];		//底层数组，循环使用
	int front;								//队首下标
	int rear;								//队尾下标
	int count;								//当前元素个数
}FireBallQueue;

FireBallQueue FireBalls;				//全局火球队列
PlayerPlace Player;	
SkeletonDragonPlace SkeletonDragon;
MagmaDragonPlace MagmaDragon;

void SaveTexture()		//存贴图
{
	loadimage(&MAINMENU, _T("E:\\蓝桥云课\\像素游戏素材\\主菜单\\主菜单.png"), 1000,750 );
	loadimage(&MAINMENU2, _T("E:\\蓝桥云课\\像素游戏素材\\主菜单\\主菜单-操作介绍.png"), 1000, 750);
	loadimage(&MAINMENU3, _T("E:\\蓝桥云课\\像素游戏素材\\主菜单\\加载动画.png"), 1000, 750);
	loadimage(&CheckoutScreen[1], _T("E:\\蓝桥云课\\像素游戏素材\\主菜单\\失败界面.png"), 1000, 750);
	loadimage(&CheckoutScreen[2], _T("E:\\蓝桥云课\\像素游戏素材\\主菜单\\胜利界面.png"), 1000, 750);

	loadimage(&MAP, _T("E:\\蓝桥云课\\像素游戏素材\\火山巢穴\\去水印\\1.png"), 1000, 800);
	
	loadimage(&PLAYERAVATAR, _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\骑士头像.png"),50, 50);
	loadimage(&PLAYERR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\1.png"), 50,80);
	loadimage(&PLAYERR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\2.png"), 50, 80);
	loadimage(&PLAYERR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\3.png"), 50, 80);
	loadimage(&PLAYERR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\4.png"), 50, 80);
	loadimage(&PLAYERR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\5.png"), 50, 80);
	loadimage(&PLAYERR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\6.png"), 50, 80);
	loadimage(&PLAYERR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\7.png"), 50, 80);
	loadimage(&PLAYERR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\8.png"), 50, 80);
	loadimage(&PLAYERR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\9.png"), 50, 80);
	loadimage(&PLAYERR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\10.png"), 50, 80);
	loadimage(&PLAYERR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\11.png"), 50, 80);
	loadimage(&PLAYERR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\12.png"), 50, 80);
	loadimage(&PLAYERR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\13.png"), 50, 80);
	loadimage(&PLAYERR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\14.png"), 50, 80);
	loadimage(&PLAYERR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\15.png"), 50, 80);
	loadimage(&PLAYERR[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\16.png"), 50, 80);
	loadimage(&PLAYERR[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\17.png"), 50, 80);
	loadimage(&PLAYERR[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\18.png"), 50, 80);
	loadimage(&PLAYERR[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\19.png"), 50, 80);
	loadimage(&PLAYERR[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\20.png"), 50, 80);
	loadimage(&PLAYERR[21], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\21.png"), 50, 80);
	loadimage(&PLAYERR[22], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\22.png"), 50, 80);
	loadimage(&PLAYERR[23], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\23.png"), 50, 80);
	loadimage(&PLAYERR[24], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\24.png"), 50, 80);
	loadimage(&PLAYERR[25], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\25.png"), 50, 80);
	loadimage(&PLAYERR[26], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\26.png"), 50, 80);
	loadimage(&PLAYERR[27], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\27.png"), 50, 80);
	loadimage(&PLAYERR[28], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\28.png"), 50, 80);
	loadimage(&PLAYERR[29], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\29.png"), 50, 80);
	loadimage(&PLAYERR[30], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机右\\30.png"), 50, 80);

	loadimage(&PLAYERL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\1.png"), 50, 80);
	loadimage(&PLAYERL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\2.png"), 50, 80);
	loadimage(&PLAYERL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\3.png"), 50, 80);
	loadimage(&PLAYERL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\4.png"), 50, 80);
	loadimage(&PLAYERL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\5.png"), 50, 80);
	loadimage(&PLAYERL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\6.png"), 50, 80);
	loadimage(&PLAYERL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\7.png"), 50, 80);
	loadimage(&PLAYERL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\8.png"), 50, 80);
	loadimage(&PLAYERL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\9.png"), 50, 80);
	loadimage(&PLAYERL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\10.png"), 50, 80);
	loadimage(&PLAYERL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\11.png"), 50, 80);
	loadimage(&PLAYERL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\12.png"), 50, 80);
	loadimage(&PLAYERL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\13.png"), 50, 80);
	loadimage(&PLAYERL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\14.png"), 50, 80);
	loadimage(&PLAYERL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\15.png"), 50, 80);
	loadimage(&PLAYERL[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\16.png"), 50, 80);
	loadimage(&PLAYERL[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\17.png"), 50, 80);
	loadimage(&PLAYERL[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\18.png"), 50, 80);
	loadimage(&PLAYERL[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\19.png"), 50, 80);
	loadimage(&PLAYERL[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\20.png"), 50, 80);
	loadimage(&PLAYERL[21], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\21.png"), 50, 80);
	loadimage(&PLAYERL[22], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\22.png"), 50, 80);
	loadimage(&PLAYERL[23], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\23.png"), 50, 80);
	loadimage(&PLAYERL[24], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\24.png"), 50, 80);
	loadimage(&PLAYERL[25], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\25.png"), 50, 80);
	loadimage(&PLAYERL[26], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\26.png"), 50, 80);
	loadimage(&PLAYERL[27], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\27.png"), 50, 80);
	loadimage(&PLAYERL[28], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\28.png"), 50, 80);
	loadimage(&PLAYERL[29], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\29.png"), 50, 80);
	loadimage(&PLAYERL[30], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\待机\\骑士待机左\\30.png"), 50, 80);

	loadimage(&PLAYERRUNR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\1.png"), 50, 80);
	loadimage(&PLAYERRUNR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\2.png"), 50, 80);
	loadimage(&PLAYERRUNR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\3.png"), 50, 80);
	loadimage(&PLAYERRUNR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\4.png"), 50, 80);
	loadimage(&PLAYERRUNR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\5.png"), 50, 80);
	loadimage(&PLAYERRUNR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\6.png"), 50, 80);
	loadimage(&PLAYERRUNR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\7.png"), 50, 80);
	loadimage(&PLAYERRUNR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\8.png"), 50, 80);
	loadimage(&PLAYERRUNR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\9.png"), 50, 80);
	loadimage(&PLAYERRUNR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\10.png"), 50, 80);
	loadimage(&PLAYERRUNR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\11.png"), 50, 80);
	loadimage(&PLAYERRUNR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\12.png"), 50, 80);
	loadimage(&PLAYERRUNR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\13.png"), 50, 80);
	loadimage(&PLAYERRUNR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\14.png"), 50, 80);
	loadimage(&PLAYERRUNR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\15.png"), 50, 80);
	loadimage(&PLAYERRUNR[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\16.png"), 50, 80);
	loadimage(&PLAYERRUNR[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\17.png"), 50, 80);
	loadimage(&PLAYERRUNR[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\18.png"), 50, 80);
	loadimage(&PLAYERRUNR[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\19.png"), 50, 80);
	loadimage(&PLAYERRUNR[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\20.png"), 50, 80);
	loadimage(&PLAYERRUNR[21], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\21.png"), 50, 80);
	loadimage(&PLAYERRUNR[22], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\22.png"), 50, 80);
	loadimage(&PLAYERRUNR[23], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\23.png"), 50, 80);
	loadimage(&PLAYERRUNR[24], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\24.png"), 50, 80);
	loadimage(&PLAYERRUNR[25], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\25.png"), 50, 80);
	loadimage(&PLAYERRUNR[26], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\26.png"), 50, 80);
	loadimage(&PLAYERRUNR[27], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\27.png"), 50, 80);
	loadimage(&PLAYERRUNR[28], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\28.png"), 50, 80);
	loadimage(&PLAYERRUNR[29], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\29.png"), 50, 80);
	loadimage(&PLAYERRUNR[30], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步右\\30.png"), 50, 80);

	loadimage(&PLAYERRUNL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\1.png"), 50, 80);
	loadimage(&PLAYERRUNL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\2.png"), 50, 80);
	loadimage(&PLAYERRUNL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\3.png"), 50, 80);
	loadimage(&PLAYERRUNL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\4.png"), 50, 80);
	loadimage(&PLAYERRUNL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\5.png"), 50, 80);
	loadimage(&PLAYERRUNL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\6.png"), 50, 80);
	loadimage(&PLAYERRUNL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\7.png"), 50, 80);
	loadimage(&PLAYERRUNL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\8.png"), 50, 80);
	loadimage(&PLAYERRUNL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\9.png"), 50, 80);
	loadimage(&PLAYERRUNL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\10.png"), 50, 80);
	loadimage(&PLAYERRUNL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\11.png"), 50, 80);
	loadimage(&PLAYERRUNL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\12.png"), 50, 80);
	loadimage(&PLAYERRUNL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\13.png"), 50, 80);
	loadimage(&PLAYERRUNL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\14.png"), 50, 80);
	loadimage(&PLAYERRUNL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\15.png"), 50, 80);
	loadimage(&PLAYERRUNL[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\16.png"), 50, 80);
	loadimage(&PLAYERRUNL[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\17.png"), 50, 80);
	loadimage(&PLAYERRUNL[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\18.png"), 50, 80);
	loadimage(&PLAYERRUNL[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\19.png"), 50, 80);
	loadimage(&PLAYERRUNL[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\20.png"), 50, 80);
	loadimage(&PLAYERRUNL[21], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\21.png"), 50, 80);
	loadimage(&PLAYERRUNL[22], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\22.png"), 50, 80);
	loadimage(&PLAYERRUNL[23], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\23.png"), 50, 80);
	loadimage(&PLAYERRUNL[24], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\24.png"), 50, 80);
	loadimage(&PLAYERRUNL[25], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\25.png"), 50, 80);
	loadimage(&PLAYERRUNL[26], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\26.png"), 50, 80);
	loadimage(&PLAYERRUNL[27], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\27.png"), 50, 80);
	loadimage(&PLAYERRUNL[28], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\28.png"), 50, 80);
	loadimage(&PLAYERRUNL[29], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\29.png"), 50, 80);
	loadimage(&PLAYERRUNL[30], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跑步\\骑士跑步左\\30.png"), 50, 80);

	loadimage(&PLAYERJUMPR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\1.png"), 50, 80);
	loadimage(&PLAYERJUMPR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\2.png"), 50, 80);
	loadimage(&PLAYERJUMPR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\3.png"), 50, 80);
	loadimage(&PLAYERJUMPR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\4.png"), 50, 80);
	loadimage(&PLAYERJUMPR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\5.png"), 50, 80);
	loadimage(&PLAYERJUMPR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\6.png"), 50, 80);
	loadimage(&PLAYERJUMPR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\7.png"), 50, 80);
	loadimage(&PLAYERJUMPR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\8.png"), 50, 80);
	loadimage(&PLAYERJUMPR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\9.png"), 50, 80);
	loadimage(&PLAYERJUMPR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\10.png"), 50, 80);
	loadimage(&PLAYERJUMPR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\11.png"), 50, 80);
	loadimage(&PLAYERJUMPR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\12.png"), 50, 80);
	loadimage(&PLAYERJUMPR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\13.png"), 50, 80);
	loadimage(&PLAYERJUMPR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\14.png"), 50, 80);
	loadimage(&PLAYERJUMPR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\15.png"), 50, 80);
	loadimage(&PLAYERJUMPR[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\16.png"), 50, 80);
	loadimage(&PLAYERJUMPR[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\17.png"), 50, 80);
	loadimage(&PLAYERJUMPR[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\18.png"), 50, 80);
	loadimage(&PLAYERJUMPR[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\19.png"), 50, 80);
	loadimage(&PLAYERJUMPR[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃右\\20.png"), 50, 80);
	
	loadimage(&PLAYERJUMPL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\1.png"), 50, 80);
	loadimage(&PLAYERJUMPL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\2.png"), 50, 80);
	loadimage(&PLAYERJUMPL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\3.png"), 50, 80);
	loadimage(&PLAYERJUMPL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\4.png"), 50, 80);
	loadimage(&PLAYERJUMPL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\5.png"), 50, 80);
	loadimage(&PLAYERJUMPL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\6.png"), 50, 80);
	loadimage(&PLAYERJUMPL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\7.png"), 50, 80);
	loadimage(&PLAYERJUMPL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\8.png"), 50, 80);
	loadimage(&PLAYERJUMPL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\9.png"), 50, 80);
	loadimage(&PLAYERJUMPL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\10.png"), 50, 80);
	loadimage(&PLAYERJUMPL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\11.png"), 50, 80);
	loadimage(&PLAYERJUMPL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\12.png"), 50, 80);
	loadimage(&PLAYERJUMPL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\13.png"), 50, 80);
	loadimage(&PLAYERJUMPL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\14.png"), 50, 80);
	loadimage(&PLAYERJUMPL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\15.png"), 50, 80);
	loadimage(&PLAYERJUMPL[16], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\16.png"), 50, 80);
	loadimage(&PLAYERJUMPL[17], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\17.png"), 50, 80);
	loadimage(&PLAYERJUMPL[18], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\18.png"), 50, 80);
	loadimage(&PLAYERJUMPL[19], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\19.png"), 50, 80);
	loadimage(&PLAYERJUMPL[20], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\跳跃\\骑士跳跃左\\20.png"), 50, 80);

	loadimage(&PLAYERATTACKR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\1.png"), 50, 80);
	loadimage(&PLAYERATTACKR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\2.png"), 50, 80);
	loadimage(&PLAYERATTACKR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\3.png"), 50, 80);
	loadimage(&PLAYERATTACKR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\4.png"), 50, 80);
	loadimage(&PLAYERATTACKR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\5.png"), 50, 80);
	loadimage(&PLAYERATTACKR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\6.png"), 50, 80);
	loadimage(&PLAYERATTACKR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\7.png"), 50, 80);
	loadimage(&PLAYERATTACKR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\8.png"), 50, 80);
	loadimage(&PLAYERATTACKR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\9.png"), 50, 80);
	loadimage(&PLAYERATTACKR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\10.png"), 50, 80);
	loadimage(&PLAYERATTACKR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\11.png"), 50, 80);
	loadimage(&PLAYERATTACKR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\12.png"), 50, 80);
	loadimage(&PLAYERATTACKR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\13.png"), 50, 80);
	loadimage(&PLAYERATTACKR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\14.png"), 50, 80);
	loadimage(&PLAYERATTACKR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击右\\15.png"), 50, 80);

	loadimage(&PLAYERATTACKL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\1.png"), 50, 80);
	loadimage(&PLAYERATTACKL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\2.png"), 50, 80);
	loadimage(&PLAYERATTACKL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\3.png"), 50, 80);
	loadimage(&PLAYERATTACKL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\4.png"), 50, 80);
	loadimage(&PLAYERATTACKL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\5.png"), 50, 80);
	loadimage(&PLAYERATTACKL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\6.png"), 50, 80);
	loadimage(&PLAYERATTACKL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\7.png"), 50, 80);
	loadimage(&PLAYERATTACKL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\8.png"), 50, 80);
	loadimage(&PLAYERATTACKL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\9.png"), 50, 80);
	loadimage(&PLAYERATTACKL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\10.png"), 50, 80);
	loadimage(&PLAYERATTACKL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\11.png"), 50, 80);
	loadimage(&PLAYERATTACKL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\12.png"), 50, 80);
	loadimage(&PLAYERATTACKL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\13.png"), 50, 80);
	loadimage(&PLAYERATTACKL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\14.png"), 50, 80);
	loadimage(&PLAYERATTACKL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骑士\\攻击\\骑士攻击左\\15.png"), 50, 80);
	
	loadimage(&MAGMADRAGONAVATAR, _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙头像.png"), 50, 50);
	loadimage(&MAGMADRAGON[1], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\1.png"), 100, 200);
	loadimage(&MAGMADRAGON[2], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\2.png"), 100, 200);
	loadimage(&MAGMADRAGON[3], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\3.png"), 100, 200);
	loadimage(&MAGMADRAGON[4], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\4.png"), 100, 200);
	loadimage(&MAGMADRAGON[5], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\5.png"), 100, 200);
	loadimage(&MAGMADRAGON[6], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\6.png"), 100, 200);
	loadimage(&MAGMADRAGON[7], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\7.png"), 100, 200);
	loadimage(&MAGMADRAGON[8], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\8.png"), 100, 200);
	loadimage(&MAGMADRAGON[9], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\9.png"), 100, 200);
	loadimage(&MAGMADRAGON[10], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\10.png"), 100, 200);
	loadimage(&MAGMADRAGON[11], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\11.png"), 100, 200);
	loadimage(&MAGMADRAGON[12], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\12.png"), 100, 200);
	loadimage(&MAGMADRAGON[13], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\13.png"), 100, 200);
	loadimage(&MAGMADRAGON[14], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\14.png"), 100, 200);
	loadimage(&MAGMADRAGON[15], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\岩浆龙待机\\15.png"), 100, 200);

	loadimage(&SKELETONDRAGON[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGON[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGON[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGON[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGON[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGON[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGON[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGON[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGON[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGON[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGON[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGON[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGON[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGON[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙待机\\14.png"), 100, 200);

	loadimage(&SKELETONDRAGONAVATAR, _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙头像.png"), 50, 50);
	loadimage(&SKELETONDRAGONRUNL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走左\\15.png"), 100, 200);

	loadimage(&SKELETONDRAGONRUNR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONRUNR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙行走右\\15.png"), 100, 200);

	loadimage(&SKELETONDRAGONATTACKL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\15.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[16], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\16.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[17], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\17.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[18], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\18.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[19], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\19.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[20], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\20.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[21], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\21.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[22], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\22.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[23], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\23.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[24], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\24.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[25], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\25.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[26], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\26.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[27], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\27.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[28], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\28.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[29], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\29.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKL[30], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击左\\30.png"), 100, 200);
	
	loadimage(&SKELETONDRAGONATTACKR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\15.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[16], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\16.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[17], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\17.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[18], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\18.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[19], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\19.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[20], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\20.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[21], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\21.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[22], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\22.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[23], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\23.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[24], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\24.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[25], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\25.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[26], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\26.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[27], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\27.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[28], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\28.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[29], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\29.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKR[30], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙甩尾攻击右\\30.png"), 100, 200);

	loadimage(&SKELETONDRAGONATTACKPAWL[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWL[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击左\\15.png"), 100, 200);

	loadimage(&SKELETONDRAGONATTACKPAWR[1], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\1.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[2], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\2.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[3], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\3.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[4], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\4.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[5], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\5.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[6], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\6.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[7], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\7.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[8], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\8.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[9], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\9.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[10], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\10.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[11], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\11.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[12], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\12.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[13], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\13.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[14], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\14.png"), 100, 200);
	loadimage(&SKELETONDRAGONATTACKPAWR[15], _T("E:\\蓝桥云课\\像素游戏素材\\骷髅龙\\骷髅龙利爪攻击右\\15.png"), 100, 200);

	loadimage(&FIREBALL[1], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\1.png"), 20, 20);
	loadimage(&FIREBALL[2], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\2.png"), 20, 20);
	loadimage(&FIREBALL[3], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\3.png"), 20, 20);
	loadimage(&FIREBALL[4], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\4.png"), 20, 20);
	loadimage(&FIREBALL[5], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\5.png"), 20, 20);
	loadimage(&FIREBALL[6], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\6.png"), 20, 20);
	loadimage(&FIREBALL[7], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\7.png"), 20, 20);
	loadimage(&FIREBALL[8], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\8.png"), 20, 20);
	loadimage(&FIREBALL[9], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\9.png"), 20, 20);
	loadimage(&FIREBALL[10], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\10.png"), 20, 20);
	loadimage(&FIREBALL[11], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\11.png"), 20, 20);
	loadimage(&FIREBALL[12], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\12.png"), 20, 20);
	loadimage(&FIREBALL[13], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\13.png"), 20, 20);
	loadimage(&FIREBALL[14], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\14.png"), 20, 20);
	loadimage(&FIREBALL[15], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\15.png"), 20, 20);
	loadimage(&FIREBALL[16], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\16.png"), 20, 20);
	loadimage(&FIREBALL[17], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\17.png"), 20, 20);
	loadimage(&FIREBALL[18], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\18.png"), 20, 20);
	loadimage(&FIREBALL[19], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\19.png"), 20, 20);
	loadimage(&FIREBALL[20], _T("E:\\蓝桥云课\\像素游戏素材\\岩浆龙\\火球\\20.png"), 20, 20);

}

// 透明贴图函数：把指定颜色当透明色，不绘制该颜色的像素
void PutImageTransparent(IMAGE* img, int x, int y, COLORREF transparentColor)
{
	int imgW = img->getwidth();
	int imgH = img->getheight();

	DWORD* pImgBuf = GetImageBuffer(img);   // 图片像素缓冲
	DWORD* pWinBuf = GetImageBuffer();       // 窗口像素缓冲

	int winW = getwidth();
	int winH = getheight();

	// 把透明色转换成 BGR 格式（EasyX内部是BGR）
	DWORD transColor = ((transparentColor & 0xFF) << 16)
		| (transparentColor & 0xFF00)
		| ((transparentColor >> 16) & 0xFF);

	for (int row = 0; row < imgH; row++)
	{
		for (int col = 0; col < imgW; col++)
		{
			int winX = x + col;
			int winY = y + row;

			// 超出窗口范围跳过
			if (winX < 0 || winX >= winW || winY < 0 || winY >= winH)
				continue;

			DWORD pixel = pImgBuf[row * imgW + col];

			// 是透明色就跳过不画
			if (pixel == transColor)
				continue;

			pWinBuf[winY * winW + winX] = pixel;
		}
	}
}

void Music()			//存音乐
{
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\胜利.mp3 alias 胜利", NULL, 0, NULL);
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\失败.mp3 alias 失败", NULL, 0, NULL);
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\剑攻击.mp3 alias 剑攻击", NULL, 0, NULL);
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\喷火球.mp3 alias 喷火球", NULL, 0, NULL);
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\龙吟.mp3 alias 龙吟", NULL, 0, NULL);
	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\火球爆炸.mp3 alias 火球爆炸", NULL, 0, NULL);

	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\战斗中背景音乐.mp3 alias 战斗中背景音乐", NULL, 0, NULL);

	mciSendString(L"open E:\\蓝桥云课\\像素游戏素材\\主菜单\\开始界面背景音乐.mp3 alias 开始界面背景音乐", NULL, 0, NULL);
	mciSendString(L"play 开始界面背景音乐 repeat", NULL, 0, NULL);
	mciSendString(L"setaudio 开始界面背景音乐 volume to 500", NULL, 0, NULL);
}

//------------火球队列操作函数---------------
void InitQueue(FireBallQueue* q)	//初始化队列
{
	q->front = 0;
	q->rear = 0;
	q->count = 0;
}

int QueueEmpty(FireBallQueue* q)	//判空：1=空 0=非空
{
	return q->count == 0;
}

int QueueFull(FireBallQueue*q)		//判满：1=满 0=未满
{
	return q->count == MAX_FIREBALL;
}

void Enqueue(FireBallQueue* q, int bx, int by,float bdx,float bdy)	//入队
{
	if (QueueFull(q)) return;
	//队列入列操作
	q->data[q->rear].x = bx;
	q->data[q->rear].y = by;
	q->data[q->rear].dx = bdx;
	q->data[q->rear].dy = bdy;
	q->rear = (q->rear + 1) % MAX_FIREBALL;
	q->count++;
}

FireBallPlace Dequeue(FireBallQueue* q)		//出队，调用前必须确保非空
{
	FireBallPlace b = q->data[q->front];	//取出头指针指向的元素
	q->front = (q->front + 1) % MAX_FIREBALL;
	q->count--;
	return b;
}

//-------------火球游戏逻辑--------------
void InitFireBall()				//初始化火球队列
{
	InitQueue(&FireBalls);
}

void UpdateFireBall(FireBallQueue*q)		//更新+渲染火球
{
	if (magmaDragonLive == 0)
	{
		int n = q->count;					//确定当前q->count的数值，防止后续q->count数值改变影响操作
		for (int i = 0; i < n; i++)
		{
			FireBallPlace b = Dequeue(q);
			b.x += (int)(b.dx * fireBallSpeed);
			b.y += (int)(b.dy * fireBallSpeed);
			if (b.y <= 750.0 && b.y >= -20 && b.x >= -20 && b.x <= 1020)
			{
				PutImageTransparent(&FIREBALL[frameFireBall], b.x, b.y, BLACK);
				if (frameCounter % animInterval == 0)
				{
					frameFireBall++;
					if (frameFireBall > 20)
					{
						frameFireBall = 1;
					}
				}
				Enqueue(q, b.x, b.y, b.dx, b.dy);
			}
		}
	}
}

//-----------------------------------------

void InitPlayerPlace()	//初始化角色位置
{
	Player.x = 300;
	Player.y = 600;
}

void InitSkeletonDragonPlace() //初始化骷髅龙位置		
{
	SkeletonDragon.x = 750;
	SkeletonDragon.y = 430;
}

void InitMagmaDragonPlace()		//初始化岩浆龙位置
{
	MagmaDragon.x = 0;
	MagmaDragon.y = 140;
}

void PlayerCollide()	//角色碰撞
{
	if (Player.x <= 290 && Player.y == 600) Player.x = 290, Player.y = 600;					//左半区
	if (Player.x <= 250 && Player.y == 575) Player.x = 250, Player.y = 575;
	if (Player.x <= 360 && Player.y == 630) Player.x = 360, Player.y = 630;
	if (Player.x <= 150 && Player.y == 535) Player.x = 150, Player.y = 535;
	if (Player.x <= 115 && Player.y == 505) Player.x = 115, Player.y = 505;
	if (Player.x <= 75 && Player.y == 450) Player.x = 75, Player.y = 450;
	if (Player.x <= 45) Player.x = 45;

	if (Player.x >= 600 && Player.y == 620)Player.x = 600, Player.y = 620;					//右半区
	if (Player.x >= 670 && Player.y == 595) Player.x = 670, Player.y = 595;
	if (Player.x >= 710 && Player.y == 570) Player.x = 710, Player.y = 570;
	if (Player.x >= 810 && Player.y == 538)Player.x = 810, Player.y = 538;
	if (Player.x >= 845 && Player.y == 505)Player.x = 845, Player.y = 505;
	if (Player.x >= 880 && Player.y == 445)Player.x = 880, Player.y = 445;
	if (Player.x >= 910) Player.x = 910;
}

bool PlayerLandDetection()	//角色落地检测
{
	if (Player.x >= 280 && Player.x <= 335 && Player.y >= 600)			//左半区
	{
		Player.y = 600; return TRUE;
	}
	else if (Player.x >= 240 && Player.x <= 280 && Player.y >= 575)
	{
		Player.y = 575; return TRUE;
	}
	else if (Player.x >= 335 && Player.x <= 380 && Player.y >= 630)
	{
		Player.y = 630; return TRUE;
	}
	else if (Player.x >= 140 && Player.x <= 240 && Player.y >= 535)
	{
		Player.y = 535; return TRUE;
	}
	else if (Player.x >= 105 && Player.x <= 130 && Player.y >= 505)
	{
		Player.y = 505; return TRUE;
	}
	else if (Player.x >= 65 && Player.x <= 100 && Player.y >= 450)
	{
		Player.y = 450; return TRUE;
	}
	else if (Player.x >= 35 && Player.x <= 65 && Player.y >= 355)
	{
		Player.y = 355; return TRUE;
	}

	else if (Player.x >= 420 && Player.x <= 480 && Player.y >= 650)		//中间
	{
		Player.y = 650; return TRUE;
	}

	else if (Player.x >= 530 && Player.x <= 615 && Player.y >= 620)		//右半区
	{
		Player.y = 620; return TRUE;
	}
	else if (Player.x >= 615 && Player.x <= 680 && Player.y >= 595)
	{
		Player.y = 595; return TRUE;
	}
	else if (Player.x >= 685 && Player.x <= 725 && Player.y >= 570)
	{
		Player.y = 570; return TRUE;
	}
	else if (Player.x >= 725 && Player.x <= 820 && Player.y >= 538)
	{
		Player.y = 538; return TRUE;
	}
	else if (Player.x >= 825 && Player.x <= 865 && Player.y >= 505)
	{
		Player.y = 505; return TRUE;
	}
	else if (Player.x >= 865 && Player.x <= 895 && Player.y >= 445)
	{
		Player.y = 445; return TRUE;
	}
	else if (Player.x >= 900 && Player.x <= 930 && Player.y >= 355)
	{
		Player.y = 355; return TRUE;
	}
	else return FALSE;

}

void MagmaDragonStandBy()		//岩浆龙待机
{
	if (magmaDragonLive == 0)
	{
		PutImageTransparent(&MAGMADRAGON[frameMagmaDragon], MagmaDragon.x, MagmaDragon.y, BLACK);
		if (frameCounter % animInterval == 0)
		{
			frameMagmaDragon++;
			if (frameMagmaDragon > 15) frameMagmaDragon = 1;
		}
	}
}

//------------骷髅龙操作函数----------------
void SkeletonDragonStandBy()	 //骷髅龙待机
{
	frameCounter2++;
	if (skeletonDragonLive == 0)
	{
		if (Player.x - SkeletonDragon.x <= 0 && isSkeletonDragonAttack == 0)		//左
		{
			PutImageTransparent(&SKELETONDRAGONRUNL[frameSkeletonDragonRunLeft], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonRunLeft++;
				SkeletonDragon.x -= skeletonDragonRunSpeed;
				if (frameSkeletonDragonRunLeft > 15) frameSkeletonDragonRunLeft = 1;
				if (SkeletonDragon.x - 30 <= Player.x)
				{
					skeletonDragonRunSpeed = 0;
				}
				else
				{
					skeletonDragonRunSpeed = 10;
				}
			}
		}
		else if (Player.x - SkeletonDragon.x > 0 && isSkeletonDragonAttack == 0)	//右
		{
			PutImageTransparent(&SKELETONDRAGONRUNR[frameSkeletonDragonRunRight], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonRunRight++;
				SkeletonDragon.x += skeletonDragonRunSpeed;
				if (frameSkeletonDragonRunRight > 15) frameSkeletonDragonRunRight = 1;
				if (SkeletonDragon.x + 70 >= Player.x)
				{
					skeletonDragonRunSpeed = 0;
				}
				else
				{
					skeletonDragonRunSpeed = 10;
				}
			}
		}
	}
}

void SkeletonDragonAttack()			//骷髅龙攻击
{
	if (skeletonDragonLive == 0)
	{
		DWORD now = GetTickCount();
		if (now - last_attack_time >= ATTACK_COOLDOWN)
		{
			isSkeletonDragonAttack = 1;
			last_attack_time = now;
		}
		if (SkeletonDragon.x - Player.x >= 0 && isSkeletonDragonAttack == 1 && skeletonDragonAttackKind == 0)	//甩尾左
		{
			PutImageTransparent(&SKELETONDRAGONATTACKL[frameSkeletonDragonTaliAttack], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (!skeletonDragonMusic)
			{
				mciSendString(L"stop 龙吟", NULL, 0, NULL);
				mciSendString(L"play 龙吟 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 龙吟 volume to 500", NULL, 0, NULL);
				skeletonDragonMusic = TRUE;
			}
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonTaliAttack++;
				if (frameSkeletonDragonTaliAttack > 30)
				{
					isSkeletonDragonAttack = 0;
					skeletonDragonAttackKind = 1;
					frameSkeletonDragonTaliAttack = 1;
					skeletonDragonMusic = FALSE;
				}
				if (Player.x + 60 >= SkeletonDragon.x && Player.x + 50 <= SkeletonDragon.x + 110 && Player.y + 30 >= SkeletonDragon.y && Player.x + 30 <= SkeletonDragon.y + 200)
				{
					playerHPx -= skeletonDragonTailHart;
				}
			}
		}
		if (SkeletonDragon.x - Player.x < 0 && isSkeletonDragonAttack == 1 && skeletonDragonAttackKind == 0)		//甩尾右
		{
			PutImageTransparent(&SKELETONDRAGONATTACKR[frameSkeletonDragonTaliAttack], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (!skeletonDragonMusic)
			{
				mciSendString(L"stop 龙吟", NULL, 0, NULL);
				mciSendString(L"play 龙吟 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 龙吟 volume to 500", NULL, 0, NULL);
				skeletonDragonMusic = TRUE;
			}
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonTaliAttack++;
				if (frameSkeletonDragonTaliAttack > 30)
				{
					isSkeletonDragonAttack = 0;
					skeletonDragonAttackKind = 1;
					frameSkeletonDragonTaliAttack = 1;
					skeletonDragonMusic = FALSE;
				}
				if (Player.x < SkeletonDragon.x + 110 && Player.x >= SkeletonDragon.x && Player.y + 30 >= SkeletonDragon.y && Player.y + 30 <= SkeletonDragon.y + 200)
				{
					playerHPx -= skeletonDragonTailHart;
				}
			}
		}
		if (SkeletonDragon.x - Player.x >= 0 && isSkeletonDragonAttack == 1 && skeletonDragonAttackKind == 1)	//利爪左
		{
			PutImageTransparent(&SKELETONDRAGONATTACKPAWL[frameSkeletonDragonPawAttack], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (!skeletonDragonMusic)
			{
				mciSendString(L"stop 龙吟", NULL, 0, NULL);
				mciSendString(L"play 龙吟 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 龙吟 volume to 500", NULL, 0, NULL);
				skeletonDragonMusic = TRUE;
			}
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonPawAttack++;
				if (frameSkeletonDragonPawAttack > 15)
				{
					isSkeletonDragonAttack = 0;
					skeletonDragonAttackKind = 0;
					frameSkeletonDragonPawAttack = 1;
					skeletonDragonMusic = FALSE;
					if (Player.x + 60 >= SkeletonDragon.x && Player.x + 50 <= SkeletonDragon.x + 110 && Player.y + 30 >= SkeletonDragon.y && Player.x + 30 <= SkeletonDragon.y + 200)
					{
						playerHPx -= skeletonDragonPawHart;
					}
				}
			}
		}
		if (SkeletonDragon.x - Player.x < 0 && isSkeletonDragonAttack == 1 && skeletonDragonAttackKind == 1)		//利爪右
		{
			PutImageTransparent(&SKELETONDRAGONATTACKPAWR[frameSkeletonDragonPawAttack], SkeletonDragon.x, SkeletonDragon.y, BLACK);
			if (!skeletonDragonMusic)
			{
				mciSendString(L"stop 龙吟", NULL, 0, NULL);
				mciSendString(L"play 龙吟 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 龙吟 volume to 500", NULL, 0, NULL);
				skeletonDragonMusic = TRUE;
			}
			if (frameCounter2 % animInterval == 0)
			{
				frameSkeletonDragonPawAttack++;
				if (frameSkeletonDragonPawAttack > 15)
				{
					isSkeletonDragonAttack = 0;
					skeletonDragonAttackKind = 0;
					frameSkeletonDragonPawAttack = 1;
					skeletonDragonMusic = FALSE;
					if (Player.x < SkeletonDragon.x + 110 && Player.x >= SkeletonDragon.x && Player.y + 30 >= SkeletonDragon.y && Player.y + 30 <= SkeletonDragon.y + 200)
					{
						playerHPx -= skeletonDragonPawHart;
					}
				}
			}
		}
	}
}
 void SkeletonDragonLandDetection()	//骷髅龙落地检测
{
	if (SkeletonDragon.x >= 280 && SkeletonDragon.x <= 335 )			//左半区
	{
		SkeletonDragon.y = 480; 
	}
	else if (SkeletonDragon.x >= 240 && SkeletonDragon.x <= 280 )
	{
		SkeletonDragon.y = 455; 
	}
	else if (SkeletonDragon.x >= 335 && SkeletonDragon.x <= 380 )
	{
		SkeletonDragon.y = 510; 
	}
	else if (SkeletonDragon.x >= 140 && SkeletonDragon.x <= 240 )
	{
		SkeletonDragon.y = 415; 
	}
	else if (SkeletonDragon.x >= 105 && SkeletonDragon.x <= 130 )
	{
		SkeletonDragon.y = 385; 
	}
	else if (SkeletonDragon.x >= 65 && SkeletonDragon.x <= 100 )
	{
		SkeletonDragon.y = 330; 
	}
	else if (SkeletonDragon.x >= 35 && SkeletonDragon.x <= 65 )
	{
		SkeletonDragon.y = 235; 
	}

	else if (SkeletonDragon.x >= 420 && SkeletonDragon.x <= 480 )		//中间
	{
		SkeletonDragon.y = 530; 
	}

	else if (SkeletonDragon.x >= 530 && SkeletonDragon.x <= 615 )		//右半区
	{
		SkeletonDragon.y = 500; 
	}
	else if (SkeletonDragon.x >= 615 && SkeletonDragon.x <= 680)
	{
		SkeletonDragon.y = 475; 
	}
	else if (SkeletonDragon.x >= 685 && SkeletonDragon.x <= 725 )
	{
		SkeletonDragon.y = 450; 
	}
	else if (SkeletonDragon.x >= 725 && SkeletonDragon.x <= 820 )
	{
		SkeletonDragon.y = 418; 
	}
	else if (SkeletonDragon.x >= 825 && SkeletonDragon.x <= 860  )
	{
		SkeletonDragon.y = 385;
	}
	else if (SkeletonDragon.x >= 865 && SkeletonDragon.x <= 895 )
	{
		SkeletonDragon.y = 325; 
	}
	else if (SkeletonDragon.x >= 900 && SkeletonDragon.x <= 930 )
	{
		SkeletonDragon.y = 235;
	}
}

//------------------------------------------

void FireBall()					//火球
{
	if (magmaDragonLive == 0)
	{
		DWORD now = GetTickCount();		//获取当前时间(毫秒)
		if (now - last_fire_time >= FIRE_COOLDOWN)
		{
			magmaDragonMusic = FALSE;
			//计算从火球指向角色的方向
			float dir_x = (float)((Player.x + 10) - (MagmaDragon.x + 50));
			float dir_y = (float)((Player.y + 20) - (MagmaDragon.y + 50));

			//归一化
			float len = sqrt(dir_x * dir_x + dir_y * dir_y);
			float dx = 0, dy = 0;
			if (len > 0)
			{
				dx = dir_x / len;
				dy = dir_y / len;
			}
			Enqueue(&FireBalls, MagmaDragon.x + 50, MagmaDragon.y + 70, dx, dy);
			last_fire_time = now;
			if (!magmaDragonMusic)
			{
				mciSendString(L"stop 喷火球", NULL, 0, NULL);
				mciSendString(L"play 喷火球 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 喷火球 volume to 500", NULL, 0, NULL);
				magmaDragonMusic = TRUE;
			}
		}
		UpdateFireBall(&FireBalls);
	}
}


void BounceFireBall(FireBallQueue* q)		//反弹火球
{
	if (magmaDragonLive == 0)
	{
		int n = q->count;
		for (int i = 0; i < n; i++)
		{
			FireBallPlace b = Dequeue(q);
			bool attacking = (isAttack == 1) && ((direction == 0 && frameAttackLeft >= 5) && frameAttackLeft <= 13) ||
				((direction == 1 && frameAttackLeft >= 5) && frameAttackLeft <= 13);
			bool hit = (b.x < Player.x + 50) && (b.x + 20 > Player.x) && (b.y < Player.y + 80) && (b.y + 20 > Player.y);

			if (attacking && hit)
			{
				//计算从火球指向角色的方向
				float dir_x = (float)((MagmaDragon.x + 50) - Player.x);
				float dir_y = (float)((MagmaDragon.y + 50) - Player.y);

				//归一化
				float len = sqrt(dir_x * dir_x + dir_y * dir_y);
				float dx = 0, dy = 0;
				if (len > 0)
				{
					dx = dir_x / len;
					dy = dir_y / len;
				}
				Enqueue(&FireBalls, Player.x + dx * 60, Player.y + 50 + dy * 60, dx, dy);

			}
			else if (hit && !attacking)
			{


				mciSendString(L"stop 火球爆炸", NULL, 0, NULL);
				mciSendString(L"play 火球爆炸 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 火球爆炸 volume to 500", NULL, 0, NULL);
				fireBallMusic = TRUE;

				playerHPx -= fireBallHert;
			}
			else
			{
				float toDragonX = (MagmaDragon.x + 50) - (b.x + 10);
				float toDragonY = (MagmaDragon.y + 100) - (b.y + 10);
				bool movingTowardDragon = (b.dx * toDragonX + b.dy * toDragonY) > 0;

				bool hitDragon = movingTowardDragon && (b.x < MagmaDragon.x + 100) && (b.x + 20 > MagmaDragon.x) &&
					(b.y < MagmaDragon.y + 200) && (b.y + 20 > MagmaDragon.y);
				if (hitDragon)
				{
					mciSendString(L"stop 火球爆炸", NULL, 0, NULL);
					mciSendString(L"play 火球爆炸 from 0", NULL, 0, NULL);
					mciSendString(L"setaudio 火球爆炸 volume to 500", NULL, 0, NULL);
					magmaDragonHPx -= bounceFireBallHert;
				}
				else
				{
					Enqueue(q, b.x, b.y, b.dx, b.dy);
				}

			}
		}
	}
}


void MovePlayer()	//移动角色
{
	if (playerLive == 0)
	{
		bool RIGHT = GetAsyncKeyState(VK_RIGHT) & 0x8000;
		bool LEFT = GetAsyncKeyState(VK_LEFT) & 0x8000;
		bool UP = GetAsyncKeyState(VK_UP) & 0x8000;

		frameCounter++;

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && isJump == 0 && isAttack == 0)		//角色向右跑
		{
			direction = 1;
			Player.x += playerRunSpeed;
			PutImageTransparent(&PLAYERRUNR[frameRunRight], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameRunRight++;
				if (frameRunRight > 30) frameRunRight = 1;
			}
		}
		else if (direction == 1 && !LEFT && isJump == 0 && isAttack == 0)					//角色向右待机
		{
			PutImageTransparent(&PLAYERR[frameStand], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameStand++;
				if (frameStand > 30) frameStand = 1;
			}
		}

		else if (GetAsyncKeyState(VK_LEFT) & 0x8000 && isJump == 0 && isAttack == 0)			//角色向左跑
		{
			direction = 0;
			Player.x -= playerRunSpeed;
			PutImageTransparent(&PLAYERRUNL[frameRunLeft], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameRunLeft++;
				if (frameRunLeft > 30) frameRunLeft = 1;
			}
		}
		else if (direction == 0 && !RIGHT && isJump == 0 && isAttack == 0)					//角色向左待机
		{
			PutImageTransparent(&PLAYERL[frameStand], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameStand++;
				if (frameStand > 30) frameStand = 1;
			}
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000 && isJump == 0 && isAttack == 0)			//角色在地面跳
		{
			isJump = 1;
			playerVy = -30;

		}
		if (GetAsyncKeyState(VK_UP) & 0x8000 && isJump == 1 && isAttack == 0)
		{
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				Player.x += playerRunSpeed;
			}

			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				Player.x -= playerRunSpeed;
			}
		}
		if (isJump == 1 && direction == 1 && isAttack == 0)								//朝向右跳
		{
			PutImageTransparent(&PLAYERJUMPR[frameJump], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameJump++;
				if (frameJump > 20) frameJump = 1;
			}
		}
		if (isJump == 1 && direction == 0 && isAttack == 0)							//朝向左跳
		{
			PutImageTransparent(&PLAYERJUMPL[frameJump], Player.x, Player.y, BLACK);
			if (frameCounter % animInterval == 0)
			{
				frameJump++;
				if (frameJump > 20) frameJump = 1;
			}
		}
		Player.y += playerVy;
		playerVy += gravity;
		if (PlayerLandDetection())			//落地检测
		{
			playerVy = 0;
			isJump = 0;
		}

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			isAttack = 1;
		}

		if (isAttack == 1 && direction == 1)		//角色攻击右
		{
			PutImageTransparent(&PLAYERATTACKR[frameAttackRight], Player.x, Player.y, BLACK);
			if (!playerMusic)
			{
				mciSendString(L"stop 剑攻击", NULL, 0, NULL);
				mciSendString(L"play 剑攻击 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 剑攻击 volume to 500", NULL, 0, NULL);
				playerMusic = TRUE;
			}
			frameAttackRight++;
			if (frameAttackRight > 15)
			{
				frameAttackRight = 1;
				isAttack = 0;
				playerMusic = FALSE;
				if (Player.x + 50 >= SkeletonDragon.x && Player.x + 50 <= SkeletonDragon.x + 100 && Player.y + 30 >= SkeletonDragon.y && Player.y + 30 <= SkeletonDragon.y + 200)
				{
					skeletonDragonHPx -= playerHert/2;
				}
			}

		}

		if (isAttack == 1 && direction == 0)		//角色攻击左
		{
			PutImageTransparent(&PLAYERATTACKL[frameAttackLeft], Player.x, Player.y, BLACK);
			if (!playerMusic)
			{
				mciSendString(L"stop 剑攻击", NULL, 0, NULL);
				mciSendString(L"play 剑攻击 from 0", NULL, 0, NULL);
				mciSendString(L"setaudio 剑攻击 volume to 500", NULL, 0, NULL);
				playerMusic = TRUE;
			}
			frameAttackLeft++;
			if (frameAttackLeft > 15)
			{
				frameAttackLeft = 1;
				isAttack = 0;
				playerMusic = FALSE;
				if (Player.x < SkeletonDragon.x + 100 && Player.x >= SkeletonDragon.x && Player.y + 30 >= SkeletonDragon.y && Player.y + 30 <= SkeletonDragon.y + 200)
				{
					skeletonDragonHPx -= playerHert/2;
				}
			}
		}

		if (Player.y >= 750)
		{
			playerHPx = 0;
			playerLive = 1;
		}
		Sleep(30);
	}
}

void MainMenu3()		//加载动画
{
	cleardevice();
	putimage(0,0,&MAINMENU3);
	setfillcolor(GREEN);
	while (1)
	{
		fillrectangle(300, 350, mainMenu3X, 400);
		mainMenu3X += 10;
		if (mainMenu3X >= 710)
		{
			return;
		}
		Sleep(25);
	}

}

void MainMenu()			//主菜单
{
	cleardevice();
	putimage(0, 0, &MAINMENU);
	setlinecolor(WHITE);
	rectangle(400, 200, 600, 260);
	rectangle(400, 280, 600, 340);
	settextstyle(24, 24, _T("楷体"));
	TCHAR k[] = _T("开始游戏");
	outtextxy(405, 215, k);
	TCHAR c[] = _T("操作介绍");
	outtextxy(405, 295, c);
	settextstyle(15, 15, _T("楷体"));
	TCHAR z[] = _T("开发人员：定了芝麻糊");
	outtextxy(10, 10, z);
	MOUSEMSG mMsg;
	while (1)
	{
		mMsg = GetMouseMsg();
		if (mMsg.uMsg == WM_LBUTTONDOWN)
		{
			if (mMsg.x > 400 && mMsg.x < 600 && mMsg.y>200 && mMsg.y < 260)
			{
				MainMenu3();
				mciSendString(L"stop 开始界面背景音乐 ", NULL, 0, NULL);
				mciSendString(L"close 开始界面背景音乐 ", NULL, 0, NULL);
				mciSendString(L"play 战斗中背景音乐 repeat", NULL, 0, NULL);
				mciSendString(L"setaudio 战斗中背景音乐 volume to 500", NULL, 0, NULL);
				return;
			}
			if (mMsg.x > 400 && mMsg.x < 600 && mMsg.y>280 && mMsg.y < 340)
			{
				cleardevice();
				putimage(0, 0, &MAINMENU2);
				setlinecolor(WHITE);
				rectangle(50, 50, 150, 80);
				rectangle(400, 200, 600, 260);
				rectangle(400, 280, 600, 340);
				rectangle(400, 360, 600, 420);
				rectangle(400, 440, 600, 500);
				settextstyle(24, 24, _T("楷体"));
				TCHAR s[] = _T("返回");
				TCHAR up[] = _T("↑:跳跃");
				TCHAR left[] = _T("←:左移动");
				TCHAR right[] = _T("→:右移动");
				TCHAR space[] = _T("空格:攻击");
				outtextxy(52, 52, s);
				settextstyle(40, 20, _T("楷体"));
				outtextxy(405, 205, up);
				outtextxy(405, 285, left);
				outtextxy(405, 365, right);
				outtextxy(405, 445, space);
				while (1)
				{
					mMsg = GetMouseMsg();
					if (mMsg.uMsg == WM_LBUTTONDOWN)
					{
						if (mMsg.x > 50 && mMsg.x < 150 && mMsg.y>50 && mMsg.y < 80)
						{
							MainMenu();
							return;
						}
					}
				}
			}

		}
	}
}

void HP()				//NPC生命值
{
	PutImageTransparent(&PLAYERAVATAR, 10, 10, BLACK);
	PutImageTransparent(&SKELETONDRAGONAVATAR, 350, 10, BLACK);
	PutImageTransparent(&MAGMADRAGONAVATAR, 350, 70, BLACK);
	setfillcolor(GREEN);
	fillrectangle(10+50, 10+15, 10+50+playerHPx, 30+15);
	setfillcolor(RED);
	fillrectangle(350+50, 10+15, 350+50+skeletonDragonHPx, 30+15);
	fillrectangle(350+50, 70+15, 350+50+magmaDragonHPx, 90+15);
	if (playerHPx <= 0)
	{
		playerHPx = 0;
		playerLive = 1;
	}
	if (magmaDragonHPx <= 0)
	{
		magmaDragonHPx = 0;
		magmaDragonLive = 1;
	}
	if (skeletonDragonHPx <= 0)
	{
		skeletonDragonHPx = 0;
		skeletonDragonLive = 1;
	}
}

int GameOver()			//游戏结束
{
	if (playerLive == 1)
	{
		cleardevice();
		mciSendString(L"stop 战斗中背景音乐 ", NULL, 0, NULL);
		mciSendString(L"close 战斗中背景音乐 ", NULL, 0, NULL);
		mciSendString(L"stop 胜利 ", NULL, 0, NULL);
		mciSendString(L"close 胜利 ", NULL, 0, NULL);

		mciSendString(L"play 失败 repeat", NULL, 0, NULL);
		mciSendString(L"setaudio 失败 volume to 500", NULL, 0, NULL);
		PutImageTransparent(&CheckoutScreen[1], 0, 0, BLACK);
		TCHAR d[] = _T("点击屏幕退出游戏");
		outtextxy(400, 600, d);
		FlushBatchDraw();
		return 1;
	}
	if (skeletonDragonLive == 1 && magmaDragonLive == 1)
	{
		cleardevice();
		mciSendString(L"stop 战斗中背景音乐 ", NULL, 0, NULL);
		mciSendString(L"close 战斗中背景音乐 ", NULL, 0, NULL);
		mciSendString(L"stop 失败 ", NULL, 0, NULL);
		mciSendString(L"close 失败 ", NULL, 0, NULL);

		mciSendString(L"play 胜利 repeat", NULL, 0, NULL);
		mciSendString(L"setaudio 胜利 volume to 500", NULL, 0, NULL);
		PutImageTransparent(&CheckoutScreen[2], 0, 0, BLACK);
		TCHAR d[] = _T("点击屏幕退出游戏");
		outtextxy(400, 600, d);
		FlushBatchDraw();
		return 1;
	}
	return 0;
}

void PlayGame()			//游戏
{
	while (1)
	{
		BeginBatchDraw();				
		cleardevice();					//清屏
		putimage(0, 0, &MAP);			//画地图
		HP();							//画血条
		MagmaDragonStandBy();			//岩浆龙待机
		SkeletonDragonStandBy();		//骷髅龙待机
		SkeletonDragonAttack();			//骷髅龙攻击
		SkeletonDragonLandDetection();	//骷髅龙落地检测
		MovePlayer();					//角色移动
		FireBall();						//火球函数
		BounceFireBall(&FireBalls);		//反弹火球函数
		if (GameOver())					//检测是否游戏结束
		{
			break;

			FlushBatchDraw();
		}
		PlayerCollide();				//角色碰撞
		
	}

	MOUSEMSG mMsg;
	while (1)
	{
		mMsg = GetMouseMsg();
		if (mMsg.uMsg == WM_LBUTTONDOWN)
		{
			if (mMsg.x >= 0 && mMsg.x <=1000 && mMsg.y>=0 && mMsg.y <= 750)
			{
				return;
			}
		}
	}
}
int main()							//主函数入口
{
	initgraph(1000, 750);			//创建窗口
	Music();						//存音乐
	InitPlayerPlace();				//初始化角色位置
	InitSkeletonDragonPlace();		//初始化骷髅龙位置
	InitMagmaDragonPlace();			//初始化岩浆龙位置
	InitFireBall();					//初始化火球
	SaveTexture();					//存贴图
	MainMenu();						//主菜单界面
	PlayGame();						//开始游戏

	return 0;
}
