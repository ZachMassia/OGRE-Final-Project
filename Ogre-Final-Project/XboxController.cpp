#include "XboxController.h"

XboxController::XboxController(PAD_ID pad) :
	id(pad)
{
}


XboxController::~XboxController()
{
	if (connected) {
		vibrate(0.0f, 0.0f);
	}
}

bool XboxController::isConnected() const
{
	return connected;
}

void XboxController::update()
{
	// Clean the state.
	memset(&state, 0, sizeof(XINPUT_STATE));

	// Get the current state.
	DWORD result = XInputGetState((DWORD)id, &state);

	connected = result == ERROR_SUCCESS;

	// Only transfer the state to our own state if connected.
	if (!connected)
		return;

	// Right Trigger.
	int rTrigger = state.Gamepad.bRightTrigger;
	if (rTrigger && rTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		currentState.rightTrigger = (float)rTrigger / 255.0f;
	}

	// Left Trigger.
	int lTrigger = state.Gamepad.bRightTrigger;
	if (lTrigger && lTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
		currentState.leftTrigger = (float)lTrigger / 255.0f;
	}
	
	// Get all button states out of the bit-mask.
	const WORD btns = state.Gamepad.wButtons;
	
	currentState.buttons[DPAD_UP]        = btns & XINPUT_GAMEPAD_DPAD_UP;
	currentState.buttons[DPAD_DOWN]      = btns & XINPUT_GAMEPAD_DPAD_DOWN;
	currentState.buttons[DPAD_LEFT]      = btns & XINPUT_GAMEPAD_DPAD_LEFT;
	currentState.buttons[DPAD_RIGHT]     = btns & XINPUT_GAMEPAD_DPAD_RIGHT;
	currentState.buttons[START]          = btns & XINPUT_GAMEPAD_START;
	currentState.buttons[BACK]           = btns & XINPUT_GAMEPAD_BACK;
	currentState.buttons[LEFT_THUMB]     = btns & XINPUT_GAMEPAD_LEFT_THUMB;
	currentState.buttons[RIGHT_THUMB]    = btns & XINPUT_GAMEPAD_RIGHT_THUMB;
	currentState.buttons[LEFT_SHOULDER]  = btns & XINPUT_GAMEPAD_LEFT_SHOULDER;
	currentState.buttons[RIGHT_SHOULDER] = btns & XINPUT_GAMEPAD_RIGHT_SHOULDER;
	currentState.buttons[A]              = btns & XINPUT_GAMEPAD_A;
	currentState.buttons[B]              = btns & XINPUT_GAMEPAD_B;
	currentState.buttons[X]              = btns & XINPUT_GAMEPAD_X;
	currentState.buttons[Y]              = btns & XINPUT_GAMEPAD_Y;

	// Convenience to make code more readable.
	const SHORT thumbLX = state.Gamepad.sThumbLX;
	const SHORT thumbLY = state.Gamepad.sThumbLY;
	const SHORT thumbRX = state.Gamepad.sThumbRX;
	const SHORT thumbRY = state.Gamepad.sThumbRY;

	static const int L_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	static const int R_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	static const auto bothInRange = [](int a, int b, int min, int max) {
		return (a > min) && (a < max) &&
			   (b > min) && (a < max);
	};

	// Left Stick.
	if (bothInRange(thumbLX, thumbLY, -L_DEADZONE, L_DEADZONE)) {
		// Left stick in dead zone.
		currentState.leftStick = Ogre::Vector2::ZERO;
	}
	else {
		// Get the left stick into a normalized Ogre::Vector3.
		currentState.leftStick.x = thumbLX;
		currentState.leftStick.y = thumbLY;
		currentState.leftStick.normalise();
	}
	
	// Right Stick.
	if (bothInRange(thumbRX, thumbRY, -R_DEADZONE, R_DEADZONE)) {
		// Left stick in dead zone.
		currentState.rightStick = Ogre::Vector2::ZERO;
	}
	else {
		// Get the left stick into a normalized Ogre::Vector3.
		currentState.rightStick.x = thumbLX;
		currentState.rightStick.y = thumbLY;
		currentState.rightStick.normalise();
	}
}

void XboxController::vibrate(float leftMotor, float rightMotor)
{
	XINPUT_VIBRATION vib;

	memset(&vib, 0, sizeof(XINPUT_VIBRATION));
		
	if (leftMotor > 1.0f) leftMotor = 1.0f;
	if (rightMotor > 1.0f) rightMotor = 1.0f;

	vib.wLeftMotorSpeed = (int)leftMotor * 65535;
	vib.wRightMotorSpeed = (int)rightMotor * 65535;

	XInputSetState((DWORD)id, &vib);
}

const XboxController::GamePadState& XboxController::getState() const
{
	return currentState;
}