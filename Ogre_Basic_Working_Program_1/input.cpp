
#include "input.h"




Input::Input() {
	mInputManager = NULL;
	mMouse = NULL;
	mKeyboard = NULL;
}
Input::~Input() {
	shutdown();
}
bool Input::setup(Ogre::RenderWindow* renderWindow, bool bufferedKeyboard, bool bufferedMouse) {
	OIS::ParamList pl;
	std::ostringstream windowHndStr;

	//this code, getCustomAttribute: windowHandle, does not appear to be platform independent
	std::size_t windowHandle = 0;
	renderWindow->getCustomAttribute("WINDOW", &windowHandle);
	windowHndStr << windowHandle;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));

	try {
		mInputManager = OIS::InputManager::createInputSystem(pl);
	}
	catch (...) {
		LogManager::get("game") << "Input::setup createInputSystem failed!";
		return false;
	}
	try {
		mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, bufferedKeyboard));
	}
	catch (...) {
		LogManager::get("game") << "Input::setup createInputObject mKeyboard failed!";
		return false;
	}
	try {
		mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, bufferedMouse));
	}
	catch (...) {
		LogManager::get("game") << "Input::setup createInputObject mMouse failed!";
		return false;
	}

	return true;
}
void Input::shutdown() {
	if (mInputManager) {
		if (mMouse)
			mInputManager->destroyInputObject(mMouse);

		if (mKeyboard)
			mInputManager->destroyInputObject(mKeyboard);

		OIS::InputManager::destroyInputSystem(mInputManager);

		mKeyboard = NULL;
		mMouse = NULL;
		mInputManager = NULL;
	}
}

void Input::setEventListeners(OIS::KeyListener* keyListener, OIS::MouseListener* mouseListener) {
	if (!mInputManager) {
		LogManager::get("game") << "Input::setEventListeners mInputManager is invalid!";
		return;
	}
	if (!mMouse) {
		LogManager::get("game") << "Input::setEventListeners mMouse is invalid!";
		return;
	}
	if (mouseListener) mMouse->setEventCallback(mouseListener);

	if (!mKeyboard) {
		LogManager::get("game") << "Input::setEventListeners mKeyboard is invalid!";
		return;
	}
	if (keyListener) mKeyboard->setEventCallback(keyListener);
}

void Input::capture() {
	if (!mInputManager) {
		LogManager::get("game") << "Input::capture mInputManager is invalid!";
		return;
	}
	if (!mMouse) {
		LogManager::get("game") << "Input::capture mMouse is invalid!";
		return;
	}
	mMouse->capture();

	if (!mKeyboard) {
		LogManager::get("game") << "Input::capture mKeyboard is invalid!";
		return;
	}
	mKeyboard->capture();
}

void Input::resize(int width, int height) {
	if (!mInputManager) {
		LogManager::get("game") << "Input::resize mInputManager is invalid!";
		return;
	}
	if (!mMouse) {
		LogManager::get("game") << "Input::resize mMouse is invalid!";
		return;
	}
	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;

}

OIS::Keyboard* Input::getKeyboard() {
	return mKeyboard;
}

OIS::Mouse* Input::getMouse() {
	return mMouse;
}

