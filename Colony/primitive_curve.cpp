#include "primitive_curve.h"

const int dgre=3, max_num_cur_pts=100;
int num_cur_points=0;
GLfloat cur_points[max_num_cur_pts][3];

// objects
float player_coords[max_num_cur_pts][3], basin_coords[max_num_cur_pts][3], basin_pipe_coords[max_num_cur_pts][3], basin_tap_coords[max_num_cur_pts][3];
float st_tp_coords[max_num_cur_pts][3], st_lt_coords[max_num_cur_pts][3], player_hand_coords[max_num_cur_pts][3];
float player_hair_coords[max_num_cur_pts][3];
int num_player_coords, num_basin_coords, num_basin_pipe_coords, num_basin_tap_coords, num_st_tp_coords, num_st_lt_coords, num_player_hand_coords;
int num_player_hair_coords;

float wcsClkDn[3],wcsClkUp[3];
class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];

GLint viewport[4]; //var to hold the viewport info
GLdouble modelview[16]; //var to hold the modelview info
GLdouble projection[16]; //var to hold the projection matrix info

void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3, const int multiplier=-1)
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

    glNormal3f(multiplier*Nx,multiplier*Ny,multiplier*Nz);
}

void scsToWcs(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;
}

//control points
long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t,  float xy[2], float ctrlpoints[][3], int l=num_player_coords-1)
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=l; i++)
    {
        int ncr=nCr(l,i);
        double oneMinusTpow=pow(1-t,double(l-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

void draw_curve_fill(float points[][3], int num_points, int nt=40, int ntheta=10, float frac_t=1, float frac_theta=1)
{
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    //number of angular slices
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve(t,  xy, points, num_points-1);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for (int i = 0; i < nt*frac_t; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy, points, num_points-1);
        x1 = xy[0];
        r1 = xy[1];

        float multiplier=-1;
        if(x1<x) multiplier=1;

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for(int j=0; j<=ntheta*frac_theta; j++)
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z, multiplier);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;
            }
            glVertex3f (x1, y1, z1);

        }
        //forms quad with next pair of points with incremented theta value

        glEnd();
        x = x1;
        r = r1;
    }
}

void draw_curve_pipe_fill(float points[][3], int num_points, float rad=0.1, int nt=40, int ntheta=10)
{
    float x, x_, y, y_, z;				//current coordinates
    float x1, x1_, y1, y1_, z1;			//next coordinates
    float theta;

    int multiplier=-1;

    //number of angular slices
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve(t,  xy, points, num_points-1);
    x_ = xy[0];
    y_ = xy[1];

//    cout<<"\n\nStart:"<<endl;
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for (int i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy, points, num_points-1);
        x1_ = xy[0];
        y1_ = xy[1];

        // tilt angle
        float alpha=0, alpha1=0;
        if(x1_-x_ == 0) alpha=PI/2;
        else alpha = atan((y1_-y_)/(x1_-x_));
        alpha =  alpha + (PI/2);
        alpha1 = alpha;

        // to keep it cool
//        if(i>0)
//        {
//            BezierCurve( t-2*dt,  xy, points, num_points-1);
//            float prev_x = xy[0], prev_y = xy[1];
//
//            float prev_alpha=0;
//            if(x_-prev_x == 0) prev_alpha=PI/2;
//            else prev_alpha = atan((y_-prev_y)/(x_-prev_x));
//            prev_alpha =  prev_alpha + (PI/2);
//
//            // update
////            alpha = (alpha+prev_alpha)/2;
//            alpha = prev_alpha;
//        }
        if(i<nt-1)
        {
            BezierCurve( t+dt,  xy, points, num_points-1);
            float next_x = xy[0], next_y = xy[1];

            float next_alpha=0;
            if(next_x-x1_ == 0) next_alpha=PI/2;
            else next_alpha = atan((next_y-y1_)/(next_x-x1_));
            next_alpha =  next_alpha + (PI/2);

            // update
//            alpha1 = (alpha1+next_alpha)/2;
            alpha1 = next_alpha;
            if((next_x-x1_)*(x1_-x_) < 0) alpha1 += PI;
        }

        // calc the current and next in_x and in_y
        float in_x = rad*cos(alpha), in_y=rad*sin(alpha), in_x1 = rad*cos(alpha1), in_y1=rad*sin(alpha1);

//        cout<<alpha*180/PI<<" "<<alpha1*180/PI<<endl;

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for(int j=0; j<=ntheta; j++)
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );

            x = in_x * cosa + x_;
            x1 = in_x1 * cosa + x1_;
