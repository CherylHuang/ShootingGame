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

int g_iLevel = 3;	//�ĴX���]��

// For Rotation
GLfloat g_fYAngle = 0;  // Z�b�����ਤ��
GLfloat g_fDir = 1;     // �����V
bool    m_bAutoMove = false; // Controlled by Space Key

// Objects
CBGStars *g_pStars;
CPlayer *g_pPlayer;
CFirstBoss *g_pFirstBoss;
CSecondBoss *g_pSecondBoss;
CThirdBoss *g_pThirdBoss;

// Player Attack
bool g_bStoringAttack;		//�W�O��
int g_iAttackStarNum;		//�W�O�P�P��
float g_fAttackCount;		//�W�O�p��
bool g_bMissileShoot;		//�O�_�n�o�g�ɼu

// Player Defense
bool g_bCanOpenDefense;	//�O�_�i�}�Ҩ��@�n
bool g_bisOpenDefense;	//���@�n�O�_�}��
float g_fDefenseCount;	//���@�n�p��

// Counting
float g_fcount = 0;
float g_fcount_boss1 = 0;
float g_fcount_boss3 = 0;

// Delay Time
float g_NextBulletDelay_Boss3 = 1.3f;

// PassiveMotion
float g_fPTx;		//���a����x�b
mat4 g_mxPT;		//���a�y��(g_fPTx, PLAYER_Y_AXIS, 0, 1)
mat4 g_mxPS;		//�԰����j�p

//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();
void CollisionDetect();		//�I������

void init(void)
{
	mat4 mxT;
	//vec4 vT;

	// ���ͩһݤ� Model View �P Projection Matrix
	// LookAt ���T�ӰѼơA�ϥβy��y��
	//point4  eye(g_fRadius*sin(g_fTheta)*sin(g_fPhi), g_fRadius*cos(g_fTheta), g_fRadius*sin(g_fTheta)*cos(g_fPhi), 1.0f);
	point4  eye(0.0f, 0.0f, 50.0f, 1.0f);
	point4  at(0.0f, 0.0f, 0.0f, 1.0f);

	auto camera = CCamera::getInstance();
	camera->updateViewLookAt(eye, at);
	camera->updateOrthographic(-5.0f, 5.0f, -7.0f, 7.0f, 1.0f, 100.0f);
	//--------------------------------------------------------------------

	g_pStars = new CBGStars;					//�I��
	g_pPlayer = new CPlayer;					//���a
	g_pFirstBoss = new CFirstBoss;				//BOSS1
	g_pSecondBoss = new CSecondBoss;			//BOSS2
	g_pThirdBoss = new CThirdBoss;				//BOSS3

	//-------------------------------------------------------------------
	//ATTACK
	g_bStoringAttack = false;
	g_iAttackStarNum = 0;	//�����W�O�P�P
	g_fAttackCount = 0;
	g_bMissileShoot = false;

	//DEFENSE
	g_bCanOpenDefense = true;
	g_bisOpenDefense = false;
	g_fDefenseCount = 0;
}

