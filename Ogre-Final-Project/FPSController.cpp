#include "FPSController.h"


FPSController::FPSController(Ogre::SceneNode* _parent, Ogre::Camera* _cam, XboxControllerManager* _xboxCtrl) :
	parent(_parent),
	cam(_cam),
	xboxCtrl(_xboxCtrl),
	cameraYawNode(nullptr),
	cameraPitchNode(nullptr),
	cameraRollNode(nullptr),
	cameraNode(nullptr),
	accel(Ogre::Vector3::ZERO),
	velocity(50, 50, 50)
{
	setUpNodeHierarchy();
}

FPSController::~FPSController(void)
{
}

void FPSController::onFrameRenderingQueued(const App::FrameEventArgs* args)
{
	const auto& leftStick = xboxCtrl->p1.getState().leftStick;
	const auto& rightStick = xboxCtrl->p1.getState().rightStick;

	accel = Ogre::Vector3(leftStick.x, 0, -leftStick.y).normalisedCopy();

	rotX = Ogre::Degree(-rightStick.x);
	rotY = Ogre::Degree(rightStick.y);

	moveCamera(args->evt->timeSinceLastFrame);
}

const Ogre::Ray& FPSController::getCameraRay() const
{
	return camRay;
}

const Ogre::AxisAlignedBox& FPSController::getCameraAABB() const
{
	return cam->getWorldBoundingBox();
}

const Ogre::Vector3& FPSController::getWorldPosition() const
{
	return cameraNode->_getDerivedPosition();
}

Ogre::SceneNode* FPSController::getCamNode()
{
	return cameraNode;
}

void FPSController::moveCamera(Ogre::Real dt)
{
	Ogre::Real pitchAngle, pitchAngleSign;

	// Yaw the camera according to the mouse relative movement.
	cameraYawNode->yaw(rotX);

	// Pitch the camera according to the mouse relative movement.
	cameraPitchNode->pitch(rotY);

	// Move the camera around the world.
	auto normDir = accel * velocity * dt;
	normDir.normalise();

	cameraNode->translate(cameraYawNode->getOrientation() * cameraPitchNode->getOrientation() * normDir, Ogre::Node::TS_LOCAL);

	// Angle of rotation around the X-Axis.
	pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(cameraPitchNode->getOrientation().w)).valueDegrees());

	// Just to determine the sign of the angle we pick up above, the value
	// itself does not interest us.
	pitchAngleSign = cameraPitchNode->getOrientation().x;

	// Limit the pitch between -90 and +90 degrees, Quake3 style.
	if (pitchAngle > 90.0f) {
		if (pitchAngleSign > 0) {
			// Set orientation to 90 degrees on X-Axis.
			cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), Ogre::Math::Sqrt(0.5f), 0, 0));
		} else if (pitchAngleSign < 0) {
			// Set orientation to -90 degrees on X-Axis.
			cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f), -Ogre::Math::Sqrt(0.5f), 0, 0));
		}
	} 

	// TODO: Terrain clamp

	// Notify cameraNode's children.
	cameraYawNode->needUpdate();
	cameraPitchNode->needUpdate();
	cameraRollNode->needUpdate();

	// Update the camera ray.
	camRay.setOrigin(cam->getRealPosition());
	camRay.setDirection(cam->getRealDirection());
}

void FPSController::setUpNodeHierarchy()
{
	// Create the camera's top node (which will only handle position).
	cameraNode = parent->createChildSceneNode();
	cameraNode->setPosition(0, 7.5, 125);
 
	// Create the camera's yaw node as a child of camera's top node.
	cameraYawNode = cameraNode->createChildSceneNode();
 
	// Create the camera's pitch node as a child of camera's yaw node.
	cameraPitchNode = cameraYawNode->createChildSceneNode();
 
	// Create the camera's roll node as a child of camera's pitch node
	// and attach the camera to it.
	cameraRollNode = cameraPitchNode->createChildSceneNode();
	cameraRollNode->attachObject(cam);
}