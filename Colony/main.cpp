#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include<time.h>
#include <bits/stdc++.h>
#include "BmpLoader.h"
#include <string>
#include <iostream>
#include "primitives.h"
#include "furnitures.h"
#include "enums.h"
#include "collision_detection.h"
#include "room_related.h"
#include "forest.h"
#include "primitive_curve.h"

using namespace std;

#define PI 3.14159265

const int width = 800;
const int height = 600;
const float rat = 1.0*width/height;

GLfloat eyeX = 0;
GLfloat eyeY = 30;
GLfloat eyeZ = 60;

GLfloat lookX = 0;
GLfloat lookY = eyeY;
GLfloat lookZ = eyeZ-15;

float sun_dist=700, sun_x=0, sun_y=sun_dist, sun_z=0, sun_theta=90;
float sun_color[]={1.0,1.0,1.0}, sun_cur_color[]={1.0,1.0,1.0};
bool sun_move=true, sun_boost=false;
float sun_light_intensity=1.0, cur_sun_light_intensity=sun_light_intensity;

float rot = 0;
float fan_rot = 0;
int wndw_rot=0;
float inter_wndw_rot=0;
float sky_rot=0;

bool wired=false, editor_mode=false, reflection_needed=true;
bool light1=true, light2=false, light3=false;
bool target_on=true,fan_on=true;
bool drone_effect=false, state[256];

const int num_tex=100;
unsigned int texture_ids[num_tex];
float texture_size[num_tex];

// collision detection
int floor_width=1000, floor_length=1000, num_level=5, floor_height=30;
int half_floor_height = floor_height/2, num_half_level=num_level*2;

extern float* half_floors[];
extern float* current_floor;
bool floor_calculation=true;

// Rotation and exploration
float x_, z_, r, theta, dx, dz, dx_norm, dz_norm, r_=1, turn_angle_step=5, height_diff_one_less, height_diff_thresh_dist;
int speed_ind=4, speed_range=7;
float speed_arr[]={0.025,0.05,0.1,0.2,0.5,1,2,4,8};

const float playerHeight=7, playerWidth=1, pre_climb_thresh=2, dist_thresh=speed_arr[speed_ind]*5;
bool third_person_view=false, bumped=false, bump_down=false, hands_up=false;
float t_p_v_cam_dist=15, t_p_v_cam_height=2.5, cur_player_height=playerHeight, cur_player_width=playerWidth, bump_thresh=15;
float climb_thresh=pre_climb_thresh;

// curve
bool show_cur_sel_pt=false;
extern int num_cur_points;

// Extras
void field(float f_width=100, float f_height=1, float f_length=100, float posX=0, float posY=0, float posZ=0, texture_id tex=FIELD1)
{
    if(floor_calculation) add_floor_height(0,0,0, 0,-1,0, f_width,f_height,f_length);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[tex]);

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glScalef(f_width,f_height,f_length);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.5,0.5,0.5,false,128,f_width,f_height,f_length,texture_size[tex]);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void axes(float length = 15)
{
    float width = 0.3;

    // X-axis
    glPushMatrix();
    glTranslatef(length/2,0,0);
    glScalef(length,width,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.8,0.1,0.1);
    glPopMatrix();

    // Y-axis
    glPushMatrix();
    glTranslatef(0,length/2,0);
    glScalef(width,length,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.8,0.1);
    glPopMatrix();

    // Z-axis
    glPushMatrix();
    glTranslatef(0,0,length/2);
    glScalef(width,width,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.1,0.8);
    glPopMatrix();
}

void target_()
{
    float width=2, height=0.1;

    // Target
    glPushMatrix();
    glTranslatef(lookX,lookY,lookZ);
    glRotatef(-theta+90, 0,1,0);

    glPushMatrix();
    glTranslatef(-(width-height)/2,(width-height)/2,0);
    glScalef(width,height,height);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(height,width,height);
    cube(1,1,1);
    glPopMatrix();

    glPopMatrix();
}

void the_player()
{
    float sc_w=0.6, sc_h=0.1;
    float hair_frac=0.6, hair_rot=-(135 + (hair_frac-0.5)/2*360);

    glPushMatrix();
    glTranslatef(eyeX,eyeY-cur_player_height,eyeZ);
    glRotatef(-theta, 0,1,0);
//    glRotatef(-90, 0,1,0);

    // body
    glPushMatrix();
    glScalef(sc_w/3,sc_h,sc_w);
    glScalef(cur_player_width,cur_player_height,cur_player_width);
    glRotatef(90, 0,0,1);
    set_mat_prop(225.0/255,172.0/255,150.0/255);
    draw_player();

    set_mat_prop(8.0/255,8.0/255,6.0/255);
    glRotatef(hair_rot, 1,0,0);
    draw_player_hair(hair_frac);
    glPopMatrix();

    // hand
    glPushMatrix();
    glScalef(1,sc_h*1.5,0.8);
    glScalef(cur_player_width,cur_player_height,cur_player_width);
    glRotatef(90, 0,1,0);

    if(hands_up)
    {
        glTranslatef(0,4.5,0);
        glRotatef(180, 1,0,0);
        glTranslatef(0,-4.5,0);
    }

    set_mat_prop(225.0/255,172.0/255,150.0/255);
    draw_player_hand();
    glPopMatrix();

    // shirt
    float shirt_width=2.5*(cur_player_width/playerWidth), mid_h=cur_player_height/6, top_h=mid_h/2, top_sh_width=shirt_width/1.5, bot_sh_wid=shirt_width/1.2;

    glPushMatrix();
    glTranslatef(0,3.2*(cur_player_height/playerHeight),0);
    glScalef(0.4,1,1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[SHIRT_CLOTH_1]);
//    glBindTexture(GL_TEXTURE_2D, texture_ids[MEME_RECT]);

    glPushMatrix();
    glTranslatef(0,top_h+mid_h,0);
    cylinder(top_sh_width/2,shirt_width/2,top_h,10, 1,1,1, false,128,false,false, 4,4);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,top_h,0);
    cylinder(shirt_width/2,shirt_width/2,mid_h,10, 1,1,1, false,128,false,false, 1.5,2);
    glPopMatrix();

    glPushMatrix();
    cylinder(shirt_width/2,bot_sh_wid/2,top_h,10, 1,1,1, false,128,false,false, 1,4);
    glPopMatrix();

    glPopMatrix();

    // lungi
    float lun_width=shirt_width+0.1, top_ln_wid=bot_sh_wid, bot_ln_h=cur_player_height/3, top_ln_h=bot_ln_h/3;

    glPushMatrix();
    glScalef(0.4,1,1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[SHIRT_CLOTH_2]);

    glPushMatrix();
    glTranslatef(0,bot_ln_h,0);
    cylinder(top_ln_wid/2,lun_width/2,top_ln_h,10, 1,1,1, false,128,false);
    glPopMatrix();

    glPushMatrix();
    cylinder(lun_width/2,lun_width/2,bot_ln_h,10, 1,1,1, false,128,false);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();

    if(drone_effect)
    {
        glPushMatrix();
        glTranslatef(0,cur_player_height,0);
        glRotatef(180, 1,0,0);
        fan(1,0,0,0, 0.1,0.25,1, 0.1,0.2,0.05);
        glPopMatrix();
    }

    glPopMatrix();
}