void CollisionDetect()		//�I������
{
	mat4 mxPlayerPos, mxPBulletPos, mxBossPos, mxBBulletPos;
	float fPlayer_x, fPlayer_y;			//player position
	float fPBullet_x, fPBullet_y;		//player bullet position
	float fBoss_x, fBoss_y;				//boss position
	float fBBullet_x, fBBullet_y;		//boss bullet position

	if (g_pPlayer) {		//���a�s�b
		mxPlayerPos = g_pPlayer->GetTranslateMatrix();			//���o���a��m
		fPlayer_x = mxPlayerPos._m[0][3];
		fPlayer_y = mxPlayerPos._m[1][3];
		mxPBulletPos = g_pPlayer->GetBulletTranslateMatrix();	//���o���a�l�u��m
		fPBullet_x = mxPBulletPos._m[0][3];
		fPBullet_y = mxPBulletPos._m[1][3];
	}

	if (g_iLevel == 1) {		//BOSS1
		if (g_pFirstBoss) {		//BOSS1�s�b
			bool isFirstBullet = g_pFirstBoss->GetFirstShoot();			//�O�_���Ĥ@�o�l�u
			mxBossPos = g_pFirstBoss->GetTranslateMatrix();				//���oBOSS��m
			fBoss_x = mxBossPos._m[0][3];
			fBoss_y = mxBossPos._m[1][3];
			mxBBulletPos = g_pFirstBoss->GetBulletTranslateMatrix();	//���oBOSS�l�u��m
			fBBullet_x = mxBBulletPos._m[0][3];
			fBBullet_y = mxBBulletPos._m[1][3];
			
			if (isFirstBullet) {	//�Ĥ@�o�l�u
				if (fBBullet_y - 0.15f < PLAYER_Y_AXIS + 1.75f && 
					fBBullet_x - 0.5f < g_fPTx + 2.f && 
					fBBullet_x + 0.5f > g_fPTx - 2.f) {					//BOSS �j�l�u �I�����a
					if (!g_bisOpenDefense) {							//���@�n���}��
						//printf("COLLISION!\n");
					}
				}
			} 
		
			if (fPBullet_y > fBoss_y - 0.61f && fPBullet_x < fBoss_x + 1.f && fPBullet_x > fBoss_x - 1.f) {	//���a�l�u�I��BOSS1
				//printf("COLLISION!\n");
			}
		}
	} //--------Level1

	else if (g_iLevel == 3) {		//BOSS3
		if (g_pThirdBoss) {			//BOSS3�s�b
			mxBossPos = g_pThirdBoss->GetTranslateMatrix();				//���oBOSS��m
			fBoss_x = mxBossPos._m[0][3];
			fBoss_y = mxBossPos._m[1][3];
			mxBBulletPos = g_pThirdBoss->GetBulletTranslateMatrix();	//���oBOSS�l�u��m
			fBBullet_x = mxBBulletPos._m[0][3];
			fBBullet_y = mxBBulletPos._m[1][3];
			
			mat4 mxLEPos[LITTLE_NUM], mxLBulletPos[LITTLE_NUM];					//�p�Ǧ�m & �p�Ǥl�u��m
			float fLE_x[LITTLE_NUM], fLE_y[LITTLE_NUM];
			float fLBullet_x[LITTLE_NUM], fLBullet_y[LITTLE_NUM];
			for (int i = 0; i < LITTLE_NUM; i++) {
				mxLEPos[i] = g_pThirdBoss->GetLETranslateMatrix(i);				//���o�p�Ǧ�m
				fLE_x[i] = mxLEPos[i]._m[0][3];
				fLE_y[i] = mxLEPos[i]._m[1][3];
				mxLBulletPos[i] = g_pThirdBoss->GetLEBulletTranslateMatrix(i);	//���o�p�� �l�u��m
				fLBullet_x[i] = mxLBulletPos[i]._m[0][3];
				fLBullet_y[i] = mxLBulletPos[i]._m[1][3];
			}
			//printf("%f, %f\n", fBBullet_x, fBBullet_y);
			if (fPBullet_y > fBoss_y - 1.296f && fPBullet_x < fBoss_x + 2.269f && fPBullet_x > fBoss_x - 2.195f) {	//���a�l�u�I��BOSS3
				//printf("COLLISION!\n");
			}
			for (int i = 0; i < LITTLE_NUM; i++) {
				if (fPBullet_y > fLE_y[i] - 0.5124f && fPBullet_x < fLE_x[i] + 0.436f && fPBullet_x > fLE_x[i] - 0.436f) {	//���a�l�u�I���p��
					//printf("COLLISION!\n");
				}
				if (fLBullet_y[i] < PLAYER_Y_AXIS + 1.75f && fLBullet_y[i] > PLAYER_Y_AXIS - 1.75f &&
					fLBullet_x[i] < g_fPTx + 2.f && fLBullet_x[i] > g_fPTx - 2.f) {						//�p�Ǥl�u�I�����a
					//printf("COLLISION!\n");
				}
			}
		}
	} //--------Level3

	  //BOSS�@��l�u�I�����a
	if (fBBullet_y < PLAYER_Y_AXIS + 1.75f && fBBullet_y > PLAYER_Y_AXIS - 1.75f &&
		fBBullet_x < g_fPTx + 2.f && fBBullet_x > g_fPTx - 2.f) {
		if (!g_bisOpenDefense) {	//���@�n���}��
			//printf("COLLISION!\n");
		}
		//else printf("NO COLLISION!\n");
	}

}

//----------------------------------------------------------------------------

