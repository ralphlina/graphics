#version 150

//---- ins
in  vec4 color;
in  vec3 fN;
in  vec3 fL;
in  vec3 fE;

//---- outs
out vec4 FragColor;

//---- uniforms
uniform vec4 AmbientLight, DiffuseLight, SpecularLight;
uniform vec4 ka, kd, ks;
uniform float Shininess;

//===========================================================================
// fshader main
//===========================================================================
void
main()
{
    //---- compute the output color for this fragment
    //FragColor = color; //vec4(1.0, 0.2, 0.4, 1.0);
    //FragColor = vec4(0.3, 0.5, 0.8, 1.0);
    
    
    
    // Normalize the input lighting vectors
    
    vec3 N = normalize(fN);
    vec3 E = normalize(fE);
    vec3 L = normalize(fL);
    
    vec3 H = normalize( L + E );
    
    // Compute terms in the illumination equation
    
    vec4 ambient = AmbientLight * ka;
    
    vec4  diffuse = DiffuseLight * kd * max( dot(L, N), 0.0 );  //(pow(length(fL),2.0));
    
    vec4  specular = SpecularLight * ks * pow( max(dot(N, H), 0.0), Shininess );
    
    if ( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0);
    
    
    FragColor = ambient + diffuse + specular;
    FragColor.a = 1.0;
}