#ifndef C_VERTEX_H
#define C_VERTEX_H

#include "vector3.h"


#define BUFFER_OFFSET(i) ((char *)NULL + (i))


struct C_Vertex
{
    vector3f pos;   // position vector
    vector3f norm;  // vertex normal
    float s0, t0;   // tex coords
};


#endif // C_VERTEX_H
