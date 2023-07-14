#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 GetLookAtMatrix();
	void DoMovement(GLfloat deltaTime);
	void SetKeysStatus(GLuint key, GLboolean status);
	void SetCurrentPos(int xpos,int ypos);
	void SetScroll(double xoffset, double yoffset);

	glm::vec3 GetCameraPos();
	glm::vec3 GetCameraFront();
	GLfloat GetAspect();
private:
	GLboolean m_isFirstMouse{ true }; // 是否是第一次使用鼠标
	GLfloat m_pitch{ 0 }; // 仰角
	GLfloat m_yaw{ 0 }; // 偏移角
	GLfloat m_lastXPos{ 0 }; // 上次的x坐标
	GLfloat m_lastYPos{ 0 }; // 上次的y坐标
	GLfloat m_aspect{ 45.f }; // 缩放大小

	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // 摄像机的坐标
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 摄像机的方向
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // 摄像机的向上
	
	GLboolean m_keys[1024] = { false }; // 按键的状态
};


#endif // !CAMERA_H
