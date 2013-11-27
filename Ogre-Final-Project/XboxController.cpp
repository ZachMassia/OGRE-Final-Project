#include "XboxController.h"

XboxController::XboxController(PAD_ID pad) : id(pad)
{
}

XboxController::~XboxController()
{
	if (connected) {
		// Don't leave the controller vibrating on shutdown.
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
	connected = XInputGetState((DWORD)id, &state) == ERROR_SUCCESS;

	// Only transfer the state to our own state if connected.
	if (!connected)
		return;

	updateTriggers();
	updateButtons();
	updateSticks();	
}

void XboxController::vibrate(float leftMotor, float rightMotor)
{
	XINPUT_VIBRATION vib;

	memset(&vib, 0, sizeof(XINPUT_VIBRATION));
		
	if (leftMotor > 1.0f) 
		leftMotor = 1.0f;
	if (rightMotor > 1.0f) 
		rightMotor = 1.0f;

	vib.wLeftMotorSpeed = (int)leftMotor * 65535;
	vib.wRightMotorSpeed = (int)rightMotor * 65535;

	XInputSetState((DWORD)id, &vib);
}

const XboxController::GamePadState& XboxController::getState() const
{
	return currentState;
}

void XboxController::updateTriggers()
{
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
}

// See http://play.golang.org/p/8PScDQfhum for reference on bit shift.
void XboxController::updateButtons()
{
	const WORD btns = state.Gamepad.wButtons;

	// Get all button states out of the bit-mask.
	for (WORD i = 0; i < XboxController::BTN_MAX; i++)
	{
		const WORD mask = 1 << i;
		currentState.buttons[i] = (btns & mask) == mask;
	}
}

void XboxController::updateSticks()
{
	// Convenience to make code more readable.
	const SHORT thumbLX = state.Gamepad.sThumbLX;
	const SHORT thumbLY = state.Gamepad.sThumbLY;
	const SHORT thumbRX = state.Gamepad.sThumbRX;
	const SHORT thumbRY = state.Gamepad.sThumbRY;

	static const int L_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	static const int R_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

	static const auto bothInRange = [](int a, int b, int min, int max) {
		return (a > min && a < max) && (b > min && b < max);
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
		currentState.rightStick.x = thumbRX;
		currentState.rightStick.y = thumbRY;
		currentState.rightStick.normalise();
	}
}