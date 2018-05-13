#include "CObjReader.h"
// Example 4 開始
// 面同 Example 3 朝上(Y軸)
// 每一個 Vertex 增加 Normal ，改成繼曾自 CShape，一併處理相關的設定需求


CObjReader::CObjReader(char *objfile)
{
	FILE *pfile;
	char pLineHead[20];
	int face[3][3]; //讀取用
	int ifaces, ivec;	//點、面數紀錄
	ifaces = ivec = 0;

	if ((pfile = fopen(objfile, "r")) == NULL) {
		printf("obj file can't open."); system("pause");
	}
	while (!feof(pfile)) { //是否到檔案尾
		fscanf(pfile, "%s", pLineHead); //讀取字串
		if (strcmp(pLineHead, "v") == 0) ivec++; //讀取face
		else if (strcmp(pLineHead, "f") == 0) ifaces++; //讀取face
	}

	m_iNumVtx = ifaces * 3;	//點數
	m_pPoints = NULL; m_pNormals = NULL; m_pTex = NULL;

	m_pPoints = new vec4[m_iNumVtx];	//使用點
	m_pNormals = new vec3[m_iNumVtx];	//Normal
	_vecPoints = new vec4[ivec]; //資料點

	int pCount = 0;
	int vCount = 0;
	rewind(pfile);	//重新指到檔案頭

	while (!feof(pfile)) { //是否到檔案尾
		fscanf(pfile, "%s", pLineHead); //讀取字串
		if (strcmp(pLineHead, "v") == 0) { //讀取vertex
			fscanf(pfile, "%f %f %f", &_vecPoints[vCount].x, &_vecPoints[vCount].y, &_vecPoints[vCount].z); //讀取3點
			_vecPoints[vCount].w = 1;
			vCount++;
		}
		else if (strcmp(pLineHead, "f") == 0) { //讀取face
			fscanf(pfile, "%d/%d/%d %d/%d/%d %d/%d/%d", &face[0][0], &face[0][1], &face[0][2],
				&face[1][0], &face[1][1], &face[1][2],
				&face[2][0], &face[2][1], &face[2][2]); //讀取face
			for (int i = 0; i < 3; i++) {
				m_pPoints[pCount + i] = _vecPoints[face[i][0] - 1];
				m_pNormals[pCount + i] = vec3(0, 1.0f, 0);  // Normal Vector 的 W 為 0
			}
			pCount += 3;
		}
	}
	fclose(pfile); //關閉檔案

	// Set shader's name
	SetShaderName("vsVtxColorPlayer.glsl", "fsVtxColor.glsl");

	// Create and initialize a buffer object 
	CreateBufferObject();

	// 初始顏色 : -1
	m_fColor[0] = -1.0f; m_fColor[1] = -1.0f; m_fColor[2] = -1.0f; m_fColor[3] = 1;
}

CObjReader::~CObjReader()
{
	//歸還空間
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