// Environment

void the_sun(float posX, float posY, float posZ)
{
    float rad=20;

    glPushMatrix();
    glTranslatef(posX+eyeX,posY-rad,posZ+eyeZ);

    glPushMatrix();
    sphere(rad,10,10, sun_cur_color[0],sun_cur_color[1],sun_cur_color[2], true, 0);
    glPopMatrix();

//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, texture_ids[SUN_GLARE_3]);

//    glPushMatrix();
//    glTranslatef(0,-250,0);
//    glScalef(150,1,150);
//    glTranslatef(-0.5,0,-0.5);
//    cube(1,1,1, false,128, 1,1,1,1, 0.5);
//    glPopMatrix();
//
//    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void sky(float rad=500, float rot=90)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[SKY_4_V2]);

    glPushMatrix();
    glTranslatef(eyeX,-rad/5,eyeZ);
    glRotatef(rot, 0,1,0);
    sphere(rad, 50,25, 1,1,1, false,128, 0.6);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

//    cout<<sun_x<<" "<<sun_y<<endl;

//    glBindTexture(GL_TEXTURE_2D, texture_ids[MOON_1]);
    the_sun(sun_x,sun_y,sun_z);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[SKY_6]);
    glPushMatrix();
    glTranslatef(eyeX,-(rad-50)/5,eyeZ);
    glRotatef(rot+sky_rot, 0,1,0);
    sphere(rad-50, 50,25, 1,1,1, false,128, 0.6, 0.4);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void roads( float rd_length=500, float rd_width=20)
{
    float rd_height=0.1;
    float block_size=5, width_block=rd_width/block_size, length_block=rd_length/block_size;

    street_lamp(0.5,0.2,20,15, GL_LIGHT6,light1, rd_length/4,0,rd_width/2,90);
    street_lamp(0.5,0.2,20,15, GL_LIGHT7,light1, -rd_length/4,0,rd_width/2,90);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[ROAD2]);

    glPushMatrix();
    for(int i=-width_block/2; i<=width_block/2; i++)
    {
        for(int j=-length_block/2; j<length_block/2; j++)
        {
            glPushMatrix();
            glTranslatef(i*block_size,0,j*block_size);
            glScalef(block_size, rd_height, block_size);
            glTranslatef(-0.5,0,-0.5);
            cube(0.5,0.5,0.5);
            glPopMatrix();
        }
    }
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0,1,0);

    for(int i=-width_block/2; i<=width_block/2; i++)
    {
        for(int j=-length_block/2; j<length_block/2; j++)
        {
            glPushMatrix();
            glTranslatef(i*block_size,0,j*block_size);
            glScalef(block_size, rd_height, block_size);
            glTranslatef(-0.5,0,-0.5);
            cube(0.5,0.5,0.5);
            glPopMatrix();
        }
    }
    glPopMatrix();

//    glPushMatrix();
//    glRotatef(90, 0,1,0);
//    glScalef(rd_width, rd_height*1.5, rd_length);
//    glTranslatef(-0.5,0,-0.5);
//    cube(0.5,0.5,0.5,false,128, rd_width,rd_height*1.5,rd_length, texture_size[ROAD2]);
//    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void forest(float posX=0, float posY=0, float posZ=0, bool mirrored=false)
{
    int row=6, column=7, tree_type=4;
    float tree_width=5, tree_height=25, bush_width=20, bush_height=10;
    float dist_trees=bush_width*2.5, forest_width=column*dist_trees, forest_length=row*dist_trees;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[GRASS1]);

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glScalef(forest_width,0.2,forest_length);
    glTranslatef(-1,0,0);
    cube(0.5,0.5,0.5, false,128, forest_width,0.2,forest_length, 10);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

//    int for_trees[10][10];
//    int seed=;
    srand(43);

    for(int i=0; i<row; i++)
    {
        for(int j=0; j<column; j++)
        {
            int seed=rand()%tree_type;
            if(seed==0) tree_1(tree_width,tree_height,bush_width,bush_height, posX-j*dist_trees-dist_trees/2,0,posZ+i*dist_trees+dist_trees/2);
            else if(seed==1) tree_2(tree_width,tree_height,bush_width,bush_height, posX-j*dist_trees-dist_trees/2,0,posZ+i*dist_trees+dist_trees/2);
            else if(seed==2) tree_3(tree_width,2*tree_height+(rand()%10),2*bush_width,3*bush_height, posX-j*dist_trees-dist_trees/2,0,posZ+i*dist_trees+dist_trees/2);

            // if mirrored don't draw all trees
            if(mirrored & (j>1)) break;
        }
    }
}

void pond(float posX=0, float posY=0, float posZ=0, float pond_width=50, float pond_length=50, float border_width=5, float border_height=1, float pond_height=0.2)
{
    float water_width=pond_width-2*border_width, water_length=pond_length-2*border_width;

    glPushMatrix();

    mirrored_surface(water_width,pond_height,water_length, 0.8, posX,posY,posZ, true,POND2);
    room(posX,posY-border_width,posZ, pond_width,pond_length, false,false, 0,false,false,0, false,false,false,false, true,true,true,true, border_height,border_width, WALL1,GRASS2);

    glPopMatrix();
}

