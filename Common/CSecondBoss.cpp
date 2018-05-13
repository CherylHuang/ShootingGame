#include "CSecondBoss.h"

CSecondBoss::CSecondBoss()
{
	//����Obj�ɹ��骫��
	_pMainBody = new CObjReader("obj/cyberpunk2.obj");
	_pMainBody->SetColor(vec4(0.0f, -1.0f, 0.0f, 1));
	_pMainBody->SetShader();
	_fMT[1] = BOSS_Y;	//y�y��
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]); // 0
	_pMainBody->SetTRSMatrix(_mxMT);

	//���ͤl���骫�� �j��
	float fBWscale = 0.55f;
	for (int i = 0; i < BIG_WHEEL_NUM; i++) {
		_pWheel[i] = new CObjReader("obj/square.obj");
		_pWheel[i]->SetShader();
		_mxBWT[i] = Translate(BW_RADIUS * cosf(M_PI*2.0f*i / BIG_WHEEL_NUM), BW_RADIUS * sinf(M_PI*2.0f*i / BIG_WHEEL_NUM), 0.0f);
		_mxBWS[i] = Scale(fBWscale, fBWscale, fBWscale);
		_pWheel[i]->SetTRSMatrix(_mxMT * _mxBWT[i] * _mxBWS[i]);
	}

	//���ͤl�l���骫��  �p��
	float fSWscale = 1.f;
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) {
		_pSWheel[i] = new CObjReader("obj/chain.obj");
		_pSWheel[i]->SetShader();
		_mxSWT[i] = Translate(SW_RADIUS * cosf(M_PI*2.0f*i / SMALL_WHEEL_NUM), SW_RADIUS * sinf(M_PI*2.0f*i / SMALL_WHEEL_NUM), 0.0f);
		_mxSWR[i] = RotateZ(-90.f + 360.f/BIG_WHEEL_NUM*i);	//���� �¦V�j��
		_mxSWS[i] = Scale(fSWscale, fSWscale, fSWscale);
		_pSWheel[i]->SetTRSMatrix(_mxMT * _mxBWT[i] * _mxSWT[i] * _mxSWS[i] * _mxSWR[i]);
	}
}

CSecondBoss::~CSecondBoss()
{
	for (int i = 0; i < BIG_WHEEL_NUM; i++) delete _pWheel[i]; //�����ɭԤ@�Ӥ@�Ӯ�->�@�Ӥ@�ӧR
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) delete _pSWheel[i];
}

//-------------------------------------------------------

void CSecondBoss::UpdateMatrix(float delta)
{
	mat4 mxBWR[BIG_WHEEL_NUM], mxBWRr[BIG_WHEEL_NUM];	//�j�� ����x�}
	mat4 mxSWR[SMALL_WHEEL_NUM];	//�p�� ����x�}
	mat4 _mxBWTt;	//�j�� ���|���ʯx�}

	float speed = 2.5f;	//�j���t��
	_fBWAngle_Track += speed * delta;
	if (_fBWAngle_Track > 360) _fBWAngle_Track -= 360; //�k�s

	float sint, cost, sin2t, cos2t, cos3t, cos4t, sin12t, sin5t2, cos9t2; //---------�w�q----------
	sint = sinf(_fBWAngle_Track);				sin2t = sinf(2 * _fBWAngle_Track);
	cost = cosf(_fBWAngle_Track);				cos2t = cosf(2 * _fBWAngle_Track);
	cos3t = cosf(3 * _fBWAngle_Track);			cos4t = cosf(4 * _fBWAngle_Track);
	sin5t2 = sinf(5 * _fBWAngle_Track / 2.f);	cos9t2 = cosf(9 * _fBWAngle_Track / 2.f);
	float fsize = 0.05f;
	_mxBWTt = Translate((13 * sint * sin2t)*fsize, (13 * cost * sin2t)*fsize, 0);	//�j�����|����
	_fMT[0] = cost * (sint + sin5t2); _fMT[1] = sint * (sint + sin5t2) + BOSS_Y;						//BOSS BODY MOVE
	_mxMT = Translate(_fMT[0], _fMT[1], _fMT[2]);

	//HEART
	//x = 16 * sint*sint*sint
	//y = 13 * cost - 5 * cos2t - 2 * cos3t - cos4t
	//FLOWER
	//x = 15 * cost * sin2t  (15 : scale)
	//y = 15 * sint * sin2t
	//BUTTERFLY
	//x = sint*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//y = cost*(expf(cost) - 2 * cos4t - powf(sin12t, 5))
	//UNTITLED 2
	//x = cost * sint * sin5t2
	//y = sint * sint * sin5t2

	//������
	_pMainBody->SetTRSMatrix(_mxMT);

	//�j��
	_fBWAngle = 360.f / BIG_WHEEL_NUM;
	for (int i = 0; i < BIG_WHEEL_NUM; i++) {
		mxBWR[i] = RotateZ(_fBWAngle*i);
		mxBWRr[i] = RotateZ(-_fBWAngle*i); //����
		_pWheel[i]->SetTRSMatrix(_mxMT * mxBWR[i] * _mxBWTt * _mxBWT[i] * _mxBWS[i] * mxBWRr[i]);
	}

	//�p��
	_fSWAngle += 250.f * delta;
	if (_fSWAngle > 360) _fSWAngle -= 360; //�k�s
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) {
		mxSWR[i] = RotateZ(_fSWAngle);
		_pSWheel[i]->SetTRSMatrix(_mxMT * mxBWR[i] * _mxBWTt * _mxBWT[i] * mxBWRr[i] * mxSWR[i] * _mxSWT[i] * _mxSWS[i] * _mxSWR[i]);
	}

}

void CSecondBoss::GL_Draw()
{
	_pMainBody->Draw();
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->DrawW();
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->DrawW();
}

void CSecondBoss::SetViewMatrix(mat4 mvx)
{
	_pMainBody->SetViewMatrix(mvx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetViewMatrix(mvx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetViewMatrix(mvx);
}

void CSecondBoss::SetProjectionMatrix(mat4 mpx)
{
	_pMainBody->SetProjectionMatrix(mpx);
	for (int i = 0; i < BIG_WHEEL_NUM; i++) _pWheel[i]->SetProjectionMatrix(mpx);
	for (int i = 0; i < SMALL_WHEEL_NUM; i++) _pSWheel[i]->SetProjectionMatrix(mpx);
}

//-----------------------------------------------------------------
void CSecondBoss::CreateBulletList()
{

}
void CSecondBoss::DeleteBulletList()
{

}
void CSecondBoss::ShootBullet(float delta)
{

}
void CSecondBoss::NextBullet()
{

}
void CSecondBoss::SetBulletPassiveMove()
{

}