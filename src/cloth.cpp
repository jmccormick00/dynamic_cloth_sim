/*==============================================================================
/ James McCormick - cloth.cpp
/ A class to simulate cloth in real time.
/=============================================================================*/


//==============================================================================
// INCLUDED LIBRARIES AND FILES
//==============================================================================
#include "cloth.h"
#include "glee.h"
#include <GL/gl.h>
#include <time.h>

//==============================================================================
// CONSTRUCTORS / DESTRUCTORS
//==============================================================================

//------------------------------------------------------------------------------
// Constructor
// Initializes the pointers to null.
//------------------------------------------------------------------------------
C_Cloth::C_Cloth() : d_particleInfo(0), d_structuralSprings(0), d_shearSprings(0),
d_uVertexBufferID(0)
{
}


//==============================================================================
// PRIVATE METHODS
//==============================================================================

//------------------------------------------------------------------------------
// void sumForces()
//
// Used by the stepSimulation() function to calculate the combined forces acting
// on the particles.  finds the accelerations for each particle
//------------------------------------------------------------------------------
void C_Cloth::sumForces()
{
    vector3f wind, d, f;

    // seed for rand
    srand((unsigned int)time(0));

    // process the gravity, drag, and wind forces
    for(unsigned i = 0; i < d_uNumParticles; ++i)
    {
        if(d_particleInfo[i].locked) continue;

        // Add acceleration due to gravity first
        d_pAccel[i] = d_vGravity;

        // wind
        if(d_windVector != vector3f(0,0,0) && d_windFactor != 0)
        {
            wind.X() = randFloat(0.0, d_windVector.X());
            wind.Y() = randFloat(0.0, d_windVector.Y());
            wind.Z() = randFloat(0.0, d_windVector.Z());
            wind.normalize();
            d_pAccel[i] += d_particleInfo[i].invMass * wind * (float)(rand() % d_windFactor);
        }
    }

    // Process the forces from the shear springs
//    for(int i = 0; i < d_numShearSprings; ++i)
//    {
//        d = (*d_shearSprings[i].p1) - (*d_shearSprings[i].p2);

//        f = -(d_shearSpringConst * (d.magnitude() - d_shearSprings[i].restLength));

//        if(!d_shearSprings[i].info1->locked)
//            (*d_shearSprings[i].acel1) += (f * d_shearSprings[i].info1->invMass);

//        if(!d_shearSprings[i].info2->locked)
//            (*d_shearSprings[i].acel2) -= (f * d_shearSprings[i].info2->invMass);
//    }

//    // Process the forces from the structural springs
//    for(int i = 0; i < d_numStructSprings; i++)
//    {
//        d = (*d_structuralSprings[i].p1) - (*d_structuralSprings[i].p2);

//        f = -(d_structSpringConst * (d.magnitude() - d_structuralSprings[i].restLength));

//        if(!d_structuralSprings[i].info1->locked)
//            (*d_structuralSprings[i].acel1) += (f * d_shearSprings[i].info1->invMass);

//        if(!d_structuralSprings[i].info2->locked)
//            (*d_structuralSprings[i].acel2) -= (f * d_shearSprings[i].info1->invMass);
//    }
}


void C_Cloth::applyConstraints()
{
    for(int j = 0; j < 3; ++j)
    {
        for(int i = 0; i < d_numStructSprings; ++i)
        {
            Spring& s = d_structuralSprings[i];
            vector3f& x1 = *s.p1;
            vector3f& x2 = *s.p2;
            vector3f delta = x1 - x2;
            float deltaLength = delta.magnitude();
            float diff = (deltaLength - s.restLength)/deltaLength;
            if(!s.info1->locked)
                x1 -= delta*0.5*diff;
            if(!s.info2->locked)
                x2 += delta*0.5*diff;
        }

        for(int i = 0; i < d_numShearSprings; ++i)
        {
            Spring& s = d_shearSprings[i];
            vector3f& x1 = *s.p1;
            vector3f& x2 = *s.p2;
            vector3f delta = x1 - x2;
            float deltaLength = delta.magnitude();
            float diff = (deltaLength - s.restLength)/deltaLength;
            if(!s.info1->locked)
                x1 -= delta*0.5*diff;
            if(!s.info2->locked)
                x2 += delta*0.5*diff;
        }
    }
}





