#include "main.h"

#ifndef CIRC_H
#define CIRC_H


class Circle {
public:
    Circle() {}
    Circle(glm::vec3 position, color_t color, int num_vertices, float r);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(glm::vec3 position);
    // void tick();
private:
    VAO *object;
};

#endif // CIRC_H
