
#include "graphics.h"
#include "input.h"

#include "camera.h"


#pragma once





class OgreTest : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::MouseListener, public OIS::KeyListener {
protected:
	Graphics* mGraphics;
	Input* mInput;

	Camera mCamera;

	virtual void createScene() {
	
		mGraphics->getSceneManager()->setAmbientLight(Ogre::ColourValue(1, 1, 1, 1));
		mCamera.setPosition(Ogre::Vector3(0, 0, 150));
		mCamera.lookAt(Ogre::Vector3(0, 0, 0.001));
		                                             
		Ogre::String planeName = "plane";
		Ogre::String material = "BaseWhiteNoLighting";



		int X1 = 0, Y1 = 0, X2 = 300, Y2 = 300;

		Ogre::ManualObject* manual = mGraphics->getSceneManager()->createManualObject(planeName);

		manual->begin(material);

		/*
			(1) 0, 1			(3) 1, 1
			(0) 0, 0			(2) 1, 0

			0, 2, 1
			2, 3, 1
		*/
		
		manual->position(Ogre::Vector3(X1, Y1, 0));// manual->normal(Ogre::Vector3(0, 0, 0)); manual->textureCoord(Ogre::Vector2(0, 0));
		manual->position(Ogre::Vector3(X1, Y2, 0));// manual->normal(Ogre::Vector3(0, 0, 0)); manual->textureCoord(Ogre::Vector2(0, 1));
		manual->position(Ogre::Vector3(X2, Y1, 0));// manual->normal(Ogre::Vector3(0, 0, 0)); manual->textureCoord(Ogre::Vector2(1, 0));
		manual->position(Ogre::Vector3(X2, Y2, 0));// manual->normal(Ogre::Vector3(0, 0, 0)); manual->textureCoord(Ogre::Vector2(1, 1));

		manual->triangle(0, 2, 1);
		manual->triangle(2, 3, 1);

		manual->end();

		manual->convertToMesh(planeName);
	
		Ogre::Entity* ent = mGraphics->getSceneManager()->createEntity(planeName);
		mGraphics->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(ent);

//		Ogre::Entity* cube = mGraphics->getSceneManager()->createEntity("cube.mesh");
//		mGraphics->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(cube);

	
	}
	virtual void destroyScene() {}
public:
	OgreTest(Graphics* graphics, Input* input)
		: mGraphics(graphics)
		, mInput(input)
		, mCamera(graphics)
	{
		mGraphics->setEventListeners(this, this);
		mInput->setEventListeners(this, this);

		windowResized(Graphics::get().getRenderWindow());
	}
	virtual ~OgreTest() {}

	void run() {

		createScene();

		mGraphics->go();

		destroyScene();

	}

	virtual bool frameStarted(const Ogre::FrameEvent& frameEvent) {

		if (mGraphics->getRenderWindow()->isClosed()) return false;

		Ogre::Real time = frameEvent.timeSinceLastFrame;

		static Ogre::Real timeSum = 0;
		static unsigned long frameCount = 0;

		timeSum += time;

		if (timeSum >= 1) {
			timeSum = 0;

			std::stringstream sstr;
			sstr << "FPS: " << frameCount << ", ";
			OutputDebugStringA(sstr.str().c_str());
			//Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(sstr.str());
			LogManager::get("game") << sstr.str() << Log::end;
			frameCount = 0;
		}
		else {
			frameCount++;
		}

		mInput->capture();

		OIS::Keyboard* keyboard = mInput->getKeyboard();
		if (!keyboard) {
			LogManager::get("game") << "OgreTest::frameStarted keyboard = NULL" << Log::end;
			return false;
		}

		if (keyboard->isKeyDown(OIS::KC_ESCAPE)) {
			LogManager::get("game") << "OgreTest::frameStarted Escape Key Hit!" << Log::end;
			return false;
		}


		Ogre::Vector3 cameraTranslation = Ogre::Vector3::ZERO;
		if (keyboard->isKeyDown(OIS::KC_UP))	cameraTranslation.y = 1;
		if (keyboard->isKeyDown(OIS::KC_DOWN))	cameraTranslation.y = -1;

		if (keyboard->isKeyDown(OIS::KC_LEFT))	cameraTranslation.x = -1;
		if (keyboard->isKeyDown(OIS::KC_RIGHT))	cameraTranslation.x = 1;

		if (keyboard->isKeyDown(OIS::KC_PGUP)) cameraTranslation.z = 1;
		if (keyboard->isKeyDown(OIS::KC_PGDOWN)) cameraTranslation.z = -1;


		bool rotate = false;
		if (keyboard->isKeyDown(OIS::KC_S)) { mCamera.rotateDown(frameEvent.timeSinceLastFrame); rotate = true; }
		if (keyboard->isKeyDown(OIS::KC_W)) { mCamera.rotateUp(frameEvent.timeSinceLastFrame); rotate = true; }
		if (keyboard->isKeyDown(OIS::KC_A)) { mCamera.rotateLeft(frameEvent.timeSinceLastFrame); rotate = true; }
		if (keyboard->isKeyDown(OIS::KC_D)) { mCamera.rotateRight(frameEvent.timeSinceLastFrame); rotate = true; }

		bool move = false;

		//moving not on cameras angle, move =false
		mCamera.adjust(rotate, move);


		if (keyboard->isKeyDown(OIS::KC_HOME))	mCamera.setPosition(Ogre::Vector3(0, 0, 300));

		mCamera.translate(cameraTranslation);

		if (keyboard->isKeyDown(OIS::KC_F1)) mCamera.setPolySolid();
		if (keyboard->isKeyDown(OIS::KC_F2)) mCamera.setPolyWireFrame();
		if (keyboard->isKeyDown(OIS::KC_F3)) mCamera.setPolyPoints();


		return true;
	}

	//RENDER WINDOW EVENTS
	virtual void windowClosed(Ogre::RenderWindow* rw) {
		if (rw == mGraphics->getRenderWindow()) {
			//Unattach OIS before window shutdown (very important under Linux)
			mInput->shutdown();
		}
	}

	virtual void windowResized(Ogre::RenderWindow* rw) {

		//resize the mouse dimensions
		unsigned int width, height, depth;
		int left, top;
		rw->getMetrics(width, height, depth, left, top);

		OIS::Mouse* mouse = mInput->getMouse();
		if (mouse) {
			const OIS::MouseState &ms = mouse->getMouseState();
			ms.width = width;
			ms.height = height;
		}
	}

	virtual bool keyPressed(const OIS::KeyEvent &key) { return true; }
	virtual bool keyReleased(const OIS::KeyEvent &key) { return true; }
	virtual bool mouseMoved(const OIS::MouseEvent &arg) { return true; }
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) { return true; }
};
