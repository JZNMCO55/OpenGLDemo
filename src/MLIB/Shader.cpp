#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// 1.从文件路径中获取顶点/片段着色器
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// 保证ifsteam对象可以抛出异常
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fshaderStream;

		if (fShaderFile.is_open())
		{
			std::cout << "it is openning" << std::endl;
		}

		// 读取文件的缓冲内容到流中
		vShaderStream << vShaderFile.rdbuf();
		fshaderStream << fShaderFile.rdbuf();

		// 关闭文件
		vShaderFile.close();
		fShaderFile.close();

		// 转换流至GLchar数组
		vertexCode = vShaderStream.str();
		fragmentCode = fshaderStream.str();

	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	// 2.编译着色器
	GLuint vertex, fragment;
	GLint success;
	GLchar infoLog[512];

	// 顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// 打印编译错误信息
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 着色器程序
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
