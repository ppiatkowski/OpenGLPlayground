#ifndef _VERTEX_DATA_H_
#define _VERTEX_DATA_H_

#include <gl/gl.h>

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f,1.0f,-1.0f,
    -1.0f,1.0f, 1.0f,
    -1.0f, 3.0f, 1.0f,
    1.0f, 3.0f,-1.0f,
    -1.0f,1.0f,-1.0f,
    -1.0f, 3.0f,-1.0f,
    1.0f,1.0f, 1.0f,
    -1.0f,1.0f,-1.0f,
    1.0f,1.0f,-1.0f,
    1.0f, 3.0f,-1.0f,
    1.0f,1.0f,-1.0f,
    -1.0f,1.0f,-1.0f,
    -1.0f,1.0f,-1.0f,
    -1.0f, 3.0f, 1.0f,
    -1.0f, 3.0f,-1.0f,
    1.0f,1.0f, 1.0f,
    -1.0f,1.0f, 1.0f,
    -1.0f,1.0f,-1.0f,
    -1.0f, 3.0f, 1.0f,
    -1.0f,1.0f, 1.0f,
    1.0f,1.0f, 1.0f,
    1.0f, 3.0f, 1.0f,
    1.0f,1.0f,-1.0f,
    1.0f, 3.0f,-1.0f,
    1.0f,1.0f,-1.0f,
    1.0f, 3.0f, 1.0f,
    1.0f,1.0f, 1.0f,
    1.0f, 3.0f, 1.0f,
    1.0f, 3.0f,-1.0f,
    -1.0f, 3.0f,-1.0f,
    1.0f, 3.0f, 1.0f,
    -1.0f, 3.0f,-1.0f,
    -1.0f, 3.0f, 1.0f,
    1.0f, 3.0f, 1.0f,
    -1.0f, 3.0f, 1.0f,
    1.0f,1.0f, 1.0f
};

// Two UV coordinatesfor each vertex. They were created withe Blender.
static const GLfloat g_uv_buffer_data[] = { 
    0.000059f, 0.000004f, 
    0.000103f, 0.336048f, 
    0.335973f, 0.335903f, 
    1.000023f, 0.000013f, 
    0.667979f, 0.335851f, 
    0.999958f, 0.336064f, 
    0.667979f, 0.335851f, 
    0.336024f, 0.671877f, 
    0.667969f, 0.671889f, 
    1.000023f, 0.000013f, 
    0.668104f, 0.000013f, 
    0.667979f, 0.335851f, 
    0.000059f, 0.000004f, 
    0.335973f, 0.335903f, 
    0.336098f, 0.000071f, 
    0.667979f, 0.335851f, 
    0.335973f, 0.335903f, 
    0.336024f, 0.671877f, 
    1.000004f, 0.671847f, 
    0.999958f, 0.336064f, 
    0.667979f, 0.335851f, 
    0.668104f, 0.000013f, 
    0.335973f, 0.335903f, 
    0.667979f, 0.335851f, 
    0.335973f, 0.335903f, 
    0.668104f, 0.000013f, 
    0.336098f, 0.000071f, 
    0.000103f, 0.336048f, 
    0.000004f, 0.671870f, 
    0.336024f, 0.671877f, 
    0.000103f, 0.336048f, 
    0.336024f, 0.671877f, 
    0.335973f, 0.335903f, 
    0.667969f, 0.671889f, 
    1.000004f, 0.671847f, 
    0.667979f, 0.335851f
};

#endif // _VERTEX_DATA_H_