

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<SOIL2/soil2.h>
#include <time.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include<stdlib.h>
#include "shader_m.h"
#include "camera.h"
#include "model.h"
#include "terrain.h"
#include "TestCube.h"
#include<filesystem>
#include<stack>

#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"Opengl32.lib")

#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"soil2-debug.lib")
#pragma comment(lib,"assimp-vc143-mtd.lib")

#include<vector>

#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, -3.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//树的数量
int treeNum = 10;
int fernNum = 10;
vector<glm::vec3> positionData;
glm::vec3 lightDir = glm::vec3(0.0, -1.0, -1.0);
glm::vec3 lightColor = glm::vec3(0.0, 0.5, 1.0);
glm::vec3 fogColor = glm::vec3(0.5, 0.5,0.5);
//glm::vec3 lightColor = glm::vec3(1.0, 0.0, 0.0);

//切换shader程序
bool isNormal = true;


float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "1901010227", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader normalShader("normalVertexShader.glsl", "normalFragShader.glsl");
    Shader reverseShader("reverseVertexShader.glsl", "reverseFragShader.glsl");
    Shader grassShader("grassVertShader.glsl", "grassFragShader.glsl");

    Model nanosuit("nanosuit/nanosuit.obj");
    Model tree("res/treeMat.obj");
    Model Grass("res/GrassMat.obj");
    Model Fern("res/Fern.obj");

    Shader treeShader("treeVertexShader.glsl", "treeFragShader.glsl");

    Shader skyboxShader("skybox_vertex.glsl", "skybox_frag.glsl");
    Terrain terrain(0.1, 0.1, "res/grass.png");
    //Terrain terrain(0, 0, "res/pinkFlowers.png");
    
    Shader terrainShader("terrain_vertex.glsl", "terrain_fargment.glsl");

    TestCube cube("res/grass.png");
    Shader cubeShader("terrain_vertex.glsl", "terrain_fargment.glsl");

    

    //Shader lightShader("multi_light_vertex.glsl", "multi_light_frag.glsl");

    

   
   
    glm::vec3 cubePositions[] = {
  glm::vec3( 10.0f,  -5.0f,  5.0f), 
  glm::vec3(-10.0f,  -5.0f,  -5.0f),
  glm::vec3( 12.0f,  -5.0f, -10.0f), 
  glm::vec3(-12.0f,  -5.0f, 10.0f),
  glm::vec3(-5.5f, -5.0f, -4.5f),  
  glm::vec3(5.5f, -5.0f, 4.5f),
  glm::vec3(-3.8f, -5.0f, -6.3f),  
  glm::vec3(3.8f, -5.0f, 6.3f),
  glm::vec3( 2.4f, -5.0f, -3.5f),  
  glm::vec3(-2.4f, -5.0f, 3.5f),
  glm::vec3(-1.7f,  -5.0f, -7.5f),  
  glm::vec3(1.7f,  -5.0f, 7.5f),
  glm::vec3( 10.3f, -5.0f, -5.5f),  
  glm::vec3(-10.3f, -5.0f, 5.5f),
  glm::vec3( 12.5f,  -5.0f, -2.5f), 
  glm::vec3(-12.5f,  -5.0f, 2.5f),
  glm::vec3( 8.5f,  -5.0f, -4.5f), 
  glm::vec3(-8.5f,  -5.0f, 4.5f),
  glm::vec3(-9.3f,  -5.0f, -5.5f),
  glm::vec3(9.3f,  -5.0f, 5.5f)
};
    glm::vec3 fernPositions[] = {
  glm::vec3(8.0f,  -5.0f,  2.0f),
  glm::vec3(-8.0f,  -5.0f,  -8.0f),
  glm::vec3(10.0f,  -5.0f, -10.0f),
  glm::vec3(-15.0f,  -5.0f, 8.0f),
  glm::vec3(-8.0f, -5.0f, -6.0f),
  glm::vec3(6.4f, -5.0f, 8.0f),
  glm::vec3(-5.0f, -5.0f, -7.0f),
  glm::vec3(5.4f, -5.0f, 8.3f),
  glm::vec3(3.4f, -5.0f, -3.5f),
  glm::vec3(-1.4f, -5.0f, 3.5f),
  glm::vec3(-6.7f,  -5.0f, -7.5f),
  glm::vec3(1.7f,  -5.0f, 2.5f),
  glm::vec3(10.3f, -5.0f, -5.5f),
  glm::vec3(-8.3f, -5.0f, 5.5f),
  glm::vec3(6.5f,  -5.0f, -2.5f),
  glm::vec3(-12.5f,  -5.0f, 2.5f),
  glm::vec3(7.5f,  -5.0f, -5.5f),
  glm::vec3(-10.5f,  -5.0f, 4.5f),
  glm::vec3(-10.3f,  -5.0f, -7.5f),
  glm::vec3(9.3f,  -5.0f, 7.5f)
    };
    

 
    
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    unsigned int cubeTexture = loadTexture("container.jpg");

    vector<std::string> faces
    {
        
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // shader configuration
    // --------------------
    /*shader.use();
    shader.setInt("texture1", 0);*/

    /*treeShader.use();
    treeShader.setInt("texture1", 0);*/
    normalShader.use();


    terrainShader.use();
    terrainShader.setInt("material.texture_diffuse1", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);


    
   

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //获取每一帧的间隔时间
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //键盘输入
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        //背面剔除
        glEnable(GL_CULL_FACE);

        // draw scene as normal
        if (isNormal)
        {
            normalShader.use();
            normalShader.setVec3("fogColor", fogColor);
            normalShader.setInt("skybox", 4);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            model = glm::mat4(1.0f);
            //model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -5.1f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

            view = camera.GetViewMatrix();
            //view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            //projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            normalShader.setMat4("model", model);
            normalShader.setMat4("view", view);
            normalShader.setVec3("cameraPos", camera.Position);
            normalShader.setMat4("projection", projection);

            //光源属性设置
            normalShader.setVec3("dirLight.direction", lightDir);
            normalShader.setVec3("dirLight.ambient", 0.8f, 0.8f, 0.8f);
            normalShader.setVec3("dirLight.diffuse", lightColor);
            normalShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            //材质属性
            normalShader.setFloat("material.shininess", 96.0f);

            nanosuit.Draw(normalShader);
        }
        else
        {
            reverseShader.use();

            model = glm::mat4(1.0f);
            //model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, -5.1f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2, 0.2, 0.2));

             view = camera.GetViewMatrix();
            //view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            //projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
            reverseShader.setMat4("model", model);
            reverseShader.setMat4("view", view);
            reverseShader.setMat4("projection", projection);
            nanosuit.Draw(reverseShader);
        }
        

        
        
        treeShader.use();
        treeShader.setVec3("fogColor", fogColor);
        //光源属性设置
        treeShader.setVec3("dirLight.direction", lightDir);
        treeShader.setVec3("dirLight.ambient", 0.8f, 0.8f, 0.8f);
        treeShader.setVec3("dirLight.diffuse", lightColor);
        treeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //材质属性
        treeShader.setFloat("material.shininess", 96.0f);
        for (int i=0; i < treeNum; i++)
        {
            
            
            model = glm::mat4(1.0f);
            
            //model = glm::translate(model, cubePositions[i]);
            model = glm::translate(model, cubePositions[i]);
            model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));
            treeShader.setMat4("model", model);
            treeShader.setMat4("view", view);
            treeShader.setMat4("projection", projection);
            tree.Draw(treeShader);
            
            
        }
        
        

        terrainShader.use();
        terrainShader.setVec3("fogColor", fogColor);
        //光源属性设置
        terrainShader.setVec3("dirLight.direction", lightDir);
        terrainShader.setVec3("dirLight.ambient", 0.8f, 0.8f, 0.8f);
        terrainShader.setVec3("dirLight.diffuse", lightColor);
        terrainShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //材质属性
        terrainShader.setFloat("material.shininess", 96.0f);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1, 1, 1));
        model = glm::translate(model, glm::vec3(-50.0f, -5.0f, -50.0f));
        terrainShader.setMat4("model", model);
        terrainShader.setMat4("view", view);
        terrainShader.setMat4("projection", projection);

        terrain.Draw(terrainShader);

        /*cubeShader.use();
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(1, 1, 1));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        cubeShader.setMat4("model", model);
        cubeShader.setMat4("view", view);
        cubeShader.setMat4("projection", projection);
        cube.Draw(cubeShader);*/
        

        glDisable(GL_CULL_FACE);
        grassShader.use();
        grassShader.setVec3("fogColor", fogColor);
        //光源属性设置
        grassShader.setVec3("dirLight.direction", lightDir);
        grassShader.setVec3("dirLight.ambient", 0.8f, 0.8f, 0.8f);
        grassShader.setVec3("dirLight.diffuse", lightColor);
        grassShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //材质属性
        grassShader.setFloat("material.shininess", 96.0f);
        model = glm::mat4(1.0f);

        //model = glm::translate(model, cubePositions[i]);
       
        model = glm::scale(model, glm::vec3(1.0, 1.0, 1.0));
        model = glm::translate(model, glm::vec3(0.0, -5.0, 0.0));
        grassShader.setMat4("model", model);
        grassShader.setMat4("view", view);
        grassShader.setMat4("projection", projection);
        Grass.Draw(grassShader);

        
       
        for (int i = 0; i < fernNum; i++)
        {


            model = glm::mat4(1.0f);

            //model = glm::translate(model, cubePositions[i]);
            model = glm::translate(model, fernPositions[i]);
            model = glm::scale(model, glm::vec3(0.3, 0.3, 0.3));
            grassShader.setMat4("model", model);
            grassShader.setMat4("view", view);
            grassShader.setMat4("projection", projection);
            Fern.Draw(grassShader);


        }



        

        // 绘制天空盒
        glDepthFunc(GL_LEQUAL);  // 修改深度测试模式
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // 移除位移量
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    //glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &skyboxVAO);
    //glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        isNormal = !isNormal;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    
    glViewport(0, 0, width, height);
}

//鼠标移动回调函数
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//鼠标滚轮回调函数
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// 加载2D贴图文件

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = SOIL_load_image(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        SOIL_free_image_data(data);
    }

    return textureID;
}

// 配置天空盒
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

