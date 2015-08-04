#include "Camera3D.h"
#include <GL/glu.h>

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
Camera3D::Camera3D()
{
        d_camPos.Z() = 1.0f;
        d_camPos.X() = d_camPos.Y() = 0.0f;

        d_camTarget.X() = d_camTarget.Y() = d_camTarget.Z() = 0.0f;

        d_camUpVector.Y() = 1.0f;
        d_camUpVector.X() = d_camUpVector.Z() = 0.0f;

        d_sceneUpVector.Y() = 1.0f;
        d_sceneUpVector.X() = d_sceneUpVector.Z() = 0.0f;

        // Calculate the side vector
        vector3f look = d_camTarget - d_camPos;
        d_camSideVector = look.crossProduct(d_camUpVector);
        d_camSideVector.normalize();
}

Camera3D::Camera3D(vector3f pos, vector3f target, vector3f camUp, vector3f sceneUp)
{
        d_camPos = pos;
        d_camTarget = target;
        d_camUpVector = camUp;
        d_sceneUpVector = sceneUp;

        d_sceneUpVector.normalize();
        d_camUpVector.normalize();

        // Calculate the side vector
        vector3f look = d_camTarget - d_camPos;
        d_camSideVector = look.crossProduct(d_camUpVector);
        d_camSideVector.normalize();
}


//------------------------------------------------------------------------------
// setPosition()
// Sets the camera's postion
//------------------------------------------------------------------------------
void Camera3D::setPosition(vector3f& pos)
{
        d_camPos = pos;
}


//------------------------------------------------------------------------------
// setTarget()
// Sets the camera's target
//------------------------------------------------------------------------------
void Camera3D::setTarget(vector3f& target)
{
        d_camTarget = target;
}


//------------------------------------------------------------------------------
// setUpVector()
// Sets the camera's up vector
//------------------------------------------------------------------------------
void Camera3D::setUpVector(vector3f& up)
{
        d_camUpVector = up;
}


//------------------------------------------------------------------------------
// setSceneUpVector()
// Sets the scene's up vector
//------------------------------------------------------------------------------
void Camera3D::setSceneUpVector(vector3f& up)
{
        d_sceneUpVector = up;
}

//------------------------------------------------------------------------------
// getPosition()
// Gets the camera's position
//------------------------------------------------------------------------------
vector3f Camera3D::getPosition()
{
        return d_camPos;
}

//------------------------------------------------------------------------------
// getTarget()
// Gets the camera's target
//------------------------------------------------------------------------------
vector3f Camera3D::getTarget()
{
        return d_camTarget;
}

//------------------------------------------------------------------------------
// getUpVector()
// Gets the camera's up vector
//------------------------------------------------------------------------------
vector3f Camera3D::getUpVector()
{
        return d_camUpVector;
}


//------------------------------------------------------------------------------
// getSceneUpVector()
// Gets the scene's up vector
//------------------------------------------------------------------------------
vector3f Camera3D::getSceneUpVector()
{
        return d_sceneUpVector;
}

//------------------------------------------------------------------------------
// updateCamera()
// Sets the camera so that the scene is drawn
//------------------------------------------------------------------------------
void Camera3D::updateCamera()
{
    gluLookAt(d_camPos.X(), d_camPos.Y(), d_camPos.Z(),
              d_camTarget.X(), d_camTarget.Y(), d_camTarget.Z(),
              d_camUpVector.X(), d_camUpVector.Y(), d_camUpVector.Z() );
}

//------------------------------------------------------------------------------
// panHorizontal()
// Pans the camera horizontally
//------------------------------------------------------------------------------
void Camera3D::panHorizontal(float x)
{
        d_camPos += d_camSideVector * x;
        d_camTarget += d_camSideVector * x;
}


//------------------------------------------------------------------------------
// panVertical()
// Pans the camera vertically
//------------------------------------------------------------------------------
void Camera3D::panVertical(float y)
{
        d_camPos += d_camUpVector * y;
        d_camTarget += d_camUpVector * y;
}


//------------------------------------------------------------------------------
// zoomCamera()
// Moves the camera forward and backwards
//------------------------------------------------------------------------------
void Camera3D::zoomCamera(float amount)
{
        vector3f look = d_camTarget - d_camPos;
        float length = look.magnitude();
        look.normalize();

        vector3f temp = look * amount;
        float len2 = temp.magnitude();

        if( amount > 0.0 )
        {
                if( (length - len2) > 1.0 )
                        d_camPos += temp;
        }
        else
                d_camPos += temp;
}


//------------------------------------------------------------------------------
// orbitX()
// Orbits the camera on the camera's X axis around the camera's target
//------------------------------------------------------------------------------
void Camera3D::orbitX(float amount)
{
        vector3f look = d_camTarget - d_camPos;

        float r = look.magnitude();
        float w = r - (r * cos(amount));
        float h = r * sin(amount);

        look.normalize();
        d_camPos += ((look * w) + (d_camSideVector * h));

        // Update the side Vector
        look = d_camTarget - d_camPos;
        d_camSideVector = look.crossProduct(d_camUpVector);
        d_camSideVector.normalize();
}


//------------------------------------------------------------------------------
// orbitY()
// Orbits the camera on the camera's Y axis around the camera's target
//------------------------------------------------------------------------------
void Camera3D::orbitY(float amount)
{
        vector3f look = d_camTarget - d_camPos;

        float r = look.magnitude();
        float w = r - (r * cos(amount));
        float h = r * sin(amount);

        look.normalize();
        d_camPos += ((look * w) + (d_camUpVector * h));

        // Update the up vector
        look = d_camTarget - d_camPos;
        d_camUpVector = d_camSideVector.crossProduct(look);
        d_camUpVector.normalize();
}