// Infrastructure
void building_1(float posX=0, float posY=0, float posZ=0, texture_id wall_tex=WALL1, bool mirrored=false)
{
    int f=2;
    bool flr_st=true, roof_r=false;
    for(int i=0; i<f; i++)
    {
        room(posX-40,posY+i*floor_height,posZ,40,40,roof_r,true,0,true,true,10,false,true,true,false, true,true,true,true, -1,1, TILE2,wall_tex);
        if(!mirrored) furniture_settings_1(posX-40,posY+i*floor_height+1,posZ,40,40,0);
        if(!mirrored) stair_room(posX,posY+i*floor_height,posZ,20,40,1,40,flr_st,0, TILE2,wall_tex);
        room(posX+40,posY+i*floor_height,posZ,40,40,roof_r,true,0,true,true,10,false,false,true,true, true,true,true,true, -1,1, TILE2,wall_tex);
        if(flr_st) top_wall_light(GL_LIGHT4,light1, 1,0.5, posX,posY+(i+1)*floor_height,posZ+15);
        if(!mirrored) furniture_settings_1(posX+40,posY+i*floor_height+1,posZ,40,40,90);

        if(!mirrored) room(posX-30,posY+i*floor_height,posZ-40,60,40,roof_r,true,0,true,true,10,false,false,false,false, false,false,true,true, -1,1, TILE2,wall_tex);
        if(!mirrored) furniture_settings_2(posX-30,posY+i*floor_height+1,posZ-40,60,40, mirrored);
        if(!mirrored) furniture_settings_2(posX+30,posY+i*floor_height+1,posZ-40,60,40, mirrored);
        room(posX+30,posY+i*floor_height,posZ-40,60,40,roof_r,true,0,true,true,10,false,false,false,false, false,true,true,false, -1,1, TILE2,wall_tex);

        wall(posX,posY+i*floor_height,posZ-40,90,60,floor_height,40,1, false,false,0,0, false,0,0, wall_tex);
        if(floor_calculation) add_floor_height(posX,posY,posZ, 0,i*floor_height,-40, 1,floor_height,40);
        if(!flr_st)
        {
            wall(posX,posY+i*floor_height,posZ+20,0,40,floor_height,40,1,true,false,20,7,false,0,0, wall_tex);
            if(floor_calculation) add_floor_height(posX,posY,posZ, 0,i*floor_height,20, 40,floor_height,1);
        }

        flr_st=false;
    }
    room(posX,posY+f*floor_height,posZ,40,40,true,false,0,false,false,10,false,true,false,true, true,true,true,true, -1,1, TILE2,wall_tex);

    float roof_r_height=5;
    room(posX+40,posY+f*floor_height,posZ, 40,40,false,true,0,false,false,0,false,false,false,false, true,true,false,false,roof_r_height,1, TILE2,wall_tex);
    if(!mirrored) room(posX-40,posY+f*floor_height,posZ, 40,40,false,true,0,false,false,0,false,false,false,false, true,false,false,true,roof_r_height,1, TILE2,wall_tex);
    room(posX,posY+f*floor_height,posZ-40, 120,40,false,true,0,false,false,0,false,false,false,false, false,true,true,true,roof_r_height,1, TILE2,wall_tex);

    // fence
    float fence_height=10;
    room(posX,posY-1,posZ-5, 150,120, false,false,0,false,false,0, false,false,false,false, false,true,true,true, fence_height,1, TILE2,FENCE1);
    if(!mirrored) room(posX+45,posY-1,posZ-5, 60,120, false,false,0,false,false,0, false,false,false,false, true,false,false,false, fence_height,1, TILE2,FENCE1);
    room(posX-45,posY-1,posZ-5, 60,120, false,false,0,false,false,0, false,false,false,false, true,false,false,false, fence_height,1, TILE2,FENCE1);
}

