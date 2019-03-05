#include "main.h"

#ifndef CUBO_H
#define CUBO_H


class Cuboid {
public:
    Cuboid() {}
    Cuboid(glm::vec3 position, color_t color, float l, float b, float h);
    glm::vec3 position;
    glm::vec3 rotation;
    void draw(glm::mat4 VP);
    void set_position(glm::vec3 position);
    void tick();
    bounding_box_t box();
    void set_axis(glm::vec3 axis);

    float l, b, h;
    
    float speed;
    glm::vec3 move_axis;
    bool is_axis_set;
private:
    VAO *object;
};

#endif // CUBO_H
