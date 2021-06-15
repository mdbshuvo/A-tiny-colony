#ifndef PRIMITIVE_CURVE_H
#define PRIMITIVE_CURVE_H

#include <GL/gl.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "primitives.h"

using namespace std;

void bottleBezier();
void mouse_click(int button, int state, int x, int y);
void draw_cur_selected_points();
void save_cur_coords();
void load_coords();
void draw_player();
void draw_basin();
void draw_basin_pipe();
void draw_basin_tap();
void draw_street_top(float rad=0.2);
void draw_street_light();
void draw_player_hand();
void draw_player_hair(float frac=0.5);

#endif // PRIMITIVE_CURVE_H