//==============================================================================
// PUBLIC METHODS
//==============================================================================


//------------------------------------------------------------------------------
// void clear()
//
// Clears out the memory allocated 
//------------------------------------------------------------------------------
void C_Cloth::clear()
{
    glDeleteBuffers(1, &d_uVertexBufferID);

    if(d_particleInfo)
    {
        delete d_particleInfo;
        d_particleInfo = 0;
    }
    if(d_structuralSprings)
    {
        delete d_structuralSprings;
        d_structuralSprings = 0;
    }
    if(d_shearSprings)
    {
        delete d_shearSprings;
        d_shearSprings = 0;
    }
}



//------------------------------------------------------------------------------
// void draw()
//
// Draws the cloth.
//------------------------------------------------------------------------------
void C_Cloth::draw()
{

}





//------------------------------------------------------------------------------
// void drawParticles()
//
// Draws the particles as red points.  Locked particles are displayed as green
//------------------------------------------------------------------------------
void C_Cloth::drawParticles()
{
    glColor3f(1.0, 0.0, 0.0);
    glEnableClientState(GL_VERTEX_ARRAY);
    //glBindBuffer(GL_ARRAY_BUFFER, d_uVertexBufferID);
    glVertexPointer(3, GL_FLOAT, sizeof(C_Vertex), &d_pPositions[0]);
    glDrawArrays(GL_POINTS, 0, d_uNumParticles);
    glDisableClientState(GL_VERTEX_ARRAY);
    glColor3f(1.0, 1.0, 1.0);
}



//------------------------------------------------------------------------------
// void drawMesh()
//
// Draws the springs as blue lines.
//------------------------------------------------------------------------------
void C_Cloth::drawMesh()
{
    glColor3f(0.0, 0.0, 1.0);

    glColor3f(1.0, 1.0, 1.0);
}


//------------------------------------------------------------------------------
// void changeWindVector()
//
// Modifies the wind Vector.
//------------------------------------------------------------------------------
void C_Cloth::setWindVector(float x, float y, float z)
{
    d_windVector.X() = x;
    d_windVector.Y() = y;
    d_windVector.Z() = z;
}


