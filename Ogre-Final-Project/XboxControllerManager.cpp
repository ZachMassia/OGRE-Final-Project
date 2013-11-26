#include "XboxControllerManager.h"



XboxControllerManager::XboxControllerManager() :
	p1(XboxController::PLAYER_ONE),
	p2(XboxController::PLAYER_TWO),
	p3(XboxController::PLAYER_THREE),
	p4(XboxController::PLAYER_FOUR)
{
	
}


XboxControllerManager::~XboxControllerManager()
{
}

void XboxControllerManager::update()
{
	p1.update();
	p2.update();
	p3.update();
	p4.update();
}