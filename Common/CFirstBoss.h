#ifndef CFIRSTBOSS_H
#define CFIRSTBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BOSS_Y 3		//BOSS1 y axis
#define BULLET_NUM 10	//�l�u�ƶq

class CFirstBoss : public CEnemy
{
protected:
	// Bullet List
	bool _isFirstBigShoot;				// �O�_���Ĥ@�o
	GLfloat _fMAngle_Track = 0;			//���|���ʨ���

public:
	CFirstBoss();
	~CFirstBoss();

	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();							//�إߤl�u��C
	void DeleteBulletList();							//�R���l�u��C
	void ShootBullet(float delta);						//�o�g�l�u
	void NextBullet();									//�U�@���l�u
	void SetBulletPassiveMove();						//�l�u���o�g ���H����

};
#endif