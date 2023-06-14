#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1.���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ��֤ifsteam��������׳��쳣
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// ���ļ�
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fshaderStream;

		if (fShaderFile.is_open())
		{
			std::cout << "it is openning" << std::endl;
		}

		// ��ȡ�ļ��Ļ������ݵ�����
		vShaderStream << vShaderFile.rdbuf();
		fshaderStream << fShaderFile.rdbuf();

		// �ر��ļ�
		vShaderFile.close();
		fShaderFile.close();

		// ת������GLchar����
		vertexCode = vShaderStream.str();
		fragmentCode = fshaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2.������ɫ��
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// ������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// ��ӡ���������Ϣ
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ��ɫ������
	this->Program = glCreateProgram();
	glAttachShader(Program, vertex);
	glAttachShader(Program, fragment);
	glLinkProgram(Program);
	glGetProgramiv(Program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(Program, 512, NULL, infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);


}

Shader::~Shader()
{
}

void Shader::Use()
{
	glUseProgram(Program);
}

GLuint Shader::GetProgram()
{
	return GLuint();
}
