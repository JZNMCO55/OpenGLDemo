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
	GLboolean m_isFirstMouse{ true }; // �Ƿ��ǵ�һ��ʹ�����
	GLfloat m_pitch{ 0 }; // ����
	GLfloat m_yaw{ 0 }; // ƫ�ƽ�
	GLfloat m_lastXPos{ 0 }; // �ϴε�x����
	GLfloat m_lastYPos{ 0 }; // �ϴε�y����
	GLfloat m_aspect{ 45.f }; // ���Ŵ�С

	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // �����������
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // ������ķ���
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // �����������
	
	GLboolean m_keys[1024] = { false }; // ������״̬
};


#endif // !CAMERA_H
