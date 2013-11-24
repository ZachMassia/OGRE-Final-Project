#include "stdafx.h"
#include "Game.h"

int main(int argc, char* argv[])
{
	Game game;

	// Launch the application.
	// Any exceptions are caught and logged to stderr.
	try {
		game.go();
	}

	catch (const Ogre::Exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "OGRE Exception: \n" << e.getFullDescription() << std::endl;
#endif
		return 1;
	}

	catch (const std::exception& e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBox(NULL, e.what(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		std::cerr << "Exception: \n" << e.what() << std::endl;
#endif
		return 1;
	}

	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}

	return 0;
}