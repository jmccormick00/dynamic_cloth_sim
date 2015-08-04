/*==============================================================================
/ James McCormick - cloth.cpp
/ A class to simulate cloth in real time.
/=============================================================================*/


//==============================================================================
// INCLUDED LIBRARIES AND FILES
//==============================================================================
#include "cloth.h"
#include <GL/gl.h>
#include <time.h>

//==============================================================================
// CONSTRUCTORS / DESTRUCTORS
//==============================================================================

//------------------------------------------------------------------------------
// Constructor
// Initializes the pointers to null.
//------------------------------------------------------------------------------
Cloth::Cloth()
{
	d_shearSprings = NULL;
	d_structuralSprings = NULL;
	d_particles = NULL;
	d_faces = NULL;
	d_numShearSprings = 0;
	d_numStructSprings = 0;
	d_numFaces = 0;
	d_numParticles = 0;
}


//==============================================================================
// PRIVATE METHODS
//==============================================================================

//------------------------------------------------------------------------------
// void calculateForces()
//
// Used by the stepSimulation() function to calculate the combined forces acting
// on the particles.
//------------------------------------------------------------------------------
void Cloth::calculateForces()
{
	Vector3 drag, f, d, v, wind;
	
	// seed for rand
	srand((unsigned int)time(0));
	
	// Zero all the forces on all particles
	for(int i = 0; i < d_numParticles; i++)
	{
                d_particles[i].forces.X() = 0.0f;
                d_particles[i].forces.Y() = 0.0f;
                d_particles[i].forces.Z() = 0.0f;
	}
	
	// process the gravity, drag, and wind forces
	for(int i = 0; i < d_numParticles; i++)
	{
		if(!d_particles[i].locked)
		{
			// gravity
                        d_particles[i].forces.Y() += (float)(GRAVITY * d_particles[i].mass);
			
			// Viscous drag
			drag = -d_particles[i].velocity;
			drag.normalize();
			d_particles[i].forces += drag * (d_particles[i].velocity.magnitude() *
							 d_particles[i].velocity.magnitude()) * DRAGCOEFFICIENT;

			// wind
                        if(d_windVector.X() != 0.0 && d_windVector.Y() != 0.0 && d_windVector.Z() != 0.0 && d_windFactor != 0)
			{
				//wind = Vector(randFloat(0.0, d_windVector.x), randFloat(0.0, d_windVector.y), randFloat(0.0, d_windVector.z));
                                wind.X() = randFloat(0.0, d_windVector.X());
                                wind.Y() = randFloat(0.0, d_windVector.Y());
                                wind.Z() = randFloat(0.0, d_windVector.Z());
				wind.normalize();
				d_particles[i].forces += wind * (float)(rand() % d_windFactor);
			}
		}
	}
	
	// Process the forces from the shear springs
	for(int i = 0; i < d_numShearSprings; i++)
	{
		d = d_shearSprings[i].p1->position - d_shearSprings[i].p2->position;
		v = d_shearSprings[i].p1->velocity - d_shearSprings[i].p2->velocity;
		
		f = -(d_shearSpringConst * (d.magnitude() - d_shearSprings[i].restLength) +
				d_dampConst * ((v * d) / d.magnitude())) * (d / d.magnitude());
		
		if(!d_shearSprings[i].p1->locked)
			d_shearSprings[i].p1->forces += f;
		
		if(!d_shearSprings[i].p2->locked)
			d_shearSprings[i].p2->forces += (-f);
	}
	
	// Process the forces from the structural springs
	for(int i = 0; i < d_numStructSprings; i++)
	{
		d = d_structuralSprings[i].p1->position - d_structuralSprings[i].p2->position;
		v = d_structuralSprings[i].p1->velocity - d_structuralSprings[i].p2->velocity;
		
		f = -(d_structSpringConst * (d.magnitude() - d_structuralSprings[i].restLength) +
				d_dampConst * ((v * d) / d.magnitude())) * (d / d.magnitude());
		
		if(!d_structuralSprings[i].p1->locked)
			d_structuralSprings[i].p1->forces += f;
		
		if(!d_structuralSprings[i].p2->locked)
			d_structuralSprings[i].p2->forces += (-f);
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
void Cloth::clear()
{
	if(d_shearSprings != NULL)
	{
		delete[] d_shearSprings;
		d_shearSprings = NULL;
	}
	if(d_structuralSprings != NULL)
	{
		delete[] d_structuralSprings;
		d_structuralSprings = NULL;
	}
	if(d_particles != NULL)
	{
		delete[] d_particles;
		d_particles = NULL;
	}
	if(d_faces != NULL)
	{
		delete[] d_faces;
		d_faces = NULL;
	}
	d_numShearSprings = 0;
	d_numStructSprings = 0;
	d_numFaces = 0;
	d_numParticles = 0;
}



//------------------------------------------------------------------------------
// void draw()
//
// Draws the cloth.
//------------------------------------------------------------------------------
void Cloth::draw(bool texture)
{
	int fCount = 0;
	if(texture) glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
		for(int i = 0; i < (d_numRow - 1); i++)
		{
			for(int j = 0; j < (d_numCol - 1); j++)
			{
				if(texture) glTexCoord2f(j * d_wTexStep, i * d_hTexStep);
                                glVertex3fv(d_faces[fCount].p1->position.getVector());
				if(texture) glTexCoord2f((j + 1) * d_wTexStep, i * d_hTexStep);
                                glVertex3fv(d_faces[fCount].p2->position.getVector());
				if(texture) glTexCoord2f(j * d_wTexStep, (i + 1) * d_hTexStep);
                                glVertex3fv(d_faces[fCount].p3->position.getVector());
				
				if(texture) glTexCoord2f((j + 1) * d_wTexStep, i * d_hTexStep);
                                glVertex3fv(d_faces[fCount + 1].p1->position.getVector());
				if(texture) glTexCoord2f((j + 1) * d_wTexStep, (i + 1) * d_hTexStep);
                                glVertex3fv(d_faces[fCount + 1].p2->position.getVector());
				if(texture) glTexCoord2f(j * d_wTexStep, (i + 1) * d_hTexStep);
                                glVertex3fv(d_faces[fCount + 1].p3->position.getVector());
				fCount += 2;
			}
		}
	glEnd();
	if(texture) glDisable(GL_TEXTURE_2D);
}





//------------------------------------------------------------------------------
// void drawParticles()
//
// Draws the particles as red points.  Locked particles are displayed as green
//------------------------------------------------------------------------------
void Cloth::drawParticles()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POINTS);
		for(int j = 0; j < d_numParticles; j++)
		{
			if(d_particles[j].locked) 
				glColor3f(0.0, 1.0, 0.0);
			else
				glColor3f(1.0, 0.0, 0.0);
                        glVertex3fv(d_particles[j].position.getVector());
		}
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}



