#include "AppDelegate.h"
#include "SplashScene.h"

// #define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}

bool AppDelegate::applicationDidFinishLaunching() {

    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("CallAMice", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("CallAMice");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    // director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    // director->setAnimationInterval(1.0f / 60);

    auto fileUtils = FileUtils::getInstance();
    auto screenSize = glview->getFrameSize();
    std::vector<std::string> resDirOrders;

    // check which assets the devices require
    // QHD
    if (screenSize.width + screenSize.height > 3500) {
    
        resDirOrders.push_back("1440x2560");
        glview->setDesignResolutionSize(1440, 2560, ResolutionPolicy::NO_BORDER);
    }
    // FHD
    else if (screenSize.width + screenSize.height > 2500) {
    
        resDirOrders.push_back("1080x1920");
        glview->setDesignResolutionSize(1080, 1920, ResolutionPolicy::NO_BORDER);
    }
    // HD
    else if (screenSize.width + screenSize.height > 1500) {
    
        resDirOrders.push_back("720x1280");
        glview->setDesignResolutionSize(720, 1280, ResolutionPolicy::NO_BORDER);
    }
    // SD
    else {
    
        resDirOrders.push_back("540x960");
        glview->setDesignResolutionSize(540, 960, ResolutionPolicy::NO_BORDER);
    }

    fileUtils->setSearchPaths(resDirOrders);

    // create a scene. it's an autorelease object
    auto scene = SplashScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}


