#include "StdAfx.h"
#include "GameObject.h"


GameObject::GameObject(
	const std::string& _name,
	const std::string& _meshName,
	Ogre::SceneNode* _parentNode,
	Ogre::SceneManager* sceneMgr) : alive(true),
									acceleration(Ogre::Vector3::ZERO),
									velocity(Ogre::Vector3::ZERO),
									name(_name),
									meshName(_meshName),
									parentNode(_parentNode),
									node(nullptr),
									entity(nullptr)

{
	if (parentNode == nullptr || sceneMgr == nullptr) {
		throw std::runtime_error("Cannot initialize object without a parentNode or sceneMgr");
	}

	init(sceneMgr);
}


GameObject::~GameObject()
{
}

void GameObject::init(Ogre::SceneManager* sceneMgr)
{
	if (name.length() > 0) {
		entity = sceneMgr->createEntity(name, meshName);
		node = parentNode->createChildSceneNode(name + "Node");
	}
	else { // Use generated name if no name was passed in.
		entity = sceneMgr->createEntity(meshName);
		node = parentNode->createChildSceneNode();
	}
	node->attachObject(entity);
}

void GameObject::update(const Ogre::FrameEvent* evt)
{
	// Don't update dead objects.
	if (!alive) {
		return;
	}

	node->translate(acceleration * velocity * evt->timeSinceLastFrame);

	// Update the object's ray.
	ray.setOrigin(node->_getDerivedPosition());
	ray.setDirection(acceleration.normalisedCopy());
}

#pragma region Getters / Setters
const bool 
GameObject::getAlive() const
{
	return alive;
}

void 
GameObject::setAlive(bool b)
{
	alive = b;
}

const Ogre::Vector3& 
GameObject::getAcceleration() const
{
	return acceleration;
}

void 
GameObject::setAcceleration(const Ogre::Vector3& a)
{
	acceleration = a;
}

const Ogre::Vector3& 
GameObject::getVelocity() const
{
	return velocity;
}

void 
GameObject::setVelocity(const Ogre::Vector3& v)
{
	velocity = v;
}

const std::string& 
GameObject::getName() const
{
	return name;
}

const std::string& 
GameObject::getMeshName() const
{
	return meshName;
}

const Ogre::Vector3& 
GameObject::getPosition() const
{
	return node->getPosition();
}

const Ogre::Vector3& 
GameObject::getWorldPosition() const
{
	return node->_getDerivedPosition();
}

Ogre::SceneNode* 
GameObject::getNode()
{
	return node;
}

const Ogre::AxisAlignedBox&
GameObject::getAABB() const
{
	return entity->getWorldBoundingBox();
}

const Ogre::Ray&
GameObject::getRay() const
{
	return ray;
}
#pragma endregion