#pragma once

#include "StdAfx.h"


class XboxController
{
public:
	enum BUTTON_ID {
		DPAD_UP,
		DPAD_DOWN,
		DPAD_LEFT,
		DPAD_RIGHT,
		START,
		BACK,
		LEFT_THUMB,
		RIGHT_THUMB,
		LEFT_SHOULDER,
		RIGHT_SHOULDER,
		A,
		B,
		X,
		Y,
		BTN_MAX
	};

	enum PAD_ID {
		PLAYER_ONE,
		PLAYER_TWO,
		PLAYER_THREE,
		PLAYER_FOUR
	};

	struct GamePadState
	{
		// Buttons
		std::array<bool, BTN_MAX> buttons;
		// Sticks
		Ogre::Vector2 leftStick;
		Ogre::Vector2 rightStick;
		// Triggers
		float leftTrigger;
		float rightTrigger;

		GamePadState() :
			leftStick(Ogre::Vector2::ZERO), rightStick(Ogre::Vector2::ZERO),
			leftTrigger(0.0f), rightTrigger(0.0f)
		{
		
		}
	};

private:
	// The exported state.
	GamePadState currentState;

	// The internal XInput state of the controller.
	XINPUT_STATE state;
	
	// Corresponds to the illuminated quadrant on the controller.
	PAD_ID id;

	bool connected;

public:
	XboxController(PAD_ID pad);
	~XboxController();

	bool isConnected() const;
	void update();
	void vibrate(float leftMotor = 0.0f, float rightMotor = 0.0f);
	const GamePadState& getState() const;
};

