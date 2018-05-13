#ifndef CSECONDBOSS_H
#define CSECONDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"

#define BIG_WHEEL_NUM 5		//�j���l  �l����
#define SMALL_WHEEL_NUM 5	//�p���l  �l����
#define BW_RADIUS 2.7f		//�j��  �Z���b�|
#define SW_RADIUS 0.5f		//�p��  �Z���b�|

class CSecondBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[BIG_WHEEL_NUM]; //�j���l  �l����
	CObjReader *_pSWheel[BIG_WHEEL_NUM]; //�p���l  �l�l����

	//���l����첾
	mat4 _mxBWT[BIG_WHEEL_NUM], _mxSWT[SMALL_WHEEL_NUM];	//for big/small wheel translation
	mat4 _mxBWS[BIG_WHEEL_NUM], _mxSWS[SMALL_WHEEL_NUM];	//for big/small wheel scale
	mat4 _mxSWR[SMALL_WHEEL_NUM];							//for small wheel rotation
	GLfloat _fBWAngle_Track = 0;	//�j�����|���ʨ���
	GLfloat _fBWAngle = 0;			//�j�����ਤ��
	GLfloat _fSWAngle = 0;			//�p�����ਤ��

public:

	CSecondBoss();
	~CSecondBoss();
	
	void UpdateMatrix(float);			//��s�B��
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

};
#endif