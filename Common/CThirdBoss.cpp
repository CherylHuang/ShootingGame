#include <time.h>
#include "CThirdBoss.h"

CThirdBoss::CThirdBoss()
{
	srand((unsigned)time(NULL));

	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk3-main-body.obj"); //cyberpunk3-main-body
	_pMainBody->SetColor(vec4(0.0f, 0.0f, -1.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;	//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	//子彈
	float BigShootScale = 3.0f;
	_mxBS = Scale(BigShootScale, BigShootScale, BigShootScale);	//for first big shoot
	_mxBR = RotateZ(180.0f);	//for bullet rotate
	_iBulletNum = 0;
	CreateBulletList();

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

	//產生小怪
	for (int i = 0; i < LITTLE_NUM; i++) {
		int RandomColor = rand() % 3;
		_pLittleEnemy[i] = new CLittleEnemy(RandomColor);
		_pLittleEnemy[i]->_fTrackSpeed = 0.5f + (rand() % 10) * 0.1f;		//隨機路徑速度
		_fLT[i][0] = -HALF_SCREEN_X + 1.f + (rand() % (HALF_SCREEN_X * 2 - 2) + (rand() % 10)*0.1);	//x座標
		_fLT[i][1] = rand() % (HALF_SCREEN_Y - 2) + (rand() % 10)*0.1;		//y座標
		_mxLT[i] = Translate(_fLT[i][0], _fLT[i][1], _fLT[i][2]);
		_pLittleEnemy[i]->GL_SetTRSMatrix(_mxLT[i]);
	}
}

CThirdBoss::~CThirdBoss()
{
	DeleteBulletList();		//子彈
	for (int i = 0; i < 2; i++) delete _pWheel[i];
	for (int i = 0; i < LITTLE_NUM; i++) delete _pLittleEnemy[i];
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

	//Little Enemy
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->UpdateMatrix(delta);
}
void CThirdBoss::GL_Draw()
{
	_pMainBody->Draw();

	//子彈顯示
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot) _pBGet->GL_Draw();		//只顯示射出的子彈
		_pBGet = _pBGet->link;
	}

	for (int i = 0; i < 2; i++) _pWheel[i]->DrawW();
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->GL_Draw();
}
void CThirdBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
	for (int i = 0; i < 2; i++) _pWheel[i]->SetViewMatrix(mvx);
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->SetViewMatrix(mvx);
}
void CThirdBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
	for (int i = 0; i < 2; i++) _pWheel[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->SetProjectionMatrix(mpx);
}

//-----------------------------------------------------------------
void CThirdBoss::CreateBulletList()
{
	//first node
	_pBHead = new CBullet(_fMT[1]);
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//子彈發射用
	_pBHead->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置
	
	float fspeed = rand() % 20 + 10.f;
	_pBHead->_fBulletSpeed = fspeed;

	_iBulletNum++;	//子彈數量紀錄

	//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet(_fMT[1])) == NULL) {
			printf("記憶體不足\n"); exit(0);
		}
		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置
		_pBGet->_fBulletSpeed = fspeed;
		_iBulletNum++;	//子彈數量紀錄
	}
}
void CThirdBoss::DeleteBulletList()
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//子彈數量歸零
}
void CThirdBoss::SetBulletPassiveMove()
{
	_pBGet = _pBHead;	//子彈串列
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //子彈尚未射出
			_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR);
		}
		_pBGet = _pBGet->link;
	}

	//Little Enemy
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->SetBulletPassiveMove();
}
void CThirdBoss::ShootBullet(float delta)
{
	_pBGet_shoot = _pBHead_shoot;
	_pBGet_shoot->ShootBulletDown(delta, _fMT[0], _mxBS);
	_mxBT = _pBGet_shoot->GetTranslateMatrix();	//更新子彈位置
	_pBGet_shoot->_isShoot = true;		//子彈射出

	//Little Enemy
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->ShootBullet(delta);
}
void CThirdBoss::NextBullet()
{
	_iBulletNum--;				//子彈數量紀錄
	_pBHead_shoot = _pBHead_shoot->link;
	_pBGet_shoot->_isShoot = false;

	if (_iBulletNum == 0) {	//沒有子彈
		_pBHead_shoot = _pBHead;
		_pBGet_shoot = _pBHead_shoot;
		while (_pBGet_shoot != nullptr) {
			_pBGet_shoot->ResetBullet(_mxBR, _fMT[0], _fMT[1]); //子彈歸位
			_pBGet_shoot = _pBGet_shoot->link;
			_iBulletNum++; //子彈數量紀錄
		}
	}

	//Little Enemy
	for (int i = 0; i < LITTLE_NUM; i++) _pLittleEnemy[i]->NextBullet();
}

//----------------------------------------------------------------
mat4 CThirdBoss::GetLETranslateMatrix(int n)
{
	return _mxLT[n];
}

mat4 CThirdBoss::GetLEBulletTranslateMatrix(int n)
{
	return _pLittleEnemy[n]->GetBulletTranslateMatrix();
}