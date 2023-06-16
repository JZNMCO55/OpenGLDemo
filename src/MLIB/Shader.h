#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GLEW/glew.h>; // ����glew����ȡ���еı���OpenGLͷ�ļ�

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
