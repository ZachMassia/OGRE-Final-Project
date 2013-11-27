#pragma once

#include "StdAfx.h"
#include "App.h"
#include "GameObject.h"

class Game : public App
{
private:
	GameObject* sinbad;
	Ogre::AnimationState* animStates[4];

public:
	Game();
	virtual ~Game();

	// These methods must be implemented.
	void createScene();
	void createGui();

protected:
	void createCallbacks();
	void setupSdkCameraMan();
};

