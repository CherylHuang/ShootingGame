#include "CCamera.h"
#include "CObjReader.h"
#include "CPlayer.h"
#include "CBullet.h"

CBullet::CBullet()
{
	_isShoot = false;

	//產生Obj檔實體物件
	_pBullet = new CObjReader("obj/bullet.obj");
	_pBullet->SetColor(vec4(1.0f, 1.0f, 1.0f, 1));
	_pBullet->SetShader();
	
	_fBT[1] = PLAYER_Y_AXIS;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);

	//_fscale = 0.4f;	//縮放
	_mxBS = Scale(_fscale, _fscale, _fscale);
	_pBullet->SetTRSMatrix(_mxBT * _mxBS);
}

CBullet::CBullet(float y)
{
	_isShoot = false;

	//產生Obj檔實體物件
	_pBullet = new CObjReader("obj/bullet.obj");
	_pBullet->SetColor(vec4(1.0f, 1.0f, 1.0f, 1));
	_pBullet->SetShader();

	_fBT[1] = y;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);

	//_fscale = 0.4f;	//縮放
	_mxBS = Scale(_fscale, _fscale, _fscale);
	_pBullet->SetTRSMatrix(_mxBT * _mxBS);
}

CBullet::~CBullet()
{
	delete _pBullet;
}

//----------------------------------------------------
void CBullet::GL_Draw()
{
	_pBullet->Draw();
}

void CBullet::GL_SetTRSMatrix(mat4 &mat)
{
	_pBullet->SetTRSMatrix(mat);
}

void CBullet::SetViewMatrix(mat4 mvx)
{
	_pBullet->SetViewMatrix(mvx);
}

void CBullet::SetProjectionMatrix(mat4 mpx)
{
	_pBullet->SetProjectionMatrix(mpx);
}

//---------------------------------------
void CBullet::ShootBulletUp(float delta, float passive_x) //向上發射子彈
{
	if (!_isShoot) {		//固定發射時的x座標
		_fBT[0] = passive_x;
		_isShoot = true;
	}
	_fBT[1] += delta * BULLET_SPEED;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT);
}

void CBullet::ShootBulletDown(float delta, float passive_x) //向下發射子彈
{
	_mxBR = RotateZ(180.0f);
	if (!_isShoot) {		//固定發射時的x座標
		_fBT[0] = passive_x;
		_isShoot = true;
	}
	_fBT[1] -= delta * BULLET_SPEED;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT * _mxBR);
}

void CBullet::ShootBulletDown(float delta, float passive_x, mat4 &mxS) //向下發射子彈
{
	_mxBR = RotateZ(180.0f);
	if (!_isShoot) {		//固定發射時的x座標
		_fBT[0] = passive_x;
		_isShoot = true;
	}
	_fBT[1] -= delta * BULLET_SPEED;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT * _mxBR * mxS);
}

void CBullet::ResetBullet(float fPTx)
{
	_fBT[0] = fPTx;
	_fBT[1] = PLAYER_Y_AXIS;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT);
}

void CBullet::ResetBullet(float x, float y)
{
	_fBT[0] = x; _fBT[1] = y;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT);
}

void CBullet::ResetBullet(mat4 &mat, float x, float y)
{
	_fBT[0] = x; _fBT[1] = y;
	_mxBT = Translate(_fBT[0], _fBT[1], _fBT[2]);
	_pBullet->SetTRSMatrix(_mxBT * mat);
}

void CBullet::SetBulletColor(vec4 vColor)
{
	_pBullet->SetColor(vColor);
}