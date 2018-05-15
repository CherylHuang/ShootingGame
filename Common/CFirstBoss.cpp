#include "CFirstBoss.h"

CFirstBoss::CFirstBoss()
{
	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk1.obj");
	_pMainBody->SetColor(vec4(-1.0f, 0.0f, 0.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;				//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	_mxBS = Scale(BIG_BULLET_SCALE, BIG_BULLET_SCALE, BIG_BULLET_SCALE);	//for first big shoot
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
void CFirstBoss::UpdateMatrix(float delta)
{
	float speed = 3.5f;	//大輪速度
	_fMAngle_Track += speed * delta;
	if (_fMAngle_Track > 360) _fMAngle_Track -= 360; //歸零

	float sint, cost, sin2t, cos2t, cos3t, cos4t, sin12t, cost2, sin5t2; //---------定義----------
	sint = sinf(_fMAngle_Track);			sin2t = sinf(2 * _fMAngle_Track);
	cost = cosf(_fMAngle_Track);			cos2t = cosf(2 * _fMAngle_Track);
	cos3t = cosf(3 * _fMAngle_Track);		cos4t = cosf(4 * _fMAngle_Track);
	cost2 = cosf(_fMAngle_Track / 2.f);		sin5t2 = sinf(5 * _fMAngle_Track / 2.f);
	float fsize = 1.0f;
	_fMT[0] = (cost * 3 * cost2)*fsize;				//x
	_fMT[1] = (sint * 3 * cost2)*fsize + BOSS_Y;	//y
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);	//路徑移動

	//HEART
	//x = 16 * sint*sint*sint
	//y = 13 * cost - 5 * cos2t - 2 * cos3t - cos4t
	//FLOWER
	//x = 15 * cost * sin2t  (15 : scale)
	//y = 15 * sint * sin2t
	//BUTTERFLY
	//x = sint*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//y = cost*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//INFINITY
	//x = cost * sqrt(9 * cos2t)
	//y = sint * sqrt(9 * cos2t)
	//UNTITLED 1
	//x = cost * 3 * cost2
	//y = sint * 3 * cost2
	//UNTITLED 2
	//x = cost * sint * sin5t2
	//y = sint * sint * sin5t2

	//物件
	_pMainBody->SetTRSMatrix(_mxMT);
}
void CFirstBoss::GL_Draw()
{
	_pMainBody->Draw();

	//子彈顯示
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot) _pBGet->GL_Draw();		//只顯示射出的子彈
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

	float fspeed = rand() % 10 + 23.f;
	_pBHead->_fBulletSpeed = fspeed;

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
		_pBGet->_fBulletSpeed = fspeed;
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

	_mxBT = _pBGet_shoot->GetTranslateMatrix();	//更新子彈位置

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

bool CFirstBoss::GetFirstShoot()
{
	return _isFirstBigShoot;
}