#version 150

//---- ins
in  vec4 color;
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
in  vec2 tex_coord;
in  vec3 rv;

//---- outs
out vec4 FragColor;

//---- uniforms
uniform vec4 AmbientLight, DiffuseLight, SpecularLight;
uniform vec4 ka, kd, ks;
uniform float Shininess;
uniform bool light_out;
uniform vec4 LightPosition;
uniform mat4 view;
uniform bool texture_on;
uniform bool cube_map_on;

uniform sampler2D myTextureSampler;
uniform samplerCube myCubeSampler;

vec4 kA, kD, kS;

void main()
{
    // Normalize the input lighting vectors
    vec3 N = normalize(fN);
    if (light_out) N = -N;
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    
    vec3 H = normalize( L + E );
    
    
    //---- get texture value
    vec4 texel = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (texture_on)
    {
        if (cube_map_on)
            texel = texture( myCubeSampler, rv );
        else
            texel = texture( myTextureSampler, tex_coord );
        
        kA = vec4(1.0,1.0,1.0,1.0);
        kD = vec4(1.0,1.0,1.0,1.0);
        kS = vec4(0.9,0.9,0.9,1.0);
    }
    else
    {
        kA = ka;
        kD = kd;
        kS = ks;
    }
    
    // Compute terms in the illumination equation
    vec4 ambient = AmbientLight * kA * texel;
    
    vec4  diffuse;
    if (light_out)
        diffuse = DiffuseLight * kD * texel * max( dot(L, N), 0.0 );
    else
        diffuse = DiffuseLight * kD * texel * max( dot(L, N), 0.0 )/(0.1*pow(length(fL),2.0));

    vec4  specular = SpecularLight * kS * texel * pow( max(dot(reflect(-L,N), E), 0.0), Shininess );
    //vec4  specular = SpecularLight * ks * pow( max(dot(N,H), 0.0), Shininess );
    
    if ( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
}


#if 0

//---- ins
in  vec4 color;
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;
in  vec2 tex_coord;
in  vec3 rv;

//---- outs
out vec4 FragColor;

//---- uniforms
uniform vec4 AmbientLight, DiffuseLight, SpecularLight;
uniform vec4 ka, kd, ks;
uniform float Shininess;
uniform bool light_out;
uniform bool texture_on;
uniform bool cube_map_on;
uniform bool bump_on;

uniform sampler2D myTextureSampler;
uniform samplerCube myCubeSampler;
uniform sampler2D normal_map;

vec4 kA, kD, kS;

//===========================================================================
// fshader main
//===========================================================================
void
main()
{
    //---- Normalize the input lighting vectors
    vec3 N;
    if (bump_on)
        N = normalize(2.0*(texture( normal_map, tex_coord ).rgb)  - 1.0);
    else
        N = normalize(fN);
    
    if (light_out) N = -N;
    
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    
    vec3 H = normalize( L + E );
    
    //---- get texture value
    vec4 texel = vec4(1.0, 1.0, 1.0, 1.0);
    
    if (texture_on)
    {
        if (cube_map_on)
            texel = texture( myCubeSampler, rv );
        else
            texel = texture( myTextureSampler, tex_coord );
        
        kA = vec4(1.0,1.0,1.0,1.0);
        kD = vec4(1.0,1.0,1.0,1.0);
        kS = vec4(0.9,0.9,0.9,1.0);
        //kS = vec4(1.0,1.0,1.0,1.0);
    }
    else
    {
        kA = ka;
        kD = kd;
        kS = ks;
    }
    
    //---- Compute terms in the illumination equation
    
    vec4 ambient = AmbientLight * kA * texel;
    //vec4 ambient =  AmbientLight * texel;
    
    vec4 diffuse = DiffuseLight * kD * texel * max( dot(L, N), 0.0 )/(pow(length(fL),2.0));
    //vec4 diffuse =  DiffuseLight * texel * max( dot(L, N), 0.0 )/(pow(length(fL),2.0));
    
    vec3 R = 2*dot(N,L)*N - L;
    vec4 specular = SpecularLight * kS * texel * pow( max(dot(R, E), 0.0), Shininess );
    //vec4 specular = SpecularLight * kS * texel * pow( max(dot(reflect(-L,N), E), 0.0), Shininess );
    //vec4 specular = SpecularLight * kS * texel * pow( max(dot(N, H), 0.0), Shininess ) * max( dot(L, N), 0.0 );
    //vec4 specular =  SpecularLight * texel * pow( max(dot(N, H), 0.0), Shininess );
    
    //if ( dot(L, N) < -0.5 ) specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    
    FragColor = ambient + diffuse + specular;
    
    //FragColor = kd * max( dot(L, N), 0.0 );//ambient + diffuse + specular;
    //FragColor = vec4(fN,1.0);//ambient + diffuse + specular;
    //FragColor.a = 1.0;
    
    //---- compute the output color for this fragment
    //FragColor = color; //vec4(1.0, 0.2, 0.4, 1.0);
}
#endif






