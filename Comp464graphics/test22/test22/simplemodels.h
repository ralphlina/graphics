//
//  Header.h
//  test22
//
//  Created by Lina, Ralph on 9/14/16.
//  Copyright © 2016 Lina, Ralph. All rights reserved.
//

#ifndef Header_h
#define Header_h


const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

extern vec4 points_cube[NumVertices];
extern vec4 colors[NumVertices];
extern vec3 normals_cube[NumVertices];

void colorcube();




const int segmentsCylinder = 64;
const int NumVerticesCylinder = segmentsCylinder*6 + segmentsCylinder*3*2;

extern vec4 points_cylinder[NumVerticesCylinder];
extern vec3 normals_cylinder[NumVerticesCylinder];

void colortube();



const int segmentsSphere = 512;
//const int NumVerticesSphere = segmentsSphere*6 + segmentsSphere*3*2;
const int NumVerticesSphere = segmentsSphere*6 * (segmentsSphere - 2) + segmentsSphere*3*2;
extern vec4 points_sphere[NumVerticesSphere];
extern vec3 normals_sphere[NumVerticesSphere];

void colorbube();


#endif /* Header_h */
