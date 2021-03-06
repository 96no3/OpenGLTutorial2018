/**
* @file MeshList.cpp
*/
#include "MeshList.h"
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <iostream>


const Vertex vTree[] = {
{ { 0.00f, 5.0f,  0.00f },{ 0.5f, 0.8f, 0.3f, 1.0f },{ 0.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.00f, 1.5f, -1.10f },{ 0.1f, 0.3f, 0.0f, 1.0f },{ 0.0f,-2.0f },{ 0.00f,-0.49f,-0.87f } },
{ { -0.75f, 1.5f,  0.40f },{ 0.1f, 0.3f, 0.0f, 1.0f },{ -2.0f,-2.0f },{ -0.76f,-0.49f, 0.43f } },
{ { 0.75f, 1.5f,  0.40f },{ 0.1f, 0.3f, 0.0f, 1.0f },{ 2.0f,-2.0f },{ 0.76f,-0.49f, 0.43f } },
{ { 0.00f, 4.0f,  0.00f },{ 0.2f, 0.1f, 0.0f, 1.0f },{ 0.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.00f, 0.0f, -0.37f },{ 0.5f, 0.3f, 0.2f, 1.0f },{ 0.0f, -2.0f },{ 0.00f,-0.49f,-0.87f } },
{ { -0.25f, 0.0f,  0.13f },{ 0.5f, 0.3f, 0.2f, 1.0f },{ -2.0f,-2.0f },{ -0.76f,-0.49f, 0.43f } },
{ { 0.25f, 0.0f,  0.13f },{ 0.5f, 0.3f, 0.2f, 1.0f },{ 2.0f,-2.0f },{ 0.76f,-0.49f, 0.43f } },
};

const GLushort iTree[] = {
	0, 1, 2, 0, 2, 3, 0, 3, 1, 1, 2, 3,
	4, 5, 6, 4, 6 ,7, 4, 7, 5,
};

const Vertex vHouse[] = {
{ { 2.8f, 0.0f,  3.0f },{ 0.4f, 0.3f, 0.2f, 1.0f },{ 1.000f,  0.0f },{ 0.71f, 0.00f, 0.71f } },
{ { 3.0f, 4.0f,  3.0f },{ 0.6f, 0.5f, 0.3f, 1.0f },{ 1.000f,  0.69f },{ 0.71f, 0.00f, 0.71f } },
{ { 0.0f, 6.0f,  3.0f },{ 0.5f, 0.4f, 0.2f, 1.0f },{ 0.875f, 1.0f },{ 0.00f, 0.71f, 0.71f } },
{ { -3.0f, 4.0f,  3.0f },{ 0.6f, 0.5f, 0.3f, 1.0f },{ 0.750f, 0.69f },{-0.71f, 0.00f, 0.71f } },
{ { -2.8f, 0.0f,  3.0f },{ 0.4f, 0.3f, 0.2f, 1.0f },{ 0.750f, 0.0f },{-0.71f, 0.00f, 0.71f } },

{ { -2.8f, 0.0f, -3.0f },{ 0.4f, 0.3f, 0.2f, 1.0f },{ 0.500f,  0.0f },{-0.71f, 0.00f, -0.71f } },
{ { -3.0f, 4.0f, -3.0f },{ 0.6f, 0.5f, 0.3f, 1.0f },{ 0.500f,  0.69f },{-0.71f, 0.00f, -0.71f } },
{ { 0.0f, 6.0f, -3.0f },{ 0.5f, 0.4f, 0.2f, 1.0f },{ 0.375f,  1.0f },{ 0.00f, 0.71f, -0.71f } },
{ { 3.0f, 4.0f, -3.0f },{ 0.6f, 0.5f, 0.3f, 1.0f },{ 0.250f, 0.69f },{ 0.71f, 0.00f, -0.71f } },
{ { 2.8f, 0.0f, -3.0f },{ 0.4f, 0.3f, 0.2f, 1.0f },{ 0.250f, 0.0f },{ 0.71f, 0.00f, -0.71f } },

{ { 2.8f, 0.0f, 3.0f },{ 0.4f, 0.3f, 0.2f, 1.0f },{ 0.00f, 0.00f },{ 0.71f, 0.00f, 0.71f } },
{ { 3.0f, 4.0f, 3.0f },{ 0.6f, 0.5f, 0.3f, 1.0f },{ 0.00f, 0.69f },{ 0.71f, 0.00f, 0.71f } },
{ { 0.0f, 6.0f, 3.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.75f, 1.00f },{ 0.00f, 0.71f, 0.71f } },
{ { 0.0f, 6.0f,-3.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.50f, 1.00f },{ 0.00f, 0.71f, -0.71f } },
{ { 0.0f, 6.0f,-3.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.25f, 1.00f },{ 0.00f, 0.71f, -0.71f } },
{ { 0.0f, 6.0f, 3.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.00f, 1.00f },{ 0.00f, 0.71f, 0.71f } },
};

