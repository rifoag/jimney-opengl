#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
    public: 
        glm::vec4 offset;
        glm::vec4 speed;
        Particle(glm::vec4 offset){
            this->offset = offset;
        }
        Particle(glm::vec4 offset, glm::vec4 speed){
            this->offset = offset;
            this->speed = speed;
        }

        void update() {
            this->offset += this->speed;
            if (this->offset.y <= -1.0f) {
                this->offset.y = 1.0f;
            }
        }
};

#endif