void school(float posX=0, float posY=0, float posZ=0)
{
    float mid_wall=50, side_wall=15, cls_width=mid_wall+2*side_wall, cor_len=15, cls_len=50, cor_height=5;
    float str_room_width=40, str_width=20, str_room_len=40;
    int storied=2, num_cls_rm=2;

    // fence
    float  field_len=125, extra_gap=20, fence_height=10, fence_door_gap=60;
    float fence_width=2*num_cls_rm*cls_width+str_room_width+extra_gap, fence_length=cls_len+cor_len+field_len+2*extra_gap;
    float field_width=250, front_half_fence=(fence_width-fence_door_gap)/2;

    room(posX,posY-1,posZ, fence_width,fence_length, false,false,0, false,false,0,false,false,false,false, false,true,true,true, fence_height,1, NONE,BRICK);
    room(posX-(front_half_fence+fence_door_gap)/2,posY-1,posZ, front_half_fence,fence_length, false,false,0, false,false,0,false,false,false,false, true,false,false,false, fence_height,1, NONE,BRICK);
    room(posX+(front_half_fence+fence_door_gap)/2,posY-1,posZ, front_half_fence,fence_length, false,false,0, false,false,0,false,false,false,false, true,false,false,false, fence_height,1, NONE,BRICK);

    // Club room
    float te_z_tr=25;

    room(posX+(fence_width-40-extra_gap)/2,posY,posZ+te_z_tr, 40,80, true,true,0,true,true,10, false,false,false,true, true,true,true,true, -1,1, FLOOR1, TILE2);
//    furniture_settings_4(posX+(fence_width-40-extra_gap)/2,posY,posZ+te_z_tr, 40,80, 0);

    // field
    float field_z_tr=45;

    field(field_width/2,1,field_len, posX-extra_gap-field_width/4,posY+0.1,posZ+field_z_tr, FOOTBALL_FIELD_1_LH);
    field(field_width/2,1,field_len, posX-extra_gap+field_width/4,posY+0.1,posZ+field_z_tr, FOOTBALL_FIELD_1_RH);
    goal_post(20,10,0.5, posX-extra_gap-(field_width/2-15.86),posY,posZ+field_z_tr, 90);
    goal_post(20,10,0.5, posX-extra_gap+(field_width/2-15.5),posY,posZ+field_z_tr, 90);

    // classrooms
    float rPosZ=posZ-((cls_len-str_room_len)/2+cor_len)-((fence_length)/2-(cls_len+cor_len));    //Aligning to the stair room and the fence
    float fPosZ=posZ-((fence_length)/2-(cls_len+cor_len));
    bool str_flr=true, last_cls=false;

    for(int i=0; i<storied; i++)
    {
        stair_room(posX,posY+i*floor_height,fPosZ, str_width,str_room_width,1,str_room_len, str_flr,0, FLOOR1,TILE2);
        if(!str_flr) room(posX,posY+i*floor_height,fPosZ, str_room_width,str_room_len, false,false,0,false,false,0, false,false,false,false, true,false,false,false, 5,1, FLOOR1,FLOOR1);

        if(str_flr) top_wall_light(GL_LIGHT5,light1, 1,0.5, posX,(i+1)*floor_height,fPosZ+str_room_len/2-8);

        for(int j=0; j<num_cls_rm; j++)
        {
            if(j==num_cls_rm-1) last_cls=true;

            float rPosX=(cls_width+str_room_width)/2+j*cls_width;

            room(posX+rPosX,posY+i*floor_height,rPosZ, mid_wall,cls_len, false,true,0,true,true,10, false,false,false,false, true,false,true,false, -1,1, FLOOR1,TILE2);
            room(posX+rPosX+(mid_wall+side_wall)/2,posY+i*floor_height,rPosZ, side_wall,cls_len, false,true,0,false,false,10, true,false,false,false, true,true,true,false, -1,1, FLOOR1,TILE2);
            room(posX+rPosX-(mid_wall+side_wall)/2,posY+i*floor_height,rPosZ, side_wall,cls_len, false,true,0,false,false,10, true,false,false,false, true,false,true,true, -1,1, FLOOR1,TILE2);
            if(str_flr && j==0) furniture_settings_4(posX+rPosX,posY+i*floor_height+1,rPosZ, cls_len,cls_width,90);
            else furniture_settings_3(posX+rPosX,posY+i*floor_height+1,rPosZ, cls_width,cls_len,0);

            room(posX+rPosX,posY+i*floor_height,rPosZ+(cls_len+cor_len)/2, cls_width,cor_len,false,true,0,false,false,10, false,false,false,false, true,last_cls,false,false, cor_height,1, FLOOR1,FLOOR1);

            room(posX-rPosX,posY+i*floor_height,rPosZ, mid_wall,cls_len, false,true,0,true,true,10, false,false,false,false, true,false,true,false, -1,1, FLOOR1,TILE2);
            room(posX-rPosX+(mid_wall+side_wall)/2,posY+i*floor_height,rPosZ, side_wall,cls_len, false,true,0,false,false,10, true,false,false,false, true,true,true,false, -1,1, FLOOR1,TILE2);
            room(posX-rPosX-(mid_wall+side_wall)/2,posY+i*floor_height,rPosZ, side_wall,cls_len, false,true,0,false,false,10, true,false,false,false, true,false,true,true, -1,1, FLOOR1,TILE2);
            furniture_settings_3(posX-rPosX,posY+i*floor_height+1,rPosZ, cls_width,cls_len,0);

            room(posX-rPosX,posY+i*floor_height,rPosZ+(cls_len+cor_len)/2, cls_width,cor_len,false,true,0,false,false,10, false,false,false,false, true,false,false,last_cls, cor_height,1, FLOOR1,FLOOR1);
            last_cls=false;
            str_flr=false;
        }
    }

    room(posX,posY+storied*floor_height,fPosZ, str_room_width,str_room_len, true,false,0,false,false,0, false,true,false,true, true,true,true,true, -1,1, FLOOR1,TILE2);

    // roof
    room(posX+(num_cls_rm*cls_width+str_room_width)/2,posY+storied*floor_height,fPosZ, num_cls_rm*cls_width,str_room_len, false,true,0,false,false,0, false,false,false,false, true,true,false,false, 5,1, FLOOR1,FLOOR1);
    room(posX-(num_cls_rm*cls_width+str_room_width)/2,posY+storied*floor_height,fPosZ, num_cls_rm*cls_width,str_room_len, false,true,0,false,false,0, false,false,false,false, true,false,false,true, 5,1, FLOOR1,FLOOR1);

    float back_r_len=cls_len+cor_len-str_room_len;
    room(posX,posY+storied*floor_height,fPosZ-(str_room_len+back_r_len)/2, 2*num_cls_rm*cls_width+str_room_width,back_r_len, false,true,0,false,false,0, false,false,false,false, false,true,true,true, 5,1, FLOOR1,FLOOR1);

}

// total scene

void the_scene()
{
    // reflection

    if(reflection_needed & !wired)
    {
        glPushMatrix();
        glScalef(1,-1,1);
        glTranslatef(0,0.2,0);
        // sun light
        light(GL_LIGHT3, sun_x,sun_y,sun_z, false,15,0.2,0,cur_sun_light_intensity);

        sky(750);
//        roads(500);
        forest(-15,0,15, true);
        building_1(-95,0,-80,WALL2,true);
        glPopMatrix();
    }

    // mirror effect holder
    pond(120,0,140, 200,250,10,1);

    // actual scene

    // sun light
    light(GL_LIGHT3, sun_x,sun_y,sun_z, false,15,0.2,0,cur_sun_light_intensity);

    building_1(-95,0,-80,WALL2);
    school(220,0,-170);

    roads(500);
    forest(-15,0,15);

    field(1000,1,1000);
    if(!wired) sky(750);
}

// callbacks and helping functions
static void resize(int width, int height)
{
    float rat_new = 1.0*width/height;
    float width_new = height*rat;
    float height_new = width/rat;
    float x_t = 0;
    float y_t = 0;

    if(rat<rat_new)
    {
        x_t = (width-width_new)/2;
        width=width_new;
    }
    else
    {
        y_t = (height-height_new)/2;
        height=height_new;
    }

    glViewport(x_t, y_t, width, height);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -1.5, 1.5, 2.0, 5000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(third_person_view)
    {
        gluLookAt(eyeX-dx/speed_arr[speed_ind]*t_p_v_cam_dist,eyeY+t_p_v_cam_height,eyeZ-dz/speed_arr[speed_ind]*t_p_v_cam_dist,
                   lookX,lookY,lookZ, 0,1,0);
    }
    else
    {
        gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    }

    // wired or fill
    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }

