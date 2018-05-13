#ifndef CPLAYER_H
#define CPLAYER_H
#include "../Header/Angel.h"
#include "CObjReader.h"
#include "CQuad.h"
#include "CBullet.h"

#define PLAYER_Y_AXIS -5.f
#define BULLET_NUM 10	//�l�u�ƶq

class CPlayer
{
protected:
	// For Objects
	CObjReader *_pPlayer;		//Ū��Obj�ɫ��Ъ���
	CObjReader *_pHPFrame;		//��q��
	CQuad *_pHP;				//��q��

	//TRS matrix
	float _fPT[3] = { 0 };
	float _fPlayerScale = 1;
	mat4 _mxPT, _mxPS;			//for player translation / scale
	float _fHPFT[3] = { 0 };
	float _fHPFS[3];
	mat4 _mxHPFT, _mxHPFS;		//for HP frame translation / scale
	float _fHPT[3] = { 0 };
	float _fHPS[3];
	mat4 _mxHPT, _mxHPS;		//for HP translation / scale

	// Bullet List
	CBullet *_pBHead, *_pBGet, *_pBTail;	//�l�u��C
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//�l�u�o�g��

public:

	CPlayer();
	~CPlayer();

	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);
	float GetPlayerScale();
	void SetHPPassiveMotion(float);						//��q������

	int _iBulletNum;
	void CreateBulletList();							//�إߤl�u��C
	void DeleteBulletList();							//�R���l�u��C
	void ShootBullet(float delta, float passive_x);		//�o�g�l�u
	void NextBullet(float);								//�U�@���l�u
	void SetBulletPassiveMove(mat4 &);					//�l�u���H�ƹ�

};
#endif