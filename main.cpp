#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <vector>

#include <shader_s.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "vertex.h"
#include "ParticleFactory.h"
#include "Particle.h"

using namespace std;


#define JIMNEY
#define LIGHT


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int rotate = 0;
float sudut = 0;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Jimney", NULL, NULL);

    if (window == NULL) {
        cout << "Failed to create window" << endl;
        glfwTerminate();
        exit(0);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize glad" << endl;
        exit(0);
    }
    

    // JIMNEY OBJECT
    Shader ourShader("shaders/jimney.vs", "shaders/jimney.fs"); 
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Wheel
    Shader wheelShader("shaders/wheel.vs", "shaders/wheel.fs"); 
    unsigned int wheelVBO, wheelVAO, wheelEBO;
    glGenVertexArrays(1, &wheelVAO);
    glGenBuffers(1, &wheelVBO);
    glGenBuffers(1, &wheelEBO);
    glBindVertexArray(wheelVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wheelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wheel_vertices), wheel_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wheelEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(wheel_indices), wheel_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);    

    // LIGHT
    Shader light_shader("shaders/lighting.vs", "shaders/lighting.fs");
    unsigned int lightVAO, lightVBO, lightEBO;
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(light_sources), light_sources, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    

    // COORDINATE SYSTEM
    model = glm::mat4(1.0f);
    view = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    glm::mat4 model_light = glm::mat4(1.0);
    model_light = glm::translate(model_light, lightPos);
    model_light = glm::scale(model_light, glm::vec3(0.2f));
    glm::mat4 jtransform = glm::mat4(1.0);
    jtransform = glm::scale(jtransform, glm::vec3(0.1, 0.1, 0.1));

    transform = glm::mat4(1.0);
    transform = glm::scale(transform, glm::vec3(0.0025, 0.005, 0.005));

    glm::vec4 rain_speed = glm::vec4(0.0f, -0.01f, 0.0f, 0.0f);
    RainFactory rain = RainFactory(5000, glm::vec4(0.0f,1.0f,1.0f,1.0f), rain_speed);
    RainFactory rain2 = RainFactory(5000, glm::vec4(1.0f,0.0f,0.0f,1.0f), rain_speed);
    RainFactory rain3 = RainFactory(5000, glm::vec4(1.0f,1.0f,0.0f,1.0f), rain_speed);
    glm::vec4 smoke_origin = glm::vec4(0.1,0.0001,0.0,0.0);
    SmokeFactory smoke = SmokeFactory(5000, glm::vec4(0.0,0.0,0.0,0.0), smoke_origin);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view = glm::rotate(view, glm::radians(sudut), glm::vec3(0.0f,1.0f,0.0f));

        // JIMNEY
        #ifdef JIMNEY
        ourShader.use();
        glBindVertexArray(VAO);

        ourShader.setMat4("model", model * jtransform);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

        glDrawElements(GL_TRIANGLES, 69, GL_UNSIGNED_INT, 0);
        // WHEEL & ORNAMENT
        wheelShader.use();
        glBindVertexArray(wheelVAO);
        wheelShader.setMat4("model", model * jtransform);
        wheelShader.setMat4("view", view);
        wheelShader.setMat4("projection", projection);
        wheelShader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
        wheelShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        wheelShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
        glDrawElements(GL_TRIANGLES, 210, GL_UNSIGNED_INT, 0);
        #endif


        #ifdef LIGHT
        light_shader.use();
        light_shader.setMat4("model", model_light);
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        #endif

        rain.draw();
        rain2.draw();
        rain3.draw();
        smoke.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVAO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && cameraPos.z < 1.0f)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        sudut -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        sudut += 1.0f;
}