//    glRotatef(rot, 0,1,0);

    if(!editor_mode)
    {
        glPushMatrix();
        glRotatef(rot, 0,1,0);
        the_scene();
        glPopMatrix();

        // target cross
        if(target_on) target_();
        the_player();

        // axes
//        axes(20);
    }
    if(show_cur_sel_pt)
    {
        draw_cur_selected_points();
    }

    // Object Creation Section
    if(editor_mode)
    {

    // Additional light
        light(GL_LIGHT3,0,20,20);
//        light(GL_LIGHT3,0,250,0,false,15,0.2,0);

    //    building_1(0,0,0);


        // Axes
        axes(20);
    }

    // The great barrier (to remove garbage)
    glPushMatrix();
    glScalef(5000,5000,5000);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0);
    glPopMatrix();

    floor_calculation=false;

    glFlush();
    glutSwapBuffers();
}

void movement_animation()
{
    // construction mode
//    const float turn_const=0.05, look_move_const=0.05, move_const=0.1;

    // Explore mode
    const float speed=speed_arr[speed_ind];
    const float turn_const=0.5*speed, look_move_const=0.5*speed, move_const=0.75*speed;

    x_=lookX-eyeX;
    z_=lookZ-eyeZ;
    r=sqrt(x_*x_+z_*z_);

    if(x_==0 && z_>0) theta = 90;
    else if(x_==0 && z_<0) theta = -90;
    else if(z_==0 && x_>0) theta = 0;
    else if(z_==0 && x_<0) theta = 180;
    else if(z_==0 && x_==0) theta = 0;
    else theta=atan(z_/x_) * 180 / PI;

    if((z_>0 && theta<0) || (z_<0 && theta>0)) theta += 180;

    dx = r_*cos(theta * PI / 180) * move_const;
    dz = r_*sin(theta * PI / 180) * move_const;

    dx_norm = r_*cos((theta-90) * PI / 180) * move_const;
    dz_norm = r_*sin((theta-90) * PI / 180) * move_const;

    // Look around
    if(state[128+'w'])
    {
        lookY+=look_move_const;
    }
    if(state[128+'a'])
    {
        theta-=turn_angle_step*turn_const;

        lookX=r*cos(theta * PI / 180)+eyeX;
        lookZ=r*sin(theta * PI / 180)+eyeZ;
    }
    if(state[128+'s'])
    {
        lookY-=look_move_const;
    }
    if(state[128+'d'])
    {
        theta+=turn_angle_step*turn_const;

        lookX=r*cos(theta * PI / 180)+eyeX;
        lookZ=r*sin(theta * PI / 180)+eyeZ;
    }

    // Move around
    if(state[128+'i'])
    {
        height_diff_thresh_dist = check_current_floor(eyeZ+dz*dist_thresh, eyeX+dx*dist_thresh) - (eyeY-playerHeight);
        height_diff_one_less = check_current_floor(eyeZ+dz*(dist_thresh-1), eyeX+dx*(dist_thresh-1)) - (eyeY-playerHeight);

//        cout<<eyeY-playerHeight<<" "<<check_current_floor(eyeZ+dz*dist_thresh, eyeX+dx*dist_thresh)<<" "<<check_current_floor(eyeZ+dz*(dist_thresh-1), eyeX+dx*(dist_thresh-1))<<" "<<eyeX<<" "<<eyeZ<<endl;

        if((height_diff_thresh_dist-height_diff_one_less<=climb_thresh) || (height_diff_thresh_dist<=climb_thresh))
        {
            eyeX += dx;
            eyeZ += dz;

            lookX += dx;
            lookZ += dz;
        }
    }
    if(state[128+'j'])
    {
        height_diff_thresh_dist = check_current_floor(eyeZ+dz_norm*dist_thresh, eyeX+dx_norm*dist_thresh) - (eyeY-playerHeight);
        height_diff_one_less = check_current_floor(eyeZ+dz_norm*(dist_thresh-1), eyeX+dx_norm*(dist_thresh-1)) - (eyeY-playerHeight);

        if((height_diff_thresh_dist-height_diff_one_less<=climb_thresh) || (height_diff_thresh_dist<=climb_thresh))
        {
            eyeX += dx_norm;
            eyeZ += dz_norm;

            lookX += dx_norm;
            lookZ += dz_norm;
        }
    }
    if(state[128+'k'])
    {
        height_diff_thresh_dist = check_current_floor(eyeZ-dz*dist_thresh, eyeX-dx*dist_thresh) - (eyeY-playerHeight);
        height_diff_one_less = check_current_floor(eyeZ-dz*(dist_thresh-1), eyeX-dx*(dist_thresh-1)) - (eyeY-playerHeight);

        if((height_diff_thresh_dist-height_diff_one_less<=climb_thresh) || (height_diff_thresh_dist<=climb_thresh))
        {
            eyeX -= dx;
            eyeZ -= dz;

            lookX -= dx;
            lookZ -= dz;
        }
    }
    if(state[128+'l'])
    {
        height_diff_thresh_dist = check_current_floor(eyeZ-dz_norm*dist_thresh, eyeX-dx_norm*dist_thresh) - (eyeY-playerHeight);
        height_diff_one_less = check_current_floor(eyeZ-dz_norm*(dist_thresh-1), eyeX-dx_norm*(dist_thresh-1)) - (eyeY-playerHeight);

        if((height_diff_thresh_dist-height_diff_one_less<=climb_thresh) || (height_diff_thresh_dist<=climb_thresh))
        {
            eyeX -= dx_norm;
            eyeZ -= dz_norm;

            lookX -= dx_norm;
            lookZ -= dz_norm;
        }
    }

    if(state[128+'g'])
    {
        theta += 180;
        theta -= turn_angle_step*turn_const;

        eyeX=r*cos(theta * PI / 180)+lookX;
        eyeZ=r*sin(theta * PI / 180)+lookZ;
    }
    if(state[128+'h'])
    {
        theta += 180;
        theta += turn_angle_step*turn_const;

        eyeX=r*cos(theta * PI / 180)+lookX;
        eyeZ=r*sin(theta * PI / 180)+lookZ;
    }
}

