#include "CHealthPoints.h"

CHealthPoints::CHealthPoints(float y_axis, float x_scale)
{
	//產生Obj檔實體物件
	//HP BAR
	_pHP = new CObjReader("obj/rect_solid.obj");
	_pHP->SetColor(vec4(1.0f, 0.0f, 0.0f, 1));		//RED
	_pHP->SetShader();
	_fHPT[1] = y_axis; //y座標
	_mxHPT = Translate(_fHPT[0], _fHPT[1], _fHPT[2]);
	_fHPS[0] = x_scale;		//X
	_fHPS[1] = HP_Y_SCALE;	//Y
	_fHPS[2] = 1.0f;		//Z
	_mxHPS = Scale(_fHPS[0], _fHPS[1], _fHPS[2]);	//縮放
	_pHP->SetTRSMatrix(_mxHPT * _mxHPS);

	_mxHPT_adjust = Translate(vec4(0.0f, 0.0f, 0.0f, 0.0f));	//初始化調整矩陣
}

CHealthPoints::~CHealthPoints()
{
	if (_pHP != NULL) delete _pHP;
}

//----------------------------------------------------
void CHealthPoints::GL_SetTranslatMatrix(mat4 &mat)
{
	_mxHPT = mat;
	_fHPT[0] = _mxHPT._m[0][3];
	_fHPT[1] = _mxHPT._m[1][3];
}

void CHealthPoints::GL_SetAdjustTranslatMatrix(mat4 &mat)
{
	_mxHPT_adjust = mat;
}

void CHealthPoints::GL_SetTRSMatrix(mat4 &mat)
{
	_pHP->SetTRSMatrix(mat);
}

//----------------------------------------------------------

void CHealthPoints::UpdateMatrix(float delta)
{
	_pHP->SetTRSMatrix(_mxHPT * _mxHPT_adjust * _mxHPS);

	//mat4 mx = _pHP->GetTRSMatrix(); //////////////////////
	//printf("%f, %f\n", mx._m[0][3], mx._m[1][3]);
}

void CHealthPoints::GL_Draw()
{
	_pHP->Draw();
}

void CHealthPoints::SetViewMatrix(mat4 &mvx)
{
	_pHP->SetViewMatrix(mvx);
}

void CHealthPoints::SetProjectionMatrix(mat4 &mpx)
{
	_pHP->SetProjectionMatrix(mpx);
}

//----------------------------------------------------
mat4 CHealthPoints::GetTRSMatrix()
{
	return(_pHP->GetTRSMatrix());
}

void CHealthPoints::SetColor(vec4 vColor)
{
	_pHP->SetColor(vColor);
}