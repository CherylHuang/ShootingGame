#ifndef CPLAYER_H
#define CPLAYER_H
#include "../Header/Angel.h"
#include "CObjReader.h"
#include "CQuad.h"
#include "CBullet.h"

#define PLAYER_Y_AXIS -5.f
#define BULLET_NUM 10	//子彈數量

class CPlayer
{
protected:
	// For Objects
	CObjReader *_pPlayer;		//讀取Obj檔指標物件
	CObjReader *_pHPFrame;		//血量框
	CQuad *_pHP;				//血量條

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
	CBullet *_pBHead, *_pBGet, *_pBTail;	//子彈串列
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//子彈發射用

public:

	CPlayer();
	~CPlayer();

	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);
	float GetPlayerScale();
	void SetHPPassiveMotion(float);						//血量條移動

	int _iBulletNum;
	void CreateBulletList();							//建立子彈串列
	void DeleteBulletList();							//刪除子彈串列
	void ShootBullet(float delta, float passive_x);		//發射子彈
	void NextBullet(float);								//下一顆子彈
	void SetBulletPassiveMove(mat4 &);					//子彈跟隨滑鼠

};
#endif