void GL_Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window �M�C��, �N�`�ײM��1

	g_pStars->GL_Draw();								//�I��
	g_pPlayer->GL_Draw();
	if (g_bisOpenDefense) g_pPlayer->GL_DrawDefense();	//���@�n���
	g_pPlayer->GL_DrawAttack(g_iAttackStarNum);			//�����W�O���

	//BOSS
	if (g_iLevel == 1) g_pFirstBoss->GL_Draw();
	if (g_iLevel == 2) g_pSecondBoss->GL_Draw();
	if (g_iLevel == 3) g_pThirdBoss->GL_Draw();

	glutSwapBuffers();	// �洫 Frame Buffer
}

void onFrameMove(float delta)
{
	mat4 mvx, mpx;	// view matrix & projection matrix
	bool bVDirty, bPDirty;	// view �P projection matrix �O�_�ݭn��s������
	auto camera = CCamera::getInstance();

	//----------------------------
	//���a�l�u
	g_fcount += delta;
	if (g_fcount < NEXT_BULLET_DELAY) g_pPlayer->ShootBullet(delta, g_fPTx);	//�o�g�l�u
	else {
		if (!g_bStoringAttack) {	//�D�W�O�ɡA�۰ʵo�g�l�u
			g_pPlayer->NextBullet(g_fPTx);	//�U�@�Ӥl�u
			g_fcount -= NEXT_BULLET_DELAY;
		}
		else g_fcount = 0;			//�W�O�����s�p��
	}
	//BOSS1�l�u
	g_fcount_boss1 += delta;
	g_pFirstBoss->SetBulletPassiveMove();	//���o�g�l�u���HBOSS1
	if (g_fcount_boss1 < NEXT_BULLET_DELAY) g_pFirstBoss->ShootBullet(delta);	//�o�g�l�u
	else {
		g_pFirstBoss->NextBullet();	//�U�@�Ӥl�u
		g_fcount_boss1 -= NEXT_BULLET_DELAY;
	}
	//BOSS3 + Little �l�u
	g_fcount_boss3 += delta;
	g_pThirdBoss->SetBulletPassiveMove();	//���o�g�l�u���Hlittle
	if (g_fcount_boss3 < g_NextBulletDelay_Boss3) g_pThirdBoss->ShootBullet(delta);	//�o�g�l�u
	else {
		g_pThirdBoss->NextBullet();	//�U�@�Ӥl�u
		g_fcount_boss3 -= g_NextBulletDelay_Boss3;
		g_NextBulletDelay_Boss3 = NEXT_BULLET_DELAY_BOSS3 + (rand() % 10)*0.1f;
	}

	//----------------------------
	//���a�W�O����
	if (g_bStoringAttack) {
		g_fAttackCount += delta;
		if (g_fAttackCount > 1.0f) {
			g_iAttackStarNum++;			//�P�P+1
			g_fAttackCount -= 1.0f;
		}
	}
	else {
		if (g_iAttackStarNum > 0) {				//�D�W�O���B�P�P�� > 0
			g_bMissileShoot = true;				//�ɼu�o�g
		}
	}

	//�ɼu�o�g
	if (g_bMissileShoot) {
		mat4 mxBossPos;
		if (g_iLevel == 1) mxBossPos = g_pFirstBoss->GetTranslateMatrix();
		if (g_iLevel == 2) mxBossPos = g_pSecondBoss->GetTranslateMatrix();
		if (g_iLevel == 3) mxBossPos = g_pThirdBoss->GetTranslateMatrix();
		g_pPlayer->ShootMissile(delta, g_fPTx, mxBossPos, g_iAttackStarNum);	//�ھڻW�O�q�A�o�g�ɼu��BOSS��m
		if (g_pPlayer->_bMissileIsShoot == false) g_bMissileShoot = false;		//�ɼu�o�g����
		g_fAttackCount = 0;
		g_iAttackStarNum = 0;
	}

	//----------------------------
	//���a���@�n
	if (g_bisOpenDefense) {									//���@�n�Q���}
		g_fDefenseCount += delta;							//�}�l�p��
		if (g_fDefenseCount > DEFENSE_TIME) {
			g_bisOpenDefense = false;						//�������@�n
			g_fDefenseCount -= DEFENSE_TIME;				//�p���k�s
		}
	}
	if (!g_bCanOpenDefense && !g_bisOpenDefense) {			//���@�n�\��|���A���}�ҡA�B���@�n������
		g_fDefenseCount += delta;							//�}�l�p��
		if (g_fDefenseCount > REOPEN_DEFENSE_TIME) {
			g_bCanOpenDefense = true;						//���s�}�Ҩ��@�n�\��
			g_fDefenseCount -= REOPEN_DEFENSE_TIME;			//�p���k�s
		}
	}

	//----------------------------
	//����B�ʧ�s
	g_pPlayer->UpdateMatrix(delta);							//���a
	g_pStars->UpdateMatrix(delta);							//�I��
	g_pFirstBoss->UpdateMatrix(delta);						//BOSS1
	g_pSecondBoss->UpdateMatrix(delta);						//BOSS2 �l����B��
	g_pThirdBoss->UpdateMatrix(delta);						//BOSS3

	CollisionDetect();										//�I������

	//----------------------------
	// �ѤW�h��s�Ҧ��n�Qø�s���� View �P Projection Matrix
	mvx = camera->getViewMatrix(bVDirty);
	if (bVDirty) { // ��s�Ҧ����� View Matrix
		g_pStars->SetViewMatrix(mvx);
		g_pPlayer->SetViewMatrix(mvx);
		g_pFirstBoss->SetViewMatrix(mvx);
		g_pSecondBoss->SetViewMatrix(mvx);
		g_pThirdBoss->SetViewMatrix(mvx);
	}
	mpx = camera->getProjectionMatrix(bPDirty);
	if (bPDirty) { // ��s�Ҧ����� View Matrix
		g_pStars->SetProjectionMatrix(mpx);
		g_pPlayer->SetProjectionMatrix(mpx);
		g_pFirstBoss->SetProjectionMatrix(mpx);
		g_pSecondBoss->SetProjectionMatrix(mpx);
		g_pThirdBoss->SetProjectionMatrix(mpx);
	}

	GL_Display();
}

//----------------------------------------------------------------------------
void Win_PassiveMotion(int x, int y) {										//�ƹ��S�����U�A�b�ù��W����
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//��m
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//�j�p
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->GL_SetTranslatMatrix(g_mxPT);		//��s���a��m
	g_pPlayer->SetPassiveMotion(g_fPTx);			//��q�� + �P�P
	g_pPlayer->SetBulletPassiveMove(g_mxPT);		//bullet
}

void Win_MouseMotion(int x, int y) {										//�ƹ����U�B�b�ù��W����
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//��m
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//�j�p
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->GL_SetTranslatMatrix(g_mxPT);		//��s���a��m
	g_pPlayer->SetPassiveMotion(g_fPTx);			//��q�� + �P�P
	g_pPlayer->SetBulletPassiveMove(g_mxPT);		//bullet
}

void Win_Mouse(int button, int state, int x, int y) {						//�ƹ����UĲ�o
	switch (button) {
	case GLUT_LEFT_BUTTON:   // �ثe���U���O�ƹ�����
		g_bStoringAttack = !g_bStoringAttack;	//�W�O
		break;
	case GLUT_MIDDLE_BUTTON:  // �ثe���U���O�ƹ����� �A���� Y �b
							  //if ( state == GLUT_DOWN ) ; 
		break;
	case GLUT_RIGHT_BUTTON:   // �ثe���U���O�ƹ��k��
		if (g_bCanOpenDefense) {
			g_bisOpenDefense = true;		//�}�Ҩ��@�n
			g_bCanOpenDefense = false;		//�������@�n�\��
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

		//--------�귽����---------
		delete g_pStars;		//�I��
		delete g_pPlayer;		//���a
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
	if (!m_bAutoMove) { // �S���۰ʱ���U�~���@��
		switch (key) {
		case GLUT_KEY_LEFT:		// �ثe���U���O�V����V��
			g_fYAngle -= 2.0;		// �f�ɰw������V
			break;
		case GLUT_KEY_RIGHT:	// �ثe���U���O�V�k��V��
			g_fYAngle += 2.0;		// ���ɰw���t��V
			break;
		default:
			break;
		}

		// �p�����x�}�ç�s�� Shader ��
		ry = RotateY(g_fYAngle); //  degree 

								 //		glutPostRedisplay();  
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor(0.0, 0.0, 0.0, 1.0); // black background
	glEnable(GL_DEPTH_TEST); //�`�״��� : �~���e�����Y�A�H�C�ӤT���ά����
							 //glEnable(GL_BLEND); //�}�z����
							 //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //�p��z����
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
	glutKeyboardFunc(Win_Keyboard);	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutSpecialFunc(Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
	glutDisplayFunc(GL_Display);
	glutReshapeFunc(GL_Reshape);
	glutIdleFunc(IdleProcess);

	glutMainLoop();
	return 0;
}
