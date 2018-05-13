#include "CSecondBoss.h"

CSecondBoss::CSecondBoss()
{
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
}

CSecondBoss::~CSecondBoss()
{
	for (int i = 0; i < BIG_WHEEL_NUM; i++) delete _pWheel[i]; //拿的時候一個一個拿->一個一個刪
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) delete _pSWheel[i];
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
}

void CSecondBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetViewMatrix(mvx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetViewMatrix(mvx);
}

void CSecondBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetProjectionMatrix(mpx);
}

//-----------------------------------------------------------------
void CSecondBoss::CreateBulletList()
{

}
void CSecondBoss::DeleteBulletList()
{

}
void CSecondBoss::ShootBullet(float delta)
{

}
void CSecondBoss::NextBullet()
{

}
void CSecondBoss::SetBulletPassiveMove()
{

}