#include "primitives.h"

extern unsigned int texture_ids[];
float light_const_att=0.9, light_1st_att=0.01, light_2nd_att=0.0002, cur_light_2nd_att=light_2nd_att*4;

static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

enum cube_face{C_F_FRONT,C_F_BACK,C_F_TOP,C_F_BOTTOM,C_F_RIGHT,C_F_LEFT};
static GLubyte c_ind[6][4] =
{
    {3,1,5,7},  //front
    {6,4,0,2},  //back
    {2,3,7,6},  //top
    {1,0,4,5},  //bottom
    {7,5,4,6},  //right
    {2,0,1,3}   //left
};

void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1,
                        GLfloat x2, GLfloat y2, GLfloat z2,
                        GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void set_mat_prop(float colR, float colG, float colB, bool em, float shine, float alpha)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { colR, colG, colB, alpha };
    GLfloat mat_diffuse[] = { colR, colG, colB, alpha };
    GLfloat mat_specular[] = { 0.5, 0.5, 0.5, alpha };
    GLfloat mat_emission[] = {colR, colG, colB, 1.0};
    GLfloat mat_shininess[] = {shine};

    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);

    if(em)
        glMaterialfv( GL_FRONT, GL_EMISSION, mat_emission);
    else
        glMaterialfv( GL_FRONT, GL_EMISSION, no_mat);
}

void cube(float colR, float colG, float colB,
          bool em, float shine,
          float dx, float dy, float dz, float tex_size, float alpha)
{
    float x_end=dx/tex_size, y_end=dy/tex_size, z_end=dz/tex_size;

    set_mat_prop(colR,colG,colB,em,shine, alpha);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        if(i==C_F_FRONT || i==C_F_BACK)
        {
            glTexCoord2f(0,y_end);
            glVertex3fv(&v_cube[c_ind[i][0]][0]);
            glTexCoord2f(0,0);
            glVertex3fv(&v_cube[c_ind[i][1]][0]);
            glTexCoord2f(x_end,0);
            glVertex3fv(&v_cube[c_ind[i][2]][0]);
            glTexCoord2f(x_end,y_end);
            glVertex3fv(&v_cube[c_ind[i][3]][0]);
        }
        else if(i==C_F_TOP || i==C_F_BOTTOM)
        {
            glTexCoord2f(0,z_end);
            glVertex3fv(&v_cube[c_ind[i][0]][0]);
            glTexCoord2f(0,0);
            glVertex3fv(&v_cube[c_ind[i][1]][0]);
            glTexCoord2f(x_end,0);
            glVertex3fv(&v_cube[c_ind[i][2]][0]);
            glTexCoord2f(x_end,z_end);
            glVertex3fv(&v_cube[c_ind[i][3]][0]);
        }
        else
        {
            glTexCoord2f(0,y_end);
            glVertex3fv(&v_cube[c_ind[i][0]][0]);
            glTexCoord2f(0,0);
            glVertex3fv(&v_cube[c_ind[i][1]][0]);
            glTexCoord2f(z_end,0);
            glVertex3fv(&v_cube[c_ind[i][2]][0]);
            glTexCoord2f(z_end,y_end);
            glVertex3fv(&v_cube[c_ind[i][3]][0]);
        }
    }
    glEnd();

//    set_mat_prop(0,0,0,em,shine);
//    glBegin(GL_LINES);
//    glColor3f(0,0,0);
//    for (GLint i = 0; i <12; i++)
//    {
//        for (GLint j=0; j<2; j++)
//        {
//            glVertex3fv(&v_cube[c_edg[i][j]][0]);
//        }
//    }
//    glEnd();
}

