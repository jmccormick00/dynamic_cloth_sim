/*==============================================================================
/ James McCormick - cloth.h
/ A class to simulate cloth in real time.
/ Uses openGL to for drawing.
/=============================================================================*/

#ifndef _CLOTH_
#define _CLOTH_

//==============================================================================
// INCLUDED LIBRARIES AND FILES
//==============================================================================
#include "I_ParticleSystem.h"
#include <stdlib.h>

//==============================================================================
// GLOBALS
//==============================================================================

#define ZAXIS 	1
#define YAXIS 	2

//==============================================================================
// ADDITIONAL FUNCTIONS USED BY THE CLASS
//==============================================================================

//------------------------------------------------------------------------------
// Generates a random float between 0.0 and 0.999...
// Assumes rand() as been seeded
//------------------------------------------------------------------------------
inline float randFloat()
{
    return ((float)rand() / 1.999);
}

//------------------------------------------------------------------------------
// Generates a random float between 0.0 and max
// Assumes rand() as been seeded
//------------------------------------------------------------------------------
inline float randFloat(float max)
{
    return (randFloat() * max);
}

//------------------------------------------------------------------------------
// Generates a random float between min and max
// Assumes rand() as been seeded
//------------------------------------------------------------------------------
inline float randFloat(float min, float max)
{
    if(min > max)
        return (randFloat() * (min - max) + max);
    else
        return (randFloat() * (max - min) + min);
}


//==============================================================================
// CLASS DEFINITION
//==============================================================================
class C_Cloth : public I_ParticleSystem<float>
{
private:
    //==============================================================================
    // STRUCTURES
    //==============================================================================

    struct ParticleInfo
    {
        float invMass;
        bool locked;        // If this is true, then the particle does not move
    };

    struct Spring
    {
        vector3f *p1, *p2;
        vector3f *acel1, *acel2;
        ParticleInfo *info1, *info2;
        float restLength;
    };


    //----------------------------------------------------------------------
    // Private Members
    //----------------------------------------------------------------------
    ParticleInfo *d_particleInfo;

    Spring *d_structuralSprings,	// The array of structural springs
            *d_shearSprings;

    unsigned d_numShearSprings,		// The number of Shear springs in the simulation
        d_numStructSprings,		// The number of Structual springs in the simulation
        d_numFaces,			// The number of faces in the cloths geometry
        d_numCol,			// The number of columns
        d_numRow,
        d_windFactor;

    float   d_shearSpringConst,		// The spring constant for the shear springs
            d_structSpringConst;	// The spring constant for the structural springs

    vector3f d_windVector;		// The vector for the wind effecting the cloth

    unsigned d_uVertexBufferID;     // ID for the vertex buffer object

    //----------------------------------------------------------------------
    // Private Methods
    //----------------------------------------------------------------------

    // Uses row major order to create a 1D index from a 2D index
    unsigned getIndex2D(unsigned i, unsigned j)   { return i*d_numCol + j; }


public:
        //----------------------------------------------------------------------
        // Public Members
        //----------------------------------------------------------------------


        //----------------------------------------------------------------------
        // Public Methods
        //----------------------------------------------------------------------
        C_Cloth();			// Constructor
        ~C_Cloth() { clear(); } 	// Destructor

        // Clear the cloth
        void clear();

        // Draw the cloth
        void draw();

        // Draws the mesh
        void drawMesh();

        // Draws the particles
        void drawParticles();

        // sum the forces
        void sumForces();

        // apply the cloth constraints
        void applyConstraints();

        void setWindFactor(float w) { d_windFactor = w; }

        // Modify the wind vector effecting the cloth
        void setWindVector(float x, float y, float z);

        // Sets the passed particle as locked
        void lockParticle(unsigned i, unsigned j) { if(i < d_numRow && j < d_numCol) d_particleInfo[getIndex2D(i,j)].locked = true; }

        // Inililize the cloth's particles
        void initialize(float width, float height, int numRow, int numCol, float mass,
                                        float tension, float shear, float damp, int axis);
};


#endif
