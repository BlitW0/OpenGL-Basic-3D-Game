#include "plane.h"
#include "main.h"

Plane::Plane(glm::vec3 position, color_t color, float r, float length) {
    this->position = position;
    this->rotation = glm::mat4(1.0f);

    fuel = 100;
    fuel_counter = 0;
    health = 100;
    score = 0;

    trans_speed = 0.5;
    rot_speed = 1;

    this->r = r;
    this->length = length;

    int circle_vert_num = 40; // Number of circle vertices
    
    int vertex_buffer_size = 3*(
        + 6*circle_vert_num + 6*circle_vert_num + 6*circle_vert_num // Cylinder
        + 2*(3 + 6) // Wings
    ); // Total size all coordinates
    
    int vertex_num_offset = 0; // offset from start of buffer

    GLfloat vertex_buffer_data[vertex_buffer_size];

    for (int i = vertex_num_offset; i < vertex_num_offset + circle_vert_num; i++) {
        float cs1 = cos(2*M_PI*(float(i)/circle_vert_num)), sn1 = sin(2*M_PI*(float(i)/circle_vert_num));
        float cs2 = cos(2*M_PI*(float(i + 1)/circle_vert_num)), sn2 = sin(2*M_PI*(float(i + 1)/circle_vert_num));

        vertex_buffer_data[18*i]      = r*cs1;
        vertex_buffer_data[18*i + 1]  = r*sn1;
        vertex_buffer_data[18*i + 2]  = -length/2;

        vertex_buffer_data[18*i + 3]  = r*cs1;
        vertex_buffer_data[18*i + 4]  = r*sn1;
        vertex_buffer_data[18*i + 5]  = length/2;

        vertex_buffer_data[18*i + 6]  = r*cs2;
        vertex_buffer_data[18*i + 7]  = r*sn2;
        vertex_buffer_data[18*i + 8]  = -length/2;

        vertex_buffer_data[18*i + 9]  = r*cs2;
        vertex_buffer_data[18*i + 10] = r*sn2;
        vertex_buffer_data[18*i + 11] = -length/2;

        vertex_buffer_data[18*i + 12] = r*cs1;
        vertex_buffer_data[18*i + 13] = r*sn1;
        vertex_buffer_data[18*i + 14] = length/2;

        vertex_buffer_data[18*i + 15] = r*cs2;
        vertex_buffer_data[18*i + 16] = r*sn2;
        vertex_buffer_data[18*i + 17] = length/2;
    }
    vertex_num_offset += circle_vert_num;

    float head_len = length/6;
    float r1 = r/sqrt(2), r2 = r/(16);
    // r1 -> head_len/2 from Plane, r2 -> head_len - head_len/16 from Plane

    for (int i = vertex_num_offset; i < vertex_num_offset + circle_vert_num; i++) {
        float cs1 = cos(2*M_PI*(float(i)/circle_vert_num)), sn1 = sin(2*M_PI*(float(i)/circle_vert_num));
        float cs2 = cos(2*M_PI*(float(i + 1)/circle_vert_num)), sn2 = sin(2*M_PI*(float(i + 1)/circle_vert_num));

        vertex_buffer_data[18*i]      = r1*cs1;
        vertex_buffer_data[18*i + 1]  = r1*sn1;
        vertex_buffer_data[18*i + 2]  = -length/2 - head_len/2;

        vertex_buffer_data[18*i + 3]  = r*cs1;
        vertex_buffer_data[18*i + 4]  = r*sn1;
        vertex_buffer_data[18*i + 5]  = -length/2;

        vertex_buffer_data[18*i + 6]  = r1*cs2;
        vertex_buffer_data[18*i + 7]  = r1*sn2;
        vertex_buffer_data[18*i + 8]  = -length/2 - head_len/2;

        vertex_buffer_data[18*i + 9]  = r1*cs2;
        vertex_buffer_data[18*i + 10] = r1*sn2;
        vertex_buffer_data[18*i + 11] = -length/2 - head_len/2;

        vertex_buffer_data[18*i + 12] = r*cs1;
        vertex_buffer_data[18*i + 13] = r*sn1;
        vertex_buffer_data[18*i + 14] = -length/2;

        vertex_buffer_data[18*i + 15] = r*cs2;
        vertex_buffer_data[18*i + 16] = r*sn2;
        vertex_buffer_data[18*i + 17] = -length/2;
    }
    vertex_num_offset += circle_vert_num;

    for (int i = vertex_num_offset; i < vertex_num_offset + circle_vert_num; i++) {
        float cs1 = cos(2*M_PI*(float(i)/circle_vert_num)), sn1 = sin(2*M_PI*(float(i)/circle_vert_num));
        float cs2 = cos(2*M_PI*(float(i + 1)/circle_vert_num)), sn2 = sin(2*M_PI*(float(i + 1)/circle_vert_num));

        vertex_buffer_data[18*i]      = r2*cs1;
        vertex_buffer_data[18*i + 1]  = r2*sn1;
        vertex_buffer_data[18*i + 2]  = -length/2 - (head_len - head_len/16);

        vertex_buffer_data[18*i + 3]  = r1*cs1;
        vertex_buffer_data[18*i + 4]  = r1*sn1;
        vertex_buffer_data[18*i + 5]  = -length/2 - head_len/2;

        vertex_buffer_data[18*i + 6]  = r2*cs2;
        vertex_buffer_data[18*i + 7]  = r2*sn2;
        vertex_buffer_data[18*i + 8]  = -length/2 - (head_len - head_len/16);

        vertex_buffer_data[18*i + 9]  = r2*cs2;
        vertex_buffer_data[18*i + 10] = r2*sn2;
        vertex_buffer_data[18*i + 11] = -length/2 - (head_len - head_len/16);

        vertex_buffer_data[18*i + 12] = r1*cs1;
        vertex_buffer_data[18*i + 13] = r1*sn1;
        vertex_buffer_data[18*i + 14] = -length/2 - head_len/2;

        vertex_buffer_data[18*i + 15] = r1*cs2;
        vertex_buffer_data[18*i + 16] = r1*sn2;
        vertex_buffer_data[18*i + 17] = -length/2 - head_len/2;
    }
    vertex_num_offset += circle_vert_num;

    vertex_num_offset *= 6*3; // 6 vertices per side of cylinder

    for (int i = vertex_num_offset; i < vertex_num_offset + 1; i++) {
        vertex_buffer_data[i]      = r;
        vertex_buffer_data[i + 1]  = 0;
        vertex_buffer_data[i + 2]  = -0.5 - 0.3;

        vertex_buffer_data[i + 3]  = r;
        vertex_buffer_data[i + 4]  = 0;
        vertex_buffer_data[i + 5]  = 0 - 0.3;

        vertex_buffer_data[i + 6]  = 2.5;
        vertex_buffer_data[i + 7]  = 0;
        vertex_buffer_data[i + 8]  = 0.5 - 0.3;
    }
    vertex_num_offset += 3*3;

    for (int i = vertex_num_offset; i < vertex_num_offset + 1; i++) {
        vertex_buffer_data[i]      = -r;
        vertex_buffer_data[i + 1]  = 0;
        vertex_buffer_data[i + 2]  = -0.5 - 0.3;

        vertex_buffer_data[i + 3]  = -r;
        vertex_buffer_data[i + 4]  = 0;
        vertex_buffer_data[i + 5]  = 0 - 0.3;

        vertex_buffer_data[i + 6]  = -2.5;
        vertex_buffer_data[i + 7]  = 0;
        vertex_buffer_data[i + 8]  = 0.5 - 0.3;
    }
    vertex_num_offset += 3*3;

    for (int i = vertex_num_offset; i < vertex_num_offset + 1; i++) {
        vertex_buffer_data[i]      = r;
        vertex_buffer_data[i + 1]  = 0;
        vertex_buffer_data[i + 2]  = 0.5 - 0.3;

        vertex_buffer_data[i + 3]  = r;
        vertex_buffer_data[i + 4]  = 0;
        vertex_buffer_data[i + 5]  = 0 - 0.3;

        vertex_buffer_data[i + 6]  = 2.5;
        vertex_buffer_data[i + 7]  = 0;
        vertex_buffer_data[i + 8]  = 0.5 - 0.3;

        vertex_buffer_data[i + 9]  = 2.5;
        vertex_buffer_data[i + 10] = 0;
        vertex_buffer_data[i + 11] = 0.5 - 0.3;

        vertex_buffer_data[i + 12] = r;
        vertex_buffer_data[i + 13] = 0;
        vertex_buffer_data[i + 14] = 0.5 - 0.3;

        vertex_buffer_data[i + 15] = 2.3;
        vertex_buffer_data[i + 16] = 0;
        vertex_buffer_data[i + 17] = 0.65 - 0.3;
    }
    vertex_num_offset += 6*3;

    for (int i = vertex_num_offset; i < vertex_num_offset + 1; i++) {
        vertex_buffer_data[i]      = -r;
        vertex_buffer_data[i + 1]  = 0;
        vertex_buffer_data[i + 2]  = 0.5 - 0.3;

        vertex_buffer_data[i + 3]  = -r;
        vertex_buffer_data[i + 4]  = 0;
        vertex_buffer_data[i + 5]  = 0 - 0.3;

        vertex_buffer_data[i + 6]  = -2.5;
        vertex_buffer_data[i + 7]  = 0;
        vertex_buffer_data[i + 8]  = 0.5 - 0.3;

        vertex_buffer_data[i + 9]  = -2.5;
        vertex_buffer_data[i + 10] = 0;
        vertex_buffer_data[i + 11] = 0.5 - 0.3;

        vertex_buffer_data[i + 12] = -r;
        vertex_buffer_data[i + 13] = 0;
        vertex_buffer_data[i + 14] = 0.5 - 0.3;

        vertex_buffer_data[i + 15] = -2.3;
        vertex_buffer_data[i + 16] = 0;
        vertex_buffer_data[i + 17] = 0.65 - 0.3;
    }
    vertex_num_offset += 6*3;

    this->object = create3DObject(GL_TRIANGLES, vertex_buffer_size/3, vertex_buffer_data, color, GL_FILL);
}

void Plane::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef

    // glm::mat4 rotate    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotation);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Plane::set_position(glm::vec3 position) {
    this->position = position;
}

void Plane::rotate_about(int axis, float dir) {
    this->rotation = glm::rotate(glm::radians(dir*this->rot_speed), glm::vec3(this->rotation[axis])) * this->rotation;
}

void Plane::tick() {
    if (this->fuel_counter == 3*60) {
        this->fuel_counter = 0;
        this->fuel -= 1;
    }

    if (controls.W)
        this->position -= glm::vec3(rotation[2])*this->trans_speed;
    if (controls.A)
        rotate_about(2, 1.0); 
    if (controls.D)
        rotate_about(2, -1.0);
    if (controls.Q)
        rotate_about(1, 1.0);
    if (controls.E)
        rotate_about(1, -1.0);
    if (controls.Z)
        rotate_about(0, 1.0);
    if (controls.C)
        rotate_about(0, -1.0);

    this->fuel_counter++;
}

bool Plane::check_collision(bounding_box_t other) {
    return detect_collision(bounding_box_t(this->position, 2*this->r, 2*this->r, this->length + this->length/6), other) or
           detect_collision(bounding_box_t(this->position, 5.0, 1.0, 1.25), other);
}
