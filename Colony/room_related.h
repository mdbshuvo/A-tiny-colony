#ifndef ROOM_REL_H
#define ROOM_REL_H

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
#include "furnitures.h"
#include "collision_detection.h"

// room building

void flr(float posY,
          float room_width, float roomHeight, float room_length,
           float wall_thickness, bool isTiled=false, texture_id tex=TILES);

void wall(float posX, float posY, float posZ, float w_rot,
           float room_width, float roomHeight, float room_length, float wall_thickness,
            bool hasWin=false, bool hasWinLid=false, float win_h=10, float win_w=8,
             bool hasDoor=false, float d_width = 10, float d_height = 15,
             texture_id tex=TILE1,bool chosen=false);

void room(float posX=0, float posY=0, float posZ=0,
           float room_width = 40, float room_length = 40,
            bool roof=true, bool flor=true, float r_rot=0, bool has_window=true, bool has_win_lid=true, float window_height=10,
            bool door_front=true, bool door_right=false, bool door_back=false, bool door_left=false,
            bool face_front=true, bool face_right=true, bool face_back=true, bool face_left=true,
            float roomHeight=-1, float wall_thick=1, texture_id floor_tex=TILE2, texture_id wall_tex=TILE1);

void furniture_settings_1(float posX=0, float posY=0, float posZ=0,
                          float room_width = 40, float room_length = 40, float r_rot=0);

void stair_room(float posX=0, float posY=0, float posZ=0, float width=10,
                float fl_width=40, float fl_thick=1, float fl_length=40, bool has_floor=true, float st_rot=0,
                texture_id floor_tex=TILE2, texture_id wall_tex=TILE1);

void furniture_settings_2(float posX=0, float posY=0, float posZ=0, float room_width = 40, float room_length = 40, float r_rot=0, bool mirrored=false);

void furniture_settings_3(float posX=0, float posY=0, float posZ=0, float width = 40, float length = 40, float r_rot=0);

void furniture_settings_4(float posX=0, float posY=0, float posZ=0, float width = 40, float length = 40, float r_rot=0);

#endif // ROOM_REL_H
