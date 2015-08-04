/*=========================================================================
/ James McCormick - camera3D.h
/ A class for representing the camera in the 3D scene
/ The camera is not free roaming.  It fixed to look at a target.
/ Gives the user the ability to pan, orbit and zoom in relation to a target
/==========================================================================*/

#ifndef _CAMERA3D_
#define _CAMERA3D_

#include "vector3.h"

class Camera3D
{
private:
        vector3f d_camPos,			// Camera's position
        d_camTarget,		// The place where the camera looks
        d_camUpVector,		// The camera's up direction, the camera's Y axis
        d_camSideVector,	// The Camera's side direction, the camera's X axis
        d_sceneUpVector;	// The scene's up vector

public:
        Camera3D();
        Camera3D(vector3f pos, vector3f target, vector3f camUp, vector3f sceneUp);

        // updates the camera for the scene
        void updateCamera();

        // Set various components of the class
        void setPosition(vector3f& pos);
        void setTarget(vector3f& target);
        void setUpVector(vector3f& up);
        void setSceneUpVector(vector3f& up);

        // Get various compents from the class
        vector3f getPosition();
        vector3f getTarget();
        vector3f getUpVector();
        vector3f getSceneUpVector();

        // Pan the camera
        void panHorizontal(float x);
        void panVertical(float y);

        void zoomCamera(float amount);

        // 3D orbit the camera around the target
        void orbitX(float amount);
        void orbitY(float amount);
};




#endif