void cylinder(float rad_t, float rad_b, float height, int granularity,
              float colR, float colG, float colB,
              bool em, float shine, bool tb_surf, bool tb_texture,
              float sp_stack_th, int sp_num_stack, float alpha)
{
    if(granularity<3)
        return;

    set_mat_prop(colR,colG,colB,em,shine, alpha);

    float step = 360.0/granularity;

    int bottom=0, top=1;
    float points[2*granularity][3];
    int tb_surfaces[2][granularity];
    int side_surfaces[granularity][4];
    float tex_cord_tb[granularity][2];

    float i=0.0;
    for(int j=0; j<granularity; j++, i+=step)
    {
        points[bottom*granularity+j][0] = rad_b*cos(i * PI / 180);
        points[bottom*granularity+j][1] = bottom*height;
        points[bottom*granularity+j][2] = -rad_b*sin(i * PI / 180);

        points[top*granularity+j][0] = rad_t*cos(i * PI / 180);
        points[top*granularity+j][1] = top*height;
        points[top*granularity+j][2] = -rad_t*sin(i * PI / 180);

        tex_cord_tb[j][0]=0.5+0.5*cos(i * PI / 180);
        tex_cord_tb[j][1]=0.5+0.5*sin(i * PI / 180);
    }

    // Assigning Surfaces
    for(int i=0; i<2; i++)
    {
        if(i==top)
            for(int j=0; j<granularity; j++)
                tb_surfaces[i][j]=granularity+j;
        else
            for(int j=0; j<granularity; j++)
                tb_surfaces[i][j]=granularity-1-j;
    }

    for(int i=0; i<granularity-1; i++)
    {
        side_surfaces[i][0]=granularity+i;
        side_surfaces[i][1]=i;
        side_surfaces[i][2]=i+1;
        side_surfaces[i][3]=granularity+i+1;
    }

    side_surfaces[granularity-1][0]=2*granularity-1;
    side_surfaces[granularity-1][1]=granularity-1;
    side_surfaces[granularity-1][2]=0;
    side_surfaces[granularity-1][3]=granularity;

    // Drawing top-bottom surfaces
    if(tb_surf)
    {
        for(int i=0; i<2; i++)
        {
            glBegin(GL_POLYGON);
            getNormal3p(points[tb_surfaces[i][0]][0], points[tb_surfaces[i][0]][1], points[tb_surfaces[i][0]][2],
                        points[tb_surfaces[i][1]][0], points[tb_surfaces[i][1]][1], points[tb_surfaces[i][1]][2],
                        points[tb_surfaces[i][2]][0], points[tb_surfaces[i][2]][1], points[tb_surfaces[i][2]][2]);

            for(int j=0; j<granularity; j++)
            {
                if(tb_texture) glTexCoord2f(tex_cord_tb[j][0],tex_cord_tb[j][1]);
                else glTexCoord2f(0,0);
                glVertex3fv(points[tb_surfaces[i][j]]);
            }
            glEnd();
        }
    }

    float tex_portion=1.0/granularity;
    float tex_ud_portion=1.0/sp_num_stack;

    // Drawing side surfaces
    glBegin(GL_QUADS);
    for(int i=0; i<granularity; i++)
    {

        if((points[side_surfaces[i][1]][0]==points[side_surfaces[i][2]][0]) &&
           (points[side_surfaces[i][1]][1]==points[side_surfaces[i][2]][1]) &&
           (points[side_surfaces[i][1]][2]==points[side_surfaces[i][2]][2]))
        {
           getNormal3p(points[side_surfaces[i][0]][0], points[side_surfaces[i][0]][1], points[side_surfaces[i][0]][2],
                    points[side_surfaces[i][1]][0], points[side_surfaces[i][1]][1], points[side_surfaces[i][1]][2],
                    points[side_surfaces[i][3]][0], points[side_surfaces[i][3]][1], points[side_surfaces[i][3]][2]);

        }
        else
        {
           getNormal3p(points[side_surfaces[i][0]][0], points[side_surfaces[i][0]][1], points[side_surfaces[i][0]][2],
                    points[side_surfaces[i][1]][0], points[side_surfaces[i][1]][1], points[side_surfaces[i][1]][2],
                    points[side_surfaces[i][2]][0], points[side_surfaces[i][2]][1], points[side_surfaces[i][2]][2]);
        }

        glTexCoord2f(i*tex_portion, sp_stack_th*tex_ud_portion);
        glVertex3fv(points[side_surfaces[i][0]]);
        glTexCoord2f(i*tex_portion, (sp_stack_th-1)*tex_ud_portion);
        glVertex3fv(points[side_surfaces[i][1]]);
        glTexCoord2f((i+1)*tex_portion, (sp_stack_th-1)*tex_ud_portion);
        glVertex3fv(points[side_surfaces[i][2]]);
        glTexCoord2f((i+1)*tex_portion, sp_stack_th*tex_ud_portion);
        glVertex3fv(points[side_surfaces[i][3]]);

    }

    glEnd();

}

