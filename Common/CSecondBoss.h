#ifndef CSECONDBOSS_H
#define CSECONDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BIG_WHEEL_NUM 7		//大輪子  子物件
#define SMALL_WHEEL_NUM 7	//小輪子  子物件
#define BW_RADIUS 2.7f		//大輪  距離半徑
#define SW_RADIUS 0.5f		//小輪  距離半徑
#define BOSS_Y 2			//BOSS1 y axis
#define BULLET_NUM 10		//子彈數量
#define BULLET_RADIUS 1.5f	//子彈射出半徑
#define BULLET_SPEED 8.f

class CSecondBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[BIG_WHEEL_NUM]; //大輪子  子物件
	CObjReader *_pSWheel[BIG_WHEEL_NUM]; //小輪子  子子物件

	//父子物件位移
	mat4 _mxBWT[BIG_WHEEL_NUM], _mxSWT[SMALL_WHEEL_NUM];	//for big/small wheel translation
	mat4 _mxBWS[BIG_WHEEL_NUM], _mxSWS[SMALL_WHEEL_NUM];	//for big/small wheel scale
	mat4 _mxSWR[SMALL_WHEEL_NUM];							//for small wheel rotation
	GLfloat _fBWAngle_Track = 0;	//大輪路徑移動角度
	GLfloat _fBWAngle = 0;			//大輪自轉角度
	GLfloat _fSWAngle = 0;			//小輪自轉角度

	// bullet
	CBullet *_bullet[BULLET_NUM];
	float _fBAngle;	//旋轉角
	float _fmxBT_Boss2[BULLET_NUM][3];
	mat4 _mxBT_Boss2[BULLET_NUM], _mxBR_Boss2[BULLET_NUM];	//bullet TRS matrix
	mat4 _mxBulletCircleT[BULLET_NUM], _mxBulletCircleR[BULLET_NUM];
	float _faddDelta[BULLET_NUM];			//累加delta用
	float _fRand[BULLET_NUM];				//隨機發射速度(第三階段)
	float _fAdjust_x;						//發射方向調整
	bool _bDirectionDecide;					//決定發射方向
	bool _bDir;								//發射方向, 0:R, 1:L
	mat4 _mxBCurrentT[BULLET_NUM];			//存取目前子彈位置

public:

	CSecondBoss();
	~CSecondBoss();
	
	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();					//建立子彈串列
	void DeleteBulletList();					//刪除子彈串列
	void ShootBullet(float delta);				//發射子彈
	void NextBullet();							//下一顆子彈
	void SetBulletPassiveMove();				//子彈未發射 跟隨角色

	//子彈三階段發射
	void BulletMoveToCircle(float delta);
	void BulletRotate(float delta);
	void BulletShootToPlayer(float delta, float player_x);
	void ReSetBullet();
};
#endif