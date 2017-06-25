#include "graphics.h"


#pragma once

class Camera {
public:
	Ogre::SceneNode* mCameraNode;
	Ogre::Camera* mCamera;
	Ogre::String mCameraName;
	Ogre::String mCameraNodeName;
public:

	Ogre::Quaternion mLookQuat;
	Ogre::Matrix3 mLookRotMat3;
	Ogre::Radian mRotY, mRotZ, mRotX;

	Ogre::Radian mRotateSpeed;
	Ogre::Real mMoveSpeed;

	Ogre::Vector3 mVelocity;


public:
	Camera() {}
	Camera(Graphics* graphics);
	virtual ~Camera();

	Ogre::Matrix4 getViewProj();
	void adjust(bool rotate, bool move = false);

	void translate(Ogre::Vector3 offset);
	void setPosition(Ogre::Vector3 pos);
	Ogre::SceneNode* getSceneNode();

	void setXRotation(Ogre::Radian rotX1);
	void setYRotation(Ogre::Radian rotY1);
	void setZRotation(Ogre::Radian rotZ1);

	void moveForward(Ogre::Real timeSinceLastFrame);
	void moveBackward(Ogre::Real timeSinceLastFrame);
	void rotateLeft(Ogre::Real timeSinceLastFrame);
	void rotateRight(Ogre::Real timeSinceLastFrame);
	void rotateUp(Ogre::Real timeSinceLastFrame);
	void rotateDown(Ogre::Real timeSinceLastFrame);

	void setPolyWireFrame();
	void setPolySolid();
	void setPolyPoints();

	void lookAt(Ogre::Vector3 position);
	void setOrthoView(Ogre::Real w, Ogre::Real h);

	void getViewportIntersection(Ogre::Plane plane, Ogre::Vector3& pointTopLeft, Ogre::Vector3& pointBotRight);
	Ogre::Vector3 getMouseIntersection(Ogre::Plane plane, Ogre::Vector2 mouse);
};

