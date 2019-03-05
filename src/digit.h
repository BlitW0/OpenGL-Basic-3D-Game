#include "main.h"
#include "rectangle.h"

#ifndef DIG_H
#define DIG_H


class Digit {
public:
    Digit() {}
    Digit(glm::vec3 position, color_t color, bool seven_segs[], float d_l, float d_b);
    glm::vec3 position;
    void draw();
    bool seven_segs[7];
    Rectangle rec[7];
private:
}; 

#endif // DIG_H
