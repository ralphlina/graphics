
//----------------------------------------------------------------------------

const int NumVerticesCube = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVerticesCube];
extern vec4 colors[NumVerticesCube];
extern vec3 normals_cube[NumVerticesCube];
extern vec2 tex_coords_cube[NumVerticesCube];

void colorcube();


//----------------------------------------------------------------------------

const int segments = 128;
const int NumVerticesCylinder = segments*6 + segments*3*2;

extern vec4 points_cylinder[NumVerticesCylinder];
extern vec3 normals_cylinder[NumVerticesCylinder];
extern vec2 tex_coords_cylinder[NumVerticesCylinder];

void colortube();


//----------------------------------------------------------------------------

const int ssegments = 512;
const int NumVerticesSphere = ssegments*6*(ssegments-2) + ssegments*3*2;

extern vec4 points_sphere[NumVerticesSphere];
extern vec3 normals_sphere[NumVerticesSphere];
extern vec2 tex_coords_sphere[NumVerticesSphere];

void colorbube();


//----------------------------------------------------------------------------

const int tsegments = 512;
const int NumVerticesTorus = tsegments*6*(tsegments-2) + tsegments*3*2;

extern vec4 points_torus[NumVerticesTorus];
extern vec3 normals_torus[NumVerticesTorus];

void colortorus();



