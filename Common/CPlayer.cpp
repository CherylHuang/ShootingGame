#include "../header/Angel.h"
#include "CCamera.h"
#include "CPlayer.h"

CPlayer::CPlayer()
{
	//����Obj�ɹ��骫��
	//PLAYER
	_pPlayer = new CObjReader("obj/jet.obj");
	_pPlayer->SetShader();
	_fPT[1] = PLAYER_Y_AXIS; //y�y��
	_mxPT = Translate(_fPT[0], _fPT[1], _fPT[2]);
	_fPlayerScale = 0.4f;	//�Y��
	_mxPS = Scale(_fPlayerScale, _fPlayerScale, _fPlayerScale);
	_pPlayer->SetTRSMatrix(_mxPT * _mxPS);

	//BLOOD FRAME
	_pHPFrame = new CObjReader("obj/square.obj");
	_pHPFrame->SetShader();
	_fHPFT[1] = -21.f;		//jet�U��
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_fHPFS[0] = 10.f; _fHPFS[1] = 2.f; _fHPFS[2] = 1.f;
	_mxHPFS = Scale(_fHPFS[0], _fHPFS[1], _fHPFS[2]);	//�Y��
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);

	_iBulletNum = 0;
	CreateBulletList();	//�l�u
}

CPlayer::~CPlayer()
{
	delete _pPlayer;
	delete _pHPFrame;
	DeleteBulletList();		//�l�u
}

//-------------------------------------------
void CPlayer::GL_Draw()
{
	_pPlayer->Draw();
	_pHPFrame->DrawW();

	//�l�u���
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
	_pPlayer->SetViewMatrix(mvx);		//���a
	_pHPFrame->SetViewMatrix(mvx);		//��q��
	_pBGet = _pBHead;					//�l�u��C
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}

void CPlayer::SetProjectionMatrix(mat4 mpx)
{
	_pPlayer->SetProjectionMatrix(mpx);	//���a
	_pHPFrame->SetViewMatrix(mpx);		//��q��
	_pBGet = _pBHead;					//�l�u��C
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
void CPlayer::CreateBulletList()	//�إߤl�u��C
{
	//first node
	_pBHead = new CBullet;
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//�l�u�o�g��
	_iBulletNum++;	//�l�u�ƶq����

					//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet) == NULL) {
			printf("�O���餣��\n"); exit(0);
		}
		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_iBulletNum++;	//�l�u�ƶq����
	}
}

void CPlayer::DeleteBulletList()	//�R���l�u��C
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//�l�u�ƶq�k�s
}

void CPlayer::SetBulletPassiveMove(mat4 &mat)
{
	_pBGet = _pBHead;	//�l�u��C
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //�l�u�|���g�X
			_pBGet->GL_SetTRSMatrix(mat);
		}
		_pBGet = _pBGet->link;
	}
}

void CPlayer::ShootBullet(float delta, float passive_x)
{
	_pBGet_shoot = _pBHead_shoot;
	_pBGet_shoot->ShootBulletUp(delta, passive_x);
	_pBGet_shoot->_isShoot = true;		//�l�u�g�X
}

void CPlayer::NextBullet(float g_fPTx)
{
	_iBulletNum--; //�l�u�ƶq����
	_pBHead_shoot = _pBHead_shoot->link;
	_pBGet_shoot->_isShoot = false;

	if (_iBulletNum == 0) {	//�S���l�u
		_pBHead_shoot = _pBHead;
		_pBGet_shoot = _pBHead_shoot;
		while (_pBGet_shoot != nullptr) {
			_pBGet_shoot->ResetBullet(g_fPTx); //�l�u�k��
			_pBGet_shoot = _pBGet_shoot->link;
			_iBulletNum++; //�l�u�ƶq����
		}
	}
}