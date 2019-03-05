#include "main.h"

#ifndef PLANE_H
#define PLANE_H


class Plane {
public:
    Plane() {}
    Plane(glm::vec3 position, color_t color, float r, float length);
    glm::vec3 position;
    glm::mat4 rotation;

    void draw(glm::mat4 VP);
    void set_position(glm::vec3 position);
    void rotate_about(int axis, float dir);
    void tick();
    bool check_collision(bounding_box_t other);

    float trans_speed;
    float rot_speed;
    float r;
    float length;
    int fuel, fuel_counter;
    int health;
    int score;

    bounding_box_t box1, box2;
private:
    VAO *object;
};

#endif // PLANE_H
