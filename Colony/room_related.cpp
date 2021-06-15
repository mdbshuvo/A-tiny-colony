#include "room_related.h"

extern unsigned int texture_ids[];
extern float texture_size[], inter_wndw_rot;
extern int floor_height;
extern bool floor_calculation;


void flr(float posY,
          float room_width, float roomHeight, float room_length,
           float wall_thickness, bool isTiled, texture_id tex)
{

    glPushMatrix();
    glTranslatef(0,posY,0);

    if(isTiled)
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,texture_ids[tex]);
        }
    else
        glDisable(GL_TEXTURE_2D);

//        glRotatef(fan_rot, 0,0,1);
    glScalef(room_width,1,room_length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.5,0.5,0.5,false,128,room_width,1,room_length,texture_size[tex]);
    glPopMatrix();
    if(isTiled)
        glDisable(GL_TEXTURE_2D);
}

void wndw(float width, float height, float roomDim, float roomHeight, bool hasLid=true, bool chosen=false)
{
    float length = 1.25;
    float b_add = 0.5, b_height = 1;
    float body_height = height-1, body_width = width-1;
    float grill_width = 0.5;
    float rot=0, x_tr=0, z_tr=0;
    float lid_width=(body_width-1)/2, lid_height=body_height-1, lid_length=length/2;

    glPushMatrix();
    glTranslatef(x_tr,(roomHeight-height)/2+0.5,z_tr);
    glRotatef(rot, 0,1,0);

    // body
    if(hasLid)
    {

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture_ids[WINDOW1]);

        glPushMatrix();

        glTranslatef(lid_width,0,0);
        glRotatef(inter_wndw_rot, 0,1,0);
        glTranslatef(-lid_width,0,0);

        glTranslatef(lid_width/2,0.5,lid_length/2);
        glScalef(lid_width,lid_height,lid_length);
        glTranslatef(-0.5,0,-0.5);
        cube(0.545, 0.271, 0.075);
        glPopMatrix();

        glPushMatrix();

        glTranslatef(-lid_width,0,0);
        glRotatef(-inter_wndw_rot, 0,1,0);
        glTranslatef(lid_width,0,0);

        glTranslatef(-lid_width/2,0.5,lid_length/2);
        glScalef(lid_width,lid_height,lid_length);
        glTranslatef(-0.5,0,-0.5);
        cube(0.545, 0.271, 0.075);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }

    // borders
    glPushMatrix();
    glTranslatef(0,-0.5,0);
    glScalef(body_width,b_height,length+b_add);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, body_height-b_height+0.5, 0);
    glScalef(body_width,b_height,length+b_add);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(body_width/2,-0.5, 0);
    glScalef(b_height,height,length+b_add);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-body_width/2,-0.5, 0);
    glScalef(b_height,height,length+b_add);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    // grills
    glPushMatrix();
    glTranslatef(0,-0.5, -length/4);
    glScalef(grill_width,body_height,length/2);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 2*body_height/3, -length/4);
    glScalef(body_width,grill_width,length/2);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, body_height/3, -length/4);
    glScalef(body_width,grill_width,length/2);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPopMatrix();

}

void door(float width, float height, float door_rot=89)
{
    float border_width = 0.75;
    float body_height = height-border_width, body_width = width-border_width*2, body_length = border_width/2;

    glPushMatrix();

    // body
    glPushMatrix();

    glTranslatef(-body_width/2,0,-body_length/2);
    glRotatef(door_rot, 0,1,0);
    glTranslatef(body_width/2,0,body_length/2);

    glScalef(body_width, body_height, body_length);
    glTranslatef(-0.5,0,-1);
    cube(0.63, 0.49, 0.38);
    glPopMatrix();

    // borders
    glPushMatrix();
    glTranslatef(-body_width/2,0,0);
    glScalef(border_width,height,border_width);
    glTranslatef(-1,0,-0.5);
    cube(0.63, 0.49, 0.38);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(body_width/2,0,0);
    glScalef(border_width,height,border_width);
    glTranslatef(0,0,-0.5);
    cube(0.63, 0.49, 0.38);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,body_height,0);
    glScalef(body_width,border_width,border_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.63, 0.49, 0.38);
    glPopMatrix();

    glPopMatrix();
}

