#include "circle.h"
#include "main.h"

Circle::Circle(glm::vec3 position, color_t color, int num_vertices, float r) {
    this->position = position;
    this->rotation = 0;

    GLfloat vertex_buffer_data[3*3*num_vertices];

    for (int i = 0; i < num_vertices; i++) {
        vertex_buffer_data[9*i]     = 0;
        vertex_buffer_data[9*i + 1] = 0;
        vertex_buffer_data[9*i + 2] = 0;

        vertex_buffer_data[9*i + 3] = r*cos(2*M_PI*(float(i)/num_vertices));
        vertex_buffer_data[9*i + 4] = r*sin(2*M_PI*(float(i)/num_vertices));;
        vertex_buffer_data[9*i + 5] = 0;

        vertex_buffer_data[9*i + 6] = r*cos(2*M_PI*(float(i + 1)/num_vertices));;
        vertex_buffer_data[9*i + 7] = r*sin(2*M_PI*(float(i + 1)/num_vertices));;
        vertex_buffer_data[9*i + 8] = 0;
    }


    this->object = create3DObject(GL_TRIANGLES, 3*num_vertices, vertex_buffer_data, color, GL_FILL);
}

void Circle::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Circle::set_position(glm::vec3 position) {
    this->position = position;
}

