//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
using namespace std;

#include <GLUT/glut.h>

#include "Angel.h"
#include "simplemodels.h"


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = { 80.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 30.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };


size_t CUBE_OFFSET;
size_t COLOR_OFFSET;
size_t CUBE_NORMALS_OFFSET;
size_t CYLINDER_OFFSET;
size_t CYLINDER_NORMALS_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE_NORMALS_OFFSET;

GLuint program;
GLuint vPosition;
GLuint vNormal;
GLuint vColor;

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

mat4 current_view_matrix;

float tr_y;
float tr_z;
float move_y;
float move_z;
float move_x;
float tilt_x;


vec4 eye = vec4( 0.5, 0.5, 3.0, 1.0);
vec4 up = vec4(0.0, 0.5, 0.0, 1.0);
vec4 at = vec4(0.0, 0.0, 1.0, 1.0);


const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

const int segments = 8;
//----------------------------------------------------------------------------
point4 points[NumVertices];
color4 colors[NumVertices];

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

int Index = 0;
void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}


void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}


vec4 points_cylinder[NumVerticesCylinder];
vec4 vertices_cylinder[4];

// quad generates two triangles for each face and assigns colors to the vertices
void
quad_cylinder( int a, int b, int c, int d )
{
    points_cylinder[Index] = vertices_cylinder[a];
    Index++;
    points_cylinder[Index] = vertices_cylinder[b];
    Index++;
    points_cylinder[Index] = vertices_cylinder[c];
    Index++;
    points_cylinder[Index] = vertices_cylinder[a];
    Index++;
    points_cylinder[Index] = vertices_cylinder[c];
    Index++;
    points_cylinder[Index] = vertices_cylinder[d];
    Index++;
}


float const bottom = -0.5;
float const top    = 0.5;

void
colortube(void)
{
    float r = 0.5;
    Index = 0;
    
    for ( int n = 0; n < segmentsCylinder; n++ )
    {
        GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsCylinder;
        GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsCylinder;
        
        points_cylinder[Index].x = 0.0;
        points_cylinder[Index].y = top;
        points_cylinder[Index].z = 0.0;
        points_cylinder[Index].w = 1.0;
        Index++;
        points_cylinder[Index].x = cos(t0) * r;
        points_cylinder[Index].y = top;
        points_cylinder[Index].z = sin(t0) * r;
        points_cylinder[Index].w = 1.0;
        Index++;
        points_cylinder[Index].x = cos(t1) * r;
        points_cylinder[Index].y = top;
        points_cylinder[Index].z = sin(t1) * r;
        points_cylinder[Index].w = 1.0;
        Index++;
    }
    
    
    int num;
    for ( int n = 0; n < segmentsCylinder; n++ )
    {
        num = 0;
        float x = 0.0, y = 0.0, r = 0.5;
        
        GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsCylinder;
        GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsCylinder;
        
        //quad vertex 0
        vertices_cylinder[num].x = cos(t0) * r;
        vertices_cylinder[num].y = bottom;
        vertices_cylinder[num].z = sin(t0) * r;
        vertices_cylinder[num++].w = 1.0;
        //quad vertex 1
        vertices_cylinder[num].x = cos(t1) * r;
        vertices_cylinder[num].y = bottom;
        vertices_cylinder[num].z = sin(t1) * r;
        vertices_cylinder[num++].w = 1.0;
        //quad vertex 2
        vertices_cylinder[num].x = cos(t1) * r;
        vertices_cylinder[num].y = top;
        vertices_cylinder[num].z = sin(t1) * r;
        vertices_cylinder[num++].w = 1.0;
        //quad vertex 3
        vertices_cylinder[num].x = cos(t0) * r;
        vertices_cylinder[num].y = top;
        vertices_cylinder[num].z = sin(t0) * r;
        vertices_cylinder[num++].w = 1.0;
        
        quad_cylinder( 0, 1, 2, 3 );
    }
    
    
    for ( int n = 0; n < segmentsCylinder; n++ )
    {
        GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsCylinder;
        GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsCylinder;
        
        points_cylinder[Index].x = 0.0;
        points_cylinder[Index].y = bottom;
        points_cylinder[Index].z = 0.0;
        points_cylinder[Index].w = 1.0;
        Index++;
        points_cylinder[Index].x = cos(t1) * r;
        points_cylinder[Index].y = bottom;
        points_cylinder[Index].z = sin(t1) * r;
        points_cylinder[Index].w = 1.0;
        Index++;
        points_cylinder[Index].x = cos(t0) * r;
        points_cylinder[Index].y = bottom;
        points_cylinder[Index].z = sin(t0) * r;
        points_cylinder[Index].w = 1.0;
        Index++;
    }
    
}


//---- sphere model
//----------------------------------------------------------------------------
vec4 points_sphere[NumVerticesSphere];
vec4 vertices_sphere[4];
vec3 normals_sphere[NumVerticesSphere];

