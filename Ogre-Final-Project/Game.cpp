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
	sinbad = new GameObject("sinbad", "Sinbad.mesh", mSceneMgr->getRootSceneNode(), mSceneMgr);

	sinbad->setVelocity(Ogre::Vector3(25, 0, 25));

	auto ent = static_cast<Ogre::Entity*>(sinbad->getNode()->getAttachedObject("sinbad"));
	ent->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	const auto setupAnimPair = [&](int i, const std::string& name) {
		animStates[i] = ent->getAnimationState(name + "Base");
		animStates[i + 1] = ent->getAnimationState(name + "Top");
		for (int j = i; j < i + 1; ++j) {
			animStates[j]->setLoop(true);
			animStates[j]->setEnabled(false);
		}
	};
	setupAnimPair(0, "Idle");
	setupAnimPair(2, "Run");
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

#pragma region Helpers
	// Update an animation pair.
	static const auto updateAnimPair = [&](int i, Ogre::Real dt) {
		animStates[i]->addTime(dt);
		animStates[i + 1]->addTime(dt);
	};

	// Toggle enabled for an animation pair.
	static const auto setEnabledAnimPair = [&](int i, bool enabled) {
		animStates[i]->setEnabled(enabled);
		animStates[i + 1]->setEnabled(enabled);
	};
#pragma endregion

	// Toggle animations. 
	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		if (sinbad->getAcceleration().isZeroLength()) {
			setEnabledAnimPair(0, true);
			setEnabledAnimPair(2, false);
			updateAnimPair(0, args->evt->timeSinceLastFrame);
		}
		else {
			setEnabledAnimPair(0, false);
			setEnabledAnimPair(2, true);
			updateAnimPair(2, args->evt->timeSinceLastFrame);
		}
	});

	// Control Sinbad.
	OnFrameRenderingQueued.Subscribe([&](const FrameEventArgs* args) {
		const auto& leftStick = xboxCtrl.p1.getState().leftStick;
		const auto& rightStick = xboxCtrl.p1.getState().rightStick;

		// Invert the y so stick up goes into screen.
		sinbad->setAcceleration(Ogre::Vector3(leftStick.x, 0, -leftStick.y).normalisedCopy());

		// Keep him facing the move direction.
		Ogre::Vector3 dir = sinbad->getAcceleration().normalisedCopy();
		Ogre::Vector3 src = sinbad->getNode()->getOrientation() * Ogre::Vector3::UNIT_Z;

		// Ignore pitch.
		dir.y = 0;
		dir.normalise();

		src.y = 0;
		src.normalise();

		sinbad->getNode()->rotate(src.getRotationTo(dir));

		sinbad->update(args->evt);

		cameraMan->setYawPitchDist(Ogre::Degree(rightStick.x), Ogre::Degree(rightStick.y), 45.0f);
	});
}

void Game::setupSdkCameraMan()
{
	App::setupSdkCameraMan();

	cameraMan->setStyle(OgreBites::CS_ORBIT);
	cameraMan->setTarget(sinbad->getNode());
	cameraMan->setYawPitchDist(Ogre::Degree(0.0f), Ogre::Degree(0.0f), 45.0f);
}

void Game::createGui()
{
	
}
