// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GLUT/glut.h>

#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

float theta_x = 15, theta_y = -9, theta_z = 20;
float theta_x1 = 15, theta_y1 = -9, theta_z1 = 20;

GLuint vPosition;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVertices];
extern vec4 colors[NumVertices];

const int segmentsCylinder = 64;
const int NumVerticesCylinder = segmentsCylinder*6 + segmentsCylinder*3*2;

extern vec4 points_cylinder[NumVerticesCylinder];

//----------------------------------------------------------------------------
//CUBES
point4 points[NumVertices];
color4 colors[NumVertices];
size_t CUBE_OFFSET;

//CYLINDERS
vec4 vertices_cylinder[4];
const float bottom = -.5;
const float top = .5;
size_t CYLINDER_OFFSET;

vec4 points_cylinder[NumVerticesCylinder];
color4 colors_cylinder[NumVerticesCylinder];
color4 vertex_colors_cylinder[NumVerticesCylinder];
//int Index1 = 0;

// SPHERES
const int segmentsSphere = 512;
//const int NumVerticesSphere = segmentsSphere*6 + segmentsSphere*3*2;
const int NumVerticesSphere = segmentsSphere*3*2;
vec4 vertices_sphere[4];
vec4 points_sphere[NumVerticesSphere];
vec3 normals_sphere[NumVerticesSphere];
color4 colors_sphere[NumVerticesSphere];
color4 vertex_colors_sphere[NumVerticesSphere];
int Index = 0;
size_t SPHERE_OFFSET;
//----------------------------------------------------------------------------

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA colors
color4 vertex_colors[8] = {
    color4( 0.1, 0.1, 0.1, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 0.9, 0.9, 0.9, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};



void
quad_sphere(int a, int b, int c, int d, float t0, float t1, float p0, float p1)
{
    /*
     colors_cylinder[Index1] = vertices_cylinder[a]; Index1++;
     colors_cylinder[Index1] = vertices_cylinder[b]; Index1++;
     colors_cylinder[Index1] = vertices_cylinder[c]; Index1++;
     colors_cylinder[Index1] = vertices_cylinder[a]; Index1++;
     colors_cylinder[Index1] = vertices_cylinder[c]; Index1++;
     colors_cylinder[Index1] = vertices_cylinder[d]; Index1++;
     */
    /*
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[a]; Index++;
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[b]; Index++;
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[c]; Index++;
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[a]; Index++;
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[c]; Index++;
     colors_sphere[Index] = vertex_colors_sphere[a]; points_sphere[Index] = vertices_sphere[d]; Index++;
     */
    
}

void colorbube()
{
    float r = 0.5;
    Index = 0;
    
    float ph_top = ((float)((segmentsSphere/2)-1)/(float)(segmentsSphere/2)) * M_PI/2.0;
    float ph_bottom = -ph_top;
    
    for ( int n = 0; n < segmentsSphere; n++ )
    {
        GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsSphere;
        GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsSphere;
        
        points_sphere[Index].x = 0.0;
        points_sphere[Index].y = top;
        points_sphere[Index].z = 0.0;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
        
        points_sphere[Index].x = cos(ph_top) * cos(t0) * r;
        points_sphere[Index].y = sin(ph_top) * r;
        points_sphere[Index].z = cos(ph_top) * sin(t0) * r;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
        points_sphere[Index].x = cos(ph_top) * cos(t1) * r;
        points_sphere[Index].y = sin(ph_top) * r;
        points_sphere[Index].z = cos(ph_top) * sin(t1) * r;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
    }
    
    for (int m = 1; m < segmentsSphere-1; m++)
    {
        float p0 = M_PI/2.0 - m * M_PI/(float)segmentsSphere;
        float p1 = M_PI/2.0 - (m+1) * M_PI/(float)segmentsSphere;
        
        int num;
        for ( int n = 0; n < segmentsSphere; n++ )
        {
            num = 0;
            float x = 0.0, y = 0.0, r = 0.5;
            
            GLfloat const t0 = M_PI/2.0 * (float)n / (float)segmentsSphere;
            GLfloat const t1 = M_PI/2.0 * (float)(n+1) / (float)segmentsSphere;
            
            //quad vertex 0
            vertices_sphere[num].x = cos(p1) * cos(t0) * r;
            vertices_sphere[num].y = sin(p1) * r;
            vertices_sphere[num].z = cos(p1) * sin(t0) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 1
            vertices_sphere[num].x = cos(p1) * cos(t1) * r;
            vertices_sphere[num].y = sin(p1) * r;
            vertices_sphere[num].z = cos(p1) * sin(t1) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 2
            vertices_sphere[num].x = cos(p0) * cos(t1) * r;
            vertices_sphere[num].y = sin(p0) * r;
            vertices_sphere[num].z = cos(p0) * sin(t1) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 3
            vertices_sphere[num].x = cos(p0) * cos(t0) * r;
            vertices_sphere[num].y = sin(p0) * r;
            vertices_sphere[num].z = cos(p0) * sin(t0) * r;
            vertices_sphere[num++].w = 1.0;
            
            quad_sphere( 0, 1, 2, 3, t0, t1, p0, p1 );
        }
    }
    
    
    for ( int n = 0; n < segmentsSphere; n++ )
    {
        GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsSphere;
        GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsSphere;
        
        points_sphere[Index].x = 0.0;
        points_sphere[Index].y = bottom;
        points_sphere[Index].z = 0.0;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
        
        points_sphere[Index].x = cos(ph_bottom) * cos(t0) * r;
        points_sphere[Index].y = sin(ph_bottom) * r;
        points_sphere[Index].z = cos(ph_bottom) * sin(t0) * r;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
        
        points_sphere[Index].x = cos(ph_bottom) * cos(t1) * r;
        points_sphere[Index].y = sin(ph_bottom) * r;
        points_sphere[Index].z = cos(ph_bottom) * sin(t1) * r;
        points_sphere[Index].w = 1.0;
        
        normals_sphere[Index] = vec3(points_sphere[Index].x, points_sphere[Index].y, points_sphere[Index].z);
        
        Index++;
    }
    
}


//----------------------------------------------------------------------------


GLuint program;

void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    //colorcube();
    //colortube();
    colorbube();
    //---------------------------------------------------------------------
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    CYLINDER_OFFSET = sizeof(points) + sizeof(colors);
    SPHERE_OFFSET = sizeof(points_sphere) + CYLINDER_OFFSET;
    
    // Create a vertex array object
    GLuint vao;
    //glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    //glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(points_cylinder) + sizeof(points_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    
    
    //---------------------------------------------------------------------
    
    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.6, 0.6, 0.6, 0.0 );
}



void
display( void )
{
    // SPHERE
    mat4 rotSphere = ( RotateX( theta_x ) * RotateY( theta_y ) * RotateZ( theta_z ) );
    mat4 transformSphere = Translate( -0.5, 0.0, 0.0 ) * rotSphere * Scale(0.25, 1.0, 0.25);
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transformSphere );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    //glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(1.0, 0.0, 0.0, 0.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    glutSwapBuffers();
    
    
}


//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------


void
reshape( int w, int h )
{
    glViewport(0,0,w,h);
}

void idle( void )
{
    theta_x = fmod(theta_x+.5, 360);
    //theta_y = fmod(theta_y+1, 360.0);
    //theta_z = fmod(theta_z-2, 360); //fmod(theta_z+2, 180);
    
    //theta_x1 = fmod(theta_x-3, 360);
    //theta_y1 = fmod(theta_y-1, 360.0);
    //theta_z1 = fmod(theta_z-5, 360);
    
    glutPostRedisplay();
}


//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Model" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}
