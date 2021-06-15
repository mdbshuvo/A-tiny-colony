#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include <bits/stdc++.h>
#include "BmpLoader.h"
#include <string>
#include <iostream>
#include "enums.h"

using namespace std;

#define PI 3.14159265

void set_mat_prop(float colR=0.5, float colG=0.5, float colB=0.5, bool em=false, float shine=128, float alpha=1);

void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1,
                        GLfloat x2, GLfloat y2, GLfloat z2,
                        GLfloat x3, GLfloat y3, GLfloat z3);

void cube(float colR=0.5, float colG=0.5, float colB=0.5,
          bool em=false, float shine=128,
          float dx=1, float dy=1, float dz=1, float tex_size=1, float alpha=1);

void cylinder(float rad_t, float rad_b, float height, int granularity,
              float colR=0.5, float colG=0.5, float colB=0.5,
              bool em=false, float shine=128, bool tb_surf=true, bool tb_texture=false,
              float sp_stack_th=1, int sp_num_stack=1, float alpha=1);

void sphere(float sp_rad=5,int granularity=3, int stack_num=3, float colR=0.5, float colG=0.5, float colB=0.5,
            float em=false, float shine=128, float frac=1, float alpha=1);

void glut_sphere(float rad, int slices, int stacks,
            float colR=0.5, float colG=0.5, float colB=0.5, bool em=false, float shine=128);

void light(GLenum light_num, float posX, float posY, float posZ,
           bool spot=false, float spot_cut=15, float amb=0.2, float homogeneous=1, float intensity=1);

void light_off(GLenum light_num);

GLubyte* make_bw_tiles_texture(int tile_width, int tile_height, int tex_width=2048, int tex_height=2048);

void LoadTexture(const char*filename, GLuint index);

void LoadTexture_from_arr(GLubyte* arr,int width, int height, GLuint index);

void mirrored_surface(float width, float height, float length, float alpha=0.5, float posX=0, float posY=0, float posZ=0, bool has_tex=false, texture_id tex=POND2);

#endif // PRIMITIVES_H
