#include <time.h>
#include "CLittleEnemy.h"

CLittleEnemy::CLittleEnemy()
{
	srand((unsigned)time(NULL));
	_fTrackSpeed = 1.5f;

	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk4.obj");
	_pMainBody->SetColor(vec4(0.0f, 0.0f, -1.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;				//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	_mxBR = RotateZ(180.0f);	//for bullet rotate
	_iBulletNum = 0;
	CreateBulletList();			//子彈
}

CLittleEnemy::CLittleEnemy(int RandomColor)
{
	//產生Obj檔實體物件
	_pMainBody = new CObjReader("obj/cyberpunk4.obj");
	if(RandomColor == 0) _pMainBody->SetColor(vec4(-1.0f, 0.0f, 0.0f, 1));		//隨機顏色
	else if (RandomColor == 1) _pMainBody->SetColor(vec4(0.0f, -1.0f, 0.0f, 1));
	else _pMainBody->SetColor(vec4(0.0f, 0.0f, -1.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;				//y座標
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	_mxBR = RotateZ(180.0f);	//for bullet rotate
	_iBulletNum = 0;
	CreateBulletList(RandomColor);	//隨機顏色 子彈
}

CLittleEnemy::~CLittleEnemy()
{
	DeleteBulletList();		//子彈
}

//----------------------------------------------
void CLittleEnemy::UpdateMatrix(float delta)
{
	//float speed = 1.5f;	//路徑速度
	_fMAngle_Track += _fTrackSpeed * delta;
	if (_fMAngle_Track > 360) _fMAngle_Track -= 360; //歸零

	float sint, cost, sin2t, cos2t, sin3t, cos3t, cos4t, sin12t, cost2, sin5t2; //---------定義----------
	sint = sinf(_fMAngle_Track);			sin2t = sinf(2 * _fMAngle_Track);
	cost = cosf(_fMAngle_Track);			cos2t = cosf(2 * _fMAngle_Track);
	cos3t = cosf(3 * _fMAngle_Track);		cos4t = cosf(4 * _fMAngle_Track);
	cost2 = cosf(_fMAngle_Track / 2.f);		sin5t2 = sinf(5 * _fMAngle_Track / 2.f);
	sin3t = sinf(3 * _fMAngle_Track);
	float fsize = 1.0f;
	_fTrack[0] = (cost * sin3t)*fsize;				//x
	_fTrack[1] = (sint * sin3t)*fsize;				//y
	_mxTrack = Translate(_fTrack[0], _fTrack[1], _fTrack[2]);	//路徑移動

	//物件
	_pMainBody->SetTRSMatrix(_mxMT * _mxTrack);
}
void CLittleEnemy::GL_Draw()
{
	_pMainBody->Draw();

	//子彈顯示
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot) _pBGet->GL_Draw();		//只顯示射出的子彈
		_pBGet = _pBGet->link;
	}
}

void CLittleEnemy::GL_SetTRSMatrix(mat4 &mat)
{
	_mxMT = mat; 
	_fMT[0] = _mxMT._m[0][3];
	_fMT[1] = _mxMT._m[1][3];
	_pMainBody->SetTRSMatrix(mat);
}

void CLittleEnemy::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}
void CLittleEnemy::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
}

//----------------------------------------------
void CLittleEnemy::CreateBulletList()	//建立子彈串列
{
	//first node
	_pBHead = new CBullet(_fMT[1]);
	_pBHead->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));	//YELLOW
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//子彈發射用
	_pBHead->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置

	float fspeed = rand() % 20 + 13.f;
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

void CLittleEnemy::CreateBulletList(int RandomColor)	//建立子彈串列 子彈隨機顏色
{
	//first node
	_pBHead = new CBullet(_fMT[1]);

	//隨機顏色
	if (RandomColor == 0) _pBHead->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));		//YELLOW		
	else if (RandomColor == 1) _pBHead->SetBulletColor(vec4(0.0f, 0.0f, 1.0f, 1));	//BLUE
	else _pBHead->SetBulletColor(vec4(1.0f, 0.0f, 0.0f, 1));						//RED

	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//子彈發射用
	_pBHead->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置

	float fspeed = rand() % 20 + 13.f;
	_pBHead->_fBulletSpeed = fspeed;

	_iBulletNum++;	//子彈數量紀錄

					//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet(_fMT[1])) == NULL) {
			printf("記憶體不足\n"); exit(0);
		}

		//隨機顏色
		if (RandomColor == 0) _pBGet->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));		//YELLOW		
		else if (RandomColor == 1) _pBGet->SetBulletColor(vec4(0.0f, 0.0f, 1.0f, 1));	//BLUE
		else _pBGet->SetBulletColor(vec4(1.0f, 0.0f, 0.0f, 1));						//RED

		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR); //設定子彈至BOSS位置
		_pBGet->_fBulletSpeed = fspeed;
		_iBulletNum++;	//子彈數量紀錄
	}
}

void CLittleEnemy::DeleteBulletList()	//刪除子彈串列
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//子彈數量歸零
}

void CLittleEnemy::SetBulletPassiveMove()
{
	_pBGet = _pBHead;	//子彈串列
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //子彈尚未射出
			_pBGet->GL_SetTRSMatrix(_mxMT * _mxTrack * _mxBR);
		}
		_pBGet = _pBGet->link;
	}
}

void CLittleEnemy::ShootBullet(float delta)
{
	_pBGet_shoot = _pBHead_shoot;
	_pBGet_shoot->ShootBulletDown(delta, _fMT[0]);
	_mxBT = _pBGet_shoot->GetTranslateMatrix();	//更新子彈位置
	_pBGet_shoot->_isShoot = true;		//子彈射出
}

void CLittleEnemy::NextBullet()
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
}