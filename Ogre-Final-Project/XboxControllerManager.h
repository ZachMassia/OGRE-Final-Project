#pragma once

#include "stdafx.h"
#include "XboxController.h"


class XboxControllerManager
{
public:
	XboxController p1, p2, p3, p4;

public:
	XboxControllerManager();
	~XboxControllerManager();

	void update();
};