//------------------------------------------------------------------------------
// void initialize()
//
// Initializes the cloth's particles position and force vectors as well as the
// structural springs data.  The cloth's center is initialized around 0,0,0 
// in 3D space.  The geometry is derived from the height, width, numRow, and 
// numCol parameters passed to the function.  The number of particles in the 
// cloth is found by (numCol*numRow).
// 
// Input: 	width - The width of the cloth.
//			height - The height of the cloth.
//			numRow - The number of particle rows in the cloth.
//			numCol - The number of particle columns in the cloth.
//			mass - The mass of the cloth.
//			structural - The spring constant for the structual springs.
//			shear - The spring constant for the shear springs.
//			damp - the dampening constant for the springs.
//			axis - The axis that the height of the cloth will lay parallel with.
//------------------------------------------------------------------------------
void C_Cloth::initialize(float width, float height, int numRow, int numCol, float mass,
					   float structural, float shear, float damp, int axis)
{	
    float wBound,		// The width boundary of the cloth.
          hBound,		// The height boundary of the cloth.
          wStep, 		// The step size in between the columns.
          hStep,		// The step size in between the rows.
          faceMass;		// The mass of one face

    float f;
    vector3f length;

    // Clear out any memory that may have been allocated
    clear();
    wipeParticleData();

    d_dragCoef = damp;

    // Initialize the spring constants
    d_shearSpringConst = shear;
    d_structSpringConst = structural;
    d_dragCoef = damp;

    d_numCol = numCol;
    d_numRow = numRow;

    // Initialize the wind factor to 0
    d_windFactor = 0;

    // Calculate the total number of faces
    d_numFaces = (numCol - 1) * (numRow - 1) * 2;

    // Define the mass of the cloth
    faceMass = mass / (float)d_numFaces;

    // Calculate the x and y bound of the cloth - center the cloth at 0,0
    wBound = width / 2.0f;
    hBound = height / 2.0f;

    // Start in the negative side of 0,0,0 and work towards the positive
    wBound = -wBound;

    // Calculate the yStep and xStep
    wStep = width / (numCol-1);
    hStep = height / (numRow-1);

    // Calculate the variables used for texture mapping
    float wTexStep = wStep / width;
    float hTexStep = hStep / height;

    // Calculate the total number of particles
    initParticleData(numCol * numRow);

    // Allocate the memory for the particles
    d_particleInfo = new ParticleInfo[d_uNumParticles];

    unsigned index;

    // Initialize the particles
    for(int i = 0; i < numRow; i++)
    {
        for(int j = 0; j < numCol; j++)
        {
            float mass;
            index = getIndex2D(i, j);

            // Set the mass of the particle
            if((i == 0) && (j == 0)) f = 1;
            else if((i == (numRow - 1)) && (j == 0)) f = 2;
            else if((i == 0) && (j == (numCol - 1))) f = 2;
            else if((i == (numRow - 1)) && (j == (numCol - 1))) f = 1;
            else if(((i == 0) || (i == (numRow - 1))) && ((j != 0) && (j != (numCol - 1)))) f = 3;
            else if(((j == 0) || (j == (numCol - 1))) && ((i != 0) && (i != (numRow - 1)))) f = 3;
            else f = 6;

            mass = (f * faceMass) / 3;
            d_particleInfo[index].invMass = 1.0 / mass;

            // Assign the texture coordinates
            d_pPositions[index].s0 = i*wTexStep;
            d_pPositions[index].t0 = j*hTexStep;

            // Set the intial position for this particle
            d_pPositions[index].pos.X() = wBound + (j * wStep);
            d_pOldPositions[index].X() = wBound + (j * wStep);
            if(axis == ZAXIS)
            {
                d_pPositions[index].pos.Z() = hBound - (i * hStep);
                d_pOldPositions[index].Z() = hBound - (i * hStep);
                d_pPositions[index].pos.Y() = 0;
                d_pOldPositions[index].Y() = 0;
            }
            else
            {
                d_pPositions[index].pos.Y() = hBound - (i * hStep);
                d_pOldPositions[index].Y() = hBound - (i * hStep);
                d_pOldPositions[index].Z() = d_pPositions[index].pos.Z() = 0;
            }

            // Set the initial acceleration for this particle
            d_pAccel[index].X() = 0;
            d_pAccel[index].Y() = 0;
            d_pAccel[index].Z() = 0;

            // Make them all false to begin with
            d_particleInfo[index].locked = false;
        }
    }


    // Calculate the total number of springs
    d_numShearSprings = ((numCol - 1) * (numRow - 1)) * 2;
    d_numStructSprings = (numCol * (numRow - 1)) + (numRow * (numCol - 1));

    // Allocate memory for the springs
    d_shearSprings = new Spring[d_numShearSprings];
    d_structuralSprings = new Spring[d_numStructSprings];

    // Set up the springs
    unsigned shearCount(0);
    unsigned structCount(0);
    for(int i = 0; i < numRow; i++)
    {
        for(int j = 0; j < numCol; j++)
        {
            // The structural spring going across the col
            if(j <= (numCol - 2))
            {
                d_structuralSprings[structCount].p1 = &d_pPositions[getIndex2D(i, j)].pos;
                d_structuralSprings[structCount].info1 = &d_particleInfo[getIndex2D(i, j)];
                d_structuralSprings[structCount].acel1 = &d_pAccel[getIndex2D(i, j)];
                d_structuralSprings[structCount].p2 = &d_pPositions[getIndex2D(i, j+1)].pos;
                d_structuralSprings[structCount].info2 = &d_particleInfo[getIndex2D(i, j+1)];
                d_structuralSprings[structCount].acel2 = &d_pAccel[getIndex2D(i, j+1)];
                length = (*d_structuralSprings[structCount].p1) -
                                 (*d_structuralSprings[structCount].p2);
                d_structuralSprings[structCount].restLength = length.magnitude();
                structCount++;
            }

            // The structural spring going down a column
            if(i <= (numRow - 2))
            {
                d_structuralSprings[structCount].p1 = &d_pPositions[getIndex2D(i, j)].pos;
                d_structuralSprings[structCount].info1 = &d_particleInfo[getIndex2D(i, j)];
                d_structuralSprings[structCount].acel1 = &d_pAccel[getIndex2D(i, j)];
                d_structuralSprings[structCount].p2 = &d_pPositions[getIndex2D(i+1, j)].pos;
                d_structuralSprings[structCount].info2 = &d_particleInfo[getIndex2D(i+1, j)];
                d_structuralSprings[structCount].acel2 = &d_pAccel[getIndex2D(i+1, j)];
                length = (*d_structuralSprings[structCount].p1) -
                                 (*d_structuralSprings[structCount].p2);
                d_structuralSprings[structCount].restLength = length.magnitude();
                structCount++;
            }

            // The shear spring going diagonal from left to right
            if((j <= (numCol - 2)) && (i <= (numRow - 2)))
            {
                d_shearSprings[shearCount].p1 = &d_pPositions[getIndex2D(i, j)].pos;
                d_shearSprings[shearCount].info1 = &d_particleInfo[getIndex2D(i, j)];
                d_shearSprings[shearCount].acel1 = &d_pAccel[getIndex2D(i, j)];
                d_shearSprings[shearCount].p2 = &d_pPositions[getIndex2D(i+1, j+1)].pos;
                d_shearSprings[shearCount].info2 = &d_particleInfo[getIndex2D(i+1, j+1)];
                d_shearSprings[shearCount].acel2 = &d_pAccel[getIndex2D(i+1, j+1)];
                length = (*d_shearSprings[shearCount].p1) -
                                 (*d_shearSprings[shearCount].p2);
                d_shearSprings[shearCount].restLength = length.magnitude();
                shearCount++;
            }

            // The shear spring going diagonal from right to left
            if(j > 0 && i <= (numRow - 2))
            {
                d_shearSprings[shearCount].p1 = &d_pPositions[getIndex2D(i, j)].pos;
                d_shearSprings[shearCount].info1 = &d_particleInfo[getIndex2D(i, j)];
                d_shearSprings[shearCount].acel1 = &d_pAccel[getIndex2D(i, j)];
                d_shearSprings[shearCount].p2 = &d_pPositions[getIndex2D(i+1, j-1)].pos;
                d_shearSprings[shearCount].info2 = &d_particleInfo[getIndex2D(i+1, j-1)];
                d_shearSprings[shearCount].acel2 = &d_pAccel[getIndex2D(i+1, j-1)];
                length = (*d_shearSprings[shearCount].p1) -
                                 (*d_shearSprings[shearCount].p2);
                d_shearSprings[shearCount].restLength = length.magnitude();
                shearCount++;
            }
        }
    }


    // Finally initialize the wind vector
    d_windVector.X() = 0;
    d_windVector.Y() = 0;
    d_windVector.Z() = 0;

    // init GL
    //glEnableClientState(GL_VERTEX_ARRAY);
//    glGenBuffers(1, &d_uVertexBufferID);
//    glBindBuffer(GL_ARRAY_BUFFER, d_uVertexBufferID);
//    glBufferData(GL_ARRAY_BUFFER, d_uNumParticles * sizeof(vector3f), 0, GL_STREAM_DRAW);
}
