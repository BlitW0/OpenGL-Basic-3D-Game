#include "rectangle.h"
#include "main.h"

Rectangle::Rectangle(glm::vec3 position, color_t color, float l, float b) {
    this->position = position;
    this->rotation = 0;

    GLfloat vertex_buffer_data[] = {
        -l/2, -b/2, 0,
        -l/2, b/2, 0,
        l/2, b/2, 0,

        -l/2, -b/2, 0,
        l/2, b/2, 0,
        l/2, -b/2, 0,
    };

    this->object = create3DObject(GL_TRIANGLES, 3*2, vertex_buffer_data, color, GL_FILL);
}

void Rectangle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rectangle::set_position(glm::vec3 position) {
    this->position = position;
}
