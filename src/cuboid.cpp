#include "cuboid.h"
#include "main.h"

Cuboid::Cuboid(glm::vec3 position, color_t color, float l, float b, float h) {
    this->position = position;
    this->rotation = glm::vec3(0, 0, 0);

    this->l = l; // along z-axis
    this->b = b; // along x-axis
    this->h = h; // along y-axis

    speed = 0.3;
    is_axis_set = 0;

    GLfloat vertex_buffer_data[] = {
        -b/2, h/2, l/2,
        -b/2, -h/2, l/2,
        b/2, h/2, l/2,

        -b/2, -h/2, l/2,
        b/2, h/2, l/2,
        b/2, -h/2, l/2,

        -b/2, h/2, l/2,
        -b/2, h/2, -l/2,
        b/2, h/2, -l/2,

        b/2, h/2, -l/2,
        -b/2, h/2, l/2,
        b/2, h/2, l/2,

        -b/2, -h/2, -l/2,
        -b/2, h/2, -l/2,
        -b/2, h/2, l/2,

        -b/2, h/2, l/2,
        -b/2, -h/2, -l/2,
        -b/2, -h/2, l/2,

        b/2, -h/2, -l/2,
        b/2, h/2, -l/2,
        b/2, h/2, l/2,

        b/2, h/2, l/2,
        b/2, -h/2, -l/2,
        b/2, -h/2, l/2,

        -b/2, h/2, -l/2,
        -b/2, -h/2, -l/2,
        b/2, h/2, -l/2,

        -b/2, -h/2, -l/2,
        b/2, h/2, -l/2,
        b/2, -h/2, -l/2,

        -b/2, -h/2, l/2,
        -b/2, -h/2, -l/2,
        b/2, -h/2, -l/2,

        b/2, -h/2, -l/2,
        -b/2, -h/2, l/2,
        b/2, -h/2, l/2,
    };

    this->object = create3DObject(GL_TRIANGLES, 12*3, vertex_buffer_data, color, GL_FILL);
}

void Cuboid::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    
    glm::mat4 rotate    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Cuboid::set_position(glm::vec3 position) {
    this->position = position;
}

bounding_box_t Cuboid::box() {
    return bounding_box_t(this->position, this->b, this->h, this->l);
}

void Cuboid::set_axis(glm::vec3 axis) {
    this->is_axis_set = 1;
    axis /= glm::length(axis);
    this->move_axis = axis;
}

void Cuboid::tick() {
    if (is_axis_set) {
        set_position(position + move_axis*speed);
    }
}