const GLushort iHouse[] = {
	0, 1, 3, 3, 4, 0, 1, 2, 3,
	5, 6, 8, 8, 9, 5, 6, 7, 8,
	9, 8, 11, 11, 10, 9,
	4, 3, 6, 6, 5, 4,
	3, 12, 13, 13, 6, 3,
	8, 14, 15, 15, 11, 8,
};

const Vertex vRock[] = {
{ { 1.0f, 0.0f,  1.0f },{ 0.25f, 0.25f, 0.25f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.5f, 1.0f,  0.5f },{ 0.75f, 0.75f, 0.75f, 1.0f },{ 0.0f, 0.5f } },
{ { -0.5f, 1.0f,  0.5f },{ 0.75f, 0.75f, 0.75f, 1.0f },{ 0.5f, 0.5f } },
{ { -1.0f, 0.0f,  1.0f },{ 0.25f, 0.25f, 0.25f, 1.0f },{ 1.0f, 0.0f } },

{ { -1.0f, 0.0f, -1.0f },{ 0.25f, 0.25f, 0.25f, 1.0f },{ 0.0f, 0.0f } },
{ { -0.5f, 1.0f, -0.5f },{ 0.75f, 0.75f, 0.75f, 1.0f },{ 0.0f, 0.5f } },
{ { 0.5f, 1.0f, -0.5f },{ 0.75f, 0.75f, 0.75f, 1.0f },{ 0.5f, 0.5f } },
{ { 1.0f, 0.0f, -1.0f },{ 0.25f, 0.25f, 0.25f, 1.0f },{ 1.0f, 0.0f } },
};

const GLushort iRock[] = {
	1, 5, 2, 5, 1, 6,
	1, 2, 3, 3, 0, 1,
	1, 0, 7, 7, 6, 1,
	6, 7, 4, 4, 5, 6,
	5, 4, 3, 3, 2, 5,
};

