#include "compass.h"
#include "main.h"

Compass::Compass(glm::vec3 position, float l, float b) {
    this->position = position;
    this->rotation = 0;

    GLfloat vertex_buffer_data[] = {
        b/2, 0, 0,
        -b/2, 0, 0,
        0, l/2, 0,

        b/2, 0, 0,
        -b/2, 0, 0,
        0, -l/2, 0,
    };

    GLfloat color_buffer_data[] = {
        255, 0, 0,
        255, 0, 0,
        255, 0, 0,

        0, 0, 255,
        0, 0, 255,
        0, 0, 255,
    };

    _rep (i, 6*3)
        color_buffer_data[i] /= 255.0f;

    this->object = create3DObject(GL_TRIANGLES, 3*2, vertex_buffer_data, color_buffer_data, GL_FILL);
}

void Compass::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Compass::tick() {
    if (controls.E) this->rotation -= 1;
    if (controls.Q) this->rotation += 1;
}