//------------------------------------------------------------------------------
// void drawSprings()
//
// Draws the springs as blue lines.
//------------------------------------------------------------------------------
void Cloth::drawSprings()
{
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
		for(int j = 0; j < d_numShearSprings; j++)
		{
                        glVertex3fv(d_shearSprings[j].p1->position.getVector());
                        glVertex3fv(d_shearSprings[j].p2->position.getVector());
		}
		for(int j = 0; j < d_numStructSprings; j++)
		{
                        glVertex3fv(d_structuralSprings[j].p1->position.getVector());
                        glVertex3fv(d_structuralSprings[j].p2->position.getVector());
		}
	glEnd();
	glColor3f(1.0, 1.0, 1.0);
}


//------------------------------------------------------------------------------
// void changeWindVector()
//
// Modifies the wind Vector.
//------------------------------------------------------------------------------
void Cloth::changeWindVector(float x, float y, float z)
{
        d_windVector.X() = x;
        d_windVector.Y() = y;
        d_windVector.Z() = z;
}


//------------------------------------------------------------------------------
// void stepSimulation()
//
// Performs the simulation for the next time interval.
// 
// Input: 	dt - The time interval in which to calculate the simulation.
//------------------------------------------------------------------------------
void Cloth::stepSimulation(float dt)
{
	Vector3 temp;
	
	// Calculate the forces for all the particles
	this->calculateForces();
	
	 
	// Integrate (Euler) the equations of motion 
	for(int i = 0; i < d_numParticles; i++)
	{
		temp = d_particles[i].forces * d_particles[i].invMass;
		d_particles[i].acceleration = temp;
		d_particles[i].velocity += temp * dt;
		d_particles[i].position += d_particles[i].velocity * dt;
	}
	
	// Check for collisions here
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
void Cloth::initialize(float width, float height, int numRow, int numCol, float mass, 
					   float structural, float shear, float damp, int axis)
{	
	float 	wBound,			// The width boundary of the cloth.
		 	hBound,			// The height boundary of the cloth.
		  	wStep, 			// The step size in between the columns. 
			hStep,			// The step size in between the rows.
			faceMass;		// The mass of one face
	
	float f;
	
	Vector3 length;			// Used to calculate the rest length of the spring
	
	// Clear out any memory that may have been allocated
	this->clear();
	
	// Initialize the spring constants
	d_shearSpringConst = shear;
	d_structSpringConst = structural;
	d_dampConst = damp;
	
	d_numCol = numCol;
	d_numRow = numRow;
	
	// Initialize the wind factor to 0
	d_windFactor = 0;
	
	// Calculate the total number of faces
	d_numFaces = (numCol - 1) * (numRow - 1) * 2;
	
	// Define the mass of the cloth
	faceMass = mass / (float)d_numFaces;
	
	// Calculate the x and y bound of the cloth
	wBound = width / 2.0f;
	hBound = height / 2.0f;
	
	// Start in the negative side of 0,0,0 and work towards the positive
	wBound = -wBound;
	
	// Calculate the yStep and xStep
	wStep = width / numCol;
	hStep = height / numRow;
	
	// Calculate the variables used for texture mapping
	d_wTexStep = wStep / width;
	d_hTexStep = hStep / height;
	
	// Calculate the total number of particles
	d_numParticles = numCol * numRow;
	
	// Allocate the memory for the particles
	d_particles = new Particle[d_numParticles];
	
	// Initialize the particles
	for(int i = 0; i < numRow; i++)
	{
		for(int j = 0; j < numCol; j++)
		{
			// Set the mass of the particle
			if((i == 0) && (j == 0)) f = 1;
			else if((i == (numRow - 1)) && (j == 0)) f = 2;
			else if((i == 0) && (j == (numCol - 1))) f = 2;
			else if((i == (numRow - 1)) && (j == (numCol - 1))) f = 1;
			else if(((i == 0) || (i == (numRow - 1))) && ((j != 0) && (j != (numCol - 1)))) f = 3;
			else if(((j == 0) || (j == (numCol - 1))) && ((i != 0) && (i != (numRow - 1)))) f = 3;
			else f = 6;	
			
			d_particles[(i * numCol) + j].mass = (f * faceMass) / 3;
			d_particles[(i * numCol) + j].invMass = 1.0 / d_particles[(i * numCol) + j].mass;
			
			// Set the intial position for this particle
                        d_particles[(i * numCol) + j].position.X() = wBound + (j * wStep);
			if(axis == ZAXIS)
			{
                                d_particles[(i * numCol) + j].position.Z() = hBound - (i * hStep);
                                d_particles[(i * numCol) + j].position.Y() = 0.0f;
			}
			else
			{
                                d_particles[(i * numCol) + j].position.Y() = hBound - (i * hStep);
                                d_particles[(i * numCol) + j].position.Z() = 0.0f;
			}
			
			// Set the initial veclocity for this particle
                        d_particles[(i * numCol) + j].velocity.X() = 0.0f;
                        d_particles[(i * numCol) + j].velocity.Y() = 0.0f;
                        d_particles[(i * numCol) + j].velocity.Z() = 0.0f;
			
			// Set the initial acceleration for this particle
                        d_particles[(i * numCol) + j].acceleration.X() = 0.0f;
                        d_particles[(i * numCol) + j].acceleration.Y() = 0.0f;
                        d_particles[(i * numCol) + j].acceleration.Z() = 0.0f;
			
			// Set the initial forces for this particle
                        d_particles[(i * numCol) + j].forces.X() = 0.0f;
                        d_particles[(i * numCol) + j].forces.Y() = 0.0f;
                        d_particles[(i * numCol) + j].forces.Z() = 0.0f;
			
			// Make them all false to begin with
			d_particles[(i * numCol) + j].locked = false;
		}
	}
	
	// Allocate the memory for the faces
	d_faces = new Face[d_numFaces];
	
	// Set up the faces for the cloth's geometry
	int fCount = 0;
	for(int i = 0; i < (numRow - 1); i++)
	{
		for(int j = 0; j < (numCol - 1); j++)
		{	
			d_faces[fCount].p1 = &d_particles[(i * numCol) + j];
			d_faces[fCount].p2 = &d_particles[(i * numCol) + j + 1];
			d_faces[fCount].p3 = &d_particles[((i + 1) * numCol) + j];
			
			d_faces[fCount + 1].p1 = &d_particles[(i * numCol) + j + 1];
			d_faces[fCount + 1].p2 = &d_particles[((i + 1) * numCol) + j + 1];
			d_faces[fCount + 1].p3 = &d_particles[((i + 1) * numCol) + j];
			
			fCount += 2;
		}
	}
	
	// Calculate the total number of springs
	d_numShearSprings = ((numCol - 1) * (numRow - 1)) * 2;
	d_numStructSprings = (numCol * (numRow - 1)) + (numRow * (numCol - 1));
	
	// Allocate memory for the springs
	d_shearSprings = new Spring[d_numShearSprings];
	d_structuralSprings = new Spring[d_numStructSprings];
	
	// Set up the springs
	int shearCount = 0;
	int structCount = 0;
	for(int i = 0; i < numRow; i++)
	{
		for(int j = 0; j < numCol; j++)
		{	
			// The structural spring going across the row
			if(j <= (numCol - 2))
			{
				d_structuralSprings[structCount].p1 = &d_particles[(i * numCol) + j];
				d_structuralSprings[structCount].p2 = &d_particles[(i * numCol) + j + 1];
				length = d_structuralSprings[structCount].p1->position - 
						 d_structuralSprings[structCount].p2->position;
				d_structuralSprings[structCount].restLength = length.magnitude();
				structCount++;
			}
			
			// The structural spring going down a column
			if(i <= (numRow - 2))
			{
				d_structuralSprings[structCount].p1 = &d_particles[(i * numCol) + j];
				d_structuralSprings[structCount].p2 = &d_particles[((i + 1) * numCol) + j];
				length = d_structuralSprings[structCount].p1->position - 
						 d_structuralSprings[structCount].p2->position;
				d_structuralSprings[structCount].restLength = length.magnitude();
				structCount++;
			}
			
			// The shear spring going diagonal from left to right 
			if((j <= (numCol - 2)) && (i <= (numRow - 2)))
			{
				d_shearSprings[shearCount].p1 = &d_particles[(i * numCol) + j];
				d_shearSprings[shearCount].p2 = &d_particles[((i + 1) * numCol) + j + 1];
				length = d_shearSprings[shearCount].p1->position - 
						 d_shearSprings[shearCount].p2->position;
				d_shearSprings[shearCount].restLength = length.magnitude();
				shearCount++;
			}
			
			// The shear spring going diagonal from right to left
			if(j > 0 && i <= (numRow - 2))
			{
				d_shearSprings[shearCount].p1 = &d_particles[(i * numCol) + j];
				d_shearSprings[shearCount].p2 = &d_particles[((i + 1) * numCol) + j - 1];
				length = d_shearSprings[shearCount].p1->position - 
						 d_shearSprings[shearCount].p2->position;
				d_shearSprings[shearCount].restLength = length.magnitude();
				shearCount++;
			}
		}
	}
	
	// Finally initialize the wind vector
        d_windVector.X() = 0.0;
        d_windVector.Y() = 0.0;
        d_windVector.Z() = 0.0;
}
