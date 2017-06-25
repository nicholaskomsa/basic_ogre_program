#include <ogre.h>

#include <memory>

#include "exception.h"
#include "log.h"


#pragma once


class Graphics {
private:

	static Graphics mSingleton;
	static bool initialized;

	Ogre::Root* mRoot;

	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneManager;
	Ogre::Camera* mCamera;

	void Graphics::addResourceLocations(std::string resourcesFileName);
	bool createCameraAndViewport(Ogre::ColourValue backgroundColor = Ogre::ColourValue::Blue);

	Ogre::FrameListener* mFrameListener;
	Ogre::WindowEventListener* mWindowEventListener;

	bool init(Ogre::String windowTitle, Ogre::SceneType sceneManagerType, Ogre::String resourcesFileName, Ogre::String pluginsFileName, Ogre::String cfgFileName, Ogre::String logFileName);
	void destroy();
	 
	Graphics();
	~Graphics();
public:

	static bool startup(Ogre::String windowTitle = "Ogre Render Window", Ogre::SceneType sceneManagerType = Ogre::SceneType::ST_GENERIC, Ogre::String resourcesFileName = "resources.cfg", Ogre::String pluginsFileName = "plugins.cfg", Ogre::String cfgFileName = "ogre.cfg", Ogre::String logFileName = "ogre.log");
	static void shutdown();

	inline static Graphics& get() {
		return mSingleton;
	}

	void go();

	Ogre::SceneManager* getSceneManager();
	Ogre::RenderWindow* getRenderWindow();
	Ogre::Camera* getCamera();
	Ogre::Root* getRoot();

	void setEventListeners(Ogre::WindowEventListener* windowListener, Ogre::FrameListener* frameListener);

};

