#pragma once

#include "stdafx.h"
#include "App.h"
#include "XboxControllerManager.h"

// This class is based on http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Creating+a+simple+first-person+camera+system
class FPSController
{
public:
	FPSController(Ogre::SceneNode* _parent, Ogre::Camera* _cam, XboxControllerManager* _xboxCtrl);
	~FPSController(void);

	void onFrameRenderingQueued(const App::FrameEventArgs* args);

	const Ogre::Ray& getCameraRay() const;
	const Ogre::AxisAlignedBox& getCameraAABB() const;
	const Ogre::Vector3& getWorldPosition() const;
	Ogre::SceneNode* getCamNode();

protected:
	void moveCamera(Ogre::Real dt);
	void updateRay();
	void setUpNodeHierarchy();

	Ogre::SceneNode* parent;
	Ogre::Camera* cam;
	XboxControllerManager* xboxCtrl;
	
	Ogre::SceneNode* cameraYawNode;
	Ogre::SceneNode* cameraPitchNode;
	Ogre::SceneNode* cameraRollNode;
	Ogre::SceneNode* cameraNode;

	Ogre::Vector3 accel, velocity;

	Ogre::Ray camRay;

	Ogre::Radian rotX, rotY;
};
