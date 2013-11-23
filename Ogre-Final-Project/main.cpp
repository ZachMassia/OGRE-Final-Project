#include "stdafx.h"
#include "App.h"

int main(int argc, char* argv[])
{
	App app;

	// Launch the application.
	// Any exceptions are caught and logged to stderr.
	try {
		app.go();
	}
	catch (const Ogre::Exception& e) {
		std::cerr << "OGRE Exception: \n" << e.getFullDescription() << std::endl;
		return 1;
	}
	catch (const std::exception& e) {
		std::cerr << "Exception: \n" << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}