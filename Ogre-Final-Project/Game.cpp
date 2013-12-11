#include "Game.h"

Game::Game() : sinbad(nullptr)
{
}

Game::~Game()
{
	if (sinbad) {
		delete sinbad;
	}
}

void Game::createScene()
{
	fpsCtrl = new FPSController(mSceneMgr->getRootSceneNode(), mCamera, &xboxCtrl);
	
	artifexLoader = new ArtifexLoader(mRoot, mSceneMgr, fpsCtrl->getCamNode(), mCamera, "../../../../media/");
	 
	artifexLoader->loadZone("level1"); 
}

void Game::createCallbacks()
{
	App::createCallbacks();

	// Quit on Xbox remote BACK.
	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		if (xboxCtrl.p1.isConnected()) {
			playing = !xboxCtrl.p1.getState().buttons[XboxController::BACK];
		}
	});

	// Update Camera
	OnFrameRenderingQueued.Subscribe(
		std::bind(&FPSController::onFrameRenderingQueued, fpsCtrl, std::placeholders::_1));
}

void Game::setupSdkCameraMan()
{
	App::setupSdkCameraMan();
}

void Game::createGui()
{
	
}
