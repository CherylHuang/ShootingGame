#ifndef CBULLET_H
#define CBULLET_H
#include "../Header/Angel.h"

#define BULLET_SPEED 30.f	//30.f

class CBullet
{
protected:
	// For Objects
	CObjReader *_pBullet;				//讀取Obj檔指標物件

	//TRS matrix
	float _fBT[3] = { 0 };
	float _fscale = 1;
	mat4 _mxBT;							//for bullet translation
	mat4 _mxBR;							//for bullet rotation
	mat4 _mxBS;							//for bullet scale

public:
	CBullet();							//傳入欲發射子彈的玩家物件
	CBullet(float y);
	~CBullet();
	CBullet *link;						//for link list

	bool _isShoot;						//子彈是否發射出去

	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void ShootBulletUp(float delta, float passive_x);				//向上發射子彈
	void ShootBulletDown(float delta, float passive_x);				//向下發射子彈
	void ShootBulletDown(float delta, float passive_x, mat4 &mxS);
	void ResetBullet(float x);										//重新定位子彈
	void ResetBullet(float x, float y);
	void ResetBullet(mat4 &mat,float x, float y);
	void SetBulletColor(vec4);										//設定子彈顏色
};
#endif