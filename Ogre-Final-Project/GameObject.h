#pragma once

#include "stdafx.h"

class GameObject
{
protected:
	// Dead objects are not updated or rendered.
	bool alive;

	Ogre::Vector3 acceleration, velocity;
	
	// Ray which is cast in the direction the object is moving.
	//
	// TODO: Some way to factor in the meshes initial looking direction.
	Ogre::Ray ray; 

	// When not empty, the object's node will be named {name}Node.
	const std::string name;

	// Mesh file used for this object.
	const std::string meshName;

	// The object's parent node.
	Ogre::SceneNode* parentNode;

	// The object's main node.
	Ogre::SceneNode* node;

	// The object's entity.
	Ogre::Entity* entity;

public:
	GameObject(
		const std::string& _name,
		const std::string& _meshName,
		Ogre::SceneNode* _parentNode,
		Ogre::SceneManager* sceneMgr
	);
	virtual ~GameObject();

	virtual void update(const Ogre::FrameEvent* evt);

protected:
	// Init creates the entity using the mesh name, and optionally,
	// the node name.
	// 
	// The entity is attached to it's own node which is a child of
	// the node passed to the constructor.
	virtual void init(Ogre::SceneManager* sceneMgr);

#pragma region Getters / Setters
public:
	// The current state of this object.
	const bool getAlive() const;
	// Set the current state of this object.
	// If b is false, this object will no longer be updated or rendered.
	void setAlive(bool b);

	// The current acceleration.
	const Ogre::Vector3& getAcceleration() const;
	// Set the acceleration.
	void setAcceleration(const Ogre::Vector3& a);
	
	// The current velocity.
	const Ogre::Vector3& getVelocity() const;
	// Set the velocity.
	void setVelocity(const Ogre::Vector3& v);

	// The object's name.
	const std::string& getName() const;

	// The object's mesh name.
	const std::string& getMeshName() const;

	// The current position.
	const Ogre::Vector3& getPosition() const;

	// The current world position.
	const Ogre::Vector3& getWorldPosition() const;

	// The object's scene node.
	Ogre::SceneNode* getNode();

	// The object's axis aligned bounding box.
	const Ogre::AxisAlignedBox& getAABB() const;

	// The ray cast in the object's looking direction.
	const Ogre::Ray& getRay() const;
#pragma endregion
};

