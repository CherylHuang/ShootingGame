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
	CObjReader *_pWheel[2];		//���l  �l����

	// For TRS
	GLfloat _fWAngle = 0;		//���l�ਤ
	float _fWT[3] = { 0 };		//���l�첾
	mat4 _mxWT[2];

public:
	CThirdBoss();
	~CThirdBoss();

	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

};
#endif