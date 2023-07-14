/* һ��ѧϰOpenGL��demo�������úû��������´���ͨ����ͼ�ķ�ʽ��ʾһ������ */

extern "C" {
#include "SOIL/SOIL.h"
}

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/glm.hpp>
#include <GLM/gtx/string_cast.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>
#include <cstring>

const GLint WIDTH = 800, HEIGHT = 600;
bool keys[1024] = { false };


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastXPos = 400;
GLfloat lastYPos = 300;

// ƫ�ƽǺ�����
GLfloat yaw = 0;
GLfloat pitch = 0;

GLboolean firstMouse = true;

Camera camera;
glm::vec3 cameraPos = camera.GetCameraPos();
glm::vec3 cameraFront = camera.GetCameraFront();
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// �ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
    // ��ʼ��glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // ��������
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // ���ûص�����
    glfwSetKeyCallback(window, key_callback);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glewExperimental = GL_TRUE;
    // ��ʼ��glew
    glewInit();

    // �����ӿ�ά��
    glViewport(0, 0, WIDTH, HEIGHT);

    Model ourModel(R"(D:\ForStu\RenderMaster\OpenGL\LearnOpenGL\mesh\src\Model\nanosuit.obj)");
    Shader modelShader(R"(D:\ForStu\RenderMaster\OpenGL\LearnOpenGL\mesh\src\Shader\Model.ver)", R"(D:\ForStu\RenderMaster\OpenGL\LearnOpenGL\mesh\src\Shader\Model.frag)");

    glEnable(GL_DEPTH_TEST);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // ����ʱ��
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // ����¼�
        glfwPollEvents();
        //do_movement();
        camera.DoMovement(deltaTime);
        // �����ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.Use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.GetAspect()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetLookAtMatrix();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        modelShader.SetInput(model, "model");
        modelShader.SetInput(projection, "projection");
        modelShader.SetInput(view, "view");
       
        ourModel.Draw(modelShader);

        // ��������
        glfwSwapBuffers(window);
    }

    // �����Դ
    glfwTerminate();
    return 0;
}

// �ص�����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        //keys[key] = true;
        camera.SetKeysStatus(key, true);
    }
    else if (action == GLFW_RELEASE)
    {
        //keys[key] = false;
        camera.SetKeysStatus(key, false);
    }

}

// ��������
void do_movement()
{
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        cameraPos += cameraSpeed * cameraFront;
    }

    if (keys[GLFW_KEY_S])
    {
        cameraPos -= cameraSpeed * cameraFront;
    }

    if (keys[GLFW_KEY_A])
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }

    if (keys[GLFW_KEY_D])
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    camera.SetCurrentPos(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.SetScroll(xoffset, yoffset);
}