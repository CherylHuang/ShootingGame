#include "CEnemy.h"

CEnemy::CEnemy()
{
	_pMainBody = NULL;
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]); // 0
}

CEnemy::~CEnemy()
{
	if (_pMainBody != NULL) delete _pMainBody;
}

mat4 CEnemy::GetTranslateMatrix()
{
	return _mxMT;
}

mat4 CEnemy::GetBulletTranslateMatrix()
{
	return _mxBT;
}