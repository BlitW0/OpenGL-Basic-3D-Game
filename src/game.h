#include "main.h"
#include "ball.h"
#include "circle.h"
#include "plane.h"
#include "rectangle.h"
#include "cuboid.h"
#include "digit.h"
#include "volcano.h"
#include "arrow.h"
#include "compass.h"

#ifndef GAME_H
#define GAME_H


class Game
{
public:
    Game() {}
    void init();
    void draw();
    void tick();
    bool drop_bomb, shoot_missile;
private:
    void init_seven_segs();
    void draw_number(glm::vec3 position, int value);
    float randrange(float M, float N);
    Cuboid create_cuboid(glm::vec3 position, color_t color, glm::vec3 to, float l, float b, float h);
    void set_point_to();
};

#endif // GAME_H