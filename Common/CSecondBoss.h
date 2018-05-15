#ifndef CSECONDBOSS_H
#define CSECONDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BIG_WHEEL_NUM 7		//�j���l  �l����
#define SMALL_WHEEL_NUM 7	//�p���l  �l����
#define BW_RADIUS 2.7f		//�j��  �Z���b�|
#define SW_RADIUS 0.5f		//�p��  �Z���b�|
#define BOSS_Y 2			//BOSS1 y axis
#define BULLET_NUM 10		//�l�u�ƶq
#define BULLET_RADIUS 1.5f	//�l�u�g�X�b�|
#define BULLET_SPEED 8.f

class CSecondBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[BIG_WHEEL_NUM]; //�j���l  �l����
	CObjReader *_pSWheel[BIG_WHEEL_NUM]; //�p���l  �l�l����

	//���l����첾
	mat4 _mxBWT[BIG_WHEEL_NUM], _mxSWT[SMALL_WHEEL_NUM];	//for big/small wheel translation
	mat4 _mxBWS[BIG_WHEEL_NUM], _mxSWS[SMALL_WHEEL_NUM];	//for big/small wheel scale
	mat4 _mxSWR[SMALL_WHEEL_NUM];							//for small wheel rotation
	GLfloat _fBWAngle_Track = 0;	//�j�����|���ʨ���
	GLfloat _fBWAngle = 0;			//�j�����ਤ��
	GLfloat _fSWAngle = 0;			//�p�����ਤ��

	// bullet
	CBullet *_bullet[BULLET_NUM];
	float _fBAngle;	//���ਤ
	float _fmxBT_Boss2[BULLET_NUM][3];
	mat4 _mxBT_Boss2[BULLET_NUM], _mxBR_Boss2[BULLET_NUM];	//bullet TRS matrix
	mat4 _mxBulletCircleT[BULLET_NUM], _mxBulletCircleR[BULLET_NUM];
	float _faddDelta[BULLET_NUM];			//�֥[delta��
	float _fRand[BULLET_NUM];				//�H���o�g�t��(�ĤT���q)
	float _fAdjust_x;						//�o�g��V�վ�
	bool _bDirectionDecide;					//�M�w�o�g��V
	bool _bDir;								//�o�g��V, 0:R, 1:L
	mat4 _mxBCurrentT[BULLET_NUM];			//�s���ثe�l�u��m

public:

	CSecondBoss();
	~CSecondBoss();
	
	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();					//�إߤl�u��C
	void DeleteBulletList();					//�R���l�u��C
	void ShootBullet(float delta);				//�o�g�l�u
	void NextBullet();							//�U�@���l�u
	void SetBulletPassiveMove();				//�l�u���o�g ���H����

	//�l�u�T���q�o�g
	void BulletMoveToCircle(float delta);
	void BulletRotate(float delta);
	void BulletShootToPlayer(float delta, float player_x);
	void ReSetBullet();
};
#endif