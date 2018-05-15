#include "CSecondBoss.h"

CSecondBoss::CSecondBoss()
{
	_mxBT = Translate(0.0f, 0.0f, 0.0f);////////////////////////////初始化子彈位置

	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk2.obj");
	_pMainBody->SetColor(vec4(0.0f, -1.0f, 0.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;	//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]); // 0
	_pMainBody->SetTRSMatrix(_mxMT);

	//產生子實體物件 大輪
	float fBWscale = 0.55f;
	for (int i = 0; i < BIG_WHEEL_NUM; i++) {
		_pWheel[i] = new CObjReader("obj/square.obj");
		_pWheel[i]->SetShader();
		_mxBWT[i] = Translate(BW_RADIUS * cosf(M_PI*2.0f*i / BIG_WHEEL_NUM), BW_RADIUS * sinf(M_PI*2.0f*i / BIG_WHEEL_NUM), 0.0f);
		_mxBWS[i] = Scale(fBWscale, fBWscale, fBWscale);
		_pWheel[i]->SetTRSMatrix(_mxMT * _mxBWT[i] * _mxBWS[i]);
	}

	//產生子子實體物件  小輪
	float fSWscale = 1.f;
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) {
		_pSWheel[i] = new CObjReader("obj/chain.obj");
		_pSWheel[i]->SetShader();
		_mxSWT[i] = Translate(SW_RADIUS * cosf(M_PI*2.0f*i / SMALL_WHEEL_NUM), SW_RADIUS * sinf(M_PI*2.0f*i / SMALL_WHEEL_NUM), 0.0f);
		_mxSWR[i] = RotateZ(-90.f + 360.f/BIG_WHEEL_NUM*i);	//自轉 朝向大輪
		_mxSWS[i] = Scale(fSWscale, fSWscale, fSWscale);
		_pSWheel[i]->SetTRSMatrix(_mxMT * _mxBWT[i] * _mxSWT[i] * _mxSWS[i] * _mxSWR[i]);
	}

	//產生子彈
	CreateBulletList();
}

CSecondBoss::~CSecondBoss()
{
	for (int i = 0; i < BIG_WHEEL_NUM; i++) delete _pWheel[i]; //拿的時候一個一個拿->一個一個刪
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) delete _pSWheel[i];
	DeleteBulletList();
}

//-------------------------------------------------------

void CSecondBoss::UpdateMatrix(float delta)
{
	mat4 mxBWR[BIG_WHEEL_NUM], mxBWRr[BIG_WHEEL_NUM];	//大輪 旋轉矩陣
	mat4 mxSWR[SMALL_WHEEL_NUM];	//小輪 旋轉矩陣
	mat4 _mxBWTt;	//大輪 路徑移動矩陣

	float speed = 2.5f;	//大輪速度
	_fBWAngle_Track += speed * delta;
	if (_fBWAngle_Track > 360) _fBWAngle_Track -= 360; //歸零

	float sint, cost, sin2t, cos2t, cos3t, cos4t, sin12t, sin5t2, cos9t2; //---------定義----------
	sint = sinf(_fBWAngle_Track);				sin2t = sinf(2 * _fBWAngle_Track);
	cost = cosf(_fBWAngle_Track);				cos2t = cosf(2 * _fBWAngle_Track);
	cos3t = cosf(3 * _fBWAngle_Track);			cos4t = cosf(4 * _fBWAngle_Track);
	sin5t2 = sinf(5 * _fBWAngle_Track / 2.f);	cos9t2 = cosf(9 * _fBWAngle_Track / 2.f);
	float fsize = 0.05f;
	_mxBWTt = Translate((13 * sint * sin2t)*fsize, (13 * cost * sin2t)*fsize, 0);	//大輪路徑移動
	_fMT[0] = cost * (sint + sin5t2); _fMT[1] = sint * (sint + sin5t2) + BOSS_Y;						//BOSS BODY MOVE
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);

	//HEART
	//x = 16 * sint*sint*sint
	//y = 13 * cost - 5 * cos2t - 2 * cos3t - cos4t
	//FLOWER
	//x = 15 * cost * sin2t  (15 : scale)
	//y = 15 * sint * sin2t
	//BUTTERFLY
	//x = sint*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//y = cost*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//UNTITLED 2
	//x = cost * sint * sin5t2
	//y = sint * sint * sin5t2

	//父物件
	_pMainBody->SetTRSMatrix(_mxMT);

	//大輪
	_fBWAngle = 360.f / BIG_WHEEL_NUM;
	for (int i = 0; i < BIG_WHEEL_NUM; i++) {
		mxBWR[i] = RotateZ(_fBWAngle*i);
		mxBWRr[i] = RotateZ(-_fBWAngle*i); //反轉
		_pWheel[i]->SetTRSMatrix(_mxMT * mxBWR[i] * _mxBWTt * _mxBWT[i] * _mxBWS[i] * mxBWRr[i]);
	}

	//小輪
	_fSWAngle += 250.f * delta;
	if (_fSWAngle > 360) _fSWAngle -= 360; //歸零
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) {
		mxSWR[i] = RotateZ(_fSWAngle);
		_pSWheel[i]->SetTRSMatrix(_mxMT * mxBWR[i] * _mxBWTt * _mxBWT[i] * mxBWRr[i] * mxSWR[i] * _mxSWT[i] * _mxSWS[i] * _mxSWR[i]);
	}

}

