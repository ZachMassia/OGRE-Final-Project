#include "App.h"


App::App() :
	frameListener(nullptr)
{
	// Seed the RNG for any future use.
	std::srand(std::time(0));
}


App::~App()
{
	if (frameListener) { delete frameListener; }
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
	ExampleApplication::createFrameListener(); // TODO: Override
}
