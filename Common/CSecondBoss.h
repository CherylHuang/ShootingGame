#ifndef CSECONDBOSS_H
#define CSECONDBOSS_H
#include "../Header/Angel.h"
#include "CCamera.h"
#include "CObjReader.h"
#include "CEnemy.h"

#define BIG_WHEEL_NUM 5		//近  ン
#define SMALL_WHEEL_NUM 5	//近  ン
#define BW_RADIUS 2.7f		//近  禯瞒畖
#define SW_RADIUS 0.5f		//近  禯瞒畖

class CSecondBoss : public CEnemy
{
protected:
	// For Objects
	CObjReader *_pWheel[BIG_WHEEL_NUM]; //近  ン
	CObjReader *_pSWheel[BIG_WHEEL_NUM]; //近  ン

	//ン簿
	mat4 _mxBWT[BIG_WHEEL_NUM], _mxSWT[SMALL_WHEEL_NUM];	//for big/small wheel translation
	mat4 _mxBWS[BIG_WHEEL_NUM], _mxSWS[SMALL_WHEEL_NUM];	//for big/small wheel scale
	mat4 _mxSWR[SMALL_WHEEL_NUM];							//for small wheel rotation
	GLfloat _fBWAngle_Track = 0;	//近隔畖簿笆à
	GLfloat _fBWAngle = 0;			//近锣à
	GLfloat _fSWAngle = 0;			//近锣à

public:

	CSecondBoss();
	~CSecondBoss();
	
	void UpdateMatrix(float);			//穝笲笆
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

};
#endif