void CSecondBoss::GL_Draw()
{
	_pMainBody->Draw();
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->DrawW();
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->DrawW();
	for (int i = 0; i < BULLET_NUM; i++) _bullet[i]->GL_Draw();
}

void CSecondBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetViewMatrix(mvx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetViewMatrix(mvx);
	for (int i = 0; i < BULLET_NUM; i++) _bullet[i]->SetViewMatrix(mvx);
}

void CSecondBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < BULLET_NUM; i++) _bullet[i]->SetProjectionMatrix(mpx);
}

//-----------------------------------------------------------------
void CSecondBoss::CreateBulletList()
{
	for (int i = 0; i < BULLET_NUM; i++) {
		_bullet[i] = new CBullet(BOSS_Y);
		_bullet[i]->SetBulletColor(vec4(0.0f, 0.0f, 1.0f, 1));
		_mxBulletCircleT[i] = Translate(BULLET_RADIUS * cosf(M_PI*2.0f*i / BULLET_NUM), BULLET_RADIUS * sinf(M_PI*2.0f*i / BULLET_NUM), 0.0f);
		_mxBulletCircleR[i] = RotateZ(-90.f + 360.f / BULLET_NUM*i);	//朝向外
		_mxBT_Boss2[i] = _mxMT;

		_faddDelta[i] = 0;
		_fRand[i] = 0;
	}
	_fBAngle = 0;
	_bDirectionDecide = false;
	_bDir = 0;
}
void CSecondBoss::DeleteBulletList()
{
	for (int i = 0; i < BULLET_NUM; i++) delete _bullet[i];
}
void CSecondBoss::ShootBullet(float delta)
{

}
void CSecondBoss::NextBullet()
{

}
void CSecondBoss::SetBulletPassiveMove()
{
	for (int i = 0; i < BULLET_NUM; i++) {
		if (_bullet[i]->_isShoot == false) _bullet[i]->GL_SetTRSMatrix(_mxMT);
	}
}

//----------------------------------------------------------
void CSecondBoss::BulletMoveToCircle(float delta)
{
	for (int i = 0; i < BULLET_NUM; i++) {
		_bullet[i]->_isShoot = true;	//子彈發射

		//TRANSLATE
		if (_mxBT_Boss2[i]._m[0][3] < _mxBulletCircleT[i]._m[0][3]) _mxBT_Boss2[i]._m[0][3] += delta * BULLET_SPEED;	//X
		else _mxBT_Boss2[i]._m[0][3] -= delta * BULLET_SPEED;
		if (_mxBT_Boss2[i]._m[1][3] < _mxBulletCircleT[i]._m[1][3]) _mxBT_Boss2[i]._m[1][3] += delta * BULLET_SPEED;	//Y
		else _mxBT_Boss2[i]._m[1][3] -= delta * BULLET_SPEED;
		_bullet[i]->GL_SetTRSMatrix(_mxMT * _mxBT_Boss2[i] * _mxBulletCircleR[i]);
	}
}
void CSecondBoss::BulletRotate(float delta)
{
	_fBAngle += delta * 1200.f;
	for (int i = 0; i < BULLET_NUM; i++) {
		_mxBR_Boss2[i] = RotateZ(_fBAngle);
		_bullet[i]->GL_SetTRSMatrix(_mxMT * _mxBulletCircleT[i] * _mxBR_Boss2[i] * _mxBulletCircleR[i]);

		//--------下階段使用---------
		_fRand[i] = (rand() % 2 + 1)*0.1f + (rand() % 10)*0.01f;	//1.0 ~ 2.9 之間 隨機速度
		_mxBCurrentT[i] = _bullet[i]->GetTRSMatrix();			//取得子彈目前狀態
	}
}

void CSecondBoss::BulletShootToPlayer(float delta, float player_x)
{
	for (int i = 0; i < BULLET_NUM; i++) {
		_faddDelta[i] += delta * _fRand[i];
		if (!_bDirectionDecide) {	//決定方向
			if (_mxBT_Boss2[i]._m[0][3] < player_x) _bDir = 0; 
			else _bDir = 1;
			_bDirectionDecide = true;
		}

		if (player_x > 2.5f || player_x < -2.5) _fAdjust_x = 1.0;			//微調發射角
		else if (player_x < 0.5 && player_x > -0.5) _fAdjust_x = 0.0f;
		else _fAdjust_x = 0.5;

		if (!_bDir)_mxBT_Boss2[i]._m[0][3] += _faddDelta[i] * _fAdjust_x;	//X
		else _mxBT_Boss2[i]._m[0][3] -= _faddDelta[i] * _fAdjust_x;
		_mxBT_Boss2[i]._m[1][3] -= _faddDelta[i];	//Y
		
		_bullet[i]->GL_SetTRSMatrix(_mxBT_Boss2[i] * _mxBCurrentT[i]);
	}
}

void CSecondBoss::ReSetBullet()
{
	for (int i = 0; i < BULLET_NUM; i++) {
		_bullet[i]->_isShoot = false;	//尚未發射
		_mxBT_Boss2[i] = _mxMT;			//子彈在玩家位置
		_bDirectionDecide = false;		//重設，第三階段使用
		_faddDelta[i] = 0;
		_fRand[i] = 0;
	}
	_fBAngle = 0;
	_bDirectionDecide = false;
	_bDir = 0;
}