int prev_x=0, prev_y=0;
static void mouse_move(int x, int y)
{
//    cout<<"Huddai.."<<x<<" "<<y<<endl;

    int move_thresh=5, angle_step=7, move_step=3;

    if(x>prev_x+move_thresh)
    {
        theta+=angle_step;

        lookX=r*cos(theta * PI / 180)+eyeX;
        lookZ=r*sin(theta * PI / 180)+eyeZ;
    }
    else if(x<prev_x-move_thresh)
    {
        theta-=angle_step;

        lookX=r*cos(theta * PI / 180)+eyeX;
        lookZ=r*sin(theta * PI / 180)+eyeZ;
    }

    if(y>prev_y+move_thresh) lookY-=move_step;
    else if(y<prev_y-move_thresh) lookY+=move_step;

    prev_x=x;
    prev_y=y;
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    // moving the look at point at a circular path or up-down
    case 'a':
    case 'w':
    case 's':
    case 'd':

    // Moving the camera front-back-left-right
    case 'j':
    case 'i':
    case 'k':
    case 'l':

    // rotating the camera around the look at point
    case 'g':
    case 'h':
        state[128+key] = true;
        break;

    // Moving the camera up-down
    case '+':
        eyeY++;
        lookY++;
        break;
    case '-':
        eyeY--;
        lookY--;
        break;

    // Toggling window open-close
    case 't':
        wndw_rot = (((wndw_rot/89)+1) % 2) * 89;
        break;

    // rotating the whole scene
    case ',':
        rot--;
        break;
    case '.':
        rot++;
        break;

    // look far or near
    case 'r':
        lookX += dx;
        lookZ += dz;
        break;
    case 'f':
        lookX -= dx;
        lookZ -= dz;
        break;

    // on-off switches to whichever it is attached
    case '1':
        light1 = !light1;
        break;
    case '2':
        light2 = !light2;
        break;
    case '3':
        light3 = !light3;
        break;

    // Target cross on-off
    case 'o':
        target_on = !target_on;
        break;

    // Fan on-off
    case 'F':
        fan_on = !fan_on;
        break;

    // Drone effect on-off
    case 'D':
        drone_effect = !drone_effect;
        break;

    // Drone effect on-off
    case 'c':
        climb_thresh=10;
        break;

    // Speed control
    case 'Q':
        speed_ind = min(speed_ind+1, speed_range-1);
        movement_animation();
        break;
    case 'S':
        speed_ind = max(speed_ind-1, 0);
        movement_animation();
        break;

    // Third person view toggle
    case 'T':
        third_person_view = !third_person_view;
        break;

    // current selected point
    case 'p':
        show_cur_sel_pt=!show_cur_sel_pt;
        if(show_cur_sel_pt)
        {
            num_cur_points=0;
            eyeZ=3;
            lookZ=eyeZ-1;

            eyeY=0;
            lookY=0;

            eyeX=0;
            lookX=0;
        }
        break;
    case 'P':
        save_cur_coords();
        break;
    case 'W':
        wired=!wired;
        break;

    case 'B':
//        cout<<endl<<eyeX<<" "<<eyeY<<" "<<eyeZ<<" "<<lookX<<" "<<lookY<<" "<<lookZ<<endl;
        eyeX=293.279;
        eyeY=67;
        eyeZ=210.644;
        lookX=281.788;
        lookY=67;
        lookZ=201.002;
        break;
    case 'b':
//        cout<<endl<<eyeX<<" "<<eyeY<<" "<<eyeZ<<" "<<lookX<<" "<<lookY<<" "<<lookZ<<endl;
        eyeX = 0;
        eyeY = 30;
        eyeZ = 60;

        lookX = 0;
        lookY = eyeY;
        lookZ = eyeZ-15;
        break;
    case 'v':
//        cout<<endl<<eyeX<<" "<<eyeY<<" "<<eyeZ<<" "<<lookX<<" "<<lookY<<" "<<lookZ<<endl;
        eyeX = 224;
        eyeY = 45;
        eyeZ = -210;

        lookX = eyeX;
        lookY = eyeY;
        lookZ = eyeZ+15;
        break;
    case 'V':
//        cout<<endl<<eyeX<<" "<<eyeY<<" "<<eyeZ<<" "<<lookX<<" "<<lookY<<" "<<lookZ<<endl;
        eyeX = -128;
        eyeY = 15;
        eyeZ = -135;

        lookX = eyeX;
        lookY = eyeY;
        lookZ = eyeZ+15;
        break;

    case 'z':
        sun_boost=true;
        break;

    case 'e':
        editor_mode = !editor_mode;
        if(editor_mode) speed_ind=0;
        else speed_ind=speed_range-1;
    }

    glutPostRedisplay();
}

void key_up(unsigned char key, int x, int y)
{
    switch (key)
    {
    // moving the look at point at a circular path or up-down
    case 'a':
    case 'w':
    case 's':
    case 'd':

    // Moving the camera front-back-left-right
    case 'j':
    case 'i':
    case 'k':
    case 'l':

    // rotating the camera around the look at point
    case 'g':
    case 'h':
        state[128+key] = false;
        break;

    case 'z':
        sun_boost=false;
        break;

    case 'c':
        climb_thresh=pre_climb_thresh;
        break;
    }
}

void bump_reverse(int val)
{
    if(cur_player_height < playerHeight)
    {
        cur_player_height+=1.0;
        cur_player_height-=0.15;
        glutTimerFunc(25, bump_reverse, 1);
    }
    else
    {
        cur_player_height=playerHeight;
        cur_player_width=playerWidth;
    }

    bumped=false;
}

