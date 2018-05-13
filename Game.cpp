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
GLfloat g_fYAngle = 0;  // Z�b�����ਤ��
GLfloat g_fDir = 1;     // �����V
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
float g_fPTx;		//���a����x�b
mat4 g_mxPT;		//���a�y��(g_fPTx, PLAYER_Y_AXIS, 0, 1)
mat4 g_mxPS;		//�԰����j�p

// For View Point
//GLfloat g_fRadius = 8.0;
//GLfloat g_fTheta = 45.0f*DegreesToRadians;
//GLfloat g_fPhi = 45.0f*DegreesToRadians;

//----------------------------------------------------------------------------
// �禡���쫬�ŧi
extern void IdleProcess();

void init( void )
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

}

void AutomaticRotation(float delta){
	mat4 ry, mxT;
	vec4 vT;

	g_fYAngle += g_fDir * delta * 250;     // �]�w�C����X��
	if( g_fYAngle > 360.0 ) g_fYAngle -= 360;
	else if (g_fYAngle < 0.0) g_fYAngle += 360.0;
	else ;
	// �p�����x�}�ç�s�� Shader ��
	ry = RotateY(g_fYAngle); //  degree 

	// For Quad
	//g_plane->SetTRSMatrix(ry*Scale(7.0f, 1.0f, 7.0f));
	// for obj file
	//g_pObjFile->SetTRSMatrix(ry*Scale(1.0f, 1.0f, 1.0f));
}

//----------------------------------------------------------------------------

void GL_Display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // clear the window �M�C��, �N�`�ײM��1

	g_pStars->GL_Draw();
	g_pPlayer->GL_Draw();
	g_pFirstBoss->GL_Draw();
	//g_pSecondBoss->GL_Draw();
	//g_pThirdBoss->GL_Draw();

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
		g_pPlayer->NextBullet(g_fPTx);	//�U�@�Ӥl�u
		g_fcount -= NEXT_BULLET_DELAY;
	}
	//BOSS1�l�u
	g_fcount_boss1 += delta;
	g_pFirstBoss->SetBulletPassiveMove();	//���o�g�l�u���HBOSS1
	if (g_fcount_boss1 < NEXT_BULLET_DELAY) g_pFirstBoss->ShootBullet(delta);	//�o�g�l�u
	else {
		g_pFirstBoss->NextBullet();	//�U�@�Ӥl�u
		g_fcount_boss1 -= NEXT_BULLET_DELAY;
	}
	//----------------------------

	//����B�ʧ�s
	g_pStars->UpdateMatrix(delta);							//�I��
	g_pFirstBoss->UpdateMatrix(delta);						//BOSS1
	if (m_bAutoMove) g_pSecondBoss->UpdateMatrix(delta);	//BOSS2 �l����B��
	g_pThirdBoss->UpdateMatrix(delta);						//BOSS3

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
// The passive motion callback for a window is called when the mouse moves within the window while no mouse buttons are pressed.
void Win_PassiveMotion(int x, int y) {
	float fPlayerScale;
	float fsize = 0.01f;
	g_fPTx = fsize*SCREEN_SIZE_X*(x - HALF_SIZE_X) / (HALF_SIZE_X);
	//g_fTy = -fsize*SCREEN_SIZE_Y*(y - HALF_SIZE_Y) / (HALF_SIZE_Y);
	g_mxPT = Translate(g_fPTx, PLAYER_Y_AXIS, 0);				//��m
	fPlayerScale = g_pPlayer->GetPlayerScale();
	g_mxPS = Scale(fPlayerScale, fPlayerScale, fPlayerScale);	//�j�p
	g_pPlayer->GL_SetTRSMatrix(g_mxPT * g_mxPS);	//jet
	g_pPlayer->SetHPPassiveMotion(g_fPTx);			//��q��
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

		//--------�귽����---------
		delete g_pStars;		//�I��
		delete g_pPlayer;		//���a
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
		case GLUT_LEFT_BUTTON:   // �ثe���U���O�ƹ�����
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_MIDDLE_BUTTON:  // �ثe���U���O�ƹ����� �A���� Y �b
			//if ( state == GLUT_DOWN ) ; 
			break;
		case GLUT_RIGHT_BUTTON:   // �ثe���U���O�ƹ��k��
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
	if( !m_bAutoMove ) { // �S���۰ʱ���U�~���@��
		switch(key) {
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

		// Cube ������
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
	glEnable(GL_DEPTH_TEST); //�`�״��� : �~���e�����Y�A�H�C�ӤT���ά����
	//glEnable(GL_BLEND); //�}�z����
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //�p��z����
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
    glutKeyboardFunc( Win_Keyboard );	// �B�z ASCI ����p A�Ba�BESC ��...����
	glutSpecialFunc( Win_SpecialKeyboard);	// �B�z NON-ASCI ����p F1�BHome�B��V��...����
	glutDisplayFunc( GL_Display );
	glutReshapeFunc( GL_Reshape );
	glutIdleFunc( IdleProcess );
	
    glutMainLoop();
    return 0;
}
