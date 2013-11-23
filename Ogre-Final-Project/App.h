#pragma once

#include "stdafx.h"

class App :
	public ExampleApplication
{
private:
	Ogre::FrameListener* frameListener;

public:
	App();
	~App();

	void createScene();
	void createCamera();
	void createFrameListener();
};