void wall(float posX, float posY, float posZ, float w_rot,
           float room_width, float roomHeight, float room_length, float wall_thickness,
            bool hasWin, bool hasWinLid, float win_h, float win_w,
             bool hasDoor, float d_width, float d_height,
             texture_id tex,bool chosen)
{
    bool tex_en=true;

    float cur_width=room_width;
    if(w_rot==90 || w_rot==-90) cur_width=room_length;

    if(hasWin)
    {

        glPushMatrix();
        glTranslatef(posX,posY,posZ);
        glRotatef(w_rot, 0,1,0);
        wndw(win_w,win_h,cur_width,roomHeight,hasWinLid,chosen);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture_ids[tex]);

        glPushMatrix();
        glScalef(cur_width,(roomHeight-win_h)/2,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,cur_width,(roomHeight-win_h)/2,wall_thickness,texture_size[tex]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,(roomHeight-win_h)/2+win_h,0);
        glScalef(cur_width,(roomHeight-win_h)/2,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,cur_width,(roomHeight-win_h)/2,wall_thickness,texture_size[tex]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef((cur_width-(cur_width-win_w)/2)/2,(roomHeight-win_h)/2,0);
        glScalef((cur_width-win_w)/2,win_h,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,(cur_width-win_w)/2,win_h,wall_thickness,texture_size[tex]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-(cur_width-(cur_width-win_w)/2)/2,(roomHeight-win_h)/2,0);
        glScalef((cur_width-win_w)/2,win_h,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,(cur_width-win_w)/2,win_h,wall_thickness,texture_size[tex]);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    else if(hasDoor)
    {
        glPushMatrix();
        glTranslatef(posX,posY,posZ);
        glRotatef(w_rot, 0,1,0);

        door(d_width,d_height);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture_ids[tex]);

        glPushMatrix();
        glTranslatef(-d_width/2,0,0);
        glScalef((cur_width-d_width)/2,roomHeight,wall_thickness);
        glTranslatef(-1,0,-0.5);
        cube(0.5,0.5,0.5,false,128,(cur_width-d_width)/2,roomHeight,wall_thickness,texture_size[tex]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(d_width/2,0,0);
        glScalef((cur_width-d_width)/2,roomHeight,wall_thickness);
        glTranslatef(0,0,-0.5);
        cube(0.5,0.5,0.5,false,128,(cur_width-d_width)/2,roomHeight,wall_thickness,texture_size[tex]);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(0,d_height,0);
        glScalef(d_width,roomHeight-d_height,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,d_width,roomHeight-d_height,wall_thickness,texture_size[tex]);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
    }
    else
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texture_ids[tex]);

        glPushMatrix();
        glTranslatef(posX,posY,posZ);
        glRotatef(w_rot, 0,1,0);
        glScalef(cur_width,roomHeight,wall_thickness);
        glTranslatef(-0.5,0,-0.5);
        cube(0.5,0.5,0.5,false,128,cur_width,roomHeight,wall_thickness,texture_size[tex]);
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
    }
}