//            x=x_;
//            x1=x1_;

            y = in_y * cosa + y_;
            y1 = in_y1 * cosa + y1_;	//current and next y
            z = rad * sina;
            z1 = rad * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                if(x>x1) multiplier=1;
                else multiplier=-1;
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z, multiplier);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;
            }
            glVertex3f (x1, y1, z1);

        }
        //forms quad with next pair of points with incremented theta value

        glEnd();
        x_ = x1_;
        y_ = y1_;
    }
}

void draw_cur_selected_points()
{
    glPointSize(5.0);
    set_mat_prop(1,1,1);
    glBegin(GL_POINTS);
    for (int i = 0; i <num_cur_points; i++)
        glVertex3fv(&cur_points[i][0]);
    glEnd();

    if(num_cur_points<5) return;

//    draw_curve_pipe_fill(cur_points, num_cur_points, 0.2, 40);
    draw_curve_fill(cur_points, num_cur_points, 40, 10);
}

void draw_player()
{
    draw_curve_fill(player_coords, num_player_coords);
//    cout<<player_coords[2][0]<<endl;
}

void draw_player_hair(float frac)
{
    draw_curve_fill(player_hair_coords, num_player_hair_coords, 10,10, 1,frac);
}

void draw_player_hand()
{
    draw_curve_pipe_fill(player_hand_coords, num_player_hand_coords,0.2);
}

void draw_basin()
{
    draw_curve_fill(basin_coords, num_basin_coords, 30, 20);
}

void draw_basin_pipe()
{
    draw_curve_pipe_fill(basin_pipe_coords, num_basin_pipe_coords,0.3);
}

void draw_basin_tap()
{
    draw_curve_pipe_fill(basin_tap_coords, num_basin_tap_coords,0.1);
}

void draw_street_top(float rad)
{
    draw_curve_pipe_fill(st_tp_coords, num_st_tp_coords, rad,10,10);
}

void draw_street_light()
{
    draw_curve_fill(st_lt_coords, num_st_lt_coords, 20, 20);
}

void save_cur_coords()
{
    ofstream myfile;

    char filename[100];

    cout<<"\n\nEnter the filename : ";
    cin>>filename;

//    if(filename=="") return;

    myfile.open(filename);

    ostringstream os;
    os<<num_cur_points<<"\n";
    for(int i=0; i<num_cur_points; i++)
    {
        for(int j=0; j<3; j++)
        {
            os<<cur_points[i][j]<<" ";
        }
    }

//    cout<<endl<<endl<<"String: "<<os.str()<<endl;
    myfile<<os.str();
    myfile.close();

    cout<<"\n\nCoordinates saved successfully in the file : "<<filename<<endl;
}

void load_cur_coords(float arr[][3], char filename[], int* len)
{
    ifstream myfile;
    myfile.open(filename);
    string content( (istreambuf_iterator<char>(myfile) ),
                       (istreambuf_iterator<char>()) );

    istringstream os(content);

    os>>(*len);
    for(int i=0; i<(*len); i++)
    {
        for(int j=0; j<3; j++)
        {
            os>>arr[i][j];
        }
    }
//    cout<<os.str();

    myfile.close();
}

void load_coords()
{
    load_cur_coords(player_coords, "player.txt", &num_player_coords);
    load_cur_coords(basin_coords, "basin.txt", &num_basin_coords);
    load_cur_coords(basin_pipe_coords, "basin_pipe.txt", &num_basin_pipe_coords);
    load_cur_coords(basin_tap_coords, "basin_tap.txt", &num_basin_tap_coords);
    load_cur_coords(st_tp_coords, "street_lamp_top.txt", &num_st_tp_coords);
    load_cur_coords(st_lt_coords, "street_lamp_light.txt", &num_st_lt_coords);
    load_cur_coords(player_hand_coords, "player_hand.txt", &num_player_hand_coords);
    load_cur_coords(player_hair_coords, "player_hair.txt", &num_player_hair_coords);
}

void mouse_click(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        clkpt[0].x=x;
        clkpt[0].y=y;
        scsToWcs(clkpt[0].x,clkpt[0].y,wcsClkDn);
        cout<<"\nD: "<<x<<" "<<y<<" wcs: "<<wcsClkDn[0]<<" "<<wcsClkDn[1];

        // Add current selected points
        cur_points[num_cur_points][0]=wcsClkDn[0];
        cur_points[num_cur_points][1]=wcsClkDn[1];
        cur_points[num_cur_points][2]=wcsClkDn[2];//-3;
        num_cur_points++;
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        clkpt[1].x=x;
        clkpt[1].y=y;
        scsToWcs(clkpt[1].x,clkpt[1].y,wcsClkUp);
//        cout<<"\nU: "<<x<<" "<<y<<" wcs: "<<wcsClkUp[0]<<" "<<wcsClkUp[1];
    }
}

