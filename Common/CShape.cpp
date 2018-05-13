#include "CShape.h"
#include "CCamera.h"

CShape::CShape()
{
	m_pPoints = NULL;
	m_pNormals = NULL;
	m_pTex = NULL;
	m_bProjUpdated = false;
	m_bViewUpdated = false;
	m_bTRSUpdated = false;
	m_bColorUpdated = false;
}

CShape::~CShape()
{
	if( m_pPoints  != NULL ) delete [] m_pPoints;  
	if( m_pNormals != NULL ) delete	[] m_pNormals;
	if( m_pTex != NULL ) delete	m_pTex;

	if( m_pVXshader != NULL ) delete [] m_pVXshader;
	if( m_pFSshader != NULL ) delete [] m_pFSshader;
}

void CShape::CreateBufferObject()
{
    glGenVertexArrays( 1, &m_uiVao );
    glBindVertexArray( m_uiVao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &m_uiBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, m_uiBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx + sizeof(vec3)*m_iNumVtx, NULL, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*m_iNumVtx, m_pPoints ); 
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*m_iNumVtx, sizeof(vec3)*m_iNumVtx, m_pNormals );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CShape::SetShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle)
{
	glBindVertexArray(m_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);

    // Load shaders and use the resulting shader program
	if( uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader( m_pVXshader, m_pFSshader );
	else m_uiProgram = uiShaderHandle;

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( m_uiProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( m_uiProgram, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*m_iNumVtx) );

	m_uiModelView = glGetUniformLocation( m_uiProgram, "ModelView" );
	m_mxMVFinal = m_mxView = mxModelView;
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxView );

    m_uiProjection = glGetUniformLocation( m_uiProgram, "Projection" );
	m_mxProjection = mxProjection;
	glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );

    m_uiColor = glGetUniformLocation( m_uiProgram, "vColor" );
	glUniform4fv(m_uiColor, 1, m_fColor ); 

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray(0);
}

void CShape::SetShader(GLuint uiShaderHandle)
{
	glBindVertexArray(m_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);

	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) m_uiProgram = InitShader(m_pVXshader, m_pFSshader);
	else m_uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(m_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(m_uiProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*m_iNumVtx));

	m_uiModelView = glGetUniformLocation(m_uiProgram, "ModelView");
	//	m_mxMVFinal , m_mxView 宣告時就是單位矩陣
	glUniformMatrix4fv(m_uiModelView, 1, GL_TRUE, m_mxView);

	m_uiProjection = glGetUniformLocation(m_uiProgram, "Projection");
	// m_mxProjection 宣告時就是單位矩陣
	glUniformMatrix4fv(m_uiProjection, 1, GL_TRUE, m_mxProjection);

	m_uiColor = glGetUniformLocation(m_uiProgram, "vColor");
	glUniform4fv(m_uiColor, 1, m_fColor);

	glBindBuffer(GL_ARRAY_BUFFER, 0);		// 取消綁定的 VBO
	glBindVertexArray(0); // 取消綁定的 VAO
}

void CShape::DrawingSetShader()
{
	glUseProgram( m_uiProgram );
	glBindVertexArray( m_uiVao );

	if (m_bViewUpdated || m_bTRSUpdated) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated = false;
	}
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );

	if (m_bProjUpdated ) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}
	if (m_bColorUpdated ) {
		glUniform4fv(m_uiColor, 1, m_fColor);
		m_bColorUpdated = false;
	}
}

// 此處預設使用前一個描繪物件所使用的 Shader
void CShape::DrawingWithoutSetShader()
{
	glBindVertexArray( m_uiVao );
	if( m_bViewUpdated || m_bTRSUpdated ) { // Model View 的相關矩陣內容有更動
		m_mxMVFinal = m_mxView * m_mxTRS;
		m_bViewUpdated = m_bTRSUpdated =  false;
	}
	glUniformMatrix4fv( m_uiModelView, 1, GL_TRUE, m_mxMVFinal );
	if (m_bProjUpdated) {
		glUniformMatrix4fv( m_uiProjection, 1, GL_TRUE, m_mxProjection );
		m_bProjUpdated = false;
	}
	glUniform4fv(m_uiColor, 1, m_fColor ); 
}

void CShape::SetShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	m_pVXshader = new char[len+1];
	memcpy(m_pVXshader, vxShader, len+1);

	len = strlen(fsShader);
	m_pFSshader = new char[len+1];
	memcpy(m_pFSshader, fsShader, len+1);
}

void CShape::SetColor(vec4 vColor)
{
	m_fColor[0] = vColor.x;
	m_fColor[1] = vColor.y;
	m_fColor[2] = vColor.z;
	m_fColor[3] = vColor.w;
	m_bColorUpdated = true;
}

void CShape::SetViewMatrix(mat4 &mat)
{
	m_mxView = mat;
	m_bViewUpdated = true;
}

void CShape::SetProjectionMatrix(mat4 &mat)
{
	m_mxProjection = mat;
	m_bProjUpdated = true;
}
void CShape::SetTRSMatrix(mat4 &mat)
{
	m_mxTRS = mat;
	m_bTRSUpdated = true;
}

mat4 CShape::GetTRSMatrix()
{
	return m_mxTRS;
}