void room(float posX, float posY, float posZ,
           float room_width, float room_length,
            bool roof, bool flor, float r_rot, bool has_window, bool has_win_lid, float window_height,
            bool door_front, bool door_right, bool door_back, bool door_left,
            bool face_front, bool face_right, bool face_back, bool face_left,
            float roomHeight, float wall_thick,
            texture_id floor_tex, texture_id wall_tex)
{
    if(roomHeight==-1) roomHeight = floor_height-wall_thick;
    float door_width=10, door_height=15, window_width=8;
    float extra=1;

    bool ax_rot_90 = (r_rot==90 || r_rot==-90);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(r_rot, 0,1,0);

    // Building the house
    if(flor) flr(0,room_width,roomHeight,room_length,wall_thick,true,floor_tex);
    if(roof) flr(floor_height,room_width,roomHeight,room_length,wall_thick);

    if(face_right) wall((room_width/2)-wall_thick/2,wall_thick,0,90,room_width,roomHeight,room_length,wall_thick, (!door_right&&has_window),has_win_lid,window_height,window_width, door_right,door_width,door_height,wall_tex);
    if(face_left) wall(-(room_width/2)+wall_thick/2,wall_thick,0,-90,room_width,roomHeight,room_length,wall_thick, (!door_left&&has_window),has_win_lid,window_height,window_width, door_left,door_width,door_height,wall_tex);
    if(face_back) wall(0,wall_thick,-(room_length/2)+wall_thick/2,180,room_width,roomHeight,room_length,wall_thick, (!door_back&&has_window),has_win_lid,window_height,window_width, door_back,door_width,door_height,wall_tex);
    if(face_front) wall(0,wall_thick,(room_length/2)-wall_thick/2,0,room_width,roomHeight,room_length,wall_thick, (!door_front&&has_window),has_win_lid,window_height,window_width, door_front,door_width,door_height,wall_tex);

    glPopMatrix();

    // Objectify
    if(floor_calculation)
    {
        if(flor) add_floor_height(posX,posY,posZ, 0,0,0, room_width,wall_thick,room_length ,ax_rot_90,r_rot); //bottom
        if(roof) add_floor_height(posX,posY,posZ, 0,floor_height,0, room_width,wall_thick,room_length ,ax_rot_90,r_rot); //top

        if(face_right)
        {
            if(door_right)
            {
                add_floor_height(posX,posY+wall_thick,posZ, (room_width/2)-wall_thick/2,0,-(room_length+door_width)/4, (room_length-door_width)/2,roomHeight,wall_thick, !ax_rot_90,r_rot); //right
                add_floor_height(posX,posY+wall_thick,posZ, (room_width/2)-wall_thick/2,0,(room_length+door_width)/4, (room_length-door_width)/2,roomHeight,wall_thick, !ax_rot_90,r_rot); //right
            }
            else add_floor_height(posX,posY+wall_thick,posZ, (room_width/2)-wall_thick/2,0,0, room_length,roomHeight,wall_thick, !ax_rot_90,r_rot); //right
        }

        if(face_left)
        {
            if(door_left)
            {
                add_floor_height(posX,posY+wall_thick,posZ, -(room_width/2)-wall_thick/2,0,-(room_length+door_width)/4, (room_length-door_width)/2,roomHeight,wall_thick, !ax_rot_90,r_rot); //right
                add_floor_height(posX,posY+wall_thick,posZ, -(room_width/2)-wall_thick/2,0,(room_length+door_width)/4, (room_length-door_width)/2,roomHeight,wall_thick, !ax_rot_90,r_rot); //right
            }
            else add_floor_height(posX,posY+wall_thick,posZ, -(room_width/2)+wall_thick/2,0,0, room_length,roomHeight,wall_thick, !ax_rot_90,r_rot);  //left
        }

        if(face_back)
        {
            if(door_back)
            {
                add_floor_height(posX,posY+wall_thick,posZ, -(room_width+door_width)/4,0,-(room_length/2)+wall_thick/2, (room_width-door_width)/2,roomHeight,wall_thick, ax_rot_90,r_rot);  //back
                add_floor_height(posX,posY+wall_thick,posZ, (room_width+door_width)/4,0,-(room_length/2)+wall_thick/2, (room_width-door_width)/2,roomHeight,wall_thick, ax_rot_90,r_rot);  //back
            }
            else add_floor_height(posX,posY+wall_thick,posZ, 0,0,-(room_length/2)+wall_thick/2, room_width,roomHeight,wall_thick, ax_rot_90,r_rot);  //front
        }

        if(face_front)
        {
            if(door_front)
            {
                add_floor_height(posX,posY+wall_thick,posZ, -(room_width+door_width)/4,0,(room_length/2)+wall_thick/2, (room_width-door_width)/2,roomHeight,wall_thick, ax_rot_90,r_rot);  //back
                add_floor_height(posX,posY+wall_thick,posZ, (room_width+door_width)/4,0,(room_length/2)+wall_thick/2, (room_width-door_width)/2,roomHeight,wall_thick, ax_rot_90,r_rot);  //back
            }
            else add_floor_height(posX,posY+wall_thick,posZ, 0,0,(room_length/2)-wall_thick/2, room_width,roomHeight,wall_thick, ax_rot_90,r_rot);  //front
        }
    }
}