// quad generates two triangles for each face and assigns colors to the vertices
void
quad_sphere( int a, int b, int c, int d, float t0, float t1, float p0, float p1 )
{
    points_sphere[Index] = vertices_sphere[a];
    normals_sphere[Index] = vec3(vertices_sphere[a].x, vertices_sphere[a].y, vertices_sphere[a].z);
    Index++;
    
    points_sphere[Index] = vertices_sphere[b];
    normals_sphere[Index] = vec3(vertices_sphere[b].x, vertices_sphere[b].y, vertices_sphere[b].z);
    Index++;
    
    points_sphere[Index] = vertices_sphere[c];
    normals_sphere[Index] = vec3(vertices_sphere[c].x, vertices_sphere[c].y, vertices_sphere[c].z);
    Index++;
    
    points_sphere[Index] = vertices_sphere[a];
    normals_sphere[Index] = vec3(vertices_sphere[a].x, vertices_sphere[a].y, vertices_sphere[a].z);
    Index++;
    
    points_sphere[Index] = vertices_sphere[c];
    normals_sphere[Index] = vec3(vertices_sphere[c].x, vertices_sphere[c].y, vertices_sphere[c].z);
    Index++;
    
    points_sphere[Index] = vertices_sphere[d];
    normals_sphere[Index] = vec3(vertices_sphere[d].x, vertices_sphere[d].y, vertices_sphere[d].z);
    Index++;
}


void
colorbube(void)
{
    float r = 0.5;
    Index = 0;
    float ph_top = ((float)((segmentsSphere/2)-1)/(float)(segmentsSphere/2)) * M_PI/2.0;
    float ph_bottom = -ph_top;
    
    //cout << ph_bottom << endl;
    
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
    
    for( int m = 1; m < segmentsSphere-1; m++ )
    {
        float p0 = M_PI/2.0 - m * M_PI/(float)segmentsSphere;
        float p1 = M_PI/2.0 - (m+1) * M_PI/(float)segmentsSphere;
        
        int num;
        for ( int n = 0; n < segmentsSphere; n++ )
        {
            num = 0;
            float x = 0.0, y = 0.0, r = 0.5;
            
            GLfloat const t0 = 2 * M_PI * (float)n / (float)segmentsSphere;
            GLfloat const t1 = 2 * M_PI * (float)(n+1) / (float)segmentsSphere;
            
            //quad vertex 0
            vertices_sphere[num].x = cos(p1)*cos(t0) * r;
            vertices_sphere[num].y = sin(p1) * r;
            vertices_sphere[num].z = cos(p1)*sin(t0) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 1
            vertices_sphere[num].x = cos(p1)*cos(t1) * r;
            vertices_sphere[num].y = sin(p1) * r;
            vertices_sphere[num].z = cos(p1)*sin(t1) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 2
            vertices_sphere[num].x = cos(p0)*cos(t1) * r;
            vertices_sphere[num].y = sin(p0) * r;
            vertices_sphere[num].z = cos(p0)*sin(t1) * r;
            vertices_sphere[num++].w = 1.0;
            //quad vertex 3
            vertices_sphere[num].x = cos(p0)*cos(t0) * r;
            vertices_sphere[num].y = sin(p0) * r;
            vertices_sphere[num].z = cos(p0)*sin(t0) * r;
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










void
init()
{
    // Load shaders and use the resulting shader program
    program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );
    
    //---------------------------------------------------------------------
    colorcube();
    colortube();
    colorbube();
    //---------------------------------------------------------------------
    
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    CYLINDER_OFFSET = sizeof(points) + sizeof(colors);
    CYLINDER_NORMALS_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    SPHERE_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_NORMALS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    //glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    //glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    //glBufferSubData( GL_ARRAY_BUFFER, SPHERE_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    
    //---------------------------------------------------------------------
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)) );
    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.75, 0.75, 0.75, 1.0 );
    //glClearColor(0.1, 0.1, 0.15, 1.0);
    current_view_matrix = LookAt(eye, at, up);
}

//----------------------------------------------------------------------------

mat4 proj_matrix;

mat4 view_matrix;

