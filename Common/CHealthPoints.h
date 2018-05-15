#ifndef CHEALTHPOINTS_H
#define CHEALTHPOINTS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CQuad.h"

#define HP_Y_SCALE 0.25f	//Y�b�Y��

class CHealthPoints
{
protected:
	// For Objects
	CObjReader *_pHP;					//��� ����

	//TRS matrix
	float _fHPT[3] = { 0 };				//for player translation / scale
	float _fHPS[3];
	mat4 _mxHPT, _mxHPS;
	mat4 _mxHPT_adjust;					//�����m�A�վ��

public:

	CHealthPoints(float y_axis, float x_scale);		//�����Y�y��, X Scale
	~CHealthPoints();

	void GL_SetTranslatMatrix(mat4 &mat);			//��s��m
	void GL_SetAdjustTranslatMatrix(mat4 &mat);		//��s�վ�x�}
	void GL_SetTRSMatrix(mat4 &mat);
	void UpdateMatrix(float);						//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4 &);
	void SetProjectionMatrix(mat4 &);

	mat4 GetTRSMatrix();							//���o�����m
	void SetColor(vec4 vColor);						//�]�w����C��

};
#endif