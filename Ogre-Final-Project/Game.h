#pragma once

#include "StdAfx.h"
#include "App.h"


class Game : public App
{
private:
	Ogre::AnimationState* animStates[2];

public:
	Game();
	virtual ~Game();

	void createScene();
};

