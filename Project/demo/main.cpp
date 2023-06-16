/* 一个学习OpenGL的demo，已配置好环境，以下代码通过贴图的方式显示一个矩形 */

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

#include <iostream>
#include <cstring>

const GLint WIDTH = 800, HEIGHT = 600;
bool keys[1024] = { false };
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat lastXPos = 400;
GLfloat lastYPos = 300;

// 偏移角和仰角
GLfloat yaw = 0;
GLfloat pitch = 0;

GLboolean firstMouse = true;

Camera camera;

// 回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
    // 初始化glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // 设置回调函数
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glewExperimental = GL_TRUE;
    // 初始化glew
    glewInit();

    // 定义视口维度
    glViewport(0, 0, WIDTH, HEIGHT);


    // 编译着色器
    Shader ourShader(R"(D:\ForStu\RenderMaster\OpenGL\LearnOpenGL\position\src\Shader\vertex.ver)", R"(D:\ForStu\RenderMaster\OpenGL\LearnOpenGL\position\src\Shader\fragment.frag)");


    // 设置顶点、颜色、纹理坐标
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 顶点位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //// 颜色
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);
    // 纹理
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(63* sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // 解绑 VAO
    glBindVertexArray(0); 

    // 创建纹理
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 加载图片
    int width, height;
    unsigned char* image = SOIL_load_image(R"(D:\ForStu\RenderMaster\OpenGL\Demo\src\Textures\container.jpg)", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); 

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    unsigned char* image2 = SOIL_load_image(R"(D:\ForStu\RenderMaster\OpenGL\Demo\src\Textures\smile.png)", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // 检查事件
        glfwPollEvents();
        //do_movement();
        camera.DoMovement(deltaTime);

        glEnable(GL_DEPTH_TEST);


        // 清除颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.GetProgram(), "ourTexture1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.GetProgram(), "ourTexture2"), 1);

        ourShader.Use();

        glm::mat4 view(1);
        glm::mat4 projection(1);
        //model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass them to the shaders
        // Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 观察矩阵
        GLfloat radius = 10.0f;
        GLfloat camX = sin(glfwGetTime()) * radius;
        GLfloat camZ = cos(glfwGetTime()) * radius;
        //view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
        view = camera.GetLookAtMatrix();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);
        for (GLuint i = 0; i < 10; i++)
        {
            glm::mat4 model(1);
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // 交换缓存
        glfwSwapBuffers(window);
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    // 清除资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

// 回调函数
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

// 键盘设置
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
    //if (firstMouse)
    //{
    //    lastXPos = xpos;
    //    lastYPos = ypos;
    //    firstMouse = false;
    //    return;
    //}

    //std::cout << xpos << " " << ypos << std::endl;
    //GLfloat xoffset = xpos - lastXPos;
    //GLfloat yoffset = lastYPos - ypos;
    //lastXPos = xpos;
    //lastYPos = ypos;

    //GLfloat sensitivity = 0.05f;
    //xoffset *= sensitivity;
    //yoffset *= sensitivity;

    //yaw += xoffset;
    //pitch += yoffset;

    //if (pitch > 89.f)
    //{
    //    pitch = 89.f;
    //}
    //else if (pitch < -89.f)
    //{
    //    pitch = -89.f;
    //}

    //glm::vec3 front;
    //front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    //front.y = sin(glm::radians(pitch));
    //front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    //cameraFront = glm::normalize(front);
}