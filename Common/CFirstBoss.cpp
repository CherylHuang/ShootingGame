#include "CFirstBoss.h"

CFirstBoss::CFirstBoss()
{
	//����Obj�ɹ��骫��
	_pMainBody = new CObjReader("obj/cyberpunk1.obj");
	_pMainBody->SetColor(vec4(-1.0f, 0.0f, 0.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = 5;				//y�y��
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);
	_pMainBody->SetTRSMatrix(_mxMT);

	float BigShootScale = 5.0f;
	_mxBS = Scale(BigShootScale, BigShootScale, BigShootScale);	//for first big shoot
	_mxBR = RotateZ(180.0f);	//for bullet rotate
	_iBulletNum = 0;
	_isFirstBigShoot = false;
	CreateBulletList();			//�l�u
}

CFirstBoss::~CFirstBoss()
{
	DeleteBulletList();		//�l�u
}

//----------------------------------------------
void CFirstBoss::UpdateMatrix(float)
{

}
void CFirstBoss::GL_Draw()
{
	_pMainBody->Draw();

	//�l�u���
	_pBGet = _pBHead;
	while (_pBGet != nullptr) {
		if(_pBGet->_isShoot) _pBGet->GL_Draw();		//�u��ܮg�X���l�u
		_pBGet = _pBGet->link;
	}
}
void CFirstBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	_pBGet = _pBHead;					//�l�u��C
	while (_pBGet != nullptr) {
		_pBGet->SetViewMatrix(mvx);
		_pBGet = _pBGet->link;
	}
}
void CFirstBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	_pBGet = _pBHead;					//�l�u��C
	while (_pBGet != nullptr) {
		_pBGet->SetProjectionMatrix(mpx);
		_pBGet = _pBGet->link;
	}
}

//----------------------------------------------
void CFirstBoss::CreateBulletList()	//�إߤl�u��C
{
	//first node
	_pBHead = new CBullet(_fMT[1]);
	_pBHead->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));
	_pBHead->link = nullptr;
	_pBTail = _pBHead;
	_pBHead_shoot = _pBHead;	//�l�u�o�g��
	_pBHead->GL_SetTRSMatrix(_mxMT * _mxBR); //�]�w�l�u��BOSS��m
	_iBulletNum++;	//�l�u�ƶq����

	//the rest of nodes
	for (int i = 0; i < BULLET_NUM - 1; i++) {
		if ((_pBGet = new CBullet(_fMT[1])) == NULL) {
			printf("�O���餣��\n"); exit(0);
		}
		_pBGet->SetBulletColor(vec4(1.0f, 1.0f, 0.0f, 1));
		_pBGet->link = nullptr;
		_pBTail->link = _pBGet;
		_pBTail = _pBGet;
		_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR); //�]�w�l�u��BOSS��m
		_iBulletNum++;	//�l�u�ƶq����
	}
}

void CFirstBoss::DeleteBulletList()	//�R���l�u��C
{
	_pBGet = _pBHead;
	while (_pBHead != nullptr) {
		_pBHead = _pBHead->link;
		delete _pBGet;
		_pBGet = _pBHead;
	}
	_iBulletNum = 0;	//�l�u�ƶq�k�s
}

void CFirstBoss::SetBulletPassiveMove()
{
	_pBGet = _pBHead;	//�l�u��C
	while (_pBGet != nullptr) {
		if (_pBGet->_isShoot == false) { //�l�u�|���g�X
			_pBGet->GL_SetTRSMatrix(_mxMT * _mxBR);
		}
		_pBGet = _pBGet->link;
	}
}

void CFirstBoss::ShootBullet(float delta)
{
	_pBGet_shoot = _pBHead_shoot;
	if (_isFirstBigShoot) {				//�Ĥ@�o�l�u
		_pBGet_shoot->ShootBulletDown(delta, _fMT[0], _mxBS);
	}
	else _pBGet_shoot->ShootBulletDown(delta, _fMT[0]);
	_pBGet_shoot->_isShoot = true;		//�l�u�g�X
}

void CFirstBoss::NextBullet()
{
	_isFirstBigShoot = false;	//�D�Ĥ@�o�l�u
	_iBulletNum--;				//�l�u�ƶq����
	_pBHead_shoot = _pBHead_shoot->link;
	_pBGet_shoot->_isShoot = false;

	if (_iBulletNum == 0) {	//�S���l�u
		_isFirstBigShoot = true;	//�U�@�Ӥl�u���Ĥ@�o
		_pBHead_shoot = _pBHead;
		_pBGet_shoot = _pBHead_shoot;
		while (_pBGet_shoot != nullptr) {
			_pBGet_shoot->ResetBullet(_mxBR, _fMT[0], _fMT[1]); //�l�u�k��
			_pBGet_shoot = _pBGet_shoot->link;
			_iBulletNum++; //�l�u�ƶq����
		}
	}
}