#ifndef CTHIRDBOSS_H
#define CTHIRDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CLittleEnemy.h"

#define LITTLE_NUM 4	//�p�Ǽƶq
#define BOSS_Y 4.7f		//BOSS1 y axis
#define BULLET_NUM 10	//�l�u�ƶq
#define HALF_SCREEN_X 5
#define HALF_SCREEN_Y 7

class CThirdBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[2];						//���l  �l����
	CLittleEnemy *_pLittleEnemy[LITTLE_NUM];	//�p��

	// For TRS
	GLfloat _fWAngle = 0;					//���l�ਤ
	float _fWT[3] = { 0 };					//���l�첾
	mat4 _mxWT[2];							// for wheel translation
	float _fLT[LITTLE_NUM][3] = { 0 };		//�p�Ǧ첾
	mat4 _mxLT[LITTLE_NUM];					// for little enemy translation

public:
	CThirdBoss();
	~CThirdBoss();

	void UpdateMatrix(float);					//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);
	mat4 GetLETranslateMatrix(int n);			//���o(��n��)�p�Ǧ�m
	mat4 GetLEBulletTranslateMatrix(int n);		//���o(��n��)�p�� �l�u��m

	void CreateBulletList();					//�إߤl�u��C
	void DeleteBulletList();					//�R���l�u��C
	void ShootBullet(float delta);				//�o�g�l�u
	void NextBullet();							//�U�@���l�u
	void SetBulletPassiveMove();				//�l�u���o�g ���H����

	int _iLE_Num;									//�p�Ǽƶq
	bool _bLEisAlive[LITTLE_NUM];					//�p�Ǧs����
	float _fHPMoveS_x[LITTLE_NUM] = { 0 };			//�p�Ǵ��
	void LE_AttackedByPlayer(float delta, int n);	//(��n��)�p�Ǩ������
};
#endif