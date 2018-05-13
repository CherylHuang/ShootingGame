#ifndef CENEMY_H
#define CENEMY_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"

class CEnemy
{
protected:
	// For Objects
	CObjReader *_pMainBody; //Ū��Obj�ɫ��Ъ��� �D��

	// For TRS
	float _fMT[3] = { 0 };
	mat4 _mxMT;					//for main object translation

public:

	CEnemy();
	virtual ~CEnemy();

	virtual void UpdateMatrix(float) = 0;			//��s�B��
	virtual void GL_Draw() = 0;
	virtual void SetViewMatrix(mat4) = 0;
	virtual void SetProjectionMatrix(mat4) = 0;


};
#endif