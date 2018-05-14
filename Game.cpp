// Model-View matrix and Projection matrix
// Perspective projection with fixed camera position
// Rotate the grid and the cube around the Y axis
#include <time.h>
#include "header/Angel.h"
#include "Common/CCamera.h"
#include "Common/CObjReader.h"
#include "Common/CBGStars.h"
#include "Common/CPlayer.h"
#include "Common/CFirstBoss.h"
#include "Common/CSecondBoss.h"
#include "Common/CThirdBoss.h"

#define SPACE_KEY 32
#define SCREEN_SIZE_X 500
#define SCREEN_SIZE_Y 700
#define HALF_SIZE_X SCREEN_SIZE_X /2
#define HALF_SIZE_Y SCREEN_SIZE_Y /2 
#define VP_HALFWIDTH  20.0f
#define VP_HALFHEIGHT 20.0f
#define NEXT_BULLET_DELAY 0.45f  //0.45f
#define NEXT_BULLET_DELAY_BOSS3 1.3f
#define DEFENSE_TIME 5.0f
#define REOPEN_DEFENSE_TIME 10.0f

int g_iLevel = 3;	//第幾個魔王

// For Rotation
GLfloat g_fYAngle = 0;  // Z軸的旋轉角度
GLfloat g_fDir = 1;     // 旋轉方向
bool    m_bAutoMove = false; // Controlled by Space Key

// Objects
CBGStars *g_pStars;
CPlayer *g_pPlayer;
CFirstBoss *g_pFirstBoss;
CSecondBoss *g_pSecondBoss;
CThirdBoss *g_pThirdBoss;

// Player Attack
bool g_bStoringAttack;		//蓄力中
int g_iAttackStarNum;		//蓄力星星數
float g_fAttackCount;		//蓄力計時
bool g_bMissileShoot;		//是否要發射導彈

// Player Defense
bool g_bCanOpenDefense;	//是否可開啟防護罩
bool g_bisOpenDefense;	//防護罩是否開啟
float g_fDefenseCount;	//防護罩計時

// Counting
float g_fcount = 0;
float g_fcount_boss1 = 0;
float g_fcount_boss3 = 0;

// Delay Time
float g_NextBulletDelay_Boss3 = 1.3f;

// PassiveMotion
float g_fPTx;		//玩家移動x軸
mat4 g_mxPT;		//玩家座標(g_fPTx, PLAYER_Y_AXIS, 0, 1)
mat4 g_mxPS;		//戰鬥機大小

//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();
void CollisionDetect();		//碰撞偵測

