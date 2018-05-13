#include "CFirstBoss.h"

CFirstBoss::CFirstBoss()
{
	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk1.obj");
	_pMainBody->SetColor(vec4(-1.0f, 0.0f, 0.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = 5;				//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	float BigShootScale = 5.0f;
	_mxBS = Scale(BigShootScale, BigShootScale, BigShootScale);	//for first big shoot
	_mxBR = RotateZ(180.0f);	//for bullet rotate
	_iBulletNum = 0;
	_isFirstBigShoot = false;
	CreateBulletList();			//子彈
}

CFirstBoss::~CFirstBoss()
{
	DeleteBulletList();		//子彈
}

//----------------------------------------------
void CFirstBoss::UpdateMatrix(float)
{

}
void CFirstBoss::GL_Draw()
{
	_pMainBody->Draw();

	//子彈顯示
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		if(_pBGet->_isShoot) _pBGet->GL_Draw();		//只顯示射出的子彈
		_pBGet = _pBGet->link;
	}
}
void CFirstBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}
void CFirstBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
}

//----------------------------------------------
void CFirstBoss::CreateBulletList()	//建立子彈串列
{
	//first node
	_pBHead = new CBullet(_fMT[1]);
	_pBHead->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//子彈發射用
	_pBHead->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置
	_iBulletNum++;	//子彈數量紀錄

	//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet(_fMT[1])) == NULL) {
			printf("記憶體不足\n"); exit(0);
		}
		_pBGet->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));
		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置
		_iBulletNum++;	//子彈數量紀錄
	}
}

void CFirstBoss::DeleteBulletList()	//刪除子彈串列
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//子彈數量歸零
}

void CFirstBoss::SetBulletPassiveMove()
{
	_pBGet = _pBHead;	//子彈串列
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //子彈尚未射出
			_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR);
		}
		_pBGet = _pBGet->link;
	}
}

void CFirstBoss::ShootBullet(float delta)
{
	_pBGet_shoot = _pBHead_shoot;
	if (_isFirstBigShoot) {				//第一發子彈
		_pBGet_shoot->ShootBulletDown(delta, _fMT[0], _mxBS);
	}
	else _pBGet_shoot->ShootBulletDown(delta, _fMT[0]);
	_pBGet_shoot->_isShoot = true;		//子彈射出
}

void CFirstBoss::NextBullet()
{
	_isFirstBigShoot = false;	//非第一發子彈
	_iBulletNum--;				//子彈數量紀錄
	_pBHead_shoot = _pBHead_shoot->link;
	_pBGet_shoot->_isShoot = false;

	if (_iBulletNum == 0) {	//沒有子彈
		_isFirstBigShoot = true;	//下一個子彈為第一發
		_pBHead_shoot = _pBHead;
		_pBGet_shoot = _pBHead_shoot;
		while (_pBGet_shoot != nullptr) {
			_pBGet_shoot->ResetBullet(_mxBR, _fMT[0], _fMT[1]); //子彈歸位
			_pBGet_shoot = _pBGet_shoot->link;
			_iBulletNum++; //子彈數量紀錄
		}
	}
}