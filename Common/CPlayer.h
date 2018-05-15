#ifndef CPLAYER_H
#define CPLAYER_H
#include "../Header/Angel.h"
#include "CObjReader.h"
#include "CQuad.h"
#include "CBullet.h"
#include "CHealthPoints.h"

#define PLAYER_Y_AXIS -5.f			//玩家Y軸位置
#define BULLET_NUM 10				//子彈數量
#define DEFENSE_NUM 48				//防護罩數量
#define DE_RADIUS 1.6f				//防護罩  距離半徑
#define MISSILE_SPEED 20.f			//導彈速度
#define LOSE_HP_SPEED 0.08f			//基本失血速
#define LOSE_HP_SPEED_SCALE 0.1f	//縮放

class CPlayer
{
protected:
	// For Objects
	CObjReader *_pPlayer;					//讀取Obj檔指標物件
	CObjReader *_pHPFrame;					//血量框
	CHealthPoints *_pPlayerHP;				//血量條

	CObjReader *_pDefense[DEFENSE_NUM];		//防護罩
	CObjReader *_pAttackStroeStar[3];		//攻擊蓄力星星
	CObjReader *_pAttackMissile;			//攻擊導彈

	//TRS matrix
	float _fPT[3] = { 0 };					//for player translation / scale
	float _fPlayerScale = 1;
	mat4 _mxPT, _mxPS;
	float _fHPFT[3] = { 0 };				//for HP frame translation / scale
	float _fHPFS[3];
	mat4 _mxHPFT, _mxHPFS;

	//HP BAR
	float _fHPMoveS_x;						//玩家減血
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
	GLfloat _fDEAngle;						//防護罩 繞轉角
	mat4 _mxDET[DEFENSE_NUM];				//防護罩
	mat4 _mxDES[DEFENSE_NUM];
	mat4 _mxDER[DEFENSE_NUM];

	// Bullet List
	mat4 _mxBT;								// for bullet translate
	CBullet *_pBHead, *_pBGet, *_pBTail;	//子彈串列
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//子彈發射用

public:

	float _fHPMoveT_x;						//玩家減血

	CPlayer();
	~CPlayer();

	bool _bMissileIsShoot;		//導彈是否發射中
	void ShootMissile(float delta, float passive_x, mat4 &mxBossPos, int starNum);	//發射導彈

	void GL_Draw();
	void GL_DrawDefense();								//顯示防護罩
	void GL_DrawAttack(int);							//顯示攻擊蓄力

	void GL_SetTRSMatrix(mat4 &mat);
	void GL_SetTranslatMatrix(mat4 &mat);				//更新位置
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);
	void UpdateMatrix(float);							//更新運動
	float GetPlayerScale();
	void SetPassiveMotion(float);						//其他跟隨移動物件

	int _iBulletNum;
	void CreateBulletList();							//建立子彈串列
	void DeleteBulletList();							//刪除子彈串列
	void ShootBullet(float delta, float passive_x);		//發射子彈
	void NextBullet(float);								//下一顆子彈
	void SetBulletPassiveMove(mat4 &);					//子彈跟隨滑鼠

	mat4 GetTranslateMatrix();							//取得玩家位置
	mat4 GetBulletTranslateMatrix();					//取得子彈位置
	mat4 GetTRSMatrix_HP();								//取得血條位置

	void AttackedByEnemies(float delta);				//玩家受到攻擊
	void AttackedByEnemies(float delta, float loseHPscale);

};
#endif