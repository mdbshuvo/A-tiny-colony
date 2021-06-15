#include "furnitures.h"

extern unsigned int texture_ids[];
extern float fan_rot;

void lamp(float rad, float height, float posX, float posY, float posZ, GLenum light_num, bool on)
{
    float base_height = 0.5;
    float head_height = (height-base_height)/2;
    float leg_height = height-base_height-head_height;
    float leg_rad = 0.15;
    bool l_em = false;
    if(on)
        l_em=true;

    glPushMatrix();
    glTranslatef(posX,posY+base_height+leg_height,posZ);
//    if(on)
//        light(light_num,0,base_height+leg_height-0.5,leg_rad,true,   glob_spot/*50*/);
//    else
//        light_off(light_num);

    // light
    glPushMatrix();
    glRotatef(45,0,1,0);
    cylinder(rad/2, rad*0.75, head_height,6,0.863, 0.078, 0.235, l_em);
    glPopMatrix();

    // rod
    glPushMatrix();
    glTranslatef(0,-leg_height,0);
    cylinder(leg_rad, leg_rad, leg_height, 10, 0.545, 0.000, 0.000);
    glPopMatrix();

    // basement
    glPushMatrix();
    glTranslatef(0,-leg_height,0);
    glScalef(rad, base_height, rad);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.545, 0.000, 0.000);
    glPopMatrix();

    glPopMatrix();
}

void pillow(float posX, float posY, float posZ, float p_w)
{
    float p_r = 1.5;
    float p_l = 5;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[BEDSHEET2]);

    glPushMatrix();
    glTranslatef(posX,posY+0.5*p_r,posZ-p_l/2);

    glRotatef(90, 0,0,1);
    glTranslatef(0,-p_l/2,0);
    glScalef(0.5,1,1.4);
    cylinder(p_r,p_r,p_l,20,0.5,0.5,0.7,false,128,true);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void bedSheet(float width, float length, float posY)
{
    float b_h = 0.5;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[BEDSHEET3]);

    glPushMatrix();
    glTranslatef(0,posY+0.5*b_h,0);
    glScalef(width,b_h,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void blanket(float width, float length, float posY, float posZ)
{
    float height=0.2;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[BLANKET1]);

    glPushMatrix();
    glTranslatef(0,posY+height,posZ);
    glScalef(width,height,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.416, 0.353, 0.804);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void bed(float width, float height, float length, float posX, float posZ, float b_rot)
{
    float num_pillow = 2;
    float p_w = 4;
    float mat_width = 3, mat_height=0.2, mat_length=5;
    float bl_width = width-4, bl_length=length/3;

    glPushMatrix();
    glTranslatef(posX,height/2,posZ);
    glRotatef(b_rot, 0,1,0);

    float gap = (width - num_pillow*p_w) / (num_pillow+1);
    for(int i=0; i<num_pillow; i++)
    {
        float p_x = (width/2) - (i*p_w + (i+1)*gap) - p_w/2;
        pillow(p_x, height/2, length/2, p_w);
    }
    bedSheet(width, length, height/2);
    blanket(bl_width,bl_length,height/2+0.5,-length/2+bl_length/2+0.5);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[WOOD4]);

    glPushMatrix();
    glScalef(width,height,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.53, 0.39, 0.28);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // mat
    glPushMatrix();
    glTranslatef((width+mat_width)/2+0.3,-(height-mat_height)/2,0);
    glScalef(mat_width,mat_height,mat_length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.294, 0.000, 0.510);
    glPopMatrix();

    glPopMatrix();
}

