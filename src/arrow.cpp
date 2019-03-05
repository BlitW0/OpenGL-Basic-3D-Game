#include "arrow.h"
#include "main.h"

Arrow::Arrow(glm::vec3 position, color_t color, float l, float b) {
    this->position = position;
    this->rotation = 0;

    GLfloat vertex_buffer_data[] = {
        -l/2, 0, -b/2,
        -l/2, 0, b/2,
        l/2,  0, b/2,

        -l/2, 0, -b/2,
        l/2, 0, b/2,
        l/2, 0, -b/2,

        0, 0, -b/2,
        0, 0, -b,
        b, 0, -b/2,

        0, 0, -b/2,
        0, 0, -b,
        -b, 0, -b/2,
    };

    this->object = create3DObject(GL_TRIANGLES, 36/3, vertex_buffer_data, color, GL_FILL);
}

void Arrow::draw(glm::mat4 VP, glm::vec3 point_to) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    
    glm::vec3 to_axis = point_to - this->position;
    to_axis /= glm::length(to_axis);

    glm::vec3 rotate_about = glm::cross(glm::vec3(0, 0, -1), to_axis);

    float angle = acos(glm::dot(glm::vec3(0, 0, -1), to_axis));
    glm::mat4 rotate = glm::rotate(angle, rotate_about);

    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Arrow::set_position(glm::vec3 position) {
    this->position = position;
}

