#ifndef CFIRSTBOSS_H
#define CFIRSTBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BULLET_NUM 10	//�l�u�ƶq

class CFirstBoss : public CEnemy
{
protected:
	// Bullet List
	bool _isFirstBigShoot;					// �O�_���Ĥ@�o
	mat4 _mxBS;								// for big shoot scale
	mat4 _mxBR;								//�l�u��V
	CBullet *_pBHead, *_pBGet, *_pBTail;	//�l�u��C
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//�l�u�o�g��

public:
	CFirstBoss();
	~CFirstBoss();

	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	int _iBulletNum;
	void CreateBulletList();							//�إߤl�u��C
	void DeleteBulletList();							//�R���l�u��C
	void ShootBullet(float delta);						//�o�g�l�u
	void NextBullet();									//�U�@���l�u
	void SetBulletPassiveMove();					//�l�u���H�ƹ�

};
#endif