// change it
void chair(int baseX, int baseZ, float legSize, float topSize, float px, float pz, float cRot)
{
    glPushMatrix();
    // Chair placement
    glTranslatef(px,2.75,pz);
    glRotatef(cRot, 0,1,0);
    glScalef(0.5,0.5,0.5);

    // ###########################
    // Base
    glPushMatrix();
    glScalef(baseX,1,baseZ);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    // ###########################
    // Legs
    glPushMatrix();
    glTranslatef(3,-2.5,3);
    glScalef(1,legSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3,-2.5,3);
    glScalef(1,legSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3,-2.5,-3);
    glScalef(1,legSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3,-2.5,-3);
    glScalef(1,legSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    // Leg Connector
    glPushMatrix();
    glTranslatef(3,-4,0);
    glScalef(1,1,baseZ-2);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3,-4,0);
    glScalef(1,1,baseZ-2);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    // Leg Top
    glPushMatrix();
    glTranslatef(3,3,-3);
    glScalef(1,topSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-3,3,-3);
    glScalef(1,topSize,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    // Leg Top Connector
    glPushMatrix();
    glTranslatef(0,2,-3);
    glScalef(baseX-2,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,4,-3);
    glScalef(baseX-2,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,6,-3);
    glScalef(baseX-2,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube();
    glPopMatrix();

    glPopMatrix();
}

void table(float width, float height, float length, float posX, float posZ, float t_rot)
{
    float base_height = 0.5;
    float leg_height = height - base_height;

    glPushMatrix();
    glTranslatef(posX,height-0.25,posZ);
    glRotatef(t_rot, 0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[WOOD1]);

    // Base
    glPushMatrix();
    glScalef(width,base_height,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    // Legs
    glPushMatrix();
    glTranslatef(-(width/2-0.25),-leg_height/2-0.25,-(length/2-0.25));
    glScalef(0.5,leg_height,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-0.25),-leg_height/2-0.25,-(length/2-0.25));
    glScalef(0.5,leg_height,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-0.25),-leg_height/2-0.25,(length/2-0.25));
    glScalef(0.5,leg_height,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-0.25),-leg_height/2-0.25,(length/2-0.25));
    glScalef(0.5,leg_height,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    float desk_height=leg_height/1.5;
    float desk_width = width/2;
    float bottom_desk_height= desk_height/4;

    // Desk
    glPushMatrix();
    glTranslatef((width-desk_width)/2,base_height/2,0);

    glPushMatrix();
    glTranslatef(0,0,(length/2-0.15));
    glScalef(desk_width,desk_height,0.3);
    glTranslatef(-0.5,0,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-(length/2-0.15));
    glScalef(desk_width,desk_height,0.3);
    glTranslatef(-0.5,0,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,desk_height,0);
    glScalef(desk_width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height,0);
    glScalef(desk_width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.56,0.38,0.29);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // books
    float book_length=0.5, book_height=(desk_height-bottom_desk_height-base_height), book_width=2*desk_width/3;
    float b_rot=20, b_rot_tr=book_height*sin(20*PI/180);

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(0.2,0.2,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-2*b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(0.2,1,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-3*b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(0.8,0.2,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-4*b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(0.5,0.2,0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-5*b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(0.3,0.5,0.7);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_desk_height+base_height,length/2-book_length/2-6*b_rot_tr);
    glRotatef(b_rot, 1,0,0);
    glScalef(book_width,book_height,book_length);
    glTranslatef(-0.5,0,-1);
    cube(1,0.2,0.2);
    glPopMatrix();

    glPopMatrix();

    // accessories
    lamp(2,3,desk_width/2,desk_height+0.75,width/8);

    glPopMatrix();
}

void fan(float blade_size, float posX, float posY, float posZ,
         float centroid_height, float centroid_width, float rod_height,
         float blade_height, float blade_width, float rod_rad)
{
    glPushMatrix();
    glTranslatef(posX,posY-(centroid_height+rod_height)+0.25,posZ);

    // Fan body and blades which rotates
    glPushMatrix();
    glRotatef(fan_rot, 0,1,0);

    // Body
    glPushMatrix();

    glPushMatrix();
    glTranslatef(0,-centroid_height/2,0);
    cylinder(centroid_width/2,centroid_width/2,centroid_height,10,0.2,0.2,0.7);
    glPopMatrix();
    glPopMatrix();

    // Blades
    glPushMatrix();
    glTranslatef(-blade_size/2,0,0);
    glScalef(blade_size,blade_height,blade_width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.2,0.2,0.7);
    glPopMatrix();

    glPushMatrix();
    glRotatef(120, 0,1,0);
    glTranslatef(-blade_size/2,0,0);
    glScalef(blade_size,blade_height,blade_width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.2,0.2,0.7);
    glPopMatrix();

    glPushMatrix();
    glRotatef(240, 0,1,0);
    glTranslatef(-blade_size/2,0,0);
    glScalef(blade_size,blade_height,blade_width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.2,0.2,0.7);
    glPopMatrix();

    glPopMatrix();

    // Rod
    glPushMatrix();
    glTranslatef(0,centroid_height/2,0);
    cylinder(rod_rad,rod_rad,rod_height,10,0.2,0.2,0.0);
    glPopMatrix();

    glPopMatrix();
}

void wardrobe(float width, float height, float length, float posX, float posZ, float w_rot)
{
    float border = 1;

    float door_length = 0.3;
    float ld_height = height-2*border;
    float ld_width = 3;

    int num_sd = 3;
    float sd_height = (height - (num_sd+1)) / num_sd;
    float sd_width = width - ld_width - 3*border;

    float dk_height = 1.5;
    float dk_width = 0.2;

    glPushMatrix();
    glTranslatef(posX,0,posZ);
    glRotatef(w_rot, 0,1,0);

//    // accessories
//    lamp(2,3,-5,height,0,GL_LIGHT0, light3);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[WOOD2]);

    // body
    glPushMatrix();
    glScalef(width,height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.63, 0.49, 0.38);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // doors
    glPushMatrix();
    glTranslatef((ld_width-width)/2+border,(height-ld_height)/2,-length/2);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture_ids[WOOD3]);

    glPushMatrix();
    glScalef(ld_width,ld_height,door_length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.53, 0.39, 0.28);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //knob
    glPushMatrix();
    glTranslatef(ld_width/4,(ld_height+dk_height)/2,-0.3);
    glScalef(dk_width,dk_height,door_length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1, 1, 1);
    glPopMatrix();

    glPopMatrix();

    for(int i=0; i<num_sd; i++)
    {
        glPushMatrix();
        glTranslatef((width-sd_width)/2-border,(i+1)*border+i*sd_height,-length/2);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture_ids[WOOD3]);

        glPushMatrix();
        glScalef(sd_width,sd_height,door_length);
        glTranslatef(-0.5,0,-0.5);
        cube(0.53, 0.39, 0.28);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);

        // door knob
        glPushMatrix();
        glTranslatef(0,(sd_height+dk_width)/2,-0.3);
        glRotatef(90,0,0,1);
        glScalef(dk_width,dk_height,door_length);
        glTranslatef(-0.5,-0.5,-0.5);
        cube(1, 1, 1);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
}

void tube(int wall, float posX, float posY, float posZ, GLenum light_num, bool on)
{
    float l_rot = 0;
    bool light_em=false;

    if(on)
        light_em=true;

    if(wall==3)
    {
        posZ = 0;
        l_rot = 90;
    }
    else if(wall==1)
    {
        posX = -posX;
        posZ = 0;
        l_rot = -90;
    }
    else if(wall==2)
    {
        posX = 0;
        posZ = -posZ;
    }
    else
    {
        posX = 0;
    }

    float light_height = .5;
    float light_width=10;
    float light_length=1;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(l_rot, 0,1,0);
//    if(on)
//        light(light_num,0,light_height/2,-light_length*2,false);
//    else
//        light_off(light_num);

    glPushMatrix();
    glTranslatef(0,0,-light_length/2);
    glScalef(light_width,light_height,light_length);
    glTranslatef(-0.5,0,-1);
    cube(1,1,1,light_em);
    glPopMatrix();

    glPushMatrix();
    glScalef(light_width, light_height, light_length/2);
    glTranslatef(-0.5,0,-1);
    cube(0.3,0.3,0.3);
    glPopMatrix();

    glPopMatrix();
}

void sofa_1(float width, float height, float length, float posX, float posY, float posZ, float s_rot)
{
    float kushon_height=height/8, handle_width=1;
    float lower_body_height=height/2-kushon_height, upper_body_heigth=height/2+kushon_height, kushon_width=width-2*handle_width;
    float kushon_length=length-kushon_height, handle_height=upper_body_heigth/2;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(s_rot, 0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[WOOD1]);

    // body
    glPushMatrix();
    glScalef(width,lower_body_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,lower_body_height,-(length-kushon_height)/2);
    glScalef(width,upper_body_heigth,kushon_height);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width-handle_width)/2,lower_body_height,kushon_height/2);
    glScalef(handle_width,handle_height,length-kushon_height);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width-handle_width)/2,lower_body_height,kushon_height/2);
    glScalef(handle_width,handle_height,kushon_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_ids[BLANKET2]);

    glPushMatrix();
    glTranslatef(0,lower_body_height,kushon_height/2);
    glScalef(kushon_width,kushon_height,kushon_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,lower_body_height+kushon_height,-(length-3*kushon_height)/2);
    glScalef(kushon_width,upper_body_heigth-kushon_height,kushon_height);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void table_2(float width, float height, float length, float base_height, float posX, float posY, float posZ, float b_rot, float leg_width, texture_id body_tex, texture_id leg_tex)
{
    float leg_height=height-base_height;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[body_tex]);

    glPushMatrix();
    glTranslatef(0,leg_height,0);
    glScalef(width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_ids[leg_tex]);

    glPushMatrix();
    glTranslatef((width-leg_width)/2,0,(length-leg_width)/2);
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width-leg_width)/2,0,(length-leg_width)/2);
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width-leg_width)/2,0,-(length-leg_width)/2);
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width-leg_width)/2,0,-(length-leg_width)/2);
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void bench_set(float width, float height, float length, float posX, float posY, float posZ, float b_rot, float leg_width)
{
    float gap_betw = length/8, base_height=height/8;
    float base_length=(length-gap_betw)/2, high_leg_height=height-base_height, low_leg_height=(height/2)-base_height;
    float additional_z_tr=(base_length+gap_betw)/2;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    // high bench
    table_2(width,base_height+high_leg_height,base_length,base_height, 0,0,-additional_z_tr);

    // low bench
    table_2(width,base_height+low_leg_height,base_length,base_height, 0,0,additional_z_tr);

    glPopMatrix();
}

