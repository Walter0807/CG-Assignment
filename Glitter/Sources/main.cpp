///////////////////////////////////////////
////Type your name and student ID here/////
////Name: Wentao ZHU
////Student ID: 1155123308


// Preprocessor Directives
#define STB_IMAGE_IMPLEMENTATION

// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Shader Program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);
        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        
    }
    // Uses the current shader
    void Use()
    {
        glUseProgram(this->Program);
    }
};

// Interact functions definition
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Camera Configuration
glm::vec3 cameraPos = glm::vec3(-4.0f, 1.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -20.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = mWidth / 2.0;
GLfloat lastY = mHeight / 2.0;
bool keys[1024];
GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f;      // Time of last frame

int main(int argc, char * argv[]) {
    
    // glfw Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //Create a window
    GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, "Assignment 1", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Load OpenGL Functions
    gladLoadGL();
    // OpenGL Version Info
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    // Global Configuration
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    // Compile the shaders
    Shader ourShader("VertexShaderCode.glsl", "FragmentShaderCode.glsl");
    // Draw the vertices
    GLfloat vertices[] = {
        //    Position + Color
        2.5f,  -0.51f, 2.5f,   0.7f, 0.9f, 0.7f,
        2.5f,  -0.51f, -2.5f,  0.7f, 0.9f, 0.7f,
        -2.5f, -0.51f, 2.5f,   0.7f, 0.9f, 0.7f,
        -2.5f, -0.51f, -2.5f,  0.7f, 0.9f, 0.7f,
        
        1.5f, -0.5f, -0.5f,  1.00,0.91,0.25, //4
        1.5f, -0.5f, +0.5f,  1.00,0.91,0.25,
        1.5f,  0.5f, -0.5f,  1.00,0.91,0.25,
        1.5f,  0.5f, +0.5f,  1.00,0.91,0.25,
        
        -1.5f, -0.5f, -0.5f,  1.00,0.91,0.25,//8
        -1.5f, -0.5f, +0.5f,  1.00,0.91,0.25,
        -1.5f,  0.5f, -0.5f,  1.00,0.91,0.25,
        -1.5f,  0.5f, +0.5f,  1.00,0.91,0.250,
        
        0.7f, 0.9f, 0.0f,     0.9f, 0.0f, 0.0f, //12
        -0.7f, 0.9f, 0.0f,    0.9f, 0.0f, 0.0f,
        
        1.5f, -0.5f, 0.5f,  1.00,1.00,1.00, //14
        -1.5f, -0.5f, 0.5f, 1.00,1.00,1.00,
        1.5f, 0.5f, 0.5f,   1.00,1.00,1.00,
        -1.5f, 0.5f, 0.5f,  1.00,1.00,1.00,
        
        1.5f, -0.5f, -0.5f,  1.00,1.00,1.00, //18
        -1.5f, -0.5f, -0.5f, 1.00,1.00,1.00,
        1.5f, 0.5f, -0.5f,   1.00,1.00,1.00,
        -1.5f, 0.5f, -0.5f,  1.00,1.00,1.00,
        
        1.5f,  0.5f, -0.5f,  0.90,0.49,0.48, //22
        1.5f,  0.5f, +0.5f,  0.90,0.49,0.48,
        -1.5f,  0.5f, -0.5f, 0.90,0.49,0.48,
        -1.5f,  0.5f, +0.5f, 0.90,0.49,0.48,
        
        0.5f, -0.5f, 0.5f,   0.55f, 0.55f, 0.55f,//26
        -0.5f, -0.5f, 0.5f,  0.55f, 0.55f, 0.55f,
        0.5f, -0.5f, 2.5f,   0.55f, 0.55f, 0.55f,
        -0.5f, -0.5f, 2.5f,  0.55f, 0.55f, 0.55f,
        
        -0.3f, -0.5f, 0.5f, 0.83,0.53,0.68, //30
        -0.3f, 0.3f, 0.5f,  0.83,0.53,0.68,
         0.3f, 0.3f, 0.5f,  0.83,0.53,0.68,
         0.3f, -0.5f, 0.5f, 0.83,0.53,0.68,
        
        -1.0f, -0.5f, 1.36f,  0.87,0.43,0.53, //34
        -1.2f, -0.5f, 1.26f,  0.87,0.43,0.53,
        -1.75f, -0.5f, 1.27f,  0.87,0.43,0.53,
        -1.54f, -0.5f, 1.46f,  0.87,0.43,0.53,
        -1.47f, -0.5f, 1.86f,  0.87,0.43,0.53,
        -1.12f, -0.5f, 1.68f,  0.87,0.43,0.53,
        -1.3f, -0.5f, 1.16f,  0.87,0.43,0.53,
        -1.6f, -0.5f, 1.96f,  0.87,0.43,0.53,
        -1.1f, -0.5f, 1.6f,  0.87,0.43,0.53,
        -1.11f, -0.5f, 1.2f,  0.87,0.43,0.53,
        -2.0f, -0.5f, 1.56f,  0.87,0.43,0.53, //44
        -1.2f, -0.5f, 1.26f,  0.87,0.43,0.53,
        -1.45f, -0.5f, 1.87f,  0.87,0.43,0.53,
        -1.54f, -0.5f, 1.26f,  0.87,0.43,0.53,
        -1.27f, -0.5f, 1.36f,  0.87,0.43,0.53,
        -2.02f, -0.5f, 1.38f,  0.87,0.43,0.53,
        -1.1f, -0.5f, 1.26f,  0.87,0.43,0.53,
        -1.6f, -0.5f, 1.06f,  0.87,0.43,0.53,
        -1.49f, -0.5f, 1.63f,  0.87,0.43,0.53,
        -1.13f, -0.5f, 1.24f,  0.87,0.43,0.53,
        
        1.0f, -0.5f, 1.26f,  0.87,0.43,0.53, //54
        1.2f, -0.5f, 1.26f,  0.87,0.43,0.53,
        1.75f, -0.5f, 1.22f,  0.87,0.43,0.53,
        1.54f, -0.5f, 1.96f,  0.87,0.43,0.53,
        1.47f, -0.5f, 1.46f,  0.87,0.43,0.53,
        1.12f, -0.5f, 1.65f,  0.87,0.43,0.53,
        1.3f, -0.5f, 1.75f,  0.87,0.43,0.53,
        1.6f, -0.5f, 1.96f,  0.87,0.43,0.53,
        1.1f, -0.5f, 1.6f,  0.87,0.43,0.53,
        1.11f, -0.5f, 1.22f,  0.87,0.43,0.53,
        2.0f, -0.5f, 1.56f,  0.87,0.43,0.53, //64
        1.2f, -0.5f, 1.22f,  0.87,0.43,0.53,
        1.45f, -0.5f, 1.82f,  0.87,0.43,0.53,
        1.54f, -0.5f, 1.26f,  0.87,0.43,0.53,
        1.27f, -0.5f, 1.92f,  0.87,0.43,0.53,
        2.02f, -0.5f, 1.38f,  0.87,0.43,0.53,
        1.1f, -0.5f, 1.76f,  0.87,0.43,0.53,
        1.6f, -0.5f, 1.26f,  0.87,0.43,0.53,
        1.49f, -0.5f, 1.63f,  0.87,0.43,0.53,
        1.13f, -0.5f, 1.04f,  0.87,0.43,0.53,
        
    };
    //Draw the objects with indexing.
    GLuint indices[] = {
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        25,24,13,22,
        12,23,13,25,
        14,15,16,17,
        18,19,20,21,
        26,27,28,29,
        30,31,32,33,
        34,35,36,37,38,39,40,41,42,43,
        44,45,46,47,48,49,50,51,52,53,
        54,55,56,57,58,59,60,61,62,63,
        64,65,66,67,68,69,70,71,72,73,
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // !!! Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    // 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Set vertices position pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    // Set vertices color pointer
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glEnable( GL_LINE_SMOOTH );
    glBindVertexArray(0);
    
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
       // Process Inputs
        glfwPollEvents();
        do_movement();

        glClearColor(0.9f, 0.9f, 0.9f, 0.6f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.Use();
        glBindVertexArray(VAO);
        // Draw a Plane (grass)
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        // Draw a House
        glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GL_UNSIGNED_INT)));
        glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (GLvoid*)(12 * sizeof(GL_UNSIGNED_INT)));
        glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, (GLvoid*)(20 * sizeof(GL_UNSIGNED_INT)));
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)(28 * sizeof(GL_UNSIGNED_INT)));
        // Draw a road
        glDrawElements(GL_LINE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)(32 * sizeof(GL_UNSIGNED_INT)));
        // Draw flowers
        glPointSize(4.0f);
        glDrawElements(GL_POINTS, 40, GL_UNSIGNED_INT, (GLvoid*)(36 * sizeof(GL_UNSIGNED_INT)));

        
        // Transformation
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(45.0f, (GLfloat)mWidth / (GLfloat)mHeight, 0.1f, 100.0f);
        // Get their uniform location
        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        // Pass them to the shaders
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glfwSwapBuffers(window);
    }
    
    // Free
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    
    return 0;
}

// Get keyboard inputs
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // Esc ==> Exit
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

// Move the camera.
void do_movement()
{
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
        cameraPos += cameraUp * cameraSpeed;
    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
        cameraPos -= cameraUp * cameraSpeed;
    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (keys[GLFW_KEY_EQUAL])
                cameraPos += cameraSpeed * cameraFront;
    if (keys[GLFW_KEY_MINUS])
                cameraPos -= cameraSpeed * cameraFront;
}

// Mouse ==> angle adjustment
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    GLfloat sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    yaw += xoffset;
    pitch += yoffset;
    
// Clamped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}


