#include "Mesh.h"

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texture)
{
	this->m_vertices = vertices;
	this->m_indices = indices;
	this->m_textures = texture;

	this->SetUpMesh();
}

void Mesh::SetUpMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

	// 设置顶点坐标指针
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	// 设置法线指针
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

	// 设置顶点的纹理坐标
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;
	for (GLint i = 0; i < this->m_textures.size(); ++i)
	{
		// 在绑定纹理前需要激活适当的纹理单元
		glActiveTexture(GL_TEXTURE0 + i);

		// 检索纹理序列号（N in diffuse_textureN）
		string number;
		string name = this->m_textures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);
			shader.SetInput(i, "diffuse");
		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normalNr++);
		}
		else if (name == "texture_height")
		{
			number = std::to_string(heightNr++);
		}

		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	// 绘制Mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::PrintSelf()
{
	std::cout << "indice: " << m_indices.size();
	for (GLuint i = 0; i < m_indices.size(); ++i)
	{
		if (i % 3 == 0)
		{
			std::cout << std::endl;
		}
		std::cout << m_indices[i] << ", ";
	}

	std::cout << "\n" << "point: " << std::endl;
	for (GLuint i = 0; i < m_vertices.size(); ++i)
	{
		std::cout << m_vertices[i].Position[0] << ", " << m_vertices[i].Position[1] << ", " << m_vertices[i].Position[2]<<", ";
		std::cout << m_vertices[i].Normal[0] << ", " << m_vertices[i].Normal[1] << ", " << m_vertices[i].Normal[2]<<", ";
		std::cout << m_vertices[i].TexCoords[0] << ", " << m_vertices[i].TexCoords[1]<<", " << std::endl;
	}

	std::cout << "texture: " << std::endl;
	for (GLuint i = 0; i < m_textures.size(); ++i)
	{
		std::cout << m_textures[i].id << " " << m_textures[i].type << " " << m_textures[i].path << std::endl;
	}
}
