#ifndef CPLAYER_H
#define CPLAYER_H
#include "../Header/Angel.h"
#include "CObjReader.h"
#include "CQuad.h"
#include "CBullet.h"
#include "CHealthPoints.h"

#define PLAYER_Y_AXIS -5.f			//���aY�b��m
#define BULLET_NUM 10				//�l�u�ƶq
#define DEFENSE_NUM 48				//���@�n�ƶq
#define DE_RADIUS 1.6f				//���@�n  �Z���b�|
#define MISSILE_SPEED 20.f			//�ɼu�t��
#define LOSE_HP_SPEED 0.08f			//�򥻥���t
#define LOSE_HP_SPEED_SCALE 0.1f	//�Y��

class CPlayer
{
protected:
	// For Objects
	CObjReader *_pPlayer;					//Ū��Obj�ɫ��Ъ���
	CObjReader *_pHPFrame;					//��q��
	CHealthPoints *_pPlayerHP;				//��q��

	CObjReader *_pDefense[DEFENSE_NUM];		//���@�n
	CObjReader *_pAttackStroeStar[3];		//�����W�O�P�P
	CObjReader *_pAttackMissile;			//�����ɼu

	//TRS matrix
	float _fPT[3] = { 0 };					//for player translation / scale
	float _fPlayerScale = 1;
	mat4 _mxPT, _mxPS;
	float _fHPFT[3] = { 0 };				//for HP frame translation / scale
	float _fHPFS[3];
	mat4 _mxHPFT, _mxHPFS;

	//HP BAR
	float _fHPMoveS_x;						//���a���
	mat4 _mxHPT_adjust;
	float _fHPT[3] = { 0 };					//for HP translation / scale
	float _fHPS[3];
	mat4 _mxHPT, _mxHPS;

	//ATTACK
	float _fAKT[3][3] = { 0 };				//for Attack Store Star translation / scale
	float _fAKS[3][3];
	mat4 _mxAKT[3], _mxAKS[3];

	//MISSILE
	float _fAKMT[3] = { 0 };				//for missile translation / scale
	float _fMissileScale = 1;
	mat4 _mxAKMT, _mxAKMS;

	//DEFENSE
	GLfloat _fDEAngle;						//���@�n ¶�ਤ
	mat4 _mxDET[DEFENSE_NUM];				//���@�n
	mat4 _mxDES[DEFENSE_NUM];
	mat4 _mxDER[DEFENSE_NUM];

	// Bullet List
	mat4 _mxBT;								// for bullet translate
	CBullet *_pBHead, *_pBGet, *_pBTail;	//�l�u��C
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//�l�u�o�g��

public:

	float _fHPMoveT_x;						//���a���

	CPlayer();
	~CPlayer();

	bool _bMissileIsShoot;		//�ɼu�O�_�o�g��
	void ShootMissile(float delta, float passive_x, mat4 &mxBossPos, int starNum);	//�o�g�ɼu

	void GL_Draw();
	void GL_DrawDefense();								//��ܨ��@�n
	void GL_DrawAttack(int);							//��ܧ����W�O

	void GL_SetTRSMatrix(mat4 &mat);
	void GL_SetTranslatMatrix(mat4 &mat);				//��s��m
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);
	void UpdateMatrix(float);							//��s�B��
	float GetPlayerScale();
	void SetPassiveMotion(float);						//��L���H���ʪ���

	int _iBulletNum;
	void CreateBulletList();							//�إߤl�u��C
	void DeleteBulletList();							//�R���l�u��C
	void ShootBullet(float delta, float passive_x);		//�o�g�l�u
	void NextBullet(float);								//�U�@���l�u
	void SetBulletPassiveMove(mat4 &);					//�l�u���H�ƹ�

	mat4 GetTranslateMatrix();							//���o���a��m
	mat4 GetBulletTranslateMatrix();					//���o�l�u��m
	mat4 GetTRSMatrix_HP();								//���o�����m

	void AttackedByEnemies(float delta);				//���a�������
	void AttackedByEnemies(float delta, float loseHPscale);

};
#endif