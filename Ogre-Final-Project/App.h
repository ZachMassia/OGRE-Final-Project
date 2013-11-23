#pragma once

#include "stdafx.h"

class App :
	public ExampleApplication,
	public Ogre::FrameListener
{
private:
	//Ogre::FrameListener* frameListener;
	
	// All callbacks for frame events will be passed this as the
	// only argument.
	struct FrameEventArgs
	{
		OIS::Keyboard* key;
		OIS::Mouse* mouse;
		const Ogre::FrameEvent* evt;

		// Constructor
		FrameEventArgs(OIS::Keyboard* _key = nullptr,
						OIS::Mouse* _mouse = nullptr,
						const Ogre::FrameEvent* _evt = nullptr) :
			key(_key), mouse(_mouse), evt(_evt) {}
	};
	
	Event<FrameEventArgs*> OnFrameStarted;
	Event<FrameEventArgs*> OnFrameRenderingQueued;



	OIS::InputManager* inputMgr;
	OIS::Keyboard* keyboard;
	OIS::Mouse* mouse;


public:
	App();
	~App();

	// Example Application Overrides //
	void createScene();
	void createCamera();
	void createFrameListener();

	// Frame Listener Overrides //
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
	void setupOIS();
	void createFrameCallbacks();
};

