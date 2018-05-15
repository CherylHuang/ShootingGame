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

	//HP FRAME
	_pHPFrame = new CObjReader("obj/square.obj");
	_pHPFrame->SetShader();
	_fHPFT[1] = -21.f;		//jet�U��
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_fHPFS[0] = 10.f; _fHPFS[1] = 2.f; _fHPFS[2] = 1.f;
	_mxHPFS = Scale(_fHPFS[0], _fHPFS[1], _fHPFS[2]);	//�Y��
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);

	//HP BAR
	_fHPMoveT_x = 0;		//�վ㲾��
	_fHPMoveS_x = 0.9f;		//���Y��
	_fHPS[0] = 0.65f;		//X�b�Y��
	_fHPS[1] = HP_Y_SCALE;	//Y
	_fHPS[2] = 1.0f;		//Z
	_mxHPS = Scale(_fHPS[0], _fHPS[1], _fHPS[2]);			//�Y��matrix
	_fHPT[1] = _fPT[1] - 1.f;
	_mxHPT = Translate(_fHPT[0], _fHPT[1], _fHPT[2]);		//�첾Matrix
	_pPlayerHP = new CHealthPoints(_fHPT[1], _fHPS[0]);		//���a���

	//ATTACK SOTRE STAR
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

	//ATTACK MISSILE
	_bMissileIsShoot = false;	//���o�g�ɼu
	_pAttackMissile = new CObjReader("obj/star.obj");
	_pAttackMissile->SetShader();
	_fAKMT[1] = PLAYER_Y_AXIS; //y�y��
	_mxAKMT = Translate(_fAKMT[0], _fAKMT[1], _fAKMT[2]);
	_pAttackMissile->SetTRSMatrix(_mxAKMT);

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
	delete _pPlayerHP;
	for (int i = 0; i < DEFENSE_NUM; i++) delete _pDefense[i];
	for (int i = 0; i < 3; i++) delete _pAttackStroeStar[i];
	delete _pAttackMissile;	//�ɼu
	DeleteBulletList();		//�l�u
}

//-------------------------------------------
void CPlayer::GL_Draw()
{
	_pPlayer->Draw();
	_pHPFrame->DrawW();
	_pPlayerHP->GL_Draw();
	if(_bMissileIsShoot) _pAttackMissile->Draw();	//�ɼu�o�g���~���

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
	_pPlayer->SetViewMatrix(mvx);			//���a
	_pHPFrame->SetViewMatrix(mvx);			//��q��
	_pPlayerHP->SetViewMatrix(mvx);			//HP BAR
	for (int i = 0; i < DEFENSE_NUM; i++) _pDefense[i]->SetViewMatrix(mvx);
	for (int i = 0; i < 3; i++) _pAttackStroeStar[i]->SetViewMatrix(mvx);
	_pAttackMissile->SetViewMatrix(mvx);	//�ɼu

	_pBGet = _pBHead;						//�l�u��C
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}

void CPlayer::SetProjectionMatrix(mat4 mpx)
{
	_pPlayer->SetProjectionMatrix(mpx);			//���a
	_pHPFrame->SetViewMatrix(mpx);				//��q��
	_pPlayerHP->SetProjectionMatrix(mpx);		//HP BAR
	for (int i = 0; i < DEFENSE_NUM; i++) _pDefense[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < 3; i++) _pAttackStroeStar[i]->SetProjectionMatrix(mpx);
	_pAttackMissile->SetProjectionMatrix(mpx);	//�ɼu

	_pBGet = _pBHead;							//�l�u��C
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

	//------------
	_pPlayerHP->UpdateMatrix(delta);			//��s���
	_mxHP_Pos = _pPlayerHP->GetTRSMatrix();		//���o�����m
}

void CPlayer::SetPassiveMotion(float x)
{
	//��q��
	_fHPFT[0] = x / 2.f;
	_mxHPFT = Translate(_fHPFT[0], _fHPFT[1], _fHPFT[2]);
	_pHPFrame->SetTRSMatrix(_mxHPFS * _mxHPFT);

	//HP BAR
	_fHPT[0] = x;
	_mxHPT = Translate(_fHPT[0], _fHPT[1], _fHPT[2]);
	_pPlayerHP->GL_SetTranslatMatrix(_mxHPT);

	//ATTACK STORE STAR
	for (int i = 0; i < 3; i++) {
		if (i == 0) _fAKT[i][0] = x - 0.4f;			//LEFT
		else if (i == 2) _fAKT[i][0] = x + 0.4f;	//RIGHT
		else _fAKT[i][0] = x;
		_mxAKT[i] = Translate(_fAKT[i][0], _fAKT[i][1], _fAKT[i][2]);
		_pAttackStroeStar[i]->SetTRSMatrix(_mxAKT[i] * _mxAKS[i]);
	}

	//MISSILE
	if (!_bMissileIsShoot) {	//���o�g ���H���a
		_fAKMT[0] = x; //x�y��
		_fAKMT[1] = PLAYER_Y_AXIS; //y�y��
		_mxAKMT = Translate(_fAKMT[0], _fAKMT[1], _fAKMT[2]);
		_pAttackMissile->SetTRSMatrix(_mxAKMT);
	}
}