static void idle(void)
{
    if(fan_on)
    {
        fan_rot += 8*speed_arr[speed_ind];
        if(fan_rot >= 360)
            fan_rot = 0;
    }

    sky_rot += 0.05;
    if(sky_rot >= 360)
        sky_rot = 0;

    // Sun movement
    if(sun_move)
    {
        float d_theta=0.05;
        if(sun_boost) d_theta=2;

        sun_theta -= d_theta;
        if(sun_theta<=-270) sun_theta=90;
        sun_x=sun_dist*cos(sun_theta*PI/180);
        sun_y=sun_dist*sin(sun_theta*PI/180);

        cur_sun_light_intensity=sun_light_intensity*sin(sun_theta*PI/180) + sun_light_intensity;
        sun_cur_color[1] = sun_color[1]*sin(sun_theta*PI/180) + 0.25;//0.2;
        sun_cur_color[2] = sun_color[2]*sin(sun_theta*PI/180) + 0.2;

        if(sun_theta<0 && sun_theta>-180) light1=true;
        else light1=false;
//        cout<<sun_theta<<" "<<sun_x<<" "<<sun_y<<endl;
    }

    if(inter_wndw_rot-wndw_rot<10)
        inter_wndw_rot+=5;
    else if(inter_wndw_rot-wndw_rot>10)
        inter_wndw_rot-=5;

    // Movement control
    if(state[128+'a']||state[128+'s']||state[128+'d']||state[128+'w']
       ||state[128+'i']||state[128+'j']||state[128+'k']||state[128+'l']
       ||state[128+'g']||state[128+'h']) movement_animation();

    // check floor
    int l=max(int((eyeY-cur_player_height)/half_floor_height), 0);
    current_floor=half_floors[l];
//    cout<<l<<" "<<eyeY-playerHeight<<" "<<eyeX<<" "<<eyeZ<<" "<<check_current_floor(eyeZ, eyeX)<<endl;

    if(!drone_effect)
    {
        // Falling down seriously
        float fall_unit=2*speed_arr[speed_ind];
        float height_from_floor=eyeY-cur_player_height - check_current_floor(eyeZ, eyeX);
        if(height_from_floor>bump_thresh) bump_down=true;

        if(height_from_floor>fall_unit)
        {
            eyeY-=fall_unit;
            lookY-=fall_unit;

            hands_up=true;
        }
        else if(height_from_floor!=0)
        {
            eyeY-=height_from_floor;
            lookY-=height_from_floor;

            hands_up=false;

            if(bump_down && !bumped && height_from_floor>0)
            {
                cur_player_height = playerHeight/2;
                cur_player_width = playerWidth*1.25;
        //            cout<<playerHeight<<" "<<height_from_floor<<endl;
                bumped=true;
                bump_down=false;
                glutTimerFunc(350, bump_reverse, 1);
            }

        }
    }


    // pond reflection toggle
    // for reference
    // void pond(float posX=0, float posY=0, float posZ=0, float pond_width=50, float pond_length=50, float border_width=5, float border_height=1, float pond_height=0.2)
    // pond(120,0,140, 200,250,10,1);

//    float refEnRange = 30;
//    float reflXStart = /*120 - (200/2)*/ 20 - refEnRange, reflXStop = /*120 + (200/2)*/ 220 + refEnRange;
//    float reflZStart = /*140 - (250/2)*/ 15 - refEnRange, reflZStop = /*140 + (250/2)*/ 265 + refEnRange;
//
//    if(((reflXStart <= eyeX) & (eyeX < reflXStop)) & ((reflZStart <= eyeZ) & (eyeZ < reflZStop))) reflection_needed = true;
//    else reflection_needed = false;

    glutPostRedisplay();
}

// Loading all textures

