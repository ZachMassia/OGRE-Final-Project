#pragma once

#include "StdAfx.h"
#include "App.h"
#include "GameObject.h"
#include "FPSController.h"

class Game : public App
{
private:
	FPSController* fpsCtrl;

	GameObject* sinbad;
	Ogre::AnimationState* animStates[4];

public:
	Game();
	virtual ~Game();

	// These methods must be implemented.
	void createScene();
	

protected:
	void createCallbacks();
	void setupSdkCameraMan();
	void createGui();
};

