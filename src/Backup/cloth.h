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
#include "vector3.h"
#include <stdlib.h>

//==============================================================================
// GLOBALS
//==============================================================================

#define GRAVITY             -32.174
#define DRAGCOEFFICIENT     0.01f

#define ZAXIS 	1
#define YAXIS 	2

//==============================================================================
// STRUCTURES
//==============================================================================

struct Particle
{
	float mass;
	float invMass;
	Vector3 position;		// The particle's position
	Vector3 velocity;		// The particle's velocity
	Vector3 acceleration;	// The particle's acceleration
	Vector3 forces;			// The acting force on the particle 
	bool locked;			// If this is true, then the particle does not move
};


struct Spring
{
	Particle* p1;			// A particle linked to the spring
	Particle* p2;			// The other particle linked to the spring
	float restLength;		// The rest length of the spring
};


struct Face
{
	Particle *p1, *p2, *p3;	// Pointers to the particles that make up a face of the cloth
};	



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
class Cloth
{
	private:
		//----------------------------------------------------------------------
		// Private Members
		//----------------------------------------------------------------------
		Spring 		*d_shearSprings,		// The array of shear springs
		 			*d_structuralSprings;	// The array of structural springs
	
		Particle 	*d_particles;			// The array of particles
	
		Face		*d_faces;				// The array of faces in the cloth used for drawing
	
		int 		d_numParticles,			// The number of particles in the simulation
			 		d_numShearSprings,		// The number of Shear springs in the simulation
					d_numStructSprings,		// The number of Structual springs in the simulation
					d_numFaces,				// The number of faces in the cloths geometry
					d_windFactor,			// The scale factor for the wind
					d_numCol,				// The number of columns
					d_numRow;
	
		float 		d_shearSpringConst,		// The spring constant for the shear springs
					d_structSpringConst,	// The spring constant for the structural springs
					d_dampConst,			// The dampening constant for the springs
					d_wTexStep,				// The width step for the tex coords
					d_hTexStep;				// The height step for the tex coords 
		
		Vector3 d_windVector;				// The vector for the wind effecting the cloth
	
		//----------------------------------------------------------------------
		// Private Methods
		//----------------------------------------------------------------------
		
		// Calculate the forces on the particles
		void calculateForces(); 
		
		
	public:
		//----------------------------------------------------------------------
		// Public Members
		//----------------------------------------------------------------------
	
	
		//----------------------------------------------------------------------
		// Public Methods
		//----------------------------------------------------------------------
		Cloth();						// Constructor 
                ~Cloth() { clear(); } 	// Destructor
	
		// Clear the cloth 
		void clear();
	
		// Draw the cloth
		void draw(bool texture);

		// Draws the springs
		void drawSprings();
	
		// Draws the particles
		void drawParticles();
		
		// Steps the simulation for the time interval dt
		void stepSimulation(float dt);
		
		// Scales the wind
                void scaleWind(int w) { d_windFactor = w; }
		
		// Modify the wind vector effecting the cloth
		void changeWindVector(float x, float y, float z);
		
		// Returns the total number of faces
                int getFaceCount() { return d_numFaces; }
		
		// Returns the total number of springs
                int getSpringCount() { return d_numShearSprings + d_numStructSprings; }
	
		// Returns the total number of particles
                int getParticleCount() { return d_numParticles; }
		
		// Sets the passed particle as locked
                void lockParticle(int p) { if(p >= 0 && p < d_numParticles) d_particles[p].locked = true; }
	
		// Inililize the cloth's particles
		void initialize(float width, float height, int numRow, int numCol, float mass, 
						float tension, float shear, float damp, int axis);
};


#endif
