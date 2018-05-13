// Model-View matrix and Projection matrix
// Perspective projection with fixed camera position
// Rotate the grid and the cube around the Y axis

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

// For Rotation
GLfloat g_fYAngle = 0;  // Z軸的旋轉角度
GLfloat g_fDir = 1;     // 旋轉方向
bool    m_bAutoMove = false; // Controlled by Space Key

// For Objects
CBGStars *g_pStars;
CPlayer *g_pPlayer;
CFirstBoss *g_pFirstBoss;
CSecondBoss *g_pSecondBoss;
CThirdBoss *g_pThirdBoss;

// For Counting
float g_fcount = 0;
float g_fcount_boss1 = 0;

// PassiveMotion
float g_fPTx;		//玩家移動x軸
mat4 g_mxPT;		//玩家座標(g_fPTx, PLAYER_Y_AXIS, 0, 1)
mat4 g_mxPS;		//戰鬥機大小

// For View Point
//GLfloat g_fRadius = 8.0;
//GLfloat g_fTheta = 45.0f*DegreesToRadians;
//GLfloat g_fPhi = 45.0f*DegreesToRadians;

//----------------------------------------------------------------------------
// 函式的原型宣告
extern void IdleProcess();

void init( void )
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

}

void AutomaticRotation(float delta){
	mat4 ry, mxT;
	vec4 vT;

	g_fYAngle += g_fDir * delta * 250;     // 設定每秒轉幾度
	if( g_fYAngle > 360.0 ) g_fYAngle -= 360;
	else if (g_fYAngle < 0.0) g_fYAngle += 360.0;
	else ;
	// 計算旋轉矩陣並更新到 Shader 中
	ry = RotateY(g_fYAngle); //  degree 

	// For Quad
	//g_plane->SetTRSMatrix(ry*Scale(7.0f, 1.0f, 7.0f));
	// for obj file
	//g_pObjFile->SetTRSMatrix(ry*Scale(1.0f, 1.0f, 1.0f));
}

//----------------------------------------------------------------------------

void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window 清顏色, 將深度清為1

	g_pStars->GL_Draw();
	g_pPlayer->GL_Draw();
	g_pFirstBoss->GL_Draw();
	//g_pSecondBoss->GL_Draw();
	//g_pThirdBoss->GL_Draw();

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
		g_pPlayer->NextBullet(g_fPTx);	//下一個子彈
		g_fcount -= NEXT_BULLET_DELAY;
	}
	//BOSS1子彈
	g_fcount_boss1 += delta;
	g_pFirstBoss->SetBulletPassiveMove();	//未發射子彈跟隨BOSS1
	if (g_fcount_boss1 < NEXT_BULLET_DELAY) g_pFirstBoss->ShootBullet(delta);	//發射子彈
	else {
		g_pFirstBoss->NextBullet();	//下一個子彈
		g_fcount_boss1 -= NEXT_BULLET_DELAY;
	}
	//----------------------------

	//物件運動更新
	g_pStars->UpdateMatrix(delta);							//背景
	g_pFirstBoss->UpdateMatrix(delta);						//BOSS1
	if (m_bAutoMove) g_pSecondBoss->UpdateMatrix(delta);	//BOSS2 子物件運動
	g_pThirdBoss->UpdateMatrix(delta);						//BOSS3

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
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//位置
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//大小
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->SetHPPassiveMotion(g_fPTx);			//血量條
	g_pPlayer->SetBulletPassiveMove(g_mxPT);		//bullet
}

//----------------------------------------------------------------------------

void Win_Keyboard( unsigned char key, int x, int y )
{
	auto camera = CCamera::getInstance();
    switch ( key ) {
	case  SPACE_KEY:
		m_bAutoMove = !m_bAutoMove;
		break;
	case 68: // D key
	case 100: // d key
		g_fDir = -1 * g_fDir;
		break;
    case 033:
		glutIdleFunc( NULL );

		//--------資源釋放---------
		delete g_pStars;		//背景
		delete g_pPlayer;		//玩家
		delete g_pFirstBoss;	//BOSS1
		delete g_pSecondBoss;	//BOSS2
		delete g_pThirdBoss;	//BOSS3
		camera->destroyInstance();

        exit( EXIT_SUCCESS );
        break;
    }
}
//----------------------------------------------------------------------------
void Win_Mouse(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:   // 目前按下的是滑鼠左鍵
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_MIDDLE_BUTTON:  // 目前按下的是滑鼠中鍵 ，換成 Y 軸
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // 目前按下的是滑鼠右鍵
			//if ( state == GLUT_DOWN ) ;
			break;
		default:
			break;
	} 
}

//----------------------------------------------------------------------------
void Win_SpecialKeyboard(int key, int x, int y) {
	mat4 ry, mxT;
	vec4 vT;
	if( !m_bAutoMove ) { // 沒有自動旋轉下才有作用
		switch(key) {
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

		// Cube 的旋轉
		//vT.x = 1.5; vT.y = 0.5; vT.z = -1.5;
		//mxT = Translate(vT);
		//g_pHouse->SetTRSMatrix(ry*mxT);

//		glutPostRedisplay();  
	}
}
//----------------------------------------------------------------------------
void GL_Reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glClearColor( 0.0, 0.0, 0.0, 1.0 ); // black background
	glEnable(GL_DEPTH_TEST); //深度測試 : 才有前後關係，以每個三角形為單位
	//glEnable(GL_BLEND); //開透明度
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //計算透明度
}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{   
	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 500, 700 );

	// If you use freeglut the two lines of code can be added to your application 
	glutInitContextVersion( 3, 2 );
	glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Shooting Game" );

	// The glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
	// which ensures that all extensions with valid entry points will be exposed.
	glewExperimental = GL_TRUE; 
    glewInit();  
	
    init();

	glutMouseFunc(Win_Mouse);
	glutPassiveMotionFunc(Win_PassiveMotion);
    glutKeyboardFunc( Win_Keyboard );	// 處理 ASCI 按鍵如 A、a、ESC 鍵...等等
	glutSpecialFunc( Win_SpecialKeyboard);	// 處理 NON-ASCI 按鍵如 F1、Home、方向鍵...等等
	glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}