void stair_room(float posX, float posY, float posZ, float width,
                 float fl_width, float fl_thick, float fl_length, bool has_floor, float st_rot,
                 texture_id floor_tex, texture_id wall_tex)
{
    bool st_rot_90 = ((st_rot==90) || (st_rot==-90));
    float r_rad=0.2, r_height=5, add_mid_top_length=3;

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glRotatef(st_rot, 0,1,0);

    // back wall
    wall(0,fl_thick,-fl_length/2+fl_thick/2, 0, fl_width,floor_height-fl_thick,fl_thick, fl_thick, false,false,0,0,false,0,0, wall_tex);
    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,fl_thick,-fl_length/2+fl_thick/2, fl_width,floor_height-fl_thick,fl_thick, st_rot_90,st_rot);

    if(has_floor)
    {
        // floor
        flr(0, fl_width,0,fl_length,fl_thick, true, floor_tex);
        if(floor_calculation) add_floor_height(posX,posY,posZ, 0,0,0, fl_width,fl_thick,fl_length, st_rot_90,st_rot);
    }

    for(float i=1; i<floor_height/2; i++)
    {
        glPushMatrix();
        glTranslatef(-width/4,fl_thick,-i+1+add_mid_top_length);
        glScalef(width/2,i,1);
        glTranslatef(-0.5,0,-0.5);
        cube();
        glPopMatrix();

        if(floor_calculation) add_floor_height(posX,posY,posZ, -width/4,fl_thick,-i+1+add_mid_top_length, width/2,i,1, st_rot_90,st_rot);

        glPushMatrix();
        glTranslatef(-width/2+r_rad,i+1,-i+1+add_mid_top_length);
        cylinder(r_rad,r_rad,r_height,10, 0.7,0.7,0.7);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-r_rad,i+1,-i+1+add_mid_top_length);
        cylinder(r_rad,r_rad,r_height,10, 0.8,0.8,0.8);
        glPopMatrix();

        if(floor_calculation) add_floor_height(posX,posY,posZ, -width/2+r_rad,i+1,-i+1+add_mid_top_length, r_rad,r_height,1, st_rot_90,st_rot);
        if(floor_calculation) add_floor_height(posX,posY,posZ, -r_rad,i+1,-i+1+add_mid_top_length, r_rad,i,r_height, st_rot_90,st_rot);
    }

    glPushMatrix();
    glTranslatef(-r_rad,r_height+r_rad*3,fl_thick+add_mid_top_length);
    glRotatef(-45, 1,0,0);
    cylinder(r_rad*2,r_rad*2,(floor_height-1)/sqrt(2),10, 0.8,0.8,0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-width/2+r_rad,r_height+r_rad*3,fl_thick+add_mid_top_length);
    glRotatef(-45, 1,0,0);
    cylinder(r_rad*2,r_rad*2,(floor_height-1)/sqrt(2),10, 0.8,0.8,0.8);
    glPopMatrix();

    // mid stair
    float mid_break_length=5+add_mid_top_length;
    glPushMatrix();
    glTranslatef(0,fl_thick,-floor_height/2+0.5);
    glScalef(width,floor_height/2,mid_break_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,fl_thick,-floor_height/2+0.75, width,floor_height/2,5, st_rot_90,st_rot);

    glPushMatrix();
    glTranslatef(-width/2+r_rad,floor_height/2+r_height,-floor_height/2+0.5+mid_break_length/2);
    glRotatef(-90, 1,0,0);
    cylinder(r_rad*2,r_rad*2,mid_break_length,10, 0.8,0.8,0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(width/2-r_rad,floor_height/2+r_height+1,-floor_height/2+0.5+mid_break_length/2);
    glRotatef(-90, 1,0,0);
    cylinder(r_rad*2,r_rad*2,mid_break_length,10, 0.8,0.8,0.8);
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, -width/2+r_rad,floor_height/2,-floor_height/2+0.5, r_rad*2,r_height,mid_break_length);
    if(floor_calculation) add_floor_height(posX,posY,posZ, width/2-r_rad,floor_height/2,-floor_height/2+0.5, r_rad*2,r_height,mid_break_length);

    glPushMatrix();
    glTranslatef(r_rad,floor_height/2+r_height+r_rad*3,-floor_height/2+mid_break_length/2);
    glRotatef(45, 1,0,0);
    cylinder(r_rad*2,r_rad*2,(floor_height-1)/sqrt(2),10, 0.8,0.8,0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(width/2-r_rad,floor_height/2+r_height+r_rad*3,-floor_height/2+mid_break_length/2);
    glRotatef(45, 1,0,0);
    cylinder(r_rad*2,r_rad*2,(floor_height-1)/sqrt(2),10, 0.8,0.8,0.8);
    glPopMatrix();

    // second part begins
    for(float i=floor_height/2+1; i<floor_height; i++)
    {
        glPushMatrix();
        glTranslatef(width/4,floor_height/2+fl_thick,i-floor_height+1+add_mid_top_length);
        glScalef(width/2,i-floor_height/2,1);
        glTranslatef(-0.5,0,-0.5);
        cube();
        glPopMatrix();

        if(floor_calculation) add_floor_height(posX,posY,posZ, width/4,floor_height/2+fl_thick,i-floor_height+1+add_mid_top_length, width/2,i-floor_height/2,1, st_rot_90,st_rot);

        glPushMatrix();
        glTranslatef(width/2-r_rad,i+1,i-floor_height+1+add_mid_top_length);
        cylinder(r_rad,r_rad,r_height,10, 0.8,0.8,0.8);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(r_rad,i+1,i-floor_height+1+add_mid_top_length);
        cylinder(r_rad,r_rad,r_height,10, 0.8,0.8,0.8);
        glPopMatrix();

        if(floor_calculation) add_floor_height(posX,posY,posZ, width/2-r_rad,i+1,i-floor_height+1+add_mid_top_length, r_rad,r_height,1, st_rot_90,st_rot);
        if(floor_calculation) add_floor_height(posX,posY,posZ, r_rad,i+1,i-floor_height+1+add_mid_top_length, r_rad,r_height,1, st_rot_90,st_rot);
    }

    // top floor
    float side_top_flr_width = (fl_width-width)/2;
    float mid_top_length = fl_length-floor_height/2-mid_break_length-fl_thick+0.5;
    float mid_top_z_tr = (fl_length-mid_top_length)/2;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[floor_tex]);

    glPushMatrix();
    glTranslatef(0,floor_height,mid_top_z_tr);
    glScalef(fl_width-side_top_flr_width*2,fl_thick,mid_top_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,floor_height,mid_top_z_tr, fl_width-side_top_flr_width*2,fl_thick,mid_top_length, st_rot_90,st_rot);

    glPushMatrix();
    glTranslatef((fl_width-side_top_flr_width)/2,floor_height,0);
    glScalef(side_top_flr_width,fl_thick,fl_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, (fl_width-side_top_flr_width)/2,floor_height,0, side_top_flr_width,fl_thick,fl_length, st_rot_90,st_rot);

    glPushMatrix();
    glTranslatef(-(fl_width-side_top_flr_width)/2,floor_height,0);
    glScalef(side_top_flr_width,fl_thick,fl_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, -(fl_width-side_top_flr_width)/2,floor_height,0, side_top_flr_width,fl_thick,fl_length, st_rot_90,st_rot);

    glPushMatrix();
    glTranslatef(-width/4,floor_height,-mid_top_length/2);
    glScalef(width/2,fl_thick,fl_length-mid_top_length);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, -width/4,floor_height,-mid_top_length/2, width/2,fl_thick,fl_length-mid_top_length, st_rot_90,st_rot);

    glPushMatrix();
    glTranslatef(0,floor_height,-(fl_length-fl_thick)/2);
    glScalef(fl_width-side_top_flr_width*2,fl_thick,fl_thick);
    glTranslatef(-0.5,0,-0.5);
    cube();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void furniture_settings_1(float posX, float posY, float posZ, float room_width, float room_length, float r_rot)
{
    float wall_thick=1;
    float roomHeight = floor_height-wall_thick;

    bool ax_rot_90 = (r_rot==90 || r_rot==-90);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(r_rot, 0,1,0);

    chair(7,7,6,8,-room_width/2+10,0,-90);
    table(6,5.25,6,-room_width/2+5,0,180);
    fan(6,0,roomHeight,0);
    sofa_1(15,7,5, 0,0,room_length/2-5, 180);

    tube(1, room_width/2-1, roomHeight-3, room_length/2-1);

    glPopMatrix();

    // Objectify
    if(floor_calculation)
    {
        add_floor_height(posX,posY,posZ, -room_width/2+10,0,0, 3,7,3, ax_rot_90,r_rot);  //chair
        add_floor_height(posX,posY,posZ, -room_width/2+5,0,0, 6,6,6, ax_rot_90,r_rot);  //table
        add_floor_height(posX,posY,posZ, 0,0,room_length/2-5, 15,7,5, ax_rot_90,r_rot);  //sofa
    }
}

