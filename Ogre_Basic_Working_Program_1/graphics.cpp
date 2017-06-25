
#include "graphics.h"

Graphics Graphics::mSingleton;
bool Graphics::initialized = false;

Graphics::Graphics() {}

Graphics::~Graphics() {
	shutdown();
}
bool Graphics::startup(Ogre::String windowTitle, Ogre::SceneType sceneManagerType, Ogre::String resourcesFileName, Ogre::String pluginsFileName, Ogre::String cfgFileName, Ogre::String logFileName) {
	if (initialized)
		EXCEPT(Exception) << "Graphics::initialize, singleton already initialized!\n";

	initialized = true;
	return mSingleton.init(windowTitle, sceneManagerType, resourcesFileName, pluginsFileName, cfgFileName, logFileName);
}
void Graphics::shutdown() {
	if (initialized)
		mSingleton.destroy();
	initialized = false;
}

bool Graphics::init(Ogre::String windowTitle, Ogre::SceneType sceneManagerType, Ogre::String resourcesFileName, Ogre::String pluginsFileName, Ogre::String cfgFileName, Ogre::String logFileName) {

	LogManager::get("game") << "init graphics" << Log::end;

	try {

		mRoot = OGRE_NEW Ogre::Root(pluginsFileName, cfgFileName, logFileName);

		if (mRoot->restoreConfig() || mRoot->showConfigDialog()) {
			mWindow = mRoot->initialise(true, windowTitle);
		}
		else {
			Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Graphics::Cancel Button Hit!");
			return false;
		}

		mSceneManager = mRoot->createSceneManager(sceneManagerType, "DefaultSceneManager");

		addResourceLocations(resourcesFileName);

		if (!createCameraAndViewport()) {
			Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Graphics::setup createCameraAndViewport failed!");
			return false;
		}

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	}
	catch (Ogre::Exception &e) {
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage(e.getFullDescription());
		return false;
	}
	return true;
}

void Graphics::destroy() {
	if (mRoot)
		OGRE_DELETE mRoot;
	mRoot = NULL;
}

void Graphics::go() {
	mRoot->startRendering();
}

void Graphics::addResourceLocations(std::string resourcesFileName) {
	Ogre::ConfigFile config;

	config.load(resourcesFileName);

	Ogre::ConfigFile::SectionIterator seci = config.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			if (!StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
				archName = Ogre::String(macBundlePath() + "/" + archName);
#endif
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);

		}
	}
}

bool Graphics::createCameraAndViewport(Ogre::ColourValue backgroundColor) {

	if (!mSceneManager) {
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Graphics::createViewport sceneManager=NULL");
		return false;
	}

	mCamera = mSceneManager->createCamera("DefaultCamera");

	if (!mCamera) {
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Graphics::createViewport camera=NULL");
		return false;
	}
	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	if (!vp) {
		Ogre::LogManager::getSingleton().getDefaultLog()->logMessage("Graphics::createViewport vp=NULL");
		return false;
	}

	vp->setBackgroundColour(backgroundColor);

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	mCamera->setNearClipDistance(5);

	vp->setCamera(mCamera);

	return true;
}

void Graphics::setEventListeners(Ogre::WindowEventListener* windowListener, Ogre::FrameListener* frameListener) {
	mWindowEventListener = windowListener;
	mFrameListener = frameListener;

	if (mFrameListener) mRoot->addFrameListener(mFrameListener);
	if (mWindowEventListener) Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mWindowEventListener);
}


Ogre::SceneManager* Graphics::getSceneManager() {
	return mSceneManager;
}
Ogre::RenderWindow* Graphics::getRenderWindow() {
	return mWindow;
}
Ogre::Camera* Graphics::getCamera() {
	return mCamera;
}
Ogre::Root* Graphics::getRoot() {
	return mRoot;
}

