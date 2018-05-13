#ifndef CLITTLEENEMY_H
#define CLITTLEENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BOSS_Y 3		//BOSS1 y axis
#define BULLET_NUM 10	//子彈數量

class CLittleEnemy : public CEnemy
{
protected:
	float _fTrack[3] = { 0 };
	mat4 _mxTrack;
	GLfloat _fMAngle_Track = 0;			//路徑移動角度

public:
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

	float _fTrackSpeed;							//路徑速度

};
#endif