const Vertex vGround[] = {
{ { -20.0f, 0.0f,  20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -2.0f,-2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -10.0f, 0.0f,  20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -1.0f,-2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.0f, 0.0f,  20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 0.0f,-2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 10.0f, 0.0f,  20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 1.0f,-2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 20.0f, 0.0f,  20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 2.0f,-2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -20.0f, 0.0f,  10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -2.0f,-1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -10.0f, 0.0f,  10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -1.0f,-1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.0f, 0.0f,  10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 0.0f,-1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 10.0f, 0.0f,  10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 1.0f,-1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 20.0f, 0.0f,  10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 2.0f,-1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -20.0f, 0.0f,   0.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -2.0f, 0.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -10.0f, 0.0f,   0.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -1.0f, 0.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.0f, 0.0f,   0.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 0.0f, 0.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 10.0f, 0.0f,   0.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 1.0f, 0.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 20.0f, 0.0f,   0.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 2.0f, 0.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -20.0f, 0.0f, -10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -2.0f, 1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -10.0f, 0.0f, -10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -1.0f, 1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.0f, 0.0f, -10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 0.0f, 1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 10.0f, 0.0f, -10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 1.0f, 1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 20.0f, 0.0f, -10.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 2.0f, 1.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -20.0f, 0.0f, -20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -2.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { -10.0f, 0.0f, -20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ -1.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 0.0f, 0.0f, -20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 0.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 10.0f, 0.0f, -20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 1.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
{ { 20.0f, 0.0f, -20.0f },{ 0.6f, 0.2f, 0.0f, 1.0f },{ 2.0f, 2.0f },{ 0.00f, 1.00f, 0.00f } },
};

const GLushort iGround[] = {
	0, 6, 5, 0, 1, 6, 1, 7, 6, 1, 2, 7, 2, 8, 7, 2, 3, 8, 3, 9, 8, 3, 4, 9,
	5, 11, 10, 5, 6, 11, 6, 12, 11, 6, 7, 12, 7, 13, 12, 7, 8, 13, 8, 14, 13, 8, 9, 14,
	10, 16, 15, 10, 11, 16, 11, 17, 16, 11, 12, 17, 12, 18, 17, 12, 13, 18, 13, 19, 18, 13, 14, 19,
	15, 21, 20, 15, 16, 21, 16, 22, 21, 16, 17, 22, 17, 23, 22, 17, 18, 23, 18, 24, 23, 18, 19, 24,
};

const Vertex vRectangle[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
{ { 0.3f, -0.3f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
{ { 0.3f,  0.5f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
{ { -0.5f,  0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },

{ { -0.3f,  0.3f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
{ { -0.3f, -0.5f, 0.1f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
{ { 0.5f,  0.3f, 0.1f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 0.0f, 0.0f, 1.0f } },

{ { -0.5f, -0.43f, 0.5f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
{ { 0.5f, -0.43f, 0.5f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
{ { 0.0f,  0.43f, 0.5f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
};

const GLushort iRectangle[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
};

/**
* Vertex Buffer Object を作成する.
*
* @param size	頂点データのサイズ.
* @param data	頂点データへのポインタ.
*
* @return		作成したVBO.
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data) {
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}

/**
* Index Buffer Object を作成する.
*
* @param size	インデックスデータのサイズ.
* @param data	インデックスデータへのポインタ.
*
* @return		作成したIBO.
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data) {
	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ibo;
}

/**
* Vertex Array Object を作成する.
*
* @param vbo	VAOに関連付けられるVBO.
* @param ibo	VAOに関連付けられるIBO.
*
* @return		作成したVAO.
*/
GLuint CreateVAO(GLuint vbo, GLuint ibo) {
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	const GLsizei stride = sizeof(Vertex);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, sizeof(Vertex::position) / sizeof(float), GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(Vertex, position)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, sizeof(Vertex::color) / sizeof(float), GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(Vertex, color)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, sizeof(Vertex::texCoord) / sizeof(float), GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(Vertex, texCoord)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, sizeof(Vertex::normal) / sizeof(float), GL_FLOAT, GL_FALSE, stride, (const GLvoid*)(offsetof(Vertex, normal)));
	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	return vao;
}

/**
* コンストラクタ.
*/
MeshList::MeshList() {
}

/**
* デストラクタ.
*/
MeshList::~MeshList() {
	Free();
}

/**
* Meshを追加する.
*
* @param vBegin	追加する頂点データ配列の先頭.
* @param vEnd	追加する頂点データ配列の終端.
* @param iBegin	追加するインデックスデータ配列の先頭.
* @param iEnd	追加するインデックスデータ配列の終端.
*/
void MeshList::Add(const Vertex* vBegin, const Vertex* vEnd, const GLushort* iBegin, const GLushort* iEnd) {
	// メッシュの追加.
	Mesh m;
	m.mode = GL_TRIANGLES;
	m.count = iEnd - iBegin;
	m.indices = (const GLvoid*)(tmpIndices.size() * sizeof(GLushort));
	m.baseVertex = (GLint)tmpVertices.size();
	meshes.push_back(m);
	// モデルのデータをvectorにコピー.
	tmpVertices.insert(tmpVertices.end(), vBegin, vEnd);
	tmpIndices.insert(tmpIndices.end(), iBegin, iEnd);
}

/**
* OBJファイルからメッシュを読み込む.
*
* @param path	読み込むOBJファイル名.
*
* @retval true		読み込み成功.
* @retval false		読み込み失敗.
*/
bool MeshList::AddFromObjFile(const char* path) {

	// ファイルを開く.
	std::ifstream ifs;
	ifs.open(path);
	if (!ifs.is_open()) {
		std::cerr << "ERROR: " << path << "を開けません\n";
		return false;
	}

	// データ読み取り用の変数を準備.
	struct Face {
		int v;
		int vt;
		int vn;
	};
	std::vector<Face> faceList;
	std::vector<Vector3> positionList;
	std::vector<Vector2> texCoordList;
	std::vector<Vector3> normalList;
	faceList.reserve(1000);
	positionList.reserve(1000);
	texCoordList.reserve(1000);
	normalList.reserve(1000);

	// ファイルからモデルのデータを読み込む.
	while (!ifs.eof()) {
		std::string line;
		getline(ifs, line);

		// 読み取り処理用の変数を定義する.
		Vector3 v;
		Vector2 vt;
		Vector3 vn;
		Face f[3];

		// データを文法に対応する変数に格納する.
		if (sscanf_s(line.data(), "v %f %f %f", &v.x, &v.y, &v.z) == 3) {
			positionList.push_back(v);
		}
		else if (sscanf_s(line.data(), "vt %f %f", &vt.x, &vt.y) == 2) {
			texCoordList.push_back(vt);
		}
		else if (sscanf_s(line.data(), "vn %f %f %f", &vn.x, &vn.y, &vn.z) == 3) {
			const float length = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
			vn.x /= length;
			vn.y /= length;
			vn.z /= length;
			normalList.push_back(vn);
		}
		else if (sscanf_s(line.data(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
			&f[0].v, &f[0].vt, &f[0].vn, &f[1].v, &f[1].vt, &f[1].vn, &f[2].v, &f[2].vt, &f[2].vn) == 9) {
			faceList.push_back(f[0]);
			faceList.push_back(f[1]);
			faceList.push_back(f[2]);
		}
	}

	// データ不足の場合は作成中止.
	if (positionList.empty()) {
		std::cerr << "WARNING: " << path << "には頂点座標の定義がありません.\n";
		/*Add(nullptr, nullptr, nullptr, nullptr);
		return false;*/
		positionList.push_back({ 0, 0, 0 });
	}
	if (texCoordList.empty()) {
		std::cerr << "WARNING: " << path << "にはテクスチャ座標の定義がありません.\n";
		/*Add(nullptr, nullptr, nullptr, nullptr);
		return false;*/
		texCoordList.push_back({ 0, 0 });
	}
	if (normalList.empty()) {
		std::cerr << "WARNING: " << path << "には法線の定義がありません.\n";
		/*Add(nullptr, nullptr, nullptr, nullptr);
		return false;*/
		normalList.push_back({ 0, 1, 0 });
	}

	// 頂点データとインデックスデータ用の変数を準備.
	std::vector<Vertex> vertices;
	std::vector<GLushort> indices;
	std::vector<Face> faceToVertexList;
	vertices.reserve(faceList.size());
	indices.reserve(faceList.size());
	faceToVertexList.reserve(faceList.size());

	// モデルのデータを頂点データとインデックスデータに変換する.
	for (size_t i = 0; i < faceList.size(); ++i) {
		// 重複する頂点データがあるか調べる.
		size_t n = 0;
		for (; n < faceToVertexList.size(); ++n) {
			if (faceToVertexList[n].v == faceList[i].v && faceToVertexList[n].vt == faceList[i].vt && faceToVertexList[n].vn == faceList[i].vn) {
				// 重複するデータを見つけた.
				break;
			}
		}

		if (n < faceToVertexList.size()) {
			// 重複する頂点データが見つかったら、見つかった頂点データのインデックスを追加する.
			indices.push_back((GLushort)n);
		}
		else {
			// 重複する頂点データが見つからなければ、新しい頂点データを作成し、そのインデックスデータを追加する.
			indices.push_back((GLushort)vertices.size());

			// 面データを追加する.
			faceToVertexList.push_back(faceList[i]);

			// 頂点データを追加する.
			Vertex vertex;
			Color color = { 1, 1, 1, 1 };
			int v = faceList[i].v - 1;
			if (v < 0 || v >= (int)positionList.size()) {
				//std::cerr << "WARNING: 不正なvインデックス(" << v << ")\n";
				color = { 0.5f, 0, 0, 1 };
				v = 0;
			}
			int vt = faceList[i].vt - 1;
			if (vt < 0 || vt >= (int)texCoordList.size()) {
				//std::cerr << "WARNING: 不正なvtインデックス(" << vt << ")\n";
				color = { 0.5f, 0, 0, 1 };
				vt = 0;
			}
			int vn = faceList[i].vn - 1;
			if (vn < 0 || vn >= (int)normalList.size()) {
				//std::cerr << "WARNING: 不正なvnインデックス(" << vn << ")\n";
				color = { 0.5f, 0, 0, 1 };
				vn = 0;
			}
			vertex.position = positionList[v];
			//vertex.color = { 1,1,1,1 };
			vertex.color = color;
			vertex.texCoord = texCoordList[vt];
			vertex.normal = normalList[vn];
			vertices.push_back(vertex);
		}
	}

	std::cout << "INFO: " << path << "[頂点数=" << vertices.size() << "インデックス数=" << indices.size() << "]\n";

	Add(vertices.data(), vertices.data() + vertices.size(), indices.data(), indices.data() + indices.size());

	return true;
}

/**
* モデルデータからMeshのリストを作成する.
*
* @retval true		作成成功.
* @retval false		作成失敗.
*/
bool MeshList::Allocate() {
	Free();

	// 容量を予約.
	meshes.reserve(100);
	tmpVertices.reserve(10'000);
	tmpIndices.reserve(10'000);	

	// メッシュを追加.
	//Add(std::begin(vTree), std::end(vTree), std::begin(iTree), std::end(iTree));
	AddFromObjFile("Res/Model/Tree.obj");
	//Add(std::begin(vHouse), std::end(vHouse), std::begin(iHouse), std::end(iHouse));
	AddFromObjFile("Res/house2.obj");
	//Add(std::begin(vRock), std::end(vRock), std::begin(iRock), std::end(iRock));
	AddFromObjFile("Res/Model/Rock2.obj");
	//Add(std::begin(vGround), std::end(vGround), std::begin(iGround), std::end(iGround));
	AddFromObjFile("Res/Model/Ground.obj");
	AddFromObjFile("Res/human.obj");

	// VAOを作成する.
	GLuint vbo = CreateVBO(tmpVertices.size() * sizeof(Vertex), tmpVertices.data());
	GLuint ibo = CreateVBO(tmpIndices.size() * sizeof(GLushort), tmpIndices.data());
	vao = CreateVAO(vbo, ibo);

	// vectorのメモリを解放する.
	std::vector<Vertex>().swap(tmpVertices);
	std::vector<GLushort>().swap(tmpIndices);

	if (!vbo || !ibo || !vao) {
		std::cerr << "ERROR: VAOの作成に失敗.\n";
		return false;
	}
	
	return true;
}

/**
* メッシュリストを破棄する.
*/
void MeshList::Free() {

	glDeleteVertexArrays(1, &vao);
	vao = 0;
	std::vector<Mesh>().swap(meshes);
}

/**
* 描画に使用するVAOを設定する.
*/
void MeshList::BindVertexArray() {
	glBindVertexArray(vao);
}

/**
* メッシュを取得する.
*
* @param index	取得するメッシュの番号.
*
* @return		index番目のメッシュ.
*/
const Mesh& MeshList::Get(size_t index) const {
	return meshes[index];
}