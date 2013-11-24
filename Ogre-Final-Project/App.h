#pragma once

#include "stdafx.h"

class App :
	public ExampleApplication,
	public Ogre::FrameListener,
	public OgreBites::SdkTrayListener,
	public OIS::KeyListener,
	public OIS::MouseListener,
	public Ogre::WindowEventListener
{
protected:
#pragma region Event Args
	// I chose to wrap all the needed objects into a single
	// struct to avoid dealing with variadic templates. 
	// 
	// There may be a way to unpack T... using a loop and deleting
	// each one if std::is_pointer<T>::value is true.


	// All callbacks for frame events will be passed this as the
	// only argument.
	struct FrameEventArgs
	{
		const OIS::Keyboard* key;
		const OIS::Mouse* mouse;
		const Ogre::FrameEvent* evt;

		// Constructor
		FrameEventArgs(const OIS::Keyboard* _key = nullptr,
					   const OIS::Mouse* _mouse = nullptr,
					   const Ogre::FrameEvent* _evt = nullptr) :
			key(_key), mouse(_mouse), evt(_evt) {}
	};

	struct MouseEventArgs
	{
		const OIS::MouseEvent* evt;
		const OIS::MouseButtonID* id;

		// Constructor
		MouseEventArgs(const OIS::MouseEvent* _evt = nullptr,
					   const OIS::MouseButtonID* _id = nullptr) :
			evt(_evt), id(_id) {}
	};

	struct KeyEventArgs
	{
		const OIS::KeyEvent* evt;

		// Constructor
		KeyEventArgs(const OIS::KeyEvent* _evt = nullptr) : evt(_evt) {}
	};
#pragma endregion
	
	// Event Objects.
	Event<const FrameEventArgs*> OnFrameStarted;
	Event<const FrameEventArgs*> OnFrameRenderingQueued;
	Event<const KeyEventArgs*>   OnKeyPressed;
	Event<const KeyEventArgs*>   OnKeyReleased;
	Event<const MouseEventArgs*> OnMouseMoved; // id is not used.
	Event<const MouseEventArgs*> OnMousePressed;
	Event<const MouseEventArgs*> OnMouseReleased;

	// Input Objects.
	OIS::InputManager* inputMgr;
	OIS::Keyboard* keyboard;
	OIS::Mouse* mouse;

	// GUI
	OgreBites::SdkTrayManager* trayMgr;
	OgreBites::SdkCameraMan* cameraMan;

	// Returned by frameStarted and frameRenderingQueued.
	bool playing;


public:
	App();
	virtual ~App();

	// ExampleApplication 
	virtual void createScene() = 0;
	void createCamera();
	void createFrameListener();

	// Ogre::FrameListener
	bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	// Ogre::WindowEventListener
	void windowMoved(RenderWindow *rw) {}
	void windowResized(RenderWindow *rw);
	bool windowClosing(RenderWindow *rw) { return true; }
	void windowClosed(RenderWindow *rw) {}
	void windowFocusChange(RenderWindow *rw) {}

	// OIS::KeyListener
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	// OIS::MouseListener
	bool mouseMoved(const OIS::MouseEvent& evt);
	bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);

	// OgreBites::SdkTrayListener
	void buttonHit(OgreBites::Button* button);
	void itemSelected(OgreBites::SelectMenu* menu);
	void labelHit(OgreBites::Label* label);
	void sliderMoved(OgreBites::Slider* slider);
	void checkBoxToggled(OgreBites::CheckBox* box);
	void okDialogClosed(const Ogre::DisplayString& message);
	void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);

protected:
	// The implementing class should override this. It is still
	// necessary to call the base version.
	virtual void createCallbacks();

private:
	// createFrameListener is the last step performed by ExampleApplications
	// setup process. Any setup that App must do should be called in bootstrapSetup.
	void bootstrapSetup();
	
	// Initializes OIS in buffered mode.
	void setupOIS();

	// Initializes the SdkTrayManager and loads the required fonts.
	void setupSdkTrays();

	// Init SdkCameraMan, but essentially disable it until required
	// by setting the style to manual.
	void setupSdkCameraMan();
};

