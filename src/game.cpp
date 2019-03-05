#include "main.h"
#include "game.h"

camera_view_t camera_view = CAMERA_FOLLOW;
int camera_views_num = 5;
double camera_zoom = 1, prev_x, prev_y, h_x, h_y;

bool seven_segs[10][7];
float d_l = 0.07, d_b = 0.008;

float plane_length = 4.0, plane_radius = 0.5;
Plane plane;
std::vector <Cuboid> plane_bomb;
std::vector <Cuboid> plane_missile;

Cuboid ocean;

int num_enemies = 5;
std::vector <std::pair <Cuboid, int> > enemy;
std::vector <Cuboid> enemy_missile;

Arrow arrow;
glm::vec3 point_to;
bool enemies_pres;

int num_volcanoes = 6;
std::vector <Volcano> volcano;

Compass compass;



void Game::init() {

    init_seven_segs();
    
    plane       = Plane(glm::vec3(0, -40, 0), COLOR_BLACK, plane_radius, plane_length);
    ocean       = Cuboid(glm::vec3(0, -50, -10), COLOR_OCEAN, 1000, 1000, 2);
    arrow       = Arrow(plane.position + glm::vec3(0, 5, -5), COLOR_RED, 1, 1);
    compass     = Compass(glm::vec3(1 - 0.3, -1 + 0.3, 0), 0.3, 0.08);
    
    _rep (i, num_volcanoes) {
            float x = randrange(-200, 200), z = randrange(plane.position.z - 200, plane.position.z - 40);
            volcano.push_back(Volcano(glm::vec3(x, -50, z)));
        }

}


void Game::draw() {

    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    double cur_x, cur_y;
    int width, height;

    switch (camera_view)
    {
        case CAMERA_FOLLOW:
            eye    = glm::vec3(plane.rotation[0])*float(0.0) + glm::vec3(plane.rotation[1])*float(2.0) + glm::vec3(plane.rotation[2])*float(5.0) + plane.position;
            target = plane.position;
            up     = glm::vec3(plane.rotation[1]);
            break;
    
        case CAMERA_TOP:
            eye    = glm::vec3(0, 11, 0) + plane.position;
            target = plane.position;
            up     = glm::vec3(0, 0, -1);
            break;

        case CAMERA_TOWER:
            eye    = glm::vec3(-8, -30, plane.position.z + 10);
            target = plane.position;
            up     = glm::vec3(0, 1, 0);
            break;
        
        case CAMERA_PLANE:
            eye    = plane.position - glm::vec3(plane.rotation[2])*(plane_length/2 + plane_length/6);
            target = eye - glm::vec3(plane.rotation[2])*10.0f;
            up     = glm::vec3(plane.rotation[1]);
            break;

        case CAMERA_HELICOPTER:
            glfwGetWindowSize(window, &width, &height);
            glfwGetCursorPos(window, &cur_x, &cur_y);
            h_x += (cur_x - prev_x)*90/width;
            h_y -= (cur_y - prev_y)*90/height;
            prev_x = cur_x;
            prev_y = cur_y;

            eye    = glm::vec3(
                        plane.position.x + (12 - 8*camera_zoom)*cos(h_x*M_PI/180)*sin(h_y*M_PI/180),
                        plane.position.y + (12 - 8*camera_zoom)*cos(h_y*M_PI/180),
                        plane.position.z - (20 - 8*camera_zoom)*sin(h_x*M_PI/180)*sin(h_y*M_PI/180)
                    );
            target = plane.position;
            up     = glm::vec3(0, 1, 0);
            break;

        default:
            break;
    }

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt(eye, target, up);
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Render Scene
    plane.draw(VP);
    ocean.draw(VP);
    
    if (not enemy.empty()) {
        set_point_to();
        arrow.draw(VP, point_to);
    }

    _for (i, enemy)
        enemy[i].first.draw(VP);
    
    _for (i, volcano)
        volcano[i].draw(VP);

    _for (i, enemy_missile)
        enemy_missile[i].draw(VP);
    
    _for (i, plane_bomb)
        plane_bomb[i].draw(VP);
    
    _for (i, plane_missile)
        plane_missile[i].draw(VP);
        

    // Dashboard
    draw_number(glm::vec3(-1 + d_l, -1 + 2*d_l, 0), plane.fuel);
    draw_number(glm::vec3(-1 + d_l, -1 + 5*d_l, 0), plane.position.y - ocean.position.y - 1);
    draw_number(glm::vec3(-1 + d_l, -1 + 8*d_l, 0), plane.score);
    draw_number(glm::vec3(-1 + d_l, -1 + 11*d_l, 0), plane.health);
    compass.draw(glm::mat4(1.0f));

}


