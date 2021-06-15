#ifndef COLLISION_DET_H
#define COLLISION_DET_H

#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.14159265

// collision detection

void init_floors(int fl_w=100, int fl_l=100);

void add_floor_height(float posX, float posY, float posZ,
                       float dx, float dy, float dz,
                        float width, float height, float length, bool obj_rotated90=false, float parent_rot=0);

float check_current_floor(float posZ, float posX);

#endif // COLLISION_DET_H