void blackboard(float width, float height, float length, float posX, float posY, float posZ, float b_rot)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[BLACKBOARD]);

    glPushMatrix();
    glScalef(width,height,length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // borders
    float bor_width=length*1.5;

    glPushMatrix();
    glTranslatef(-width/2,0,0);
    glScalef(bor_width,height,bor_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.1,0.1,0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(width/2,0,0);
    glScalef(bor_width,height,bor_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.1,0.1,0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,height-bor_width/2,0);
    glScalef(width+bor_width,bor_width,bor_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.1,0.1,0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,-bor_width/2,0);
    glScalef(width+bor_width,bor_width,bor_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.1,0.1,0.1);
    glPopMatrix();

    glPopMatrix();
}

void chair_2(float width, float height, float length, float posX, float posY, float posZ, float b_rot)
{
    float base_height=1, leg_h_fr=0.5, leg_height=(height-base_height)*leg_h_fr, leg_width=0.5;
    float upper_height=leg_height/leg_h_fr*(1-leg_h_fr);
    float top_mid_fr=0.8, top_mid_height=upper_height*top_mid_fr, top_mid_y_tr=upper_height*(1-top_mid_fr)/2;
    float top_mid_width=width-2*leg_width;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    table_2(width,base_height+leg_height,length,base_height, 0,0,0, b_rot,leg_width);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[WOOD3]);

    // upper body
    glPushMatrix();
    glTranslatef((width-leg_width)/2,leg_height+base_height,-(length-leg_width)/2);
    glScalef(leg_width,upper_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width-leg_width)/2,leg_height+base_height,-(length-leg_width)/2);
    glScalef(leg_width,upper_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, texture_ids[CHAIR1]);

    glPushMatrix();
    glTranslatef(0,leg_height+base_height+top_mid_y_tr,-(length-leg_width)/2);
    glScalef(top_mid_width,top_mid_height,leg_width);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void goal_post(float width, float height, float length, float posX, float posY, float posZ, float b_rot)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    glPushMatrix();
    glTranslatef(width/2,0,0);
    cylinder(length/2,length/2, height,6, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-width/2,0,0);
    cylinder(length/2,length/2, height,6, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,height-length/2,0);
    glRotatef(90, 0,0,1);
    glTranslatef(0,-width/2,0);
    cylinder(length/2,length/2, width,6, 1,1,1);
    glPopMatrix();

    glPopMatrix();
}

