#ifndef CSHAPE_H
#define CSHAPE_H
#include "../Header/Angel.h"

class CShape 
{
protected:
	// 指向儲存幾何資料的指標
	vec4 *m_pPoints;
	vec3 *m_pNormals;
	vec2 *m_pTex;
	int  m_iNumVtx;

	// 每一個 Shape 自己的位移位置，縮放與旋轉角度

	GLfloat m_fColor[4]; // Object's color
	// For shaders' name
	char *m_pVXshader, *m_pFSshader;

	// For VAO
	GLuint m_uiVao;

	// For Shader
	GLuint  m_uiModelView, m_uiProjection, m_uiColor;
	GLuint  m_uiProgram ;
	GLuint  m_uiBuffer;

	// For Matrices
	mat4    m_mxView, m_mxProjection, m_mxTRS;
	mat4    m_mxMVFinal;
	bool    m_bProjUpdated, m_bViewUpdated, m_bTRSUpdated;
	bool    m_bColorUpdated;

	void		CreateBufferObject();
	void		DrawingSetShader();
	void		DrawingWithoutSetShader();

public:
	CShape();
	virtual ~CShape();
	virtual void Draw() = 0;
	virtual void DrawW() = 0; // Drawing without setting shaders
	virtual void Update(float dt) = 0;	// 提供讓每一個 frame 都可以對幾何模型改變內容

	void SetShaderName(const char vxShader[], const char fsShader[]);
	void SetShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle=MAX_UNSIGNED_INT);
	void SetShader(GLuint uiShaderHandle = MAX_UNSIGNED_INT);
	void SetColor(vec4 vColor);
	void SetViewMatrix(mat4 &mat);
	void SetProjectionMatrix(mat4 &mat);
	void SetTRSMatrix(mat4 &mat);
	mat4 GetTRSMatrix();
};

#endif
