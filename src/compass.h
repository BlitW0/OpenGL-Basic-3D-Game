#include "main.h"

#ifndef COMP_H
#define COMP_H


class Compass {
public:
    Compass() {}
    Compass(glm::vec3 position, float l, float b);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void tick();
private:
    VAO *object;
};

#endif // COMP_H
