#include "forest.h"

extern unsigned int texture_ids[];
extern bool floor_calculation;

void tree_1(float width, float height, float bush_width, float bush_height, float posX, float posY, float posZ)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[TREE_BODY1]);

    glPushMatrix();
    cylinder(width/2,width/2,height,10);
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,0,0, width,height,width);

    glBindTexture(GL_TEXTURE_2D,texture_ids[TREE_LEAF3]);

    glPushMatrix();
    glTranslatef(0,height,0);
    glScalef(bush_width,bush_height,bush_width);
    sphere(1,10,5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(bush_width/2,height-bush_height/4,0);
    glScalef(bush_width/2,bush_height/2,bush_width/2);
    sphere(1,10,3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-bush_width/4,height-bush_height/2,0);
    glScalef(bush_width/2,bush_height/2,bush_width/2);
    sphere(1,10,3);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void tree_2(float width, float height, float bush_width, float bush_height, float posX, float posY, float posZ)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[TREE_BODY2]);

    glPushMatrix();
    cylinder(width/2,width/2,height,10);
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,0,0, width,height,width);

    glPushMatrix();
    glTranslatef(0,height/3,0);
    glRotatef(45, 0,0,1);
    cylinder(width/4,width/4,height/2,10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,height/2,0);
    glRotatef(-45, 0,0,1);
    cylinder(width/4,width/4,height/2,10);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,texture_ids[TREE_LEAF2]);

    glPushMatrix();
    glTranslatef(0,height,0);
    glScalef(bush_width,bush_height,bush_width);
    sphere(1,10,5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(bush_width/2,height-bush_height/2,0);
    glScalef(bush_width/2,bush_height/2,bush_width/2);
    sphere(1,10,5);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-bush_width/2,height-bush_height,0);
    glScalef(bush_width/2,bush_height/2,bush_width/2);
    sphere(1,10,5);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}

void tree_3(float width, float height, float bush_width, float bush_height, float posX, float posY, float posZ)
{
    glPushMatrix();
    glTranslatef(posX,posY,posZ);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[TREE_BODY3]);

    glPushMatrix();
    cylinder(width/2,width/2,height,10);
    glPopMatrix();

    if(floor_calculation) add_floor_height(posX,posY,posZ, 0,0,0, width,height,width);

    glPushMatrix();
    glTranslatef(0,2*height/3,0);
    glRotatef(45, 0,0,1);
    cylinder(width/4,width/4,height/2,10);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,2*height/3,0);
    glRotatef(-45, 0,0,1);
    cylinder(width/4,width/4,height/2,10);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D,texture_ids[TREE_LEAF2_V3]);

    glPushMatrix();
    glTranslatef(0,height,0);
    cylinder(bush_width/3,bush_width/2,bush_height, 4, 0.5,0.5,0.5,false,128, true,true);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
