#include "collision_detection.h"

float* half_floors[20];
float* current_floor=0;
extern int floor_width, floor_length, num_level, floor_height ,half_floor_height, num_half_level;

void init_floors(int fl_w, int fl_l)
{
    for(int k=0; k<num_half_level; k++)
    {
        half_floors[k] = new float[fl_w*fl_l];
        for(int i=0; i<fl_l; i++)
        {
            for(int j=0; j<fl_w; j++)
            {
                 half_floors[k][i*fl_w+j]=-1;
            }
        }
    }
}

void y_rotated_coord(float prev_x, float prev_z, float par_rot, float* new_x, float* new_z)
{
    float theta;
    float r=sqrt(prev_x*prev_x+prev_z*prev_z);

    if(prev_x==0 && prev_z>0) theta = 90;
    else if(prev_x==0 && prev_z<0) theta = -90;
    else if(prev_z==0 && prev_x>0) theta = 0;
    else if(prev_z==0 && prev_x<0) theta = 180;
    else if(prev_z==0 && prev_x==0) theta = 0;
    else theta=atan(prev_z/prev_x) * 180 / PI;

    if((prev_z>0 && theta<0) || (prev_z<0 && theta>0))
        theta += 180;

//    cout<<theta<<endl;
    // Ulta rotate kore (Geometry er khela shob)
    theta -= par_rot;

    *new_x = r*cos(theta * PI / 180);
    *new_z = r*sin(theta * PI / 180);
}

void add_floor_height(float posX, float posY, float posZ,
                       float dx, float dy, float dz,
                        float width, float height, float length, bool obj_rotated90, float parent_rot)
{
    float new_x, new_z;
    y_rotated_coord(dx,dz,parent_rot, &new_x,&new_z);

    // That's How
    posZ+=floor_length/2;
    posX+=floor_width/2;

    //adding the rotated coordinates
    posZ+=new_z;
    posX+=new_x;
    posY+=dy;

//    if(floor_calculation) cout<<dx<<"="<<new_x<<"\t\t\t"<<dz<<"="<<new_z<<"\t\t\t"<<((abs(dx-new_x)<0.00001)&&(abs(dz-new_z)<0.00001))<<endl;

//    posZ+=dz;
//    posX+=dx;
//    posY+=dy;

    if(obj_rotated90)
    {
        float temp=width;
        width=length;
        length=temp;
    }

    int level=posY/half_floor_height;

    float x_lbound=posX-width/2;
    float x_ubound=posX+width/2;
    float z_lbound=posZ-length/2;
    float z_ubound=posZ+length/2;

    for(int k=level; k<num_half_level; k++)
    {
        for(int i=z_lbound; i<z_ubound; i++)
        {
            for(int j=x_lbound; j<x_ubound; j++)
            {
                half_floors[k][i*floor_width+j]=max(height+posY, half_floors[k][i*floor_width+j]);
            }
        }
    }
}

float check_current_floor(float posZ, float posX)
{
    posZ+=floor_length/2;
    posX+=floor_width/2;

    return current_floor[int(posZ)*floor_width+int(posX)];
}

