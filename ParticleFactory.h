#ifndef PARTICLE_FACTORY_H
#define PARTICLE_FACTORY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <vector>
#include "vertex.h"

#include <shader_s.h>
#include "Particle.h"

class RainFactory {
    public:
        std::vector<Rain> particles;
        unsigned int VAO, VBO, EBO;
        unsigned int nr_particles;
        glm::vec4 color, speed;
        Shader shader;

        RainFactory(unsigned int nr_particles, glm::vec4 color, glm::vec4 speed){
            Shader nshader("shaders/particle.vs", "shaders/particle.fs");
            this->shader = nshader;
            this->nr_particles = nr_particles;
            this->color = color;
            for(unsigned int i = 0; i < nr_particles; i++) {
                float x = (rand()%200)/100.0 - 1;
                float y = (rand()%200)/100.0 - 1;
                float z = (rand()%200)/100.0 - 1;
                particles.push_back(Rain(glm::vec4(x, y, z, 0.0), speed));
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(base_particle), base_particle, GL_STREAM_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(base_particle_indices), base_particle_indices, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);
        }

        void draw() {
            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            shader.setMat4("transform", transform);
            shader.setVec4("color", color);
            glBindVertexArray(VAO);

            for (int i=0; i<nr_particles; i++) {
                shader.setVec4("offset", particles[i].offset);
                glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
                particles[i].update();
            }
        }
};


class SmokeFactory {
    public:
        std::vector<Smoke> particles;
        unsigned int VAO, VBO, EBO;
        unsigned int nr_particles;
        glm::vec4 color, speed;
        Shader shader;

        SmokeFactory(unsigned int nr_particles, glm::vec4 color, glm::vec4 origin){
            Shader nshader("shaders/particle.vs", "shaders/particle.fs");
            this->shader = nshader;
            this->nr_particles = nr_particles;
            this->color = color;
            for(unsigned int i = 0; i < nr_particles; i++) {
                float vx = random(0.001f, 0.003f);
                float vy = random(-0.001f, 0.001f);
                float vz = random(-0.001f, 0.001f);
                glm::vec4 speed = glm::vec4(vx, vy, vz, 0.0f);
                particles.push_back(Smoke(origin, speed, origin, origin + glm::vec4(0.15f)));
            }

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(base_particle), base_particle, GL_STREAM_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(base_particle_indices), base_particle_indices, GL_STREAM_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);
        }

        void draw() {
            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            shader.setMat4("transform", transform);
            shader.setVec4("color", color);
            glBindVertexArray(VAO);

            for (int i=0; i<nr_particles; i++) {
                shader.setVec4("offset", particles[i].offset);
                glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
                particles[i].update();
            }
        }
};

#endif
