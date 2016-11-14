#version 150

//---- ins
in vec4    vPosition;          // object-space vertex position.
in vec4    vColor;             // per-vertex color
in   vec3 vNormal;

//---- outs
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;
out  vec4 color;


//---- uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 obj_color;
uniform bool obj_color_on = false;
uniform vec4 LightPosition;

//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    //---- transform the vertex
    /*
    gl_Position = projection * view * model * vPosition;
    
    
    if (obj_color_on)
        color = obj_color;
    else
        color = vColor;
     */
    gl_Position = projection * view * model * vPosition;
    
    vec3 pos = (view * model * vPosition).xyz;
    
    fN = (view*model*vec4(vNormal, 0.0)).xyz;
    fE = -pos;
    fL = (view * LightPosition).xyz - pos;
}