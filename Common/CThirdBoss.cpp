#include "CThirdBoss.h"

CThirdBoss::CThirdBoss()
{
	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk3-main-body.obj"); //cyberpunk3-main-body
	_pMainBody->SetColor(vec4(0.0f, 0.0f, -1.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = 4.5f;	//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]); // 0
	_pMainBody->SetTRSMatrix(_mxMT);

	//產生子物件 輪子
	for (int i = 0; i < 2; i++) {
		_pWheel[i] = new CObjReader("obj/cyberpunk3-propeller.obj");
		_pWheel[i]->SetColor(vec4(0.0f, 0.0f, -1.0f, 1));
		_pWheel[i]->SetShader();
		if(i == 0) _fWT[0] = -1.63f;	//Left wheel
		else _fWT[0] = 1.63f;			//Right wheel
		_fWT[1] = 0.51f;				//wheel y-axis
		_mxWT[i] = Translate(_fWT[0], _fWT[1], _fWT[2]);
		_pWheel[i]->SetTRSMatrix(_mxMT * _mxWT[i]);
	}
}

CThirdBoss::~CThirdBoss()
{
	for (int i = 0; i < 2; i++) delete _pWheel[i];
}

//----------------------------------------------
void CThirdBoss::UpdateMatrix(float delta)
{
	//Main Body
	_pMainBody->SetTRSMatrix(_mxMT);

	//Wheel
	mat4 mxWR[2];
	_fWAngle += 200.f * delta;
	if (_fWAngle > 360) _fWAngle -= 360; //歸零
	mxWR[0] = RotateZ(_fWAngle);
	mxWR[1] = RotateZ(-_fWAngle);
	for (int i = 0; i < 2; i++) {
		_pWheel[i]->SetTRSMatrix(_mxMT * _mxWT[i] * mxWR[i]);
	}
}
void CThirdBoss::GL_Draw()
{
	_pMainBody->Draw();
	for (int i = 0; i < 2; i++) _pWheel[i]->DrawW();
}
void CThirdBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	for (int i = 0; i < 2; i++) _pWheel[i]->SetViewMatrix(mvx);
}
void CThirdBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	for (int i = 0; i < 2; i++) _pWheel[i]->SetProjectionMatrix(mpx);
}