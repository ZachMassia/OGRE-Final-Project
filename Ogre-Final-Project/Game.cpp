#include "Game.h"


Game::Game()
{
}


Game::~Game()
{
}

void Game::createScene()
{
	// Setup a basic entity with idle animation as a placeholder.
	auto ent = mSceneMgr->createEntity("Sinbad.mesh");
	auto node = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	node->attachObject(ent);

	animStates[0] = ent->getAnimationState("IdleBase");
	animStates[0]->setLoop(true);
	animStates[0]->setEnabled(true);

	animStates[1] = ent->getAnimationState("IdleTop");
	animStates[1]->setLoop(true);
	animStates[1]->setEnabled(true);

	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		animStates[0]->addTime(args->evt->timeSinceLastFrame);
		animStates[1]->addTime(args->evt->timeSinceLastFrame);
	});
}