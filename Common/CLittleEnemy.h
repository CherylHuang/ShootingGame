#ifndef CLITTLEENEMY_H
#define CLITTLEENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BOSS_Y 3		//BOSS1 y axis
#define BULLET_NUM 10	//�l�u�ƶq

class CLittleEnemy : public CEnemy
{
protected:
	float _fTrack[3] = { 0 };
	mat4 _mxTrack;
	GLfloat _fMAngle_Track = 0;			//���|���ʨ���

public:
	CLittleEnemy();
	CLittleEnemy(int RandomColor);
	~CLittleEnemy();

	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();					//�إߤl�u��C
	void CreateBulletList(int RandomColor);
	void DeleteBulletList();					//�R���l�u��C
	void ShootBullet(float delta);				//�o�g�l�u
	void NextBullet();							//�U�@���l�u
	void SetBulletPassiveMove();				//�l�u���o�g ���H����

	float _fTrackSpeed;							//���|�t��

};
#endif