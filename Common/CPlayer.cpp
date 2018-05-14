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

	//ATTACK
	float fAKscale = 0.6f;
	for (int i = 0; i < 3; i++) {
		_pAttackStroeStar[i] = new CObjReader("obj/star_little.obj");
		_pAttackStroeStar[i]->SetShader();
		_fAKT[i][1] = -4.0f;
		if (i == 0) _fAKT[i][0] = -0.4f;		//LEFT
		else if (i == 2) _fAKT[i][0] = 0.4f;	//RIGHT
		_mxAKT[i] = Translate(_fAKT[i][0], _fAKT[i][1], _fAKT[i][2]);
		_mxAKS[i] = Scale(fAKscale, fAKscale, fAKscale);
		_pAttackStroeStar[i]->SetTRSMatrix(_mxAKT[i] * _mxAKS[i]);
	}

	//DEFNESE
	_fDEAngle = 0;
	float fDEscale = 0.3f;
	for (int i = 0; i < DEFENSE_NUM; i++) {
		_pDefense[i] = new CObjReader("obj/square.obj");
		_pDefense[i]->SetColor(vec4(1.0f, 1.0f, 1.0f, 1));	//WHITE
		_pDefense[i]->SetShader();
		_mxDET[i] = Translate(DE_RADIUS * cosf(M_PI*2.0f*i / DEFENSE_NUM), DE_RADIUS * sinf(M_PI*2.0f*i / DEFENSE_NUM), 0.0f);
		_mxDES[i] = Scale(fDEscale, fDEscale, fDEscale);
		_mxDER[i] = RotateZ(-45.f + 360.f / DEFENSE_NUM*i);	//���� �¦V�Q�g��
		_pDefense[i]->SetTRSMatrix(_mxPT * _mxDET[i] * _mxDES[i] * _mxDER[i]);
	}

	//--------------------------------------
	_iBulletNum = 0;
	CreateBulletList();	//�l�u
}

CPlayer::~CPlayer()
{
	delete _pPlayer;
	delete _pHPFrame;
	for (int i = 0; i < DEFENSE_NUM; i++) delete _pDefense[i];
	for (int i = 0; i < 3; i++) delete _pAttackStroeStar[i];
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

void CPlayer::GL_DrawAttack(int num)
{
	if (num >= 1) _pAttackStroeStar[0]->Draw();	//�@���P
	if (num >= 2) _pAttackStroeStar[1]->Draw();	//�����P
	if (num >= 3) _pAttackStroeStar[2]->Draw();	//�T���P
}

void CPlayer::GL_DrawDefense()
{
	for (int i = 0; i < DEFENSE_NUM; i++) _pDefense[i]->Draw();
}

void CPlayer::GL_SetTRSMatrix(mat4 &mat)
{
	_pPlayer->SetTRSMatrix(mat);
}

void CPlayer::GL_SetTranslatMatrix(mat4 &mat)
{
	_mxPT = mat;
	_fPT[0] = _mxPT._m[0][3];
	_fPT[1] = _mxPT._m[1][3];
}

void CPlayer::SetViewMatrix(mat4 mvx)
{
	_pPlayer->SetViewMatrix(mvx);		//���a
	_pHPFrame->SetViewMatrix(mvx);		//��q��
	for (int i = 0; i < DEFENSE_NUM; i++) _pDefense[i]->SetViewMatrix(mvx);
	for (int i = 0; i < 3; i++) _pAttackStroeStar[i]->SetViewMatrix(mvx);
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
	for (int i = 0; i < DEFENSE_NUM; i++) _pDefense[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < 3; i++) _pAttackStroeStar[i]->SetProjectionMatrix(mpx);
	_pBGet = _pBHead;					//�l�u��C
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
}

//----------------------------------------------
void CPlayer::UpdateMatrix(float delta)
{
	mat4 mxDER[DEFENSE_NUM];	//���@�n ¶��x�}

	//���@�n
	_fDEAngle += 50.f * delta;
	if (_fDEAngle > 360) _fDEAngle -= 360; //�k�s
	for (int i = 0; i < DEFENSE_NUM; i++) {
		mxDER[i] = RotateZ(_fDEAngle);
		_pDefense[i]->SetTRSMatrix(_mxPT * mxDER[i] * _mxDET[i] * _mxDES[i] * _mxDER[i]);
	}
}

void CPlayer::SetPassiveMotion(float x)
{
	//��q��
	_fHPFT[0] = x / 2.f;
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);

	//ATTACK STORE STAR
	for (int i = 0; i < 3; i++) {
		if (i == 0) _fAKT[i][0] = x - 0.4f;			//LEFT
		else if (i == 2) _fAKT[i][0] = x + 0.4f;	//RIGHT
		else _fAKT[i][0] = x;
		_mxAKT[i] = Translate(_fAKT[i][0], _fAKT[i][1], _fAKT[i][2]);
		_pAttackStroeStar[i]->SetTRSMatrix(_mxAKT[i] * _mxAKS[i]);
	}
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