void sphere(float sp_rad, int granularity, int stack_num, float colR, float colG, float colB, float em, float shine, float frac, float alpha)
{
    float stack_height=(sp_rad*2*frac) / stack_num;
    float stack_rads[stack_num];

    float d=sp_rad;
    for(int i=0; i<stack_num+1; i++)
    {
        stack_rads[i] = sqrt(sp_rad*sp_rad - d*d);
        d-=stack_height;
    }

    glPushMatrix();

    for(int i=0; i<stack_num; i++)
    {
        glPushMatrix();
        glTranslatef(0,(stack_num-1-i)*stack_height,0);
        cylinder(stack_rads[i],stack_rads[i+1],stack_height,granularity, colR,colG,colB, em,shine,false,false, stack_num-i,stack_num, alpha);
        glPopMatrix();
    }

    glPopMatrix();
}

void glut_sphere(float rad, int slices, int stacks,
            float colR, float colG, float colB, bool em, float shine, float alpha)
{
    set_mat_prop(colR,colG,colB,em,shine, alpha);
    glutSolidSphere(rad, slices, stacks);
}

void light(GLenum light_num, float posX, float posY, float posZ,
           bool spot, float spot_cut, float amb, float homogeneous, float intensity)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {amb*intensity, amb*intensity, amb*intensity, 1.0};
    GLfloat light_diffuse[]  = {1.0*intensity, 1.0*intensity, 1.0*intensity, 1.0};
    GLfloat light_specular[] = {1.0*intensity, 1.0*intensity, 1.0*intensity, 1.0};
    GLfloat light_position[] = {posX, posY, posZ, homogeneous};
    GLfloat light_attenuation[] = {light_const_att,light_1st_att,cur_light_2nd_att};


    glEnable(light_num);

    glLightfv(light_num, GL_AMBIENT, light_ambient);
    glLightfv(light_num, GL_DIFFUSE, light_diffuse);
    glLightfv(light_num, GL_SPECULAR, light_specular);

    glLightf(light_num, GL_CONSTANT_ATTENUATION, light_attenuation[0]);
    glLightf(light_num, GL_LINEAR_ATTENUATION, light_attenuation[1]);
    glLightf(light_num, GL_QUADRATIC_ATTENUATION, light_attenuation[2]);

    glLightfv(light_num, GL_POSITION, light_position);

    if(spot)
    {
        GLfloat spot_direction[] = { -0.1, -1.0, 0.0 };
        glLightfv(light_num, GL_SPOT_DIRECTION, spot_direction);
        glLightf( light_num, GL_SPOT_CUTOFF, spot_cut);
    }
}

void light_off(GLenum light_num)
{
    glDisable(light_num);
}

GLubyte* make_bw_tiles_texture(int tile_width, int tile_height, int tex_width, int tex_height)
{
    GLubyte* bw_tiles = new GLubyte[tex_width*tex_height*3];

    for(int i=0; i<tex_height; i++)
    {
        for(int j=0; j<tex_width; j++)
        {
            int c=(i/tile_width + j/tile_width) % 2;

            if(c==0)
            {
                bw_tiles[(i*tex_width+j)*3+0]=255;
                bw_tiles[(i*tex_width+j)*3+1]=255;
                bw_tiles[(i*tex_width+j)*3+2]=255;
            }
            else
            {
                bw_tiles[(i*tex_width+j)*3+0]=0;
                bw_tiles[(i*tex_width+j)*3+1]=0;
                bw_tiles[(i*tex_width+j)*3+2]=0;
            }
        }
    }

    return bw_tiles;
}

void LoadTexture(const char*filename, GLuint index)
{
    glBindTexture(GL_TEXTURE_2D, texture_ids[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, texture_ids[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void LoadTexture_from_arr(GLubyte* arr,int width, int height, GLuint index)
{
    glBindTexture(GL_TEXTURE_2D, texture_ids[index]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, texture_ids[index]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,arr);
}

void mirrored_surface(float width, float height, float length, float alpha, float posX, float posY, float posZ, bool has_tex, texture_id tex)
{
    // configuring reflected scene
    glClear(GL_DEPTH_BUFFER_BIT);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 1);
    glEnable(GL_STENCIL_TEST);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_ids[tex]);

    glPushMatrix();
    glTranslatef(posX,posY,posZ);
    glScalef(width, height, length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.5,0.5,0.5, false,128, 1,1,1,1, alpha);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // reconfigure stencil tests
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glDisable(GL_STENCIL_TEST);
}

