#include "volcano.h"
#include "main.h"

Volcano::Volcano(glm::vec3 position) {
    this->position = position;

    int num_vertices = 40;
    int vertex_buffer_size = 3*(6*num_vertices);

    GLfloat vertex_buffer_data[vertex_buffer_size];

    float r = 7.0, r1 = 2.0;
    height = 10.0;

    box = bounding_box_t(position, 3*r, 1000, 3*r);

    for (int i = 0; i < num_vertices; i++) {
        float cs1 = cos(2*M_PI*(float(i)/num_vertices)), sn1 = sin(2*M_PI*(float(i)/num_vertices));
        float cs2 = cos(2*M_PI*(float(i + 1)/num_vertices)), sn2 = sin(2*M_PI*(float(i + 1)/num_vertices));

        vertex_buffer_data[18*i]      = r1*cs1;
        vertex_buffer_data[18*i + 1]  = height;
        vertex_buffer_data[18*i + 2]  = r1*sn1;;

        vertex_buffer_data[18*i + 3]  = r*cs1;
        vertex_buffer_data[18*i + 4]  = 0;
        vertex_buffer_data[18*i + 5]  = r*sn1;

        vertex_buffer_data[18*i + 6]  = r1*cs2;
        vertex_buffer_data[18*i + 7]  = height;
        vertex_buffer_data[18*i + 8]  = r1*sn2;

        vertex_buffer_data[18*i + 9]  = r1*cs2;
        vertex_buffer_data[18*i + 10] = height;
        vertex_buffer_data[18*i + 11] = r1*sn2;

        vertex_buffer_data[18*i + 12] = r*cs1;
        vertex_buffer_data[18*i + 13] = 0;
        vertex_buffer_data[18*i + 14] = r*sn1;

        vertex_buffer_data[18*i + 15] = r*cs2;
        vertex_buffer_data[18*i + 16] = 0;
        vertex_buffer_data[18*i + 17] = r*sn2;
    }

    GLfloat vertex_buffer_data_top[3*3*num_vertices];
    for (int i = 0; i < num_vertices; i++) {
        vertex_buffer_data_top[9*i]     = 0;
        vertex_buffer_data_top[9*i + 1] = 0;
        vertex_buffer_data_top[9*i + 2] = 0;

        vertex_buffer_data_top[9*i + 3] = r1*cos(2*M_PI*(float(i)/num_vertices));
        vertex_buffer_data_top[9*i + 4] = 0;
        vertex_buffer_data_top[9*i + 5] = r1*sin(2*M_PI*(float(i)/num_vertices));;

        vertex_buffer_data_top[9*i + 6] = r1*cos(2*M_PI*(float(i + 1)/num_vertices));
        vertex_buffer_data_top[9*i + 7] = 0;
        vertex_buffer_data_top[9*i + 8] = r1*sin(2*M_PI*(float(i + 1)/num_vertices));
    }
    this->top_object = create3DObject(GL_TRIANGLES, 3*num_vertices, vertex_buffer_data_top, COLOR_BRIGHT_ORANGE, GL_FILL);

    this->object = create3DObject(GL_TRIANGLES, vertex_buffer_size/3, vertex_buffer_data, COLOR_DARK_BROWN, GL_FILL);
}

void Volcano::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    Matrices.model *= (translate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);

    glm::mat4 MVP_top = VP * glm::translate(this->position + glm::vec3(0, height, 0));
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP_top[0][0]);
    draw3DObject(this->top_object);
}