float r = 3.5;

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    
    //---- lights
    
    vec4 lpos = vec4( 3.0, 3.0, 3.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    vec4 la(0.2, 0.2, 0.2, 1.0);
    vec4 ld(0.7, 0.7, 0.7, 1.0);
    vec4 ls(0.7, 0.7, 0.7, 1.0);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
    
    vec4 ka(0.9, 0.1, 0.1, 1.0);
    vec4 kd(1.0, 0.0, 0.0, 1.0);
    vec4 ks(1.0, 0.4, 0.4, 1.0);
    float s = 5.0;
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    
    
    //---- camera intrinsic parameters
    
    float left = -0.5;
    float right = 0.5;
    float bottom = -0.5;
    float top = 0.5;
    float zNear = 0.5;
    float zFar = 15.0;
    
    mat4 zt = Translate(0.0, 0.0, (zFar + zNear)/2.0);
    
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    //proj_matrix = Scale(2.0/(right-left), 2.0/(top-bottom), 2.0/(zNear-zFar)) * zt;
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );

    
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    float tr_y22 = Theta3[Yaxis]* M_PI/180.0;
    float tr_z22 = Theta3[Zaxis]* M_PI/180.0;
    
    //vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    //cout << up << ' ' << normalize(up) << endl;
    
    //spin or roll?
    //current_view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(move_x, move_y, 0, move_z), vec4(tilt_x, cos(tr_z), 0.0, 0.0));
    
    //strafe
    //current_view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(move_x, move_y, 0, move_z), vec4(tilt_x, 100, 0.0, 0.0));
    
    
    //glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, view_matrix );
    
    
    //current_view_matrix = LookAt(eye, at, up);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, current_view_matrix );

    
    //---- cylinder 1
    
    mat4  rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    mat4 transform = Translate( 0.5, 0.2, 0.0 ) * rot1 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.9, 0.2, 0.2, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    //---- cylinder 2
    
    rot1 = ( RotateX( Theta1[Xaxis] ) * RotateY( Theta1[Yaxis] ) * RotateZ( Theta1[Zaxis] ) );
    transform = Translate( 1.5, 0.2, 0.0 ) * rot1 * Scale(1.0, 0.5, 1.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.9, 0.9, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    //---- cube 1
    
    mat4 rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -0.5, 0.0, 0.0 ) * rot2 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    //---- cube 2
    
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -1.5, 0.0, 0.0 ) * rot2 * Scale(0.25, 1.0, 0.25);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    //---- sphere 1
    mat4  rot5 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    mat4 transform5 = Translate( 1.5, 1.5, 0.0 ) * rot5 * Scale(1.0, 1.0, 1.0);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform5 );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    
    glutSwapBuffers();
}


//----------------------------------------------------------------------------


void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033:    // Escape key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        case '1':
            Theta3[Yaxis] = 0;
            Theta3[Zaxis] = 0;
            Theta3[Xaxis] = 0;
            //r = 4.5;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case '2':
            Theta3[Yaxis] = 90;
            Theta3[Zaxis] = 0;
            //r = 7;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;

        case 'h':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'f':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 't':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'g':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            
        case 'r':    //---- increase radius // walk backward
            r += .1;
            glutPostRedisplay();
            break;
        case 'R':    //---- decrease radius // walk forward
            r -= .1;
            glutPostRedisplay();
            break;
        case 'n':
            Theta3[Yaxis] -= 5;
            Theta3[Zaxis] -= 5;
            r -= .1;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'm':
            Theta3[Yaxis] += 5;
            Theta3[Zaxis] += 5;
            r += .1;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'j':
            Theta3[Yaxis] += 5;
            Theta3[Zaxis] += 5;
            r -= .1;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'k':
            Theta3[Yaxis] -= 5;
            Theta3[Zaxis] -= 5;
            r += .1;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;

            
            
        case 'w':
            current_view_matrix = Translate(0.0, 0.0, 0.5) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 's':
            current_view_matrix = Translate(0.0, 0.0, -0.5) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 'S':
            current_view_matrix = Translate(0.0, 0.5, 0.0) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 'W':
            current_view_matrix = Translate(0.0, -0.5, 0.0) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 'A':
            current_view_matrix = Translate(0.5, 0.0, 0.0) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 'D':
            current_view_matrix = Translate(-0.5, 0.0, 0.0) * current_view_matrix;
            glutPostRedisplay();
            break;
        case 'd':
            current_view_matrix = RotateY(5)*current_view_matrix;
            glutPostRedisplay();
            break;
        case 'a':
            current_view_matrix = RotateY(-5)*current_view_matrix;
            glutPostRedisplay();
            break;
        case 'c':
            current_view_matrix = RotateX(5)*current_view_matrix;
            glutPostRedisplay();
            break;
        case 'e':
            current_view_matrix = RotateX(-5)*current_view_matrix;
            glutPostRedisplay();
            break;
        case 'x':
            current_view_matrix = RotateZ(-5)*current_view_matrix;
            glutPostRedisplay();
            break;
        case 'z':
            current_view_matrix = RotateZ(5)*current_view_matrix;
            glutPostRedisplay();
            break;
            
        //case 'w':
         //   view_matrix = polymo
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 0.0;
            move_x = 0;
            move_y = 0;
            move_z = 0;
            tilt_x = 0;
            r = 3.5;
            glutPostRedisplay();
            break;
    }
}




//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta1[Axis] = fmod(Theta1[Axis]+1, 360.0);
    Theta2[Axis] = fmod(Theta2[Axis]+2, 360.0);
    
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
reshape( int w, int h )
{
    glViewport(0,0,w,h);
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutCreateWindow( "Color Cube" );
    
    init();
    
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    //glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
}