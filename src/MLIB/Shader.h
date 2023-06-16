#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW/glew.h>; // 包含glew来获取所有的必须OpenGL头文件

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();

	void Use();

	GLuint GetProgram();
public:
	GLuint Program;

};

#endif // !SHADER_H