void Game::tick() {

    // Drop Bomb
    if (drop_bomb) {
        plane_bomb.push_back(create_cuboid(plane.position - glm::vec3(0, plane_radius, 0) - float(plane_length/2)*glm::vec3(plane.rotation[2]), COLOR_BOMB, glm::vec3(plane.position.x, ocean.position.y, plane.position.z) - float(plane_length/2)*glm::vec3(plane.rotation[2]), 1, 1, 1));
        drop_bomb = 0;
    }

    // Shoot Missile
    if (shoot_missile) {
        plane_missile.push_back(Cuboid(plane.position - glm::vec3(0, plane_radius, 0), COLOR_BRIGHT_ORANGE, 1, 1, 1));
        plane_missile.back().set_axis(-1.0f*glm::vec3(plane.rotation[2]));
        shoot_missile = 0;
    }

    // Enemy grounds
    if (enemy.empty()) {
        // No enemies, so generate
        _rep (i, num_enemies) {
            float x = randrange(plane.position.x - 100, plane.position.x + 100), z = randrange(plane.position.z - 100, plane.position.z - 20);
            enemy.push_back(std::make_pair(
                Cuboid(glm::vec3(x, -50 + 1, z), COLOR_GREEN, 10, 10, 2),
                int(randrange(300, 400))
            ));
        }
    }
    else {
        // Check coll b/w enemies and dropped bombs
        _for (i, plane_bomb) {
            _for (j, enemy)
                if (detect_collision(plane_bomb[i].box(), enemy[j].first.box())) {
                    enemy.erase(enemy.begin() + j);
                    plane_bomb.erase(plane_bomb.begin() + i);
                    i--, j--;
                    plane.score += 10;
                }
            // Bomb goes into ocean or goes too high
            if (detect_collision(ocean.box(), plane_bomb[i].box()) or plane_bomb[i].position.y > 150) {
                plane_bomb.erase(plane_bomb.begin() + i);
                i--;
            }
        }
        
        // Check coll b/w enemies and shot missiles
        _for (i, plane_missile) {
            _for (j, enemy)
                if (detect_collision(plane_missile[i].box(), enemy[j].first.box())) {
                    enemy.erase(enemy.begin() + j);
                    plane_missile.erase(plane_missile.begin() + i);
                    i--, j--;
                    plane.score += 10;
                }
            // Missile goes into ocean or goes too high
            if (detect_collision(ocean.box(), plane_missile[i].box()) or plane_missile[i].position.y > 150) {
                plane_missile.erase(plane_missile.begin() + i);
                i--;
            }
        }

        // Generate enemy missiles
        _for (i, enemy) {
            if (enemy[i].second == 0)
                enemy_missile.push_back(create_cuboid(enemy[i].first.position, COLOR_BRIGHT_ORANGE, plane.position, 1, 1, 1));
            enemy[i].second = (enemy[i].second + 1) % (400);
        }
    }

    // Check coll b/w enemy missiles and plane or if enemy missile goes too high
    _for (i, enemy_missile)
        if (plane.check_collision(enemy_missile[i].box()) or enemy_missile[i].position.y > 150) {
            enemy_missile.erase(enemy_missile.begin() + i);
            i--;
            plane.health -= 3;
        }

    // Check coll b/w plane and volcanoes
    _for (i, volcano)   
        if (plane.check_collision(volcano[i].box)) {
            std::cout << "\n\n------------------- GAME OVER: No Fly Zone --------------------\n\n";
            quit(window);
        }

    // Check if plane drowns in ocean
    if (plane.position.y <= ocean.position.y + 1) {
        std::cout << "\n\n--------------------- GAME OVER: Crash -------------------\n\n";
        quit(window);
    }

    // Check if plane health is finished
    if (plane.health <= 0) {
        std::cout << "\n\n------------------ GAME OVER: Health finished -----------------\n\n";
        quit(window);
    }

    // Check if plane fuel is finished
    if (plane.fuel <= 0) {
        std::cout << "\n\n------------------ GAME OVER: Fuel finished -----------------\n\n";
        quit(window);
    }

    // Object Movements
    plane.tick();
    arrow.set_position(plane.position + glm::vec3(plane.rotation[1])*5.0f + glm::vec3(plane.rotation[2])*(-5.0f));
    compass.tick();

    _for (i, enemy_missile)
        enemy_missile[i].tick();

    _for (i, plane_bomb)
        plane_bomb[i].tick();

    _for (i, plane_missile)
        plane_missile[i].tick();
    
}



void Game::init_seven_segs() {

    bool temp[][7] = {
        {0, 1, 1, 1, 1, 1, 1}, // 0
        {0, 1, 0, 0, 0, 0, 1}, // 1
        {1, 1, 1, 0, 1, 1, 0}, // 2
        {1, 1, 1, 0, 0, 1, 1}, // 3
        {1, 1, 0, 1, 0, 0, 1}, // 4
        {1, 0, 1, 1, 0, 1, 1}, // 5
        {1, 0, 1, 1, 1, 1, 1}, // 6
        {0, 1, 1, 0, 0, 0, 1}, // 7
        {1, 1, 1, 1, 1, 1, 1}, // 8
        {1, 1, 1, 1, 0, 1, 1}, // 9
    };
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 7; j++)
            seven_segs[i][j] = temp[i][j];

}


void Game::draw_number(glm::vec3 position, int value) {

    std::vector <int> digits;
    for (int t = value; t; t /= 10)
        digits.push_back(t % 10);
    if (!value) digits.push_back(0);

    float gap = d_l + 4*d_b;
    glm::vec3 centre = position;

    Digit digit = Digit(centre, COLOR_BLACK, seven_segs[digits.back()], d_l, d_b);
    digit.draw();
    digits.pop_back();
    centre += glm::vec3(gap, 0, 0);

    for (; !digits.empty(); digits.pop_back()) {
        digit = Digit(centre, COLOR_BLACK, seven_segs[digits.back()], d_l, d_b);
        digit.draw();
        centre += glm::vec3(gap, 0, 0);
    }

}


Cuboid Game::create_cuboid(glm::vec3 position, color_t color, glm::vec3 to, float l, float b, float h) {
    
    Cuboid tmp = Cuboid(position, color, l, b, h);
    tmp.set_axis(to - position);
    return tmp;

}


void Game::set_point_to() {

    if (not enemy.empty()) {
        float max_z = -1000.0, ind = 0;
        _for (i, enemy)
            if (max_z < enemy[i].first.position.z) {
                max_z = enemy[i].first.position.z;
                ind = i;
            }
        point_to = enemy[ind].first.position;
    }

}


float Game::randrange(float M, float N) {

    return M + (rand()/(RAND_MAX/(N - M))) ;  

}