void CPlayer::ShootMissile(float delta, float passive_x, mat4 &mxBossPos, int starNum)		//�g�X�ɼu
{
	//SCALE----------------------------
	if(starNum == 1) _fMissileScale = 0.5f;			//�ھڻW�O���Y��
	else if(starNum == 2) _fMissileScale = 0.8f;
	else if (starNum == 3) _fMissileScale = 1.2f;
	_mxAKMS = Scale(_fMissileScale, _fMissileScale, _fMissileScale);

	//TRANSLATE------------------------
	if (!_bMissileIsShoot && _mxAKMT != mxBossPos) {
		_fAKMT[0] = passive_x;		//�T�w�o�g�ɪ�x�y��
		_bMissileIsShoot = true;
	}

	//BOSS X�b > �ɼu X�b
	if (mxBossPos._m[0][3] > _fAKMT[0])_fAKMT[0] += delta * (MISSILE_SPEED - 10.f);	//�L��X�b�t��
	else _fAKMT[0] -= delta * (MISSILE_SPEED - 10.f);
	//BOSS Y�b > �ɼu Y�b
	if (mxBossPos._m[1][3] > _fAKMT[1])_fAKMT[1] += delta * MISSILE_SPEED;
	else _fAKMT[1] -= delta * MISSILE_SPEED;
	_mxAKMT = Translate(_fAKMT[0], _fAKMT[1], _fAKMT[2]);

	//�ɼu�PBOSS�ۮt�Z��
	float fDis_x, fDis_y;
	fDis_x = mxBossPos._m[0][3] - _fAKMT[0];
	if (fDis_x < 0) fDis_x = -fDis_x;
	fDis_y = mxBossPos._m[1][3] - _fAKMT[1];
	if (fDis_y < 0)fDis_y = -fDis_y;

	if (fDis_x > 0.5f || fDis_y > 0.5f) {		//�ɼu�|����BOSS��m
		_pAttackMissile->SetTRSMatrix(_mxAKMT * _mxAKMS);	//��BOSS��V����
	}
	else {
		_bMissileIsShoot = false;	//�ɼu�o�g����

		//�ɼu�k��
		_fAKMT[0] = passive_x;
		_fAKMT[1] = PLAYER_Y_AXIS;
		_mxAKMT = Translate(_fAKMT[0], _fAKMT[1], _fAKMT[2]);
		_pAttackMissile->SetTRSMatrix(_mxAKMT);
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
	_iBulletNum++;				//�l�u�ƶq����

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
	_mxBT = _pBGet_shoot->GetTranslateMatrix();	//��s�l�u��m
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

//----------------------------------------------
mat4 CPlayer::GetTranslateMatrix()
{
	return _mxPT;
}

mat4 CPlayer::GetBulletTranslateMatrix()
{
	return _mxBT;
}

//----------------------------------------------
mat4 CPlayer::GetTRSMatrix_HP()
{
	return(_mxHP_Pos);
}

void CPlayer::AttackedByEnemies(float delta)
{
	_fHPMoveT_x -= delta * LOSE_HP_SPEED;	//�򥻥���t
	if(_fHPMoveT_x < 0.5f)_fHPMoveS_x -= delta * (LOSE_HP_SPEED_SCALE + 0.05f); //�Y��
	else _fHPMoveS_x -= delta * LOSE_HP_SPEED_SCALE;

	_mxHPT_adjust = Translate(_fHPMoveT_x, 0.0f, 0.0f) * Scale(_fHPMoveS_x, 1.0f, 1.0f);
	_pPlayerHP->GL_SetAdjustTranslatMatrix(_mxHPT_adjust);	//������� + ���Y
}

void CPlayer::AttackedByEnemies(float delta, float loseHPscale)
{
	_fHPMoveT_x -= (delta * LOSE_HP_SPEED) * loseHPscale;	//�򥻥���t���W���孿�v
	if (_fHPMoveT_x < 0.5f)_fHPMoveS_x -= (delta * (LOSE_HP_SPEED_SCALE + 0.05f)) * loseHPscale;	//�Y��
	else _fHPMoveS_x -= (delta * LOSE_HP_SPEED_SCALE) * loseHPscale;

	_mxHPT_adjust = Translate(_fHPMoveT_x, 0.0f, 0.0f) * Scale(_fHPMoveS_x, 1.0f, 1.0f);
	_pPlayerHP->GL_SetAdjustTranslatMatrix(_mxHPT_adjust);	//������� + ���Y
}