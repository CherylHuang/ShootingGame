#ifndef CENEMY_H
#define CENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CBullet.h"

class CEnemy
{
protected:
	// For Objects
	CObjReader *_pMainBody; //讀取Obj檔指標物件 主體

	// For TRS
	float _fMT[3] = { 0 };
	mat4 _mxMT;					//for main object translation

	// Bullet List
	mat4 _mxBS;								// for big shoot scale
	mat4 _mxBR;								//子彈轉向
	CBullet *_pBHead, *_pBGet, *_pBTail;	//子彈串列
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//子彈發射用

public:

	CEnemy();
	virtual ~CEnemy();

	virtual void UpdateMatrix(float) = 0;			//更新運動
	virtual void GL_Draw() = 0;
	virtual void SetViewMatrix(mat4) = 0;
	virtual void SetProjectionMatrix(mat4) = 0;
	mat4 GetTranslateMatrix();						//取得BOSS位置

	int _iBulletNum;
	virtual void CreateBulletList() = 0;							//建立子彈串列
	virtual void DeleteBulletList() = 0;							//刪除子彈串列
	virtual void ShootBullet(float delta) = 0;						//發射子彈
	virtual void NextBullet() = 0;									//下一顆子彈
	virtual void SetBulletPassiveMove() = 0;						//子彈未發射 跟隨角色

};
#endif