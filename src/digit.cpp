#include "digit.h"
#include "main.h"

Digit::Digit(glm::vec3 position, color_t color, bool seven_segs[], float d_l, float d_b) {
    
    this->position = position;

    rec[0]      = Rectangle(glm::vec3(0, 0, 0), color, d_l, d_b);
    rec[1]      = Rectangle(glm::vec3(d_l/2 + d_b/2, d_l/2 + d_b/2, 0), color, d_b, d_l + 2*d_b);
    rec[2]      = Rectangle(glm::vec3(0, d_b + d_l, 0), color, d_l, d_b);
    rec[3]      = Rectangle(glm::vec3(-d_l/2 - d_b/2, d_l/2 + d_b/2, 0), color, d_b, d_l + 2*d_b);
    rec[4]      = Rectangle(glm::vec3(-d_l/2 - d_b/2, -d_l/2 - d_b/2, 0), color, d_b, d_l + 2*d_b);
    rec[5]      = Rectangle(glm::vec3(0, -d_b - d_l, 0), color, d_l, d_b);
    rec[6]      = Rectangle(glm::vec3(d_l/2 + d_b/2, -d_l/2 - d_b/2, 0), color, d_b, d_l + 2*d_b);

    _rep(i, 7) {
        rec[i].position += position;
        this->seven_segs[i] = seven_segs[i];
    }

}


void Digit::draw() {
     
    _rep(i, 7)
        if (this->seven_segs[i])
            rec[i].draw(glm::mat4(1.0f));

}
