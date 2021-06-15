#ifndef FURNITURES_H
#define FURNITURES_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include "enums.h"
#include "primitives.h"
#include "primitive_curve.h"

using namespace std;

#define PI 3.14159265

void lamp(float rad, float height, float posX, float posY, float posZ, GLenum light_num=-1, bool on=false);

void pillow(float posX, float posY, float posZ, float p_w=4);

void bedSheet(float width, float length, float posY);

void blanket(float width, float length, float posY, float posZ);

void bed(float width, float height, float length, float posX, float posZ, float b_rot=0);

void chair(int baseX, int baseZ, float legSize, float topSize, float px, float pz, float cRot);

void table(float width, float height, float length, float posX, float posZ, float t_rot=0);

void fan(float blade_size, float posX, float posY, float posZ,
         float centroid_height=0.5, float centroid_width=2, float rod_height=5,
         float blade_height = 0.2, float blade_width = 1, float rod_rad=0.15);

void wardrobe(float width, float height, float length, float posX, float posZ, float w_rot=0);

void tube(int wall, float posX, float posY, float posZ, GLenum light_num=-1, bool on=false);

void sofa_1(float width=25, float height=15, float length=7, float posX=0, float posY=0, float posZ=0, float s_rot=0);

void table_2(float width=15, float height=8, float length=8, float base_height=1, float posX=0, float posY=0, float posZ=0, float b_rot=0, float leg_width=0.5, texture_id body_tex=WOOD3, texture_id leg_tex=WOOD3);

void bench_set(float width=15, float height=8, float length=8, float posX=0, float posY=0, float posZ=0, float b_rot=0, float leg_width=0.5);

void blackboard(float width=20, float height=10, float length=0.5, float posX=0, float posY=0, float posZ=0, float b_rot=0);

void chair_2(float width=10, float height=10, float length=5, float posX=0, float posY=0, float posZ=0, float b_rot=0);

void goal_post(float width=15, float height=10, float length=0.5, float posX=0, float posY=0, float posZ=0, float b_rot=0);

void almirah(float width=10, float height=20, float length=5, float posX=0, float posY=0, float posZ=0, float b_rot=0, texture_id front_tex=BRICK, texture_id tex=BRICK);

void basin(float width=3, float height=5, float length=2, float posX=0, float posY=0, float posZ=0, float b_rot=0);

void street_lamp(float rad=0.5, float top_rad=0.2, float height=15, float bottom_height=10, GLenum light_num=GL_LIGHT0, bool swtch=false, float posX=0, float posY=0, float posZ=0, float b_rot=0);

void top_wall_light(GLenum light_num, bool on=false, float rad=1, float height=0.5, float posX=0, float posY=0, float posZ=0);

#endif // FURNITURES_H
