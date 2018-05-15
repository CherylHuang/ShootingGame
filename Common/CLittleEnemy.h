#ifndef CLITTLEENEMY_H
#define CLITTLEENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CHealthPoints.h"

#define BOSS_Y 3		//BOSS1 y axis
#define BULLET_NUM 10	//�l�u�ƶq
#define LOSE_HP_SPEED 0.6f			//�򥻥���t
#define LOSE_HP_SPEED_SCALE 0.6f	//�Y��
#define HP_HIGHT 0.9f	//HP �Z���p�ǰ���

class CLittleEnemy : public CEnemy
{
protected:
	float _fTrack[3] = { 0 };
	mat4 _mxTrack;
	GLfloat _fMAngle_Track = 0;			//���|���ʨ���

	//HP BAR
	CHealthPoints *_pLE_HP;				//��q�� ����
	float _fHPT[3] = { 0 };				//for HP translation / scale
	float _fHPS[3];
	mat4 _mxHPT, _mxHPS;

public:

	float _fTrackSpeed;					//���|�t��
	mat4 _mxHPT_adjust;
	float _fHPMoveT_x;					//�p�Ǵ��
	float _fHPMoveS_x;					//�p�Ǵ��

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

	void AttackedByPlayer(float delta);			//�p�Ǩ������
	void SetColor(int RandomColor);				//�]�w�p���C��
	void SetHPScaleMatrix(mat4 &);				//�]�w����Y��

};
#endif