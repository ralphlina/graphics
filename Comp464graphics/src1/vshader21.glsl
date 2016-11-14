#version 150

//---- ins
in vec4    vPosition;          // object-space vertex position.
in vec4    vColor;             // per-vertex color

//---- outs
out vec4 color;

//---- uniforms
uniform mat4 model;
uniform vec4 obj_color;
uniform bool obj_color_on = false;

//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    //---- transform the vertex
    gl_Position = model * vPosition;
    
    
    if (obj_color_on)
        color = obj_color;
    else
        color = vColor;
}