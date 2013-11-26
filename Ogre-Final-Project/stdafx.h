#pragma once

#include <exception>
#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <set>
#include <array>
#include <sstream>
#include <functional>
#include <algorithm>
#include <ctime>
#include <cstdlib>

#include <OIS/OIS.h>
#include <OGRE/Ogre.h>
#include <OGRE/ExampleApplication.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#pragma comment(lib, "XInput.lib")

// ------------------------------------------------------------ //
// Any files in this project must be included after this point. //
// ------------------------------------------------------------ //
#include "Event.h"