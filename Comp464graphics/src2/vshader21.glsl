#version 150

//---- ins
in vec4    vPosition;          // object-space vertex position.
in vec4    vColor;             // per-vertex color

//---- outs
out vec4 color;

//---- uniforms
uniform mat4 model;

//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    //---- transform the vertex
    
    gl_Position = vPosition;
    
    color = vColor;
}
