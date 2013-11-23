#include "App.h"


App::App() :
	inputMgr(nullptr),
	keyboard(nullptr),
	mouse(nullptr)
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
}

void App::createScene()
{
	auto ent = mSceneMgr->createEntity("Barrel.mesh");
	auto node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	node->attachObject(ent);
}

void App::createCamera()
{
	ExampleApplication::createCamera(); // TODO: Override
}

void App::createFrameListener()
{
	mRoot->addFrameListener(this);

	setupOIS();
	createFrameCallbacks();
}

bool App::frameStarted(const Ogre::FrameEvent& evt)
{
	keyboard->capture();
	mouse->capture();

	// Notify event subscribers.
	OnFrameStarted.RaiseEvent(new FrameEventArgs(keyboard, mouse, &evt));

	return true;
}

bool App::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	keyboard->capture();
	mouse->capture();

	// Exit on Escape
	if (keyboard->isKeyDown(OIS::KC_ESCAPE)) {
		return false;
	}

	// Notify event subscribers.
	OnFrameRenderingQueued.RaiseEvent(new FrameEventArgs(keyboard, mouse, &evt));

	return true;
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
	keyboard = static_cast<Keyboard*>(inputMgr->createInputObject(OISKeyboard, false));
	mouse    = static_cast<Mouse*>(   inputMgr->createInputObject(OISMouse,    false));
}

void App::createFrameCallbacks()
{
	typedef FrameEventArgs* Args;

	// Polygon Mode Toggle
	OnFrameRenderingQueued.Subscribe([&](Args args) {
		if (args->key->isKeyDown(OIS::KC_R)) {
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
		}
	});
}