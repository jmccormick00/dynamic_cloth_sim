#ifndef I_PARTICLESYSTEM_H
#define I_PARTICLESYSTEM_H


#include "C_Vertex.h"

template<class real>
class I_ParticleSystem
{
public:
    I_ParticleSystem() : d_dragCoef(0), d_pPositions(0), d_pOldPositions(0), d_pAccel(0)
    {}

    virtual ~I_ParticleSystem()
    {
        wipeParticleData();
    }

    virtual void draw() = 0;
    virtual void stepSimulation(const real& dt)
    {
        d_dt = dt;
        sumForces();
        integrate();
        applyConstraints();
    }

    virtual void setGravity(const vector3f& g) { d_vGravity = g; }

protected:

    virtual void integrate()
    {
        vector3f temp;
        for(unsigned i = 0; i < d_uNumParticles; ++i)
        {
            vector3f& pos = d_pPositions[i].pos;
            vector3f& oldPos = d_pOldPositions[i];
            vector3f& a = d_pAccel[i];
            temp = pos;
            //pos += pos - oldPos + (a * d_dt * d_dt);
            pos = (2.0-d_dragCoef)*pos - (1.0-d_dragCoef)*oldPos + (a * d_dt * d_dt);
            oldPos = temp;
        }
    }

    virtual void sumForces() = 0;
    virtual void applyConstraints() = 0;
    virtual void initParticleData(unsigned particleCount)
    {
        d_uNumParticles = particleCount;
        d_pPositions = new C_Vertex[particleCount];
        d_pOldPositions = new vector3f[particleCount];
        d_pAccel = new vector3f[particleCount];
    }

    virtual void wipeParticleData()
    {
        if(d_pPositions)
        {
            delete d_pPositions;
            d_pPositions = 0;
        }
        if(d_pOldPositions)
        {
            delete d_pOldPositions;
            d_pOldPositions = 0;
        }
        if(d_pAccel)
        {
            delete d_pAccel;
            d_pAccel = 0;
        }

        d_uNumParticles = 0;
    }

    real d_dt;
    real d_dragCoef;
    unsigned d_uNumParticles;
    C_Vertex* d_pPositions;
    vector3f* d_pOldPositions;
    vector3f* d_pAccel;
    vector3f d_vGravity;
};

#endif // I_PARTICLESYSTEM_H
