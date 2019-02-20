#include "main.h"

#ifndef ARR_H
#define ARR_H


class Arrow {
public:
    Arrow() {}
    Arrow(glm::vec3 position, color_t color, float l, float b);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP, glm::vec3 point_to);
    void set_position(glm::vec3 position);
private:
    VAO *object;
};

#endif // ARR_H
