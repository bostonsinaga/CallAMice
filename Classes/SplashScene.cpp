#include "SplashScene.h"
#include "WelcomeScene.h"
#include "AudioEngine.h"

USING_NS_CC;

int id_mice_curious_mp3;

Scene* SplashScene::createScene() {

    auto scene = Scene::create();
    auto layer = SplashScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B::WHITE)) {

        return false;
    }

    vSize = Director::getInstance()->getVisibleSize();
    vOrigin = Director::getInstance()->getVisibleOrigin();

    AudioEngine::preload("sounds/explode.mp3");
    AudioEngine::preload("sounds/mice_curious.mp3");
    AudioEngine::preload("sounds/mice_sound.mp3");
    AudioEngine::preload("sounds/mice_trapped.mp3");
    AudioEngine::preload("sounds/new_stars.mp3");
    AudioEngine::preload("sounds/notice.mp3");
    AudioEngine::preload("sounds/select_off.mp3");
    AudioEngine::preload("sounds/select_on.mp3");

    auto devLogo = Sprite::create("BACKGROUND/callapet_logo.png");
    devLogo->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
    this->addChild(devLogo, 1);

    this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&SplashScene::goToWelcomeScene), 2.5f);

    return true;
}

void SplashScene::goToWelcomeScene(float dt) {
    
    auto scene = WelcomeScene::createScene(true);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}