void load_textures()
{
    LoadTexture("images\\brick.bmp", BRICK);
    texture_size[BRICK]=25;
    LoadTexture("images\\wood1.bmp", WOOD1);
    LoadTexture("images\\wood2.bmp", WOOD2);
    LoadTexture("images\\wood3.bmp", WOOD3);
    LoadTexture("images\\wood4.bmp", WOOD4);
    LoadTexture("images\\wall1.bmp", WALL1);
    texture_size[WALL1]=5;
    LoadTexture("images\\wall2.bmp", WALL2);
    texture_size[WALL2]=20;
    LoadTexture("images\\wall3.bmp", WALL3);
    texture_size[WALL3]=5;
    LoadTexture("images\\wall4.bmp", WALL4);
    texture_size[WALL4]=5;
    LoadTexture("images\\wall5.bmp", WALL5);
    texture_size[WALL5]=10;
    LoadTexture("images\\tile1.bmp", TILE1);
    texture_size[TILE1]=5;
    LoadTexture("images\\tile2.bmp", TILE2);
    texture_size[TILE2]=5;
    LoadTexture("images\\window1.bmp", WINDOW1);
    LoadTexture("images\\bedsheet1.bmp", BEDSHEET1);
    LoadTexture("images\\bedsheet2.bmp", BEDSHEET2);
    LoadTexture("images\\bedsheet3.bmp", BEDSHEET3);
    LoadTexture("images\\blanket1.bmp", BLANKET1);
    LoadTexture("images\\blanket2.bmp", BLANKET2);
    LoadTexture("images\\pillow1.bmp", PILLOW1);
    LoadTexture("images\\pillow2.bmp", PILLOW2);
    LoadTexture("images\\pillow3.bmp", PILLOW3);
    LoadTexture("images\\field1.bmp", FIELD1);
    texture_size[FIELD1]=100;
    LoadTexture("images\\road_horizontal.bmp", ROAD1);
    texture_size[ROAD1]=20;
    LoadTexture("images\\road_vertical.bmp", ROAD1_VER);
    texture_size[ROAD1_VER]=20;
    LoadTexture("images\\road_intersection.bmp", ROAD_INT1);
    texture_size[ROAD_INT1]=20;
    LoadTexture("images\\road_intersection_2.bmp", ROAD_INT2);
    texture_size[ROAD_INT2]=20;
    LoadTexture("images\\road_2.bmp", ROAD2);
    texture_size[ROAD2]=5;
    LoadTexture("images\\fence_1.bmp", FENCE1);
    texture_size[FENCE1]=10;
    LoadTexture("images\\tree_body.bmp", TREE_BODY1);
    LoadTexture("images\\tree_body_2.bmp", TREE_BODY2);
    LoadTexture("images\\tree_body_3.bmp", TREE_BODY3);
    LoadTexture("images\\tree_leaf_1.bmp", TREE_LEAF1);
    LoadTexture("images\\tree_leaf_2.bmp", TREE_LEAF2);
    LoadTexture("images\\tree_leaf_2_v2.bmp", TREE_LEAF2_V2);
    LoadTexture("images\\tree_leaf_2_v3_cubic.bmp", TREE_LEAF2_V3);
    LoadTexture("images\\tree_leaf_3.bmp", TREE_LEAF3);
    LoadTexture("images\\tree_leaf_3_v2.bmp", TREE_LEAF3_V2);
    LoadTexture("images\\tree_leaf_4.bmp", TREE_LEAF4);
    LoadTexture("images\\tree_leaf_4_v2.bmp", TREE_LEAF4_V2);
    LoadTexture("images\\grass_1.bmp", GRASS1);
    LoadTexture("images\\grass_2.bmp", GRASS2);
    texture_size[GRASS2]=10;
    LoadTexture("images\\grass_3.bmp", GRASS3);
    texture_size[GRASS3]=10;
    LoadTexture("images\\pond_1.bmp", POND1);
    LoadTexture("images\\pond_2.bmp", POND2);
    LoadTexture("images\\pond_3.bmp", POND3);
    LoadTexture("images\\pond_4.bmp", POND4);
    LoadTexture("images\\pond_5.bmp", POND5);
    LoadTexture("images\\blackboard.bmp", BLACKBOARD);
    LoadTexture("images\\chair_1.bmp", CHAIR1);
    LoadTexture("images\\chair_2.bmp", CHAIR2);
    LoadTexture("images\\chair_3.bmp", CHAIR3);
    LoadTexture("images\\floor_1.bmp", FLOOR1);
    texture_size[FLOOR1]=5;
    LoadTexture("images\\the_meme_rect.bmp", MEME_RECT);
    LoadTexture("images\\sky1.bmp", SKY_1);
    LoadTexture("images\\sky2.bmp", SKY_2);
    LoadTexture("images\\sky3.bmp", SKY_3);
    LoadTexture("images\\sky4.bmp", SKY_4);
    LoadTexture("images\\sky4_v2.bmp", SKY_4_V2);
    LoadTexture("images\\sky5.bmp", SKY_5);
    LoadTexture("images\\sky6.bmp", SKY_6);
    LoadTexture("images\\table_cloth_1.bmp", TABLE_CLOTH_1);
    LoadTexture("images\\table_cloth_1_v2.bmp", TABLE_CLOTH_1_V2);
    LoadTexture("images\\table_cloth_2.bmp", TABLE_CLOTH_2);
    LoadTexture("images\\table_cloth_3.bmp", TABLE_CLOTH_3);
    LoadTexture("images\\football_field.bmp", FOOTBALL_FIELD_1);
    LoadTexture("images\\football_field_lh.bmp", FOOTBALL_FIELD_1_LH);
    texture_size[FOOTBALL_FIELD_1_LH]=125;
    LoadTexture("images\\football_field_rh.bmp", FOOTBALL_FIELD_1_RH);
    texture_size[FOOTBALL_FIELD_1_RH]=125;
    LoadTexture("images\\almirah_1_front.bmp", ALMIRAH_1_FRONT);
    LoadTexture("images\\almirah_2.bmp", ALMIRAH_2);
    LoadTexture("images\\almirah_2_front.bmp", ALMIRAH_2_FRONT);
    LoadTexture("images\\almirah_3.bmp", ALMIRAH_3);
    LoadTexture("images\\almirah_3_front.bmp", ALMIRAH_3_FRONT);
    LoadTexture("images\\mirror_1.bmp", MIRROR_1);
    LoadTexture("images\\mirror_2.bmp", MIRROR_2);
    LoadTexture("images\\mirror_3.bmp", MIRROR_3);
    LoadTexture("images\\sun_1.bmp", SUN_1);
    LoadTexture("images\\sun_glare_1.bmp", SUN_GLARE_1);
    LoadTexture("images\\sun_glare_2.bmp", SUN_GLARE_2);
    LoadTexture("images\\sun_glare_3.bmp", SUN_GLARE_3);
    LoadTexture("images\\moon_1.bmp", MOON_1);
    LoadTexture("images\\shirt_cloth_1.bmp", SHIRT_CLOTH_1);
    LoadTexture("images\\shirt_cloth_2.bmp", SHIRT_CLOTH_2);

    LoadTexture_from_arr(make_bw_tiles_texture(64,64,128,128),128,128,TILES);
    texture_size[TILES]=5;
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(30,30);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("A Tiny Colony");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutKeyboardUpFunc(key_up);
//    glutPassiveMotionFunc(mouse_move);
//    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse_click);
    glutIdleFunc(idle);
    glutReshapeFunc(resize);

    glClearColor(0,0,0,1);
    glGenTextures(num_tex, texture_ids);
    load_textures();
    load_coords();
    init_floors(floor_width,floor_length);
    current_floor=half_floors[0]; //bottom floor
    movement_animation();

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    printf("########################################################################################\n");
    printf("########################################################################################\n");
    printf("##############                                                         #################\n");
    printf("##############           PLEASE FOLLOW THE BELOW INSTRUCTIONS          #################\n");
    printf("##############                                                         #################\n");
    printf("########################################################################################\n");
    printf("########################################################################################\n\n\n");
    printf("Use 'w' to look up,\n 's' to look down,\n 'd' to look right,\n and 'a' to look left.\n\n");
    printf("Use 'i' to move camera front (i.e. zoom in),\n 'k' to move camera back (i.e. zoom out),\n 'l' to move camera right,\n and 'j' to move camera left.\n\n");
    printf("Use '+' to move camera up\n and '-' to move camera down. [only available in drone mode]\n\n");
    printf("Use 't' to toggle all windows open or close.\n\n");
    printf("Use 'r' to look far,\n and 'f' to look near.\n\n");
    printf("Use 'g' to rotate left,\n and 'h' to rotate right taking the look at point as center.\n\n");
    printf("Use 'o' to toggle the target cross.\n\n");
    printf("Use 'F' to toggle the fans on-off.\n\n");

    printf("Use 'D' to toggle the drone mode on-off.\n\n");
    printf("Use 'T' to toggle the third person view on-off.\n\n");
    printf("Use 'Q' and 'S' to make the movement faster or slower.\n\n");
    printf("Use 'p' to show or hide-reset current selected points and curve. [try it in editor mode]\n\n");
    printf("Use 'P' to save the current selected points. [after selecting points]\n\n");
    printf("Use 'W' to toggle the wired mode on-off.\n\n");

    printf("Use 'B' to go to birds eye view.\n\n");
    printf("Use 'b' to go to start view.\n\n");
    printf("Use 'v' to jump at school 2nd floor.\n\n");

    printf("Use 'z' to boost the sun.\n\n");
    printf("Use 'e' to toggle the editor mode on-off.\n\n");


    glutMainLoop();

    return EXIT_SUCCESS;
}
