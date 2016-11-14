// spherical camera + persp + lighting
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

GLuint loadBMP_custom(unsigned char** data, int* w, int* h, const char * imagepath);

// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Zaxis;
GLfloat  Theta1[NumAxes] = { 80.0, 0.0, 0.0 };
GLfloat  Theta2[NumAxes] = { 0.0, 30.0, 0.0 };
GLfloat  Theta3[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  Theta4[NumAxes] = { 0.0, 0.0, 0.0 };

//----------------------------------------------------------------------------

size_t CUBE_OFFSET;
size_t COLOR_OFFSET;
size_t CUBE_NORMALS_OFFSET;
size_t CUBE_TEXCOORDS_OFFSET;
size_t CYLINDER_OFFSET;
size_t CYLINDER_NORMALS_OFFSET;
size_t CYLINDER_TEXCOORDS_OFFSET;
size_t SPHERE_OFFSET;
size_t SPHERE_NORMALS_OFFSET;
size_t SPHERE_TEXCOORDS_OFFSET;
vec2 tex_coords_sphere[NumVerticesSphere];
vec2 tex_coords_cylinder[NumVerticesCylinder];
vec2 tex_coords_cube[NumVerticesCube];

GLuint program;

GLuint vPosition;
GLuint vNormal;
GLuint vColor;
GLuint vTexCoord;

GLuint textures[6];

mat4 current_view_matrix;

vec4 eye = vec4( 0.5, 0.5, 3.0, 1.0);
vec4 up = vec4(0.0, 0.5, 0.0, 1.0);
vec4 at = vec4(0.0, 0.0, 1.0, 1.0);

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
    
    
    
    //----- generate a checkerboard pattern for texture mapping
    const int  TextureSize  = 256;
    GLubyte checker_tex[TextureSize][TextureSize][3];
    
    for ( int i = 0; i < TextureSize; i++ )
    {
        for ( int j = 0; j < TextureSize; j++ )
        {
            //GLubyte c = (((i & 0x4) == 0) ^ ((j & 0x1)  == 0)) * 255;
            GLubyte c = (((j & 0x2)  == 0)) * 255;
            checker_tex[i][j][0]  = c;
            checker_tex[i][j][1]  = c;
            checker_tex[i][j][2]  = c;
        }
    }
    
    //---- Initialize texture objects
    glGenTextures( 4, textures );
    
    glActiveTexture( GL_TEXTURE0 );
    
    
    unsigned char* pic2 = NULL;
    int w,h;
    //loadBMP_custom(&pic2, &w, &h, "mars.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    // glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0, GL_BGR, GL_UNSIGNED_BYTE, checker_tex );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic2 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic1 = NULL;
    //   int w,h;
    //loadBMP_custom(&pic1, &w, &h, "earth.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic1 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    unsigned char* pic3 = NULL;
    //   int w,h;
    //loadBMP_custom(&pic3, &w, &h, "planet.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic3 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    unsigned char* pic4 = NULL;
    //   int w,h;
    //loadBMP_custom(&pic4, &w, &h, "coke.bmp");
    loadBMP_custom(&pic4, &w, &h, "building.jpeg");
    
    glBindTexture( GL_TEXTURE_2D, textures[3] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic4 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    unsigned char* pic5 = NULL;
    //int w,h;
    //loadBMP_custom(&pic5, &w, &h, "cilo.bmp");
    
    glBindTexture( GL_TEXTURE_2D, textures[4] );
    //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0, GL_BGR, GL_UNSIGNED_BYTE, checker_tex );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic5 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    //glTexParameterf( GL_TEXTURE_2D, GL_NEAREST, GL_LINEAR );
    //glTexParameterf( GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR );
    
    
    
    glBindTexture( GL_TEXTURE_2D, textures[5] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TextureSize, TextureSize, 0, GL_BGR, GL_UNSIGNED_BYTE, checker_tex );
    //glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, pic5 );
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    
    glEnable(GL_DEPTH_CLAMP);
    
    
    
    
    //----set offset variables
    
    CUBE_OFFSET = 0;
    COLOR_OFFSET = sizeof(points_cube);
    CUBE_NORMALS_OFFSET = COLOR_OFFSET + sizeof(colors);
    CUBE_TEXCOORDS_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    CYLINDER_OFFSET = CUBE_NORMALS_OFFSET + sizeof(normals_cube);
    CYLINDER_NORMALS_OFFSET = CYLINDER_OFFSET + sizeof(points_cylinder);
    CYLINDER_TEXCOORDS_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_OFFSET = CYLINDER_NORMALS_OFFSET + sizeof(normals_cylinder);
    SPHERE_NORMALS_OFFSET = SPHERE_OFFSET + sizeof(points_sphere);
    SPHERE_TEXCOORDS_OFFSET = SPHERE_NORMALS_OFFSET + sizeof(normals_sphere);
    
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );  // removed 'APPLE' suffix for 3.2
    glBindVertexArray( vao );
    
    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points_cube) + sizeof(normals_cube) + sizeof(colors) + sizeof(points_cylinder) + sizeof(normals_cylinder) + sizeof(points_sphere) + sizeof(normals_sphere), NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_OFFSET, sizeof(points_cube), points_cube );
    glBufferSubData( GL_ARRAY_BUFFER, COLOR_OFFSET, sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_NORMALS_OFFSET, sizeof(normals_cube), normals_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CUBE_TEXCOORDS_OFFSET, sizeof(tex_coords_cube), tex_coords_cube );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_OFFSET, sizeof(points_cylinder), points_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_NORMALS_OFFSET, sizeof(normals_cylinder), normals_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, CYLINDER_TEXCOORDS_OFFSET, sizeof(tex_coords_cylinder), tex_coords_cylinder );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_OFFSET, sizeof(points_sphere), points_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_NORMALS_OFFSET, sizeof(normals_sphere), normals_sphere );
    glBufferSubData( GL_ARRAY_BUFFER, SPHERE_TEXCOORDS_OFFSET, sizeof(tex_coords_sphere), tex_coords_sphere );
    
    
    //---------------------------------------------------------------------
    
    // set up vertex arrays
    vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    
    vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(COLOR_OFFSET) );
    //---------------------------------------------------------------------
    
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.550, 0.750, 1.0, 1.0 );
    current_view_matrix = LookAt(eye, at, up);
}