void furniture_settings_2(float posX, float posY, float posZ, float room_width, float room_length, float r_rot, bool mirrored)
{
    float wall_thick=1;
    float roomHeight = floor_height-wall_thick;

    bool ax_rot_90 = (r_rot==90 || r_rot==-90);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(r_rot, 0,1,0);

//    if(!mirrored)
//    {
//        glPushMatrix();
//        glTranslatef(0,0,-(room_length/2-2.5)+3);
//        glScalef(1,1,-1);
//        glTranslatef(0,0,(room_length/2-2.5)-3);
//        bed(14,4,15,room_width/2-10,room_length/2-10,90);
//        glPopMatrix();
//
//        mirrored_surface(4,8,0.2, 0.5, room_width/2-8-5,3,-room_length/2+6);
//    }

    bed(14,4,15,room_width/2-10,room_length/2-10,90);
    fan(6,0,roomHeight,0);
    wardrobe(15,10,6,-room_width/2+10,-room_length/2+5,180);
    almirah(15,15,6, room_width/2-8,0,-room_length/2+4, 0, ALMIRAH_3_FRONT, ALMIRAH_3);
    basin(3,6,2, -room_width/2+10,0,room_length/2-1.5, 180);

    tube(1, room_width/2-1, roomHeight-3, room_length/2-1);

    glPopMatrix();

    // Objectify
    if(floor_calculation)
    {
        add_floor_height(posX,posY,posZ, room_width/2-10,0,room_length/2-10, 14,4,15, !ax_rot_90,r_rot);  //bed
        add_floor_height(posX,posY,posZ, -room_width/2+10,0,-room_length/2+5, 15,10,6, ax_rot_90,r_rot);  //wardrobe
        add_floor_height(posX,posY,posZ, room_width/2-8,0,-room_length/2+2.5, 15,15,6, ax_rot_90,r_rot);  //almirah
    }
}

