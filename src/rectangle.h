#include "main.h"

#ifndef REC_H
#define REC_H


class Rectangle {
public:
    Rectangle() {}
    Rectangle(glm::vec3 position, color_t color, float l, float b);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(glm::vec3 position);
    void tick();
private:
    VAO *object;
};

#endif // REC_H