//----------------------------------------------------------------------------

mat4 proj_matrix;
mat4 view_matrix;

float r = 2.9;


void
setMaterials(vec4 ka, vec4 kd, vec4 ks, float s, float flip)
{
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
    glUniform1f( glGetUniformLocation(program, "Underlight"), flip);
}

void setLight(vec4 lpos, vec4 la, vec4 ld, vec4 ls)
{
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);

}


void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
    //---- lights
  /*
    vec4 lpos = vec4( 3.0, 3.0, 3.0, 1.0 );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"), 1, lpos);
    
    vec4 la(0.2, 0.2, 0.2, 1.0);
    vec4 ld(0.7, 0.7, 0.7, 1.0);
    vec4 ls(0.7, 0.7, 0.7, 1.0);
    glUniform4fv( glGetUniformLocation(program, "AmbientLight"), 1, la);
    glUniform4fv( glGetUniformLocation(program, "DiffuseLight"), 1, ld);
    glUniform4fv( glGetUniformLocation(program, "SpecularLight"), 1, ls);
   */
   
    

    
    /*
    vec4 ka(0.9, 0.1, 0.1, 1.0);
    vec4 kd(1.0, 0.0, 0.0, 1.0);
    vec4 ks(1.0, 0.4, 0.4, 1.0);
    float s = 5.0;
    glUniform4fv( glGetUniformLocation(program, "ka"), 1, ka );
    glUniform4fv( glGetUniformLocation(program, "kd"), 1, kd );
    glUniform4fv( glGetUniformLocation(program, "ks"), 1, ks );
    glUniform1f( glGetUniformLocation(program, "Shininess"), s );
*/
    
    //---- camera intrinsic parameters
    
    float left = -1.0;
    float right = 1.0;
    float bottom = -1.0;
    float top = 1.0;
    float zNear = 1.0;
    float zFar = 100.0;
    
    proj_matrix = Frustum( left, right, bottom, top, zNear, zFar );
    
    glUniformMatrix4fv( glGetUniformLocation( program, "projection" ), 1, GL_TRUE, proj_matrix );
    
    //---- camera extrinsic parameters
    
    float tr_y = Theta3[Yaxis]* M_PI/180.0;
    float tr_z = Theta3[Zaxis]* M_PI/180.0;
    float eye_z = r * (cos(tr_z)) * cos(tr_y);
    float eye_x = r * (cos(tr_z)) * sin(tr_y);
    float eye_y = r * sin(tr_z);
    
    //vec4 up = vec4(0.0, cos(tr_z), 0.0, 0.0);
    //cout << up << ' ' << normalize(up) << endl;
    
    //view_matrix = LookAt( vec4(eye_x, eye_y, eye_z, 1.0), vec4(0.0, 0.0, 0.0, 1.0), vec4(0.0, cos(tr_z), 0.0, 0.0));
    
    
    glUniformMatrix4fv( glGetUniformLocation( program, "view" ), 1, GL_TRUE, current_view_matrix );

    
    setLight(vec4(3.2, 0.4, 0.75, 1.0), vec4(0.3, 0.3, 0.3, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0));
    // sphere RIGHT
    mat4 rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    mat4 transform = Translate( 3.0, 0.2, 0.75 ) * Scale(0.4, 0.4, 0.4);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    
    setMaterials(vec4(0.9, 0.9, 0.9, 1.0), vec4(0.9, 0.9, 0.9, 0.0), vec4(0.9, 0.9, 0.9, 1.0), 30, true );

    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );

    setLight(vec4(3, 3, 0.75, 1.0), vec4(0.1, 0.1, 0.1, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    

    //---- grass RIGHT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 10.0, -1.00, 0.0 ) * Scale(20, 0.01, 100);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    //setLight(vec4(3.0, 3.0, 3.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    setMaterials(vec4(0.7, 0.1, 0.1, 1.0), vec4(0.0, 0.80, 0.0, 1.0), vec4(1.0, 0.4, 0.4, 1.0), 3, false);
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    

    //---- cylinder pole RIGHT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 3.0, -0.5, 0.75 ) * Scale(0.03, 1.0, 0.05);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.1, 0.1, 0.1, 1.0), vec4(0.1, 0.1, 0.1, 0.0), vec4(0.1, 0.1, 0.1, 1.0), 4, false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    // base pole RIGHT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 3.0, -0.95, 0.75 ) * Scale(0.55, 0.1, 0.55);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.1, 0.1, 0.1, 1.0), vec4(0.1, 0.1, 0.1, 0.0), vec4(0.1, 0.1, 0.1, 1.0), 4, false );
    
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    

    
    setLight(vec4(-3.2, 0.4, 0.75, 1.0), vec4(0.3, 0.3, 0.3, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    // sphere LEFT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -3.0, 0.2, 0.75 ) * Scale(0.4, 0.4, 0.4);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.9, 0.9, 0.9, 1.0), vec4(0.9, 0.9, 0.9, 1.0), vec4(0.9, 0.9, 0.9, 1.0), 4, true );

    
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    setLight(vec4(-3, 3.0, 0.75, 1.0), vec4(0.1, 0.1, 0.1, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0));
    
    //---- grass LEFT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -10.0, -1.00, 0.0 ) * Scale(20, 0.01, 100);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    //setLight(vec4(3.0, 3.0, 3.0, 1.0), vec4(0.2, 0.2, 0.2, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    setMaterials(vec4(0.7, 0.1, 0.1, 1.0), vec4(0.0, 0.80, 0.0, 1.0), vec4(1.0, 0.4, 0.4, 1.0), 3, false );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    
    
    
    //---- cylinder pole LEFT
    
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -3.0, -0.5, 0.75 ) * Scale(0.03, 1.0, 0.05);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.1, 0.1, 0.1, 1.0), vec4(0.1, 0.1, 0.1, 0.0), vec4(0.1, 0.1, 0.1, 1.0), 4 , false);
    
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    
    
    
    // base pole LEFT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -3.0, -0.95, 0.75 ) * Scale(0.55, 0.1, 0.55);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CYLINDER_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.1, 0.1, 0.1, 1.0), vec4(0.1, 0.1, 0.1, 0.0), vec4(0.1, 0.1, 0.1, 1.0), 4, false );
    
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCylinder );
    

    //road
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 0.0, -0.9999, 0.0 ) * Scale(2, 0.01, 100);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    setMaterials(vec4(0.9, 0.1, 0.1, 1.0), vec4(0.70, 0.70, 0.0, 1.0), vec4(1.0, 0.4, 0.4, 1.0), 5, false );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    


    
    

    setLight(vec4(-3.0, 0.1, 0.75, 1.0), vec4(0.2, 0.2, 0.2, 1.0), vec4(0.7, 0.7, 0.7, 1.0), vec4(0.7, 0.7, 0.7, 1.0) );
    //---- cube LEFT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -3, -0.75, -0.2 ) * Scale(0.5, 0.5, 0.5);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    setMaterials(vec4(0.9, 0.1, 0.1, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.4, 0.4, 1.0), 9, false );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
    // sphere RIGHT
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( 100, 200, -100.75 ) * Scale(50, 50, 50);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(SPHERE_NORMALS_OFFSET) );
    
    glUniform1i( glGetUniformLocation(program, "obj_color_on"), true );
    glUniform4fv( glGetUniformLocation(program, "obj_color"), 1, vec4(0.0, 0.9, 0.0, 1.0) );
    
    setMaterials(vec4(0.9, 0.9, 0.9, 1.0), vec4(0.9, 0.9, 0.9, 0.0), vec4(0.9, 0.9, 0.9, 1.0), 4, false );
    
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesSphere );
    
    

    //---- BUILDING? LEFT
    
    glBindTexture( GL_TEXTURE_2D, textures[2] );
    glUniform1i( glGetUniformLocation(program, "texture_on"), true );
    
    rot2 = ( RotateX( Theta2[Xaxis] ) * RotateY( Theta2[Yaxis] ) * RotateZ( Theta2[Zaxis] ) );
    transform = Translate( -3, 3, -10 ) * Scale(3, 8, 3);
    
    glUniformMatrix4fv( glGetUniformLocation( program, "model" ), 1, GL_TRUE, transform );
    
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_OFFSET) );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_NORMALS_OFFSET) );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(CUBE_TEXCOORDS_OFFSET) );
    
    //glUniform1i( glGetUniformLocation(program, "obj_color_on"), false );
    setMaterials(vec4(0.9, 0.1, 0.1, 1.0), vec4(1.0, 0.0, 0.0, 1.0), vec4(1.0, 0.4, 0.4, 1.0), 9, false );
    glDrawArrays( GL_TRIANGLES, 0, NumVerticesCube );
    
    
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
            
        case 'y':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 't':    //---- theta
            Axis = Yaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
            /*
        case 'z':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] += 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
        case 'x':    //---- phi
            Axis = Zaxis;
            Theta3[Axis] -= 5.0;
            Theta3[Axis] = fmod(Theta3[Axis], 360.0);
            glutPostRedisplay();
            break;
             */
            
        case 'r':    //---- increase radius
            r += 0.5;
            glutPostRedisplay();
            break;
        case 'R':    //---- decrease radius
            r -= 0.5;
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

            
            
            
            
        case 'u':
            //view_matrix = default_view_matrix;
            Theta3[Xaxis] = 0.0;
            Theta3[Yaxis] = 0.0;
            Theta3[Zaxis] = 0.0;
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

















