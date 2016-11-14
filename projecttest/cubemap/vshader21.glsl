#version 150

//---- ins
in   vec4 vPosition;
in   vec3 vNormal;
in   vec2 vTexCoord;

//---- outs
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;
out  vec4 color;
out  vec2 tex_coord;
out  vec3 rv;

//---- uniforms
uniform vec4 LightPosition;
uniform vec4 cam_location;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 obj_color;
uniform bool obj_color_on = false;
uniform bool light_out;


void main()
{
    gl_Position = projection * view * model * vPosition;
    
    vec3 pos = (view * model * vPosition).xyz;
    
    fN = (view*model*vec4(vNormal, 0.0)).xyz;
    fE = -pos;
    fL = (view * LightPosition).xyz - pos;
    
    tex_coord    = vTexCoord;
    
    //---- cube mapping
    vec3 v = (model*vPosition - cam_location).xyz;
    vec3 n = normalize(model*vec4(vNormal, 0.0)).xyz;
    if (light_out)
        rv = reflect(-v, normalize(model*vec4(vNormal, 0.0)).xyz);
    else
        rv = 2*dot(-v,n) * n + v;  //reflect(v, normalize(model*vec4(vNormal, 0.0)).xyz);

}


#if 0
//---- ins
in vec4	vPosition;          // object-space vertex position.
in vec4	vColor;             // per-vertex color
in vec3 vNormal;
in vec2 vTexCoord;
in vec4	vTangent;

//---- outs
out vec4 color;
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 tex_coord;
out vec3 rv;

//---- uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 LightPosition;
uniform vec4 cam_location;
uniform bool light_out;
uniform bool bump_on;

uniform vec4 obj_color;
uniform bool obj_color_on = false;
uniform float st_factor;

//===========================================================================
// vshader main
//===========================================================================
void
main()
{
    //---- transform the vertex
    gl_Position = projection * view * model * vPosition;
    
    //---- if we want to just color the object
    if (obj_color_on)
        color = obj_color;
    else
        color = vColor;
    
    //---- lighting vectors
    fN = (view * model*vec4(vNormal, 0.0)).xyz;
    fE = -(view * model * vPosition).xyz;
    fL = (view * LightPosition).xyz - (view*model * vPosition).xyz;
    
    //---- bump mapping
    vec4 Tangent = view * model * vTangent;
	vec3 bitangent = cross(fN, Tangent.xyz) * vTangent.w;
    
    if (bump_on)
    {
        fE = vec3(dot(normalize(Tangent.xyz), normalize(fE)), dot(normalize(bitangent), normalize(fE)), dot(normalize(fN), normalize(fE)));
        fL = vec3(dot(normalize(Tangent.xyz), normalize(fL)), dot(normalize(bitangent), normalize(fL)), dot(normalize(fN), normalize(fL)));
    }
    
    //---- cube mapping
    vec3 v = (model*vPosition - cam_location).xyz;
    
    if (light_out)
        rv = reflect(-v, normalize(model*vec4(vNormal, 0.0)).xyz);
    else
        rv = reflect(v, normalize(model*vec4(vNormal, 0.0)).xyz);
    
    //---- texture tiling
    tex_coord    = st_factor*vTexCoord;
}
#endif

