#ifndef CTHIRDBOSS_H
#define CTHIRDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CLittleEnemy.h"

#define LITTLE_NUM 4	//小怪數量
#define BOSS_Y 4.7f		//BOSS1 y axis
#define BULLET_NUM 10	//子彈數量
#define HALF_SCREEN_X 5
#define HALF_SCREEN_Y 7

class CThirdBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[2];						//輪子  子物件
	CLittleEnemy *_pLittleEnemy[LITTLE_NUM];	//小怪

	// For TRS
	GLfloat _fWAngle = 0;					//輪子轉角
	float _fWT[3] = { 0 };					//輪子位移
	mat4 _mxWT[2];							// for wheel translation
	float _fLT[LITTLE_NUM][3] = { 0 };		//小怪位移
	mat4 _mxLT[LITTLE_NUM];					// for little enemy translation

public:
	CThirdBoss();
	~CThirdBoss();

	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	void CreateBulletList();					//建立子彈串列
	void DeleteBulletList();					//刪除子彈串列
	void ShootBullet(float delta);				//發射子彈
	void NextBullet();							//下一顆子彈
	void SetBulletPassiveMove();				//子彈未發射 跟隨角色
};
#endif