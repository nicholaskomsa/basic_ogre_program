#include "graphics.h"

#include <OIS/OISEvents.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>

#include <chrono>

#pragma once

#ifndef _INPUT_H_
#define _INPUT_H_

class Input {
	// OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
public:

	Input();
	virtual ~Input();

	bool setup(Ogre::RenderWindow* renderWindow, bool bufferedKeyboard = true, bool bufferedMouse = true);
	void shutdown();

	void setEventListeners(OIS::KeyListener* keyListener, OIS::MouseListener* mMouseListener);

	void resize(int width, int height);

	void capture();

	OIS::Keyboard* getKeyboard();
	OIS::Mouse* getMouse();


};

class MouseButtonHandler {
public:
	class Button {
		friend class MouseButtonHandler;
		std::chrono::milliseconds mTimeSinceLastClick{ 0 };
		std::chrono::milliseconds mTimeBetweenClicks{ 0 };
		bool mDown{ false };
		OIS::MouseButtonID mID;
	public:
		Button(OIS::MouseButtonID id, std::chrono::milliseconds timeBetweenClicks)
			:mID(id)
			, mTimeBetweenClicks(timeBetweenClicks)
		{}

		const OIS::MouseButtonID getID() const { return mID; }
	};

private:
	std::vector< Button > mButtons;
public:

	void addButton(const Button& button) {
		mButtons.push_back(button);
	}

	void handleMouse(const OIS::MouseState& ms) {
		std::chrono::milliseconds curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());

		for (std::size_t b = 0; b < mButtons.size(); ++b) {
			std::chrono::milliseconds elapsedTime = curTime - mButtons[b].mTimeSinceLastClick;

			bool down = ms.buttonDown(mButtons[b].mID);

			if (elapsedTime > mButtons[b].mTimeBetweenClicks && down) {
				mouseDown(mButtons[b], ms);
				mButtons[b].mTimeSinceLastClick = curTime;
				mButtons[b].mDown = true;
			}
			else if (!down && mButtons[b].mDown) {
				mouseUp(mButtons[b], ms);
				mButtons[b].mDown = false;
			}

		}
	}

	virtual void mouseDown(const Button& button, const OIS::MouseState& state) = 0;
	virtual void mouseUp(const Button& button, const OIS::MouseState& state) = 0;
};


class KeyHandler {
public:
	class Key {
		friend class KeyHandler;
		std::chrono::milliseconds mTimeSinceLastClick{ 0 };
		std::chrono::milliseconds mTimeBetweenClicks{ 0 };
		bool mDown{ false };
		OIS::KeyCode mID;
		std::string mText;
	public:
		Key(OIS::KeyCode id, std::chrono::milliseconds timeBetweenClicks)
			:mID(id)
			, mTimeBetweenClicks(timeBetweenClicks)
		{}
		const OIS::KeyCode getID() const {
			return mID;
		}
		const std::string getString() const {
			return mText;
		}
	};
private:
	std::vector< Key > mKeys;
protected:
	OIS::Keyboard* mKeyboard;
public:

	void addKey(const Key& key) {
		//if( key.mID == OIS::KeyCode::KC_UNASSIGNED) mKey.mID = mKeyboard->getAsString()
		mKeys.push_back(key);
	}

	void handleKeys(OIS::Keyboard* keyboard) {
		std::chrono::milliseconds curTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch());

		for (std::size_t k = 0; k < mKeys.size(); ++k) {
			std::chrono::milliseconds elapsedTime = curTime - mKeys[k].mTimeSinceLastClick;

			bool down = keyboard->isKeyDown(mKeys[k].mID);

			if (elapsedTime > mKeys[k].mTimeBetweenClicks && down) {
				keyDown(mKeys[k]);
				mKeys[k].mTimeSinceLastClick = curTime;
				mKeys[k].mDown = true;
			}
			else if (!down && mKeys[k].mDown) {
				keyUp(mKeys[k]);
				mKeys[k].mDown = false;
			}
		}
	}

	void addArrows(std::chrono::milliseconds timeBetweenClicks) {
		addKey(Key(OIS::KC_LEFT, timeBetweenClicks));
		addKey(Key(OIS::KC_RIGHT, timeBetweenClicks));
		addKey(Key(OIS::KC_UP, timeBetweenClicks));
		addKey(Key(OIS::KC_DOWN, timeBetweenClicks));
	}

	void addAlphaNumeric(std::chrono::milliseconds timeBetweenClicks) {
		addKey(Key(OIS::KC_A, timeBetweenClicks));
		addKey(Key(OIS::KC_B, timeBetweenClicks));
		addKey(Key(OIS::KC_C, timeBetweenClicks));
		addKey(Key(OIS::KC_D, timeBetweenClicks));
		addKey(Key(OIS::KC_E, timeBetweenClicks));
		addKey(Key(OIS::KC_F, timeBetweenClicks));
		addKey(Key(OIS::KC_G, timeBetweenClicks));
		addKey(Key(OIS::KC_H, timeBetweenClicks));
		addKey(Key(OIS::KC_I, timeBetweenClicks));
		addKey(Key(OIS::KC_J, timeBetweenClicks));
		addKey(Key(OIS::KC_K, timeBetweenClicks));
		addKey(Key(OIS::KC_L, timeBetweenClicks));
		addKey(Key(OIS::KC_M, timeBetweenClicks));
		addKey(Key(OIS::KC_N, timeBetweenClicks));
		addKey(Key(OIS::KC_O, timeBetweenClicks));
		addKey(Key(OIS::KC_P, timeBetweenClicks));
		addKey(Key(OIS::KC_Q, timeBetweenClicks));
		addKey(Key(OIS::KC_R, timeBetweenClicks));
		addKey(Key(OIS::KC_S, timeBetweenClicks));
		addKey(Key(OIS::KC_T, timeBetweenClicks));
		addKey(Key(OIS::KC_U, timeBetweenClicks));
		addKey(Key(OIS::KC_V, timeBetweenClicks));
		addKey(Key(OIS::KC_W, timeBetweenClicks));
		addKey(Key(OIS::KC_X, timeBetweenClicks));
		addKey(Key(OIS::KC_Y, timeBetweenClicks));
		addKey(Key(OIS::KC_Z, timeBetweenClicks));

		addKey(Key(OIS::KC_0, timeBetweenClicks));
		addKey(Key(OIS::KC_1, timeBetweenClicks));
		addKey(Key(OIS::KC_2, timeBetweenClicks));
		addKey(Key(OIS::KC_3, timeBetweenClicks));
		addKey(Key(OIS::KC_4, timeBetweenClicks));
		addKey(Key(OIS::KC_5, timeBetweenClicks));
		addKey(Key(OIS::KC_6, timeBetweenClicks));
		addKey(Key(OIS::KC_7, timeBetweenClicks));
		addKey(Key(OIS::KC_8, timeBetweenClicks));
		addKey(Key(OIS::KC_9, timeBetweenClicks));
	}


	virtual void keyDown(const Key& key) = 0;
	virtual void keyUp(const Key& key) = 0;
};




#endif