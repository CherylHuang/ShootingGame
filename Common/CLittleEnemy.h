#ifndef CLITTLEENEMY_H
#define CLITTLEENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CHealthPoints.h"

#define BOSS_Y 3		//BOSS1 y axis
#define BULLET_NUM 10	//子彈數量
#define LOSE_HP_SPEED 0.6f			//基本失血速
#define LOSE_HP_SPEED_SCALE 0.6f	//縮放
#define HP_HIGHT 0.9f	//HP 距離小怪高度

class CLittleEnemy : public CEnemy
{
protected:
	float _fTrack[3] = { 0 };
	mat4 _mxTrack;
	GLfloat _fMAngle_Track = 0;			//路徑移動角度

	//HP BAR
	CHealthPoints *_pLE_HP;				//血量條 物件
	float _fHPT[3] = { 0 };				//for HP translation / scale
	float _fHPS[3];
	mat4 _mxHPT, _mxHPS;

public:

	float _fTrackSpeed;					//路徑速度
	mat4 _mxHPT_adjust;
	float _fHPMoveT_x;					//小怪減血
	float _fHPMoveS_x;					//小怪減血

	CLittleEnemy();
	CLittleEnemy(int RandomColor);
	~CLittleEnemy();

	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void GL_SetTRSMatrix(mat4 &mat);
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();					//建立子彈串列
	void CreateBulletList(int RandomColor);
	void DeleteBulletList();					//刪除子彈串列
	void ShootBullet(float delta);				//發射子彈
	void NextBullet();							//下一顆子彈
	void SetBulletPassiveMove();				//子彈未發射 跟隨角色

	void AttackedByPlayer(float delta);			//小怪受到攻擊
	void SetColor(int RandomColor);				//設定小怪顏色
	void SetHPScaleMatrix(mat4 &);				//設定血條縮放

};
#endif