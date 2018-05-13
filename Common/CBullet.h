#ifndef CBULLET_H
#define CBULLET_H
#include "../Header/Angel.h"

#define BULLET_SPEED 30.f	//30.f

class CBullet
{
protected:
	// For Objects
	CObjReader *_pBullet;				//Ū��Obj�ɫ��Ъ���

	//TRS matrix
	float _fBT[3] = { 0 };
	float _fscale = 1;
	mat4 _mxBT;							//for bullet translation
	mat4 _mxBR;							//for bullet rotation
	mat4 _mxBS;							//for bullet scale

public:
	CBullet();							//�ǤJ���o�g�l�u�����a����
	CBullet(float y);
	~CBullet();
	CBullet *link;						//for link list

	bool _isShoot;						//�l�u�O�_�o�g�X�h

	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void ShootBulletUp(float delta, float passive_x);				//�V�W�o�g�l�u
	void ShootBulletDown(float delta, float passive_x);				//�V�U�o�g�l�u
	void ShootBulletDown(float delta, float passive_x, mat4 &mxS);
	void ResetBullet(float x);										//���s�w��l�u
	void ResetBullet(float x, float y);
	void ResetBullet(mat4 &mat,float x, float y);
	void SetBulletColor(vec4);										//�]�w�l�u�C��
};
#endif