void init(void)
{
	mat4 mxT;
	//vec4 vT;

	// 產生所需之 Model View 與 Projection Matrix
	// LookAt 的三個參數，使用球體座標
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  eye(0.0f, 0.0f, 50.0f, 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
	camera->updateOrthographic(-5.0f, 5.0f, -7.0f, 7.0f, 1.0f, 100.0f);
	//--------------------------------------------------------------------

	g_pStars = new CBGStars;					//背景
	g_pPlayer = new CPlayer;					//玩家
	g_pFirstBoss = new CFirstBoss;				//BOSS1
	g_pSecondBoss = new CSecondBoss;			//BOSS2
	g_pThirdBoss = new CThirdBoss;				//BOSS3

	//-------------------------------------------------------------------
	//ATTACK
	g_bStoringAttack = false;
	g_iAttackStarNum = 0;	//攻擊蓄力星星
	g_fAttackCount = 0;
	g_bMissileShoot = false;

	//DEFENSE
	g_bCanOpenDefense = true;
	g_bisOpenDefense = false;
	g_fDefenseCount = 0;
}

void CollisionDetect()		//碰撞偵測
{
	mat4 mxPlayerPos, mxPBulletPos, mxBossPos, mxBBulletPos;
	float fPlayer_x, fPlayer_y;			//player position
	float fPBullet_x, fPBullet_y;		//player bullet position
	float fBoss_x, fBoss_y;				//boss position
	float fBBullet_x, fBBullet_y;		//boss bullet position

	if (g_pPlayer) {		//玩家存在
		mxPlayerPos = g_pPlayer->GetTranslateMatrix();			//取得玩家位置
		fPlayer_x = mxPlayerPos._m[0][3];
		fPlayer_y = mxPlayerPos._m[1][3];
		mxPBulletPos = g_pPlayer->GetBulletTranslateMatrix();	//取得玩家子彈位置
		fPBullet_x = mxPBulletPos._m[0][3];
		fPBullet_y = mxPBulletPos._m[1][3];
	}

	if (g_iLevel == 1) {		//BOSS1
		if (g_pFirstBoss) {		//BOSS1存在
			bool isFirstBullet = g_pFirstBoss->GetFirstShoot();			//是否為第一發子彈
			mxBossPos = g_pFirstBoss->GetTranslateMatrix();				//取得BOSS位置
			fBoss_x = mxBossPos._m[0][3];
			fBoss_y = mxBossPos._m[1][3];
			mxBBulletPos = g_pFirstBoss->GetBulletTranslateMatrix();	//取得BOSS子彈位置
			fBBullet_x = mxBBulletPos._m[0][3];
			fBBullet_y = mxBBulletPos._m[1][3];
			
			if (isFirstBullet) {	//第一發子彈
				if (fBBullet_y - 0.15f < PLAYER_Y_AXIS + 1.75f && 
					fBBullet_x - 0.5f < g_fPTx + 2.f && 
					fBBullet_x + 0.5f > g_fPTx - 2.f) {					//BOSS 大子彈 碰撞玩家
					if (!g_bisOpenDefense) {							//防護罩未開啟
						//printf("COLLISION!\n");
					}
				}
			} 
		
			if (fPBullet_y > fBoss_y - 0.61f && fPBullet_x < fBoss_x + 1.f && fPBullet_x > fBoss_x - 1.f) {	//玩家子彈碰撞BOSS1
				//printf("COLLISION!\n");
			}
		}
	} //--------Level1

	else if (g_iLevel == 3) {		//BOSS3
		if (g_pThirdBoss) {			//BOSS3存在
			mxBossPos = g_pThirdBoss->GetTranslateMatrix();				//取得BOSS位置
			fBoss_x = mxBossPos._m[0][3];
			fBoss_y = mxBossPos._m[1][3];
			mxBBulletPos = g_pThirdBoss->GetBulletTranslateMatrix();	//取得BOSS子彈位置
			fBBullet_x = mxBBulletPos._m[0][3];
			fBBullet_y = mxBBulletPos._m[1][3];
			
			mat4 mxLEPos[LITTLE_NUM], mxLBulletPos[LITTLE_NUM];					//小怪位置 & 小怪子彈位置
			float fLE_x[LITTLE_NUM], fLE_y[LITTLE_NUM];
			float fLBullet_x[LITTLE_NUM], fLBullet_y[LITTLE_NUM];
			for (int i = 0; i < LITTLE_NUM; i++) {
				mxLEPos[i] = g_pThirdBoss->GetLETranslateMatrix(i);				//取得小怪位置
				fLE_x[i] = mxLEPos[i]._m[0][3];
				fLE_y[i] = mxLEPos[i]._m[1][3];
				mxLBulletPos[i] = g_pThirdBoss->GetLEBulletTranslateMatrix(i);	//取得小怪 子彈位置
				fLBullet_x[i] = mxLBulletPos[i]._m[0][3];
				fLBullet_y[i] = mxLBulletPos[i]._m[1][3];
			}
			//printf("%f, %f\n", fBBullet_x, fBBullet_y);
			if (fPBullet_y > fBoss_y - 1.296f && fPBullet_x < fBoss_x + 2.269f && fPBullet_x > fBoss_x - 2.195f) {	//玩家子彈碰撞BOSS3
				//printf("COLLISION!\n");
			}
			for (int i = 0; i < LITTLE_NUM; i++) {
				if (fPBullet_y > fLE_y[i] - 0.5124f && fPBullet_x < fLE_x[i] + 0.436f && fPBullet_x > fLE_x[i] - 0.436f) {	//玩家子彈碰撞小怪
					//printf("COLLISION!\n");
				}
				if (fLBullet_y[i] < PLAYER_Y_AXIS + 1.75f && fLBullet_y[i] > PLAYER_Y_AXIS - 1.75f &&
					fLBullet_x[i] < g_fPTx + 2.f && fLBullet_x[i] > g_fPTx - 2.f) {						//小怪子彈碰撞玩家
					//printf("COLLISION!\n");
				}
			}
		}
	} //--------Level3

	  //BOSS一般子彈碰撞玩家
	if (fBBullet_y < PLAYER_Y_AXIS + 1.75f && fBBullet_y > PLAYER_Y_AXIS - 1.75f &&
		fBBullet_x < g_fPTx + 2.f && fBBullet_x > g_fPTx - 2.f) {
		if (!g_bisOpenDefense) {	//防護罩未開啟
			//printf("COLLISION!\n");
		}
		//else printf("NO COLLISION!\n");
	}

}

//----------------------------------------------------------------------------

void GL_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window 清顏色, 將深度清為1

	g_pStars->GL_Draw();								//背景
	g_pPlayer->GL_Draw();
	if (g_bisOpenDefense) g_pPlayer->GL_DrawDefense();	//防護罩顯示
	g_pPlayer->GL_DrawAttack(g_iAttackStarNum);			//攻擊蓄力顯示

	//BOSS
	if (g_iLevel == 1) g_pFirstBoss->GL_Draw();
	if (g_iLevel == 2) g_pSecondBoss->GL_Draw();
	if (g_iLevel == 3) g_pThirdBoss->GL_Draw();

	glutSwapBuffers();	// 交換 Frame Buffer
}

