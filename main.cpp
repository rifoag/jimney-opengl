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

using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int rotate = 0;


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
    Shader ourShader("shaders/jimney.vs", "shaders/jimney.fs"); 

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 light_model = glm::mat4(1.0);
    light_model = glm::translate(light_model, lightPos);
    light_model = glm::scale(light_model, glm::vec3(0.2f));

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    // JIMNEY OBJECT
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/metal.jpg", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);

    Shader light_shader("shaders/lighting.vs", "shaders/lighting.fs");


    // LIGHT
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
    

    // Particles
    unsigned int nr_particles = 1;
    vector<Particle> particles;
    for(unsigned int i = 0; i < nr_particles; i++) {
        float x = (rand()%100)/100.0;
        float y = (rand()%100)/100.0;
        float z = (rand()%100)/100.0;
        particles.push_back(Particle(glm::vec4(x, y, z, 0.0)));
    }

    unsigned int particleVAO, particleVBO, particleEBO;
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);
    glGenBuffers(1, &particleEBO);
    glBindVertexArray(particleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(base_particle), base_particle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(base_particle_indices), base_particle_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 transform = glm::mat4(1.0);
    transform = glm::scale(transform, glm::vec3(0.05, 0.05, 0.05));

    Shader particle_shader("shaders/particle.vs", "shaders/particle.fs");


    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // JIMNEY
        model = glm::rotate(model, glm::radians(1.0f) * rotate, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        view = glm::rotate(view, glm::radians(1.0f) * rotate, glm::vec3(0.0f, 1.0f, 0.0f));

        #ifdef JIMNEY
        ourShader.use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);

        glDrawElements(GL_TRIANGLES, 69, GL_UNSIGNED_INT, 0);
        #endif


        #ifdef LIGHT
        light_shader.use();
        light_shader.setMat4("model", light_model);
        light_shader.setMat4("view", view);
        light_shader.setMat4("projection", projection);

        glBindVertexArray(lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        #endif


        //PARTICLE
        particle_shader.setMat4("model", model);
        particle_shader.use();
        particle_shader.setMat4("view", view);
        particle_shader.setMat4("projection", projection);
        particle_shader.setMat4("transform", transform);
        for (Particle particle: particles) {
            //particle_shader.setVec2("offset", particle.Position.x, particle.Position.y);
            //particle_shader.setVec4("color", particle.color);
            cout<< particle.offset.x << endl;
            particle_shader.setVec4("offset", particle.offset);
            glBindVertexArray(particleVAO);
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        }

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
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraUp * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rotate = 1;
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rotate = -1;
    else
        rotate = 0;
}
