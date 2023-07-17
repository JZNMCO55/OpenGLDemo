#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLEW/glew.h>
#include <GLUT/glut.h>


#include <iostream>
#include <cstring>
#include <string>

#include "Shader.h"

using namespace std;
struct Vertex
{
	// 顶点坐标、法向量、纹理坐标
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture
{
	GLuint id;
	string type;
	string path;
};

class Mesh
{
public:
	Mesh();
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texture);

	~Mesh();

	void Draw(Shader& shader);

	void PrintSelf();

	double* GetBox();

private:
	void SetUpMesh();

public:
	vector<Vertex> m_vertices;
	vector<Texture> m_textures;
	vector<GLuint> m_indices;
	GLuint VAO;
private:
	GLuint VBO, EBO;
	double box[6];

};

#endif // !MESH_H