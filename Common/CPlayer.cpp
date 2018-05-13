#include "../header/Angel.h"
#include "CCamera.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	//產生Obj檔實體物件
	//PLAYER
	_pPlayer = new CObjReader("obj/jet.obj");
	_pPlayer->SetShader();
	_fPT[1] = PLAYER_Y_AXIS; //y座標
	_mxPT = Translate(_fPT[0], _fPT[1], _fPT[2]);
	_fPlayerScale = 0.4f;	//縮放
	_mxPS = Scale(_fPlayerScale, _fPlayerScale, _fPlayerScale);
	_pPlayer->SetTRSMatrix(_mxPT * _mxPS);

	//BLOOD FRAME
	_pHPFrame = new CObjReader("obj/square.obj");
	_pHPFrame->SetShader();
	_fHPFT[1] = -21.f;		//jet下方
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_fHPFS[0] = 10.f; _fHPFS[1] = 2.f; _fHPFS[2] = 1.f;
	_mxHPFS = Scale(_fHPFS[0], _fHPFS[1], _fHPFS[2]);	//縮放
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);

	_iBulletNum = 0;
	CreateBulletList();	//子彈
}

CPlayer::~CPlayer()
{
	delete _pPlayer;
	delete _pHPFrame;
	DeleteBulletList();		//子彈
}

//-------------------------------------------
void CPlayer::GL_Draw()
{
	_pPlayer->Draw();
	_pHPFrame->DrawW();

	//子彈顯示
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		_pBGet->GL_Draw();
		_pBGet = _pBGet->link;
	}
}

void CPlayer::GL_SetTRSMatrix(mat4 &mat)
{
	_mxPT = mat;
	_fPT[0] = _mxPT._m[0][3];
	_fPT[1] = _mxPT._m[1][3];
	_pPlayer->SetTRSMatrix(mat);
}

void CPlayer::SetViewMatrix(mat4 mvx)
{
	_pPlayer->SetViewMatrix(mvx);		//玩家
	_pHPFrame->SetViewMatrix(mvx);		//血量框
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}

void CPlayer::SetProjectionMatrix(mat4 mpx)
{
	_pPlayer->SetProjectionMatrix(mpx);	//玩家
	_pHPFrame->SetViewMatrix(mpx);		//血量框
	_pBGet = _pBHead;					//子彈串列
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
}

//----------------------------------------------
void CPlayer::SetHPPassiveMotion(float x)
{
	_fHPFT[0] = x / 2.f;
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);
}

float CPlayer::GetPlayerScale()
{
	return _fPlayerScale;
}

//----------------------------------------------
void CPlayer::CreateBulletList()	//建立子彈串列
{
	//first node
	_pBHead = new CBullet;
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//子彈發射用
	_iBulletNum++;	//子彈數量紀錄

					//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet) == NULL) {
			printf("記憶體不足\n"); exit(0);
		}
		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_iBulletNum++;	//子彈數量紀錄
	}
}

void CPlayer::DeleteBulletList()	//刪除子彈串列
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//子彈數量歸零
}

void CPlayer::SetBulletPassiveMove(mat4 &mat)
{
	_pBGet = _pBHead;	//子彈串列
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //子彈尚未射出
			_pBGet->GL_SetTRSMatrix(mat);
		}
		_pBGet = _pBGet->link;
	}
}

void CPlayer::ShootBullet(float delta, float passive_x)
{
	_pBGet_shoot = _pBHead_shoot;
	_pBGet_shoot->ShootBulletUp(delta, passive_x);
	_pBGet_shoot->_isShoot = true;		//子彈射出
}

void CPlayer::NextBullet(float g_fPTx)
{
	_iBulletNum--; //子彈數量紀錄
	_pBHead_shoot = _pBHead_shoot->link;
	_pBGet_shoot->_isShoot = false;

	if (_iBulletNum == 0) {	//沒有子彈
		_pBHead_shoot = _pBHead;
		_pBGet_shoot = _pBHead_shoot;
		while (_pBGet_shoot != nullptr) {
			_pBGet_shoot->ResetBullet(g_fPTx); //子彈歸位
			_pBGet_shoot = _pBGet_shoot->link;
			_iBulletNum++; //子彈數量紀錄
		}
	}
}