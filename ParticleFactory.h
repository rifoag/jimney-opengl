#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream> 
#include <vector>
#include "vertex.h"

#include <shader_s.h>

class ParticleFactory {
    public:
        std::vector<Particle> particles;
        unsigned int VAO, VBO, EBO;
        unsigned int nr_particles;
        glm::vec4 color;
        float speed;

        ParticleFactory(unsigned int nr_particles, glm::vec4 color, float speed, Shader shader){
            this->nr_particles = nr_particles;
            this->color = color;
            this->speed = speed;
            for(unsigned int i = 0; i < nr_particles; i++) {
                float x = (rand()%200)/100.0 - 1;
                float y = (rand()%200)/100.0 - 1;
                float z = (rand()%200)/100.0 - 1;
                particles.push_back(Particle(glm::vec4(x, y, z, 0.0)));
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
            Shader shader("shaders/particle.vs", "shaders/particle.fs");
            shader.use();
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            shader.setMat4("transform", transform);
            shader.setVec4("color", color);
            for (int i=0; i<nr_particles; i++) {
                shader.setVec4("offset", particles[i].offset);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
                particles[i].offset += glm::vec4(0.0f, -speed, 0.0f, 0.0f);
                if (particles[i].offset.y <= -1.0f) {
                    particles[i].offset.y = 1.0f;
                }
            }
        }
};

#endif
