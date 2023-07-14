#ifndef MODEL_H
#define MODEL_H

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "Mesh.h"

class Shader;

class Model
{
public:
	Model(GLchar* path);
	~Model();

	void Draw(Shader shader);
private:
	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	GLuint TextureFromFile(std::string file, std::string dirctory);
private:
	std::vector<Mesh> m_meshes; //模型所包含的所有网格
	std::vector<Texture>m_loadedTexture; // 已经加载过的纹理
	string m_directory; // 
};

#endif // !MODEL_H
