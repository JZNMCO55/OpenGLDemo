extern "C" {
#include "SOIL/SOIL.h"
}
#include "Model.h"
#include "Shader.h"
#include "Iconv/iconv.h"

Model::Model(GLchar* path)
{
	this->LoadModel(path);
}

Model::~Model()
{

}

void Model::Draw(Shader shader)
{
	for (GLuint i = 0; i < m_meshes.size(); ++i)
	{
		this->m_meshes[i].Draw(shader);
	}
}

void Model::LoadModel(string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	this->m_directory = path.substr(0, path.find_last_of('\\'));
	this->ProcessNode(scene->mRootNode, scene);

}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// 添加当前节点中的所有Mesh
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->m_meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	// 递归处理该节点的子孙节点
	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// 处理顶点坐标、法线、纹理坐标
		glm::vec3 vec;

		vec.x = mesh->mVertices[i].x;
		vec.y = mesh->mVertices[i].y;
		vec.z = mesh->mVertices[i].z;
		vertex.Position = vec;

		if (mesh->HasNormals())
		{
			vec.x = mesh->mNormals[i].x;
			vec.y = mesh->mNormals[i].y;
			vec.z = mesh->mNormals[i].z;
			vertex.Normal = vec;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texCoords;
			texCoords.x = mesh->mTextureCoords[0][i].x;
			texCoords.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texCoords;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// 处理顶点索引
	for (GLuint i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// 处理材质
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuseMaps = this->LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

	std::vector<Texture> specularMaps = this->LoadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;
		for (GLuint j = 0; j < m_loadedTexture.size(); ++j)
		{
			if (std::strcmp(m_loadedTexture[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(m_loadedTexture[i]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->m_directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			m_loadedTexture.push_back(texture);
		}
	}
	return textures;
}

GLuint Model::TextureFromFile(std::string path, std::string dirctory)
{
	//生成纹理ID和纹理数据 
	string filename = path;
	filename = m_directory + '\\' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, nComponent;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, &nComponent, SOIL_LOAD_RGB);

	if (image)
	{
		GLenum format = GL_RGBA;
		if (1 == nComponent)
		{
			format = GL_RED;
		}
		else if (3 == nComponent)
		{
			format = GL_RGB;
		}
		else if (4 == nComponent)
		{
			format = GL_RGBA;
		}
		// 绑定id
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// 贴图方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		SOIL_free_image_data(image);
	}
	else
	{
		std::cout << "Texture failed to load at path " << path << std::endl;
		SOIL_free_image_data(image);
	}

	return textureID;
}
