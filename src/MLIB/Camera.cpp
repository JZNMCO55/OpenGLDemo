#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::SetCurrentPos(int xpos, int ypos)
{
	if (m_isFirstMouse)
	{
		m_lastXPos = xpos;
		m_lastYPos = ypos;
		m_isFirstMouse = false;
		return;
	}

	GLfloat xoffset = xpos - m_lastXPos;
	GLfloat yoffset = m_lastYPos - ypos;
	m_lastXPos = xpos;
	m_lastYPos = ypos;

	m_yaw += xoffset * 0.05;
	m_pitch += yoffset * 0.05;

	if (m_pitch > 89.f)
	{
		m_pitch = 89.f;
	}
	else if (m_pitch < -89.f)
	{
		m_pitch = -89.f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	front.y = sin(glm::radians(m_pitch));
	front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_cameraFront = glm::normalize(front);
}

void Camera::SetScroll(double xoffset, double yoffset)
{
	if (m_aspect >= 1.0f && m_aspect <= 45.0f)
	{
		m_aspect -= yoffset * 0.05;
	}
	else if (m_aspect <= 1.0f)
	{
		m_aspect = 1.0f;
	}
	else
	{
		m_aspect = 45.0f;
	}
}

glm::vec3 Camera::GetCameraPos()
{
	return m_cameraPos;
}

glm::vec3 Camera::GetCameraFront()
{
	return m_cameraFront;
}

GLfloat Camera::GetAspect()
{
	return m_aspect;
}

glm::mat4 Camera::GetLookAtMatrix()
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::DoMovement(GLfloat deltaTime)
{
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (m_keys[GLFW_KEY_W])
	{
		m_cameraPos += cameraSpeed * m_cameraFront;
	}

	if (m_keys[GLFW_KEY_S])
	{
		m_cameraPos -= cameraSpeed * m_cameraFront;
	}

	if (m_keys[GLFW_KEY_A])
	{
		m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	}

	if (m_keys[GLFW_KEY_D])
	{
		m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * cameraSpeed;
	}
}

void Camera::SetKeysStatus(GLuint key, GLboolean status)
{
	m_keys[key] = status;
}

