#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW/glew.h>; // 包含glew来获取所有的必须OpenGL头文件
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	Shader();
	~Shader();

	void Use();

	GLuint GetProgram();
	void SetInput(GLint num, const char* str);
	void SetInput(GLfloat value, const char* str);
	void SetInput(glm::vec3 vector3f, const char* str);
	void SetInput(glm::vec2 vector2f, const char* str);
	void SetInput(glm::mat4 matrix4f, const char* str);

public:
	GLuint Program;

};

#endif // !SHADER_H
