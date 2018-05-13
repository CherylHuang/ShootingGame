#ifndef CBGSTARS_H
#define CBGSTARS_H
#include "../Header/Angel.h"

#define STAR_NUM 30			//背景星星數量
#define HALF_SCREEN_X 5
#define HALF_SCREEN_Y 7

class CBGStars
{
protected:
	// For Objects
	CObjReader *_pStars[STAR_NUM];

	//TRS matrix
	float _fSpeed;						//stars moving speed
	float _fStT[STAR_NUM][3] = { 0 };	//每顆星星座標
	float _fStarScale = 1;
	mat4 _mxStT, _mxStS;				//for star translation / scale

public:

	CBGStars();
	~CBGStars();

	void UpdateMatrix(float);			//更新運動
	void GL_Draw();
	void SetViewMatrix(mat4);
	void SetProjectionMatrix(mat4);

};
#endif