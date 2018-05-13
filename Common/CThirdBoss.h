#ifndef CTHIRDBOSS_H
#define CTHIRDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"

class CThirdBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[2];		//輪子  子物件

	// For TRS
	GLfloat _fWAngle = 0;		//輪子轉角
	float _fWT[3] = { 0 };		//輪子位移
	mat4 _mxWT[2];

public:
	CThirdBoss();
	~CThirdBoss();

	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

};
#endif