void onFrameMove(float delta)
{
	mat4 mvx, mpx;	// view matrix & projection matrix
	bool bVDirty, bPDirty;	// view 與 projection matrix 是否需要更新給物件
	auto camera = CCamera::getInstance();

	//----------------------------
	//玩家子彈
	g_fcount += delta;
	if (g_fcount < NEXT_BULLET_DELAY) g_pPlayer->ShootBullet(delta, g_fPTx);	//發射子彈
	else {
		if (!g_bStoringAttack) {	//非蓄力時，自動發射子彈
			g_pPlayer->NextBullet(g_fPTx);	//下一個子彈
			g_fcount -= NEXT_BULLET_DELAY;
		}
		else g_fcount = 0;			//蓄力完重新計數
	}
	//BOSS1子彈
	g_fcount_boss1 += delta;
	g_pFirstBoss->SetBulletPassiveMove();	//未發射子彈跟隨BOSS1
	if (g_fcount_boss1 < NEXT_BULLET_DELAY) g_pFirstBoss->ShootBullet(delta);	//發射子彈
	else {
		g_pFirstBoss->NextBullet();	//下一個子彈
		g_fcount_boss1 -= NEXT_BULLET_DELAY;
	}
	//BOSS3 + Little 子彈
	g_fcount_boss3 += delta;
	g_pThirdBoss->SetBulletPassiveMove();	//未發射子彈跟隨little
	if (g_fcount_boss3 < g_NextBulletDelay_Boss3) g_pThirdBoss->ShootBullet(delta);	//發射子彈
	else {
		g_pThirdBoss->NextBullet();	//下一個子彈
		g_fcount_boss3 -= g_NextBulletDelay_Boss3;
		g_NextBulletDelay_Boss3 = NEXT_BULLET_DELAY_BOSS3 + (rand() % 10)*0.1f;
	}

	//----------------------------
	//玩家蓄力攻擊
	if (g_bStoringAttack) {
		g_fAttackCount += delta;
		if (g_fAttackCount > 1.0f) {
			g_iAttackStarNum++;			//星星+1
			g_fAttackCount -= 1.0f;
		}
	}
	else {
		if (g_iAttackStarNum > 0) {				//非蓄力中、星星數 > 0
			g_bMissileShoot = true;				//導彈發射
		}
	}

	//導彈發射
	if (g_bMissileShoot) {
		mat4 mxBossPos;
		if (g_iLevel == 1) mxBossPos = g_pFirstBoss->GetTranslateMatrix();
		if (g_iLevel == 2) mxBossPos = g_pSecondBoss->GetTranslateMatrix();
		if (g_iLevel == 3) mxBossPos = g_pThirdBoss->GetTranslateMatrix();
		g_pPlayer->ShootMissile(delta, g_fPTx, mxBossPos, g_iAttackStarNum);	//根據蓄力量，發射導彈至BOSS位置
		if (g_pPlayer->_bMissileIsShoot == false) g_bMissileShoot = false;		//導彈發射完成
		g_fAttackCount = 0;
		g_iAttackStarNum = 0;
	}

	//----------------------------
	//玩家防護罩
	if (g_bisOpenDefense) {									//防護罩被打開
		g_fDefenseCount += delta;							//開始計時
		if (g_fDefenseCount > DEFENSE_TIME) {
			g_bisOpenDefense = false;						//關閉防護罩
			g_fDefenseCount -= DEFENSE_TIME;				//計時歸零
		}
	}
	if (!g_bCanOpenDefense && !g_bisOpenDefense) {			//防護罩功能尚未再次開啟，且防護罩關閉中
		g_fDefenseCount += delta;							//開始計時
		if (g_fDefenseCount > REOPEN_DEFENSE_TIME) {
			g_bCanOpenDefense = true;						//重新開啟防護罩功能
			g_fDefenseCount -= REOPEN_DEFENSE_TIME;			//計時歸零
		}
	}

	//----------------------------
	//物件運動更新
	g_pPlayer->UpdateMatrix(delta);							//玩家
	g_pStars->UpdateMatrix(delta);							//背景
	g_pFirstBoss->UpdateMatrix(delta);						//BOSS1
	g_pSecondBoss->UpdateMatrix(delta);						//BOSS2 子物件運動
	g_pThirdBoss->UpdateMatrix(delta);						//BOSS3

	CollisionDetect();										//碰撞偵測

	//----------------------------
	// 由上層更新所有要被繪製物件的 View 與 Projection Matrix
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) { // 更新所有物件的 View Matrix
		g_pStars->SetViewMatrix(mvx);
		g_pPlayer->SetViewMatrix(mvx);
		g_pFirstBoss->SetViewMatrix(mvx);
		g_pSecondBoss->SetViewMatrix(mvx);
		g_pThirdBoss->SetViewMatrix(mvx);
	}
	mpx = camera->getProjectionMatrix(bPDirty);
	if (bPDirty) { // 更新所有物件的 View Matrix
		g_pStars->SetProjectionMatrix(mpx);
		g_pPlayer->SetProjectionMatrix(mpx);
		g_pFirstBoss->SetProjectionMatrix(mpx);
		g_pSecondBoss->SetProjectionMatrix(mpx);
		g_pThirdBoss->SetProjectionMatrix(mpx);
	}

	GL_Display();
}