void almirah(float width, float height, float length, float posX, float posY, float posZ, float b_rot, texture_id front_tex, texture_id tex)
{
    float door_len=0.2, body_len=length-door_len;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[front_tex]);

    glPushMatrix();
    glTranslatef(0,0,(body_len+door_len)/2);
    glScalef(width,height,door_len);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    //body
    glBindTexture(GL_TEXTURE_2D, texture_ids[tex]);

    glPushMatrix();
    glScalef(width,height,body_len);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void basin(float width, float height, float length, float posX, float posY, float posZ, float b_rot)
{
    float sc_w=0.5,sc_h=0.8,sc_l=0.5;
    float base_height=1, pipe_height=height-base_height;

    glPushMatrix();
    glTranslatef(posX,posY+3,posZ);
    glRotatef(b_rot, 0,1,0);

    glPushMatrix();
    glScalef(width,base_height,length);
    glScalef(sc_w,sc_h,sc_l);
    glRotatef(90, 0,0,1);
    set_mat_prop(1,1,1);
    draw_basin();
    glPopMatrix();

    glPushMatrix();
    glScalef(1,0.5,0.9);
    glRotatef(90, 0,1,0);
    glTranslatef(-.8,-7,0);
    draw_basin_pipe();
    glPopMatrix();

    glPushMatrix();
    glScalef(0.8,0.8,0.8);
    glTranslatef(0,-2.8,-1.3);
    glRotatef(-90, 0,1,0);
    set_mat_prop(122.0/255,127.0/255,128.0/255);
    draw_basin_tap();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.1,base_height+0.2,-1);
    glRotatef(-90, 0,0,1);
    cylinder(0.15,0.15,0.3,5, 122.0/255,127.0/255,128.0/255,false,128,true,true);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[MIRROR_1]);

    glPushMatrix();
    glTranslatef(0,height/2,-width/2);
    glScalef(width,width+1,0.2);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void street_lamp(float rad, float top_rad, float height, float bottom_height, GLenum light_num, bool swtch, float posX, float posY, float posZ, float b_rot)
{
    float top_height=height-bottom_height;
    float extra=height-15;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(b_rot, 0,1,0);

    glPushMatrix();
    cylinder(rad,rad,bottom_height, 6, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_height,0);
    cylinder(top_rad,rad,top_height/2, 6, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,bottom_height+top_height/2,0);
    cylinder(top_rad,top_rad,top_height/2, 6, 1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,extra,0);
    set_mat_prop(1,1,1);
    draw_street_top(top_rad);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,17.32+extra,0);
    set_mat_prop(1,1,1);
    draw_street_light();

    if(swtch) light(light_num, 8,0,0,true,60,0.1);
    else light_off(light_num);

    glPopMatrix();

    glPopMatrix();
}

void top_wall_light(GLenum light_num, bool on, float rad, float height, float posX, float posY, float posZ)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);

    if(on) light(light_num,0,0,0, false,80);
    else light_off(light_num);

    glRotatef(180, 1,0,0);
    glScalef(1,height,1);
    sphere(rad,10,10, 1,1,1, on,128,0.5);

    glPopMatrix();
}




