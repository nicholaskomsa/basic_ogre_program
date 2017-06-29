
#include "input.h"


#include "ogretests.h"


INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT) {

	Input input;
	try {

		if (!Graphics::startup()) return 0;


		if (!input.setup(Graphics::get().getRenderWindow())) return 0;

		{
			OgreTest test(&Graphics::get(), &input);



			test.run();
		}
	}
	catch (Ogre::Exception& e) {
		OutputDebugStringA(e.getFullDescription().c_str());
	}
	catch (Exception& e) {
		e.writeDebugString();
	}
	catch (...) {
		OutputDebugStringA("unhandled Exception!");
	}

	input.shutdown();
	Graphics::shutdown();

	return 0;
}