void furniture_settings_3(float posX, float posY, float posZ, float width, float length, float r_rot)
{
    float wall_thick=1;
    float roomHeight = floor_height-wall_thick;
    float fan_blade=6, inter_fan_dist=20;
    int num_col_fans=(width-inter_fan_dist)/inter_fan_dist, num_row_fans=length/inter_fan_dist;
    float plt_len=10, plt_width=20, plt_ben_dist=20, ben_behind_dist=15, ben_width=10, ben_len=8, ben_gap=2;
    float num_ben=(width-plt_len-plt_ben_dist-ben_behind_dist)/(ben_len+ben_gap);

    bool ax_rot_90 = (r_rot==90 || r_rot==-90);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(r_rot, 0,1,0);

    table_2(plt_width,2,plt_len, 0.5, -(width-plt_len)/2+wall_thick,0,0, 90,0.5);
    blackboard(plt_width+5,roomHeight/3,0.5, -(width-0.5)/2+wall_thick,roomHeight/4,0, 90);

    float fan_x_start=-width/2+inter_fan_dist, fan_z_start=-length/2+inter_fan_dist;
    for(int i=0; i<num_row_fans; i++)
    {
        for(int j=0; j< num_col_fans; j++)
        {
            fan(fan_blade, fan_x_start+j*inter_fan_dist,roomHeight,fan_z_start+i*inter_fan_dist);
        }
    }

    float ben_z_tr=(length-ben_width)/2-wall_thick-5, ben_x_start=-width/2+plt_len+plt_ben_dist;
    for(int i=0; i<num_ben; i++)
    {
        bench_set(ben_width,5,ben_len, ben_x_start+i*(ben_gap+ben_len),0,ben_z_tr, 90,0.5);

        if(floor_calculation) add_floor_height(posX,posY,posZ, ben_x_start+i*(ben_gap+ben_len),0,ben_z_tr, ben_width,5,ben_len, true,0);
    }
    for(int i=0; i<num_ben; i++)
    {
        bench_set(ben_width,5,ben_len, ben_x_start+i*(ben_gap+ben_len),0,-ben_z_tr, 90,0.5);

        if(floor_calculation) add_floor_height(posX,posY,posZ, ben_x_start+i*(ben_gap+ben_len),0,-ben_z_tr, ben_width,5,ben_len, true,0);
    }


    tube(1, width/2-1, roomHeight-3, length/2-1);
    tube(3, width/2-1, roomHeight-3, length/2-1);

    glPopMatrix();

    // Objectify
    if(floor_calculation)
    {
        add_floor_height(posX,posY,posZ, -(width-plt_len)/2+wall_thick,0,0, plt_width,2,plt_len, !ax_rot_90,r_rot);  //desk
    }
}

