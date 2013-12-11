#include "App.h"


App::App() :
	inputMgr(nullptr),
	keyboard(nullptr),
	mouse(nullptr),
	trayMgr(nullptr),
	cameraMan(nullptr),
	artifexLoader(nullptr),
	playing(true)
{
	// Seed the RNG for any future use.
	std::srand(std::time(0));
}


App::~App()
{
	// Shutdown OIS.
	if (inputMgr != nullptr) {
		inputMgr->destroyInputObject(keyboard);
		inputMgr->destroyInputObject(mouse);
		OIS::InputManager::destroyInputSystem(inputMgr);
	}
	// Shutdown SDK Trays
	if (trayMgr != nullptr) {
		delete trayMgr;
	}
	// Shutdown SDK CameraManager
	if (cameraMan != nullptr) {
		delete cameraMan;
	}

	if (artifexLoader != nullptr) {
		delete artifexLoader;
	}
}

#pragma region Listener Overrides
#pragma region ExampleApplication
void App::createCamera()
{
	ExampleApplication::createCamera(); // TODO: Override createCamera

	mCamera->setPosition(0.0f, 0.0f, 15.0f);
}

void App::createFrameListener()
{
	mRoot->addFrameListener(this);

	bootstrapSetup();

	
}
#pragma endregion

#pragma region Ogre::FrameListener
bool App::frameStarted(const Ogre::FrameEvent& evt)
{
	keyboard->capture();
	mouse->capture();

	// Notify event subscribers.
	OnFrameStarted.RaiseEvent(new FrameEventArgs(keyboard, mouse, &evt));

	return playing;
}

bool App::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	keyboard->capture();
	mouse->capture();

	// Notify event subscribers.
	OnFrameRenderingQueued.RaiseEvent(new FrameEventArgs(keyboard, mouse, &evt));

	return playing;
}
#pragma endregion

#pragma region Ogre::WindowEventListener
void App::windowResized(RenderWindow *rw)
{
	// Adjust mouse clipping area.
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mouse->getMouseState();
	ms.width = width;
	ms.height = height;
}
#pragma endregion

#pragma region OIS::KeyListener
bool App::keyPressed(const OIS::KeyEvent& evt)
{
	cameraMan->injectKeyDown(evt);

	OnKeyPressed.RaiseEvent(new KeyEventArgs(&evt));

	return true;
}

bool App::keyReleased(const OIS::KeyEvent& evt)
{
	cameraMan->injectKeyUp(evt);

	OnKeyReleased.RaiseEvent(new KeyEventArgs(&evt));

	return true;
}
#pragma endregion

#pragma region OIS::MouseListener
bool App::mouseMoved(const OIS::MouseEvent& evt)
{
	if (trayMgr->injectMouseMove(evt)) {
		return true;
	}

	cameraMan->injectMouseMove(evt);
	
	OnMouseMoved.RaiseEvent(new MouseEventArgs(&evt, nullptr));

	return true;
}

bool App::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if (trayMgr->injectMouseDown(evt, id)) {
		return true;
	}

	cameraMan->injectMouseDown(evt, id);

	OnMousePressed.RaiseEvent(new MouseEventArgs(&evt, &id));

	return true;
}

bool App::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if (trayMgr->injectMouseUp(evt, id)) {
		return true;
	}

	cameraMan->injectMouseUp(evt, id);

	OnMouseReleased.RaiseEvent(new MouseEventArgs(&evt, &id));

	return true;
}
#pragma endregion

#pragma region OgreBites::SdkTrayListener
void App::buttonHit(OgreBites::Button* button)
{
	OgreBites::SdkTrayListener::buttonHit(button);
}

void App::itemSelected(OgreBites::SelectMenu* menu)
{
	OgreBites::SdkTrayListener::itemSelected(menu);
}

void App::labelHit(OgreBites::Label* label)
{
	OgreBites::SdkTrayListener::labelHit(label);
}

void App::sliderMoved(OgreBites::Slider* slider)
{
	OgreBites::SdkTrayListener::sliderMoved(slider);
}

void App::checkBoxToggled(OgreBites::CheckBox* box)
{
	OgreBites::SdkTrayListener::checkBoxToggled(box);
}

void App::okDialogClosed(const Ogre::DisplayString& message)
{
	OgreBites::SdkTrayListener::okDialogClosed(message);
}

void App::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
	OgreBites::SdkTrayListener::yesNoDialogClosed(question, yesHit);
}
#pragma endregion
#pragma endregion

void App::bootstrapSetup()
{
	// Register App as WindowEventListener.
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	setupOIS();
	createCallbacks();
	setupSdkTrays();
	createGui();
	setupSdkCameraMan();
}

void App::setupOIS()
{
	using namespace OIS;

	size_t windowHandle = 0;
	std::stringstream windowHandleStr;

	// Get the window handle.
	mWindow->getCustomAttribute("WINDOW", &windowHandle);
	// Convert to a string.
	windowHandleStr << windowHandle;

	// Create the input system using the parameter list.
	ParamList pl;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHandleStr.str()));
	inputMgr = InputManager::createInputSystem(pl);
	
	// Create input objects.
	keyboard = static_cast<Keyboard*>(inputMgr->createInputObject(OISKeyboard, true));
	mouse = static_cast<Mouse*>(inputMgr->createInputObject(OISMouse, true));

	// Register for events.
	mouse->setEventCallback(this);
	keyboard->setEventCallback(this);

	// Set initial mouse clipping size.
	windowResized(mWindow);
}

void App::setupSdkTrays()
{
	// Load the fonts required.
	Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
	Ogre::FontManager::getSingleton().getByName("SdkTrays/Value")->load();

	// SdkTrays is initialized here because it requires a pointer to
	// the mouse.
	trayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mouse, this);

	trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
}

void App::setupSdkCameraMan()
{
	
	cameraMan = new OgreBites::SdkCameraMan(mCamera);
	cameraMan->setStyle(OgreBites::CS_MANUAL);
}

void App::createCallbacks()
{
	typedef const FrameEventArgs* fArgs;
	typedef const KeyEventArgs*   kArgs;
	typedef const MouseEventArgs* mArgs;
	
	// Basic key events.
	OnKeyPressed.Subscribe([&](kArgs args) {
		using namespace OIS;

		switch (args->evt->key)
		{
		// Exit on Escape.
		case KC_ESCAPE:			
			playing = false;
			break;
		// Polygon Mode Toggle.
		case KC_R:				
			switch (mCamera->getPolygonMode())
			{
			case Ogre::PolygonMode::PM_SOLID:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_WIREFRAME);
				break;
			case Ogre::PolygonMode::PM_WIREFRAME:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_POINTS);
				break;
			case Ogre::PolygonMode::PM_POINTS:
				mCamera->setPolygonMode(Ogre::PolygonMode::PM_SOLID);
				break;
			}
			break;
		}
	});

	// This must be the first event for frame rendering queued.
	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		xboxCtrl.update();
	});

	// SdkCameraMan and SdkTrayManager need to be updated.
	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		cameraMan->frameRenderingQueued(*args->evt);
		trayMgr->frameRenderingQueued(*args->evt);
	});
}