//----------------------------------------------------------------------------
void Win_PassiveMotion(int x, int y) {										//滑鼠沒有按下，在螢幕上移動
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//位置
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//大小
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->GL_SetTranslatMatrix(g_mxPT);		//更新玩家位置
	g_pPlayer->SetPassiveMotion(g_fPTx);			//血量條 + 星星
	g_pPlayer->SetBulletPassiveMove(g_mxPT);		//bullet
}

void Win_MouseMotion(int x, int y) {										//滑鼠按下且在螢幕上移動
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//位置
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//大小
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->GL_SetTranslatMatrix(g_mxPT);		//更新玩家位置
	g_pPlayer->SetPassiveMotion(g_fPTx);			//血量條 + 星星
	g_pPlayer->SetBulletPassiveMove(g_mxPT);		//bullet
}

void Win_Mouse(int button, int state, int x, int y) {						//滑鼠按下觸發
	switch (button) {
	case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
		g_bStoringAttack = !g_bStoringAttack;	//蓄力
		break;
	case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
							  //if ( state == GLUT_DOWN ) ; 
		break;
	case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
		if (g_bCanOpenDefense) {
			g_bisOpenDefense = true;		//開啟防護罩
			g_bCanOpenDefense = false;		//關閉防護罩功能
		}
		break;
	default:
		break;
	}
}

//----------------------------------------------------------------------------

void Win_Keyboard(unsigned char key, int x, int y)
{
	auto camera = CCamera::getInstance();
	switch (key) {
	case  SPACE_KEY:
		m_bAutoMove = !m_bAutoMove;
		break;
	case 68: // D key
	case 100: // d key
		g_fDir = -1 * g_fDir;
		break;
	case 033:
		glutIdleFunc(NULL);

		//--------資源釋放---------
		delete g_pStars;		//背景
		delete g_pPlayer;		//玩家
		delete g_pFirstBoss;	//BOSS1
		delete g_pSecondBoss;	//BOSS2
		delete g_pThirdBoss;	//BOSS3
		camera->destroyInstance();

		exit(EXIT_SUCCESS);
		break;
	}
}

void Win_SpecialKeyboard(int key, int x, int y) {
	mat4 ry, mxT;
	vec4 vT;
	if (!m_bAutoMove) { // 沒有自動旋轉下才有作用
		switch (key) {
		case GLUT_KEY_LEFT:		// 目前按下的是向左方向鍵
			g_fYAngle -= 2.0;		// 逆時針為正方向
			break;
		case GLUT_KEY_RIGHT:	// 目前按下的是向右方向鍵
			g_fYAngle += 2.0;		// 順時針為負方向
			break;
		default:
			break;
		}

		// 計算旋轉矩陣並更新到 Shader 中
		ry = RotateY(g_fYAngle); //  degree 

								 //		glutPostRedisplay();  
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor(0.0, 0.0, 0.0, 1.0); // black background
	glEnable(GL_DEPTH_TEST); //深度測試 : 才有前後關係，以每個三角形為單位
							 //glEnable(GL_BLEND); //開透明度
							 //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //計算透明度
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 700);

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("Shooting Game");

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE;
	glewInit();

	init();
	
	glutMouseFunc(Win_Mouse);
	glutMotionFunc(Win_MouseMotion);
	glutPassiveMotionFunc(Win_PassiveMotion);
	glutKeyboardFunc(Win_Keyboard);	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc(Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;
}
