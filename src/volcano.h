#include "main.h"
#include "circle.h"

#ifndef VOL_H
#define VOL_H


class Volcano {
public:
    Volcano() {}
    Volcano(glm::vec3 position);
    glm::vec3 position;
    void draw(glm::mat4 VP);
    bounding_box_t box;
    Circle top, bottom;
    float height;
private:
    VAO *object;
    VAO *top_object;
};

#endif // VOL_H
