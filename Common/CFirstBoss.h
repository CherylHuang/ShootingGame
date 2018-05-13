#ifndef CFIRSTBOSS_H
#define CFIRSTBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"
#include "CBullet.h"

#define BULLET_NUM 10	//子彈數量

class CFirstBoss : public CEnemy
{
protected:
	// Bullet List
	bool _isFirstBigShoot;					// 是否為第一發
	mat4 _mxBS;								// for big shoot scale
	mat4 _mxBR;								//子彈轉向
	CBullet *_pBHead, *_pBGet, *_pBTail;	//子彈串列
	CBullet *_pBHead_shoot, *_pBGet_shoot;	//子彈發射用

public:
	CFirstBoss();
	~CFirstBoss();

	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

	int _iBulletNum;
	void CreateBulletList();							//建立子彈串列
	void DeleteBulletList();							//刪除子彈串列
	void ShootBullet(float delta);						//發射子彈
	void NextBullet();									//下一顆子彈
	void SetBulletPassiveMove();					//子彈跟隨滑鼠

};
#endif