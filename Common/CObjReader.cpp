#include "CObjReader.h"
// Example 4 �}�l
// ���P Example 3 �¤W(Y�b)
// �C�@�� Vertex �W�[ Normal �A�令�~���� CShape�A�@�ֳB�z�������]�w�ݨD


CObjReader::CObjReader(char *objfile)
{
	FILE *pfile;
	char pLineHead[20];
	int face[3][3]; //Ū����
	int ifaces, ivec;	//�I�B���Ƭ���
	ifaces = ivec = 0;

	if ((pfile = fopen(objfile, "r")) == NULL) {
		printf("obj file can't open."); system("pause");
	}
	while (!feof(pfile)) { //�O�_���ɮק�
		fscanf(pfile, "%s", pLineHead); //Ū���r��
		if (strcmp(pLineHead, "v") == 0) ivec++; //Ū��face
		else if (strcmp(pLineHead, "f") == 0) ifaces++; //Ū��face
	}

	m_iNumVtx = ifaces * 3;	//�I��
	m_pPoints = NULL; m_pNormals = NULL; m_pTex = NULL;

	m_pPoints = new vec4[m_iNumVtx];	//�ϥ��I
	m_pNormals = new vec3[m_iNumVtx];	//Normal
	_vecPoints = new vec4[ivec]; //����I

	int pCount = 0;
	int vCount = 0;
	rewind(pfile);	//���s�����ɮ��Y

	while (!feof(pfile)) { //�O�_���ɮק�
		fscanf(pfile, "%s", pLineHead); //Ū���r��
		if (strcmp(pLineHead, "v") == 0) { //Ū��vertex
			fscanf(pfile, "%f %f %f", &_vecPoints[vCount].x, &_vecPoints[vCount].y, &_vecPoints[vCount].z); //Ū��3�I
			_vecPoints[vCount].w = 1;
			vCount++;
		}
		else if (strcmp(pLineHead, "f") == 0) { //Ū��face
			fscanf(pfile, "%d/%d/%d %d/%d/%d %d/%d/%d", &face[0][0], &face[0][1], &face[0][2],
				&face[1][0], &face[1][1], &face[1][2],
				&face[2][0], &face[2][1], &face[2][2]); //Ū��face
			for (int i = 0; i < 3; i++) {
				m_pPoints[pCount + i] = _vecPoints[face[i][0] - 1];
				m_pNormals[pCount + i] = vec3(0, 1.0f, 0);  // Normal Vector �� W �� 0
			}
			pCount += 3;
		}
	}
	fclose(pfile); //�����ɮ�

	// Set shader's name
	SetShaderName("vsVtxColorPlayer.glsl", "fsVtxColor.glsl");

	// Create and initialize a buffer object 
	CreateBufferObject();

	// ��l�C�� : -1
	m_fColor[0] = -1.0f; m_fColor[1] = -1.0f; m_fColor[2] = -1.0f; m_fColor[3] = 1;
}

CObjReader::~CObjReader()
{
	//�k�٪Ŷ�
	if(_vecPoints != NULL) delete[] _vecPoints;
}

void CObjReader::Update(float dt)
{
}

void CObjReader::Draw()
{
	DrawingSetShader();
	glDrawArrays( GL_TRIANGLES, 0, m_iNumVtx);
}

void CObjReader::DrawW()
{
	DrawingWithoutSetShader();
	glDrawArrays( GL_TRIANGLES, 0, m_iNumVtx);
}
