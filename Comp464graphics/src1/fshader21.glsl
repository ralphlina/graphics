#version 150

//---- ins
in  vec4 color;

//---- outs
out vec4 FragColor;

//===========================================================================
// fshader main
//===========================================================================
void
main()
{
    //---- compute the output color for this fragment
    //FragColor = color; //vec4(1.0, 0.2, 0.4, 1.0);
    FragColor = vec4(0.3, 0.5, 0.8, 1.0);
}