void furniture_settings_4(float posX, float posY, float posZ, float width, float length, float r_rot)
{
    float wall_thick=1;
    float roomHeight = floor_height-wall_thick;
    float fan_blade=6, table_width=10, table_length=25, table_height=6;
    float chair_z_tr=(table_length/2+2.5);
    float chair_x_tr=(table_width/2+2.5);

    bool ax_rot_90 = (r_rot==90 || r_rot==-90);

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(r_rot, 0,1,0);

    table_2(table_width,table_height,table_length, 1, 0,0,0, 0,0.5, TABLE_CLOTH_1);
    chair_2(5,8,5, 0,0,-chair_z_tr, 0);
    chair_2(5,8,5, 0,0,chair_z_tr, 180);
    chair_2(5,8,5, chair_x_tr,0,-chair_z_tr/3, -90);
    chair_2(5,8,5, chair_x_tr,0,chair_z_tr/3, -90);
    chair_2(5,8,5, -chair_x_tr,0,-chair_z_tr/3, 90);
    chair_2(5,8,5, -chair_x_tr,0,chair_z_tr/3, 90);
    almirah(10,15,5, width/2-4,0,length/2-10, -90, ALMIRAH_1_FRONT,WOOD3);
    almirah(10,15,5, width/2-4,0,-length/2+10, -90, ALMIRAH_1_FRONT,WOOD3);

    fan(fan_blade, 0,roomHeight,0);
    tube(1, width/2-1, roomHeight-3, length/2-1);
    tube(3, width/2-1, roomHeight-3, length/2-1);

    glPopMatrix();

    // Objectify
    if(floor_calculation)
    {
        add_floor_height(posX,posY,posZ, 0,0,0, table_width,table_height,table_length, ax_rot_90,r_rot);  //table
        add_floor_height(posX,posY,posZ, width/2-4,0,length/2-10, 10,15,5, !ax_rot_90,r_rot);  //almirah_1
        add_floor_height(posX,posY,posZ, width/2-4,0,-length/2+10, 10,15,5, !ax_rot_90,r_rot);  //almirah_2

    }
}

