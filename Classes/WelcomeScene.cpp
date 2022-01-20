#include "WelcomeScene.h"
#include "MainMenuScene.h"
#include "AudioEngine.h"

USING_NS_CC;

extern int id_mice_curious_mp3;
bool isFromSplashScene;

Scene* WelcomeScene::createScene(bool isFromSplashScene_in) {

    isFromSplashScene = isFromSplashScene_in;

    auto scene = Scene::create();
    auto layer = WelcomeScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool WelcomeScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!Layer::init()) {

        return false;
    }

    vSize = Director::getInstance()->getVisibleSize();
    vOrigin = Director::getInstance()->getVisibleOrigin();

    uDef = UserDefault::getInstance();

    auto backButton_listener = EventListenerKeyboard::create();
    backButton_listener->onKeyReleased = CC_CALLBACK_2(WelcomeScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButton_listener, this);

    touching();


    ///////////
    // STUFF //
    ///////////

    if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) == -1) {

        uDef->setIntegerForKey("LEVEL 1 STARS", 0);
        uDef->flush();

        uDef->setIntegerForKey("REWARDED ADS LIMIT", 10);
        uDef->flush();

        uDef->setIntegerForKey("ENERGY AMOUNT", 10);
        uDef->flush();

        uDef->setIntegerForKey("ENERGY TIME", 0);
        uDef->flush();
    }

    auto measureBlock = Sprite::create("BLOCK/block.png");
    blockWidth = measureBlock->getContentSize().width;
    blockHeight = measureBlock->getContentSize().height;
    measureBlock->runAction(RemoveSelf::create());

    if (isFromSplashScene == true) {

        uDef->setBoolForKey("WELCOME FIRST", true);
        uDef->flush();
        
        uDef->setBoolForKey("MUSIC", true);
        uDef->flush();

        uDef->setBoolForKey("SOUND EFFECT", true);
        uDef->flush();
        
        id_mice_curious_mp3 = AudioEngine::play2d("sounds/mice_curious.mp3", true, 1.0f);
    }

    background = Sprite::create("BACKGROUND/welcome_background.png");
    background->setContentSize(Size(vSize.width + vOrigin.x, vSize.height + vOrigin.y));
    background->setPosition(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y);
    this->addChild(background, -1);

    playButton = Sprite::create("GUI/play_button.png");
    playButton->setPosition(background->getPosition());
    this->addChild(playButton, 2);

    playButton_border = Sprite::create("GUI/play_button.png");
    playButton_border->setPosition(background->getPosition());
    playButton_border->setScale(1.25);
    playButton_border->setColor(Color3B(0, 255, 255));
    this->addChild(playButton_border, 1);

    settingsButton = Sprite::create("GUI/settings_button.png");
    settingsButton->setContentSize(Size(settingsButton->getContentSize().width / 2, settingsButton->getContentSize().height / 2));
    settingsButton->setPosition(Point(vSize.width + vOrigin.x - settingsButton->getContentSize().width * 3/4, vSize.height + vOrigin.y - settingsButton->getContentSize().height * 3/4));
    this->addChild(settingsButton, 4);

    settingsButton_border = Sprite::create("GUI/settings_button.png");
    settingsButton_border->setContentSize(Size(settingsButton_border->getContentSize().width / 2, settingsButton_border->getContentSize().height / 2));
    settingsButton_border->setPosition(Point(settingsButton->getPosition().x, settingsButton->getPosition().y - blockHeight / 8));
    settingsButton_border->setColor(Color3B::GRAY);
    this->addChild(settingsButton_border, 3);

    musicButton = Sprite::create("GUI/music_button.png");
    musicButton->setPosition(settingsButton->getPosition());
    musicButton->setContentSize(Size(musicButton->getContentSize().width / 2, musicButton->getContentSize().height / 2));
    musicButton->setOpacity(0);
    
    musicButton_border = Sprite::create("GUI/music_button.png");
    musicButton_border->setPosition(settingsButton_border->getPosition());
    musicButton_border->setContentSize(Size(musicButton_border->getContentSize().width / 2, musicButton_border->getContentSize().height / 2));
    musicButton_border->setOpacity(0);

    if (uDef->getBoolForKey("MUSIC", true)) {
        
        musicButton->setColor(Color3B::YELLOW);
        musicButton_border->setColor(Color3B(127, 127, 0));
    }
    else {

        musicButton->setColor(Color3B::GRAY);
        musicButton_border->setColor(Color3B(63, 63, 63));
    }

    this->addChild(musicButton, 2);
    this->addChild(musicButton_border, 1);

    soundEffectButton = Sprite::create("GUI/sound_effect_button.png");
    soundEffectButton->setPosition(settingsButton->getPosition());
    soundEffectButton->setContentSize(Size(soundEffectButton->getContentSize().width / 2, soundEffectButton->getContentSize().height / 2));
    soundEffectButton->setOpacity(0);

    soundEffectButton_border = Sprite::create("GUI/sound_effect_button.png");
    soundEffectButton_border->setPosition(settingsButton_border->getPosition());
    soundEffectButton_border->setContentSize(Size(soundEffectButton_border->getContentSize().width / 2, soundEffectButton_border->getContentSize().height / 2));
    soundEffectButton_border->setOpacity(0);

    if (uDef->getBoolForKey("SOUND EFFECT", true)) {
        
        soundEffectButton->setColor(Color3B(0, 127, 255));
        soundEffectButton_border->setColor(Color3B(0, 63, 127));
    }
    else {

        soundEffectButton->setColor(Color3B::GRAY);
        soundEffectButton_border->setColor(Color3B(63, 63, 63));
    }

    this->addChild(soundEffectButton, 2);
    this->addChild(soundEffectButton_border, 1);

    exitButton = Sprite::create("GUI/exit_button.png");
    exitButton->setPosition(settingsButton->getPosition());
    exitButton->setContentSize(Size(exitButton->getContentSize().width / 2, exitButton->getContentSize().height / 2));
    exitButton->setColor(Color3B::RED);
    exitButton->setOpacity(0);
    this->addChild(exitButton, 2);

    exitButton_border = Sprite::create("GUI/exit_button.png");
    exitButton_border->setPosition(settingsButton_border->getPosition());
    exitButton_border->setContentSize(Size(exitButton_border->getContentSize().width / 2, exitButton_border->getContentSize().height / 2));
    exitButton_border->setColor(Color3B(127, 0, 0));
    exitButton_border->setOpacity(0);
    this->addChild(exitButton_border, 1);

    settingsBoard = Sprite::create("board_mini.png");
    settingsBoard->setPosition(settingsButton_border->getPosition());
    settingsBoard->setContentSize(Size(settingsButton->getContentSize().width, blockHeight * 9/2));
    settingsBoard->setColor(Color3B::BLACK);
    settingsBoard->setOpacity(127);
    settingsBoard->setScaleY(0.0);
    this->addChild(settingsBoard);
    
    playLabel = Label::createWithSystemFont("PLAY", "Roboto", blockWidth);
    playLabel->setPosition(Point(playButton->getPosition().x, playButton->getPosition().y - playButton->getContentSize().height / 2 - playLabel->getContentSize().height));
    playLabel->enableOutline(Color4B(0, 255, 255, 255), 5);
    this->addChild(playLabel, 2);

    return true;
}

void WelcomeScene::touching() {

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        xBegan = -1.0;
        yBegan = -1.0;

        if (isTouched == false) {

            isTouched = true;

            xBegan = touch->getLocation().x;
            yBegan = touch->getLocation().y;

            if (isTouchable == true && isNoticeTouch == false) {
                
                if (isSettingsOpened == false) {

                    if (isAtTouchPos(settingsButton, 'b')) {

                        audio_selectOn();
                        settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y));
                    }
                }
                else {

                    if (isAtTouchPos(musicButton, 'b')) {
                        
                        audio_selectOn();
                        musicButton->setPosition(Point(musicButton_border->getPosition().x, musicButton_border->getPosition().y));
                    }
                    else if (isAtTouchPos(soundEffectButton, 'b')) {
                        
                        audio_selectOn();
                        soundEffectButton->setPosition(Point(soundEffectButton_border->getPosition().x, soundEffectButton_border->getPosition().y));
                    }
                    else if (isAtTouchPos(exitButton, 'b')) {
                        
                        audio_selectOn();
                        exitButton->setPosition(Point(exitButton_border->getPosition().x, exitButton_border->getPosition().y));
                    }
                    else if (isAtTouchPos(settingsButton, 'b')) {
                        
                        audio_selectOff();
                        settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y));
                    }
                }
            }

            {   // notice of exit
                if (isNoticeTouch == true) {

                    if (isAtTouchPos(yesButton, 'b')) {

                        audio_selectOn();
                        yesButton_pressed->setVisible(true);
                        yesButton->setVisible(false);
                    }
                    else if (isAtTouchPos(noButton, 'b')) {
                        
                        audio_selectOff();
                        noButton_pressed->setVisible(true);
                        noButton->setVisible(false);
                    }
                }
            }

            {
                if (isAtTouchPos(playButton, 'b') && isTouchable == true && isNoticeTouch == false) {
                    
                    audio_selectOn();
                    playButton_border->runAction(ScaleTo::create(0.1, 0.75));
                    playLabel->enableOutline(Color4B::WHITE, 5);
                }
            }
        }

        return true;
    };

    touchListener->onTouchEnded = [=] (Touch *touch, Event *event) {

        isTouched = false;

        xEnded = touch->getLocation().x;
        yEnded = touch->getLocation().y;

        if (isTouchable == true && isNoticeTouch == false) {

            if (isSettingsOpened == false) {

                settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y + blockHeight / 8));

                if (isAtTouchPos(settingsButton, 'b') && isAtTouchPos(settingsButton, 'e')) {

                    isTouchable = false;

                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&WelcomeScene::set_isTouchable), 0.25f);
                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&WelcomeScene::set_isSettingsOpened), 0.25f);
                    
                    settingsBoard->runAction(Spawn::create(ScaleTo::create(0.25, 1.0, 1.0), MoveBy::create(0.25, Point(0, -(settingsBoard->getContentSize().height / 2 + settingsButton->getContentSize().height / 2 + blockHeight / 8))), nullptr));

                    musicButton->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 3/2)), nullptr));
                    musicButton_border->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 3/2)), nullptr));

                    soundEffectButton->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 3)), nullptr));
                    soundEffectButton_border->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 3)), nullptr));

                    exitButton->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 9/2)), nullptr));
                    exitButton_border->runAction(Spawn::create(FadeTo::create(0.0, 255), MoveBy::create(0.25, Point(0, -blockHeight * 9/2)), nullptr));
                }
            }
            else {

                musicButton->setPosition(Point(musicButton_border->getPosition().x, musicButton_border->getPosition().y + blockHeight / 8));
                soundEffectButton->setPosition(Point(soundEffectButton_border->getPosition().x, soundEffectButton_border->getPosition().y + blockHeight / 8));
                exitButton->setPosition(Point(exitButton_border->getPosition().x, exitButton_border->getPosition().y + blockHeight / 8));
                settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y + blockHeight / 8));

                if (isAtTouchPos(musicButton, 'b') && isAtTouchPos(musicButton, 'e')) {
                    
                    if (uDef->getBoolForKey("MUSIC", true) == true) {
                        
                        uDef->setBoolForKey("MUSIC", false);
                        uDef->flush();

                        musicButton->setColor(Color3B::GRAY);
                        musicButton_border->setColor(Color3B(63, 63, 63));

                        AudioEngine::stop(id_mice_curious_mp3);
                    }
                    else {

                        uDef->setBoolForKey("MUSIC", true);
                        uDef->flush();
                        
                        musicButton->setColor(Color3B::YELLOW);
                        musicButton_border->setColor(Color3B(127, 127, 0));

                        id_mice_curious_mp3 = AudioEngine::play2d("sounds/mice_curious.mp3", true, 1.0f);
                    }
                }
                else if (isAtTouchPos(soundEffectButton, 'b') && isAtTouchPos(soundEffectButton, 'e')) {
                    
                    if (uDef->getBoolForKey("SOUND EFFECT", true) == true) {
                        
                        uDef->setBoolForKey("SOUND EFFECT", false);
                        uDef->flush();

                        soundEffectButton->setColor(Color3B::GRAY);
                        soundEffectButton_border->setColor(Color3B(63, 63, 63));
                    }
                    else {

                        uDef->setBoolForKey("SOUND EFFECT", true);
                        uDef->flush();
                        
                        soundEffectButton->setColor(Color3B(0, 127, 255));
                        soundEffectButton_border->setColor(Color3B(0, 63, 127));
                    }
                }
                else if (isAtTouchPos(exitButton, 'b') && isAtTouchPos(exitButton, 'e')) {
                    
                    generateNotice();
                }
                else if (isAtTouchPos(settingsButton, 'b') && isAtTouchPos(settingsButton, 'e')) {

                    isTouchable = false;
                    isSettingsOpened = false;

                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&WelcomeScene::set_isTouchable), 0.25f);

                    // settings party back to origin condition
                    settingsBoard->runAction(Spawn::create(ScaleTo::create(0.25, 1.0, 0.0), MoveBy::create(0.25, Point(0, settingsBoard->getContentSize().height / 2 + settingsButton->getContentSize().height / 2 + blockHeight / 8)), nullptr));

                    musicButton->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 3/2)), FadeTo::create(0.0, 0), nullptr));
                    musicButton_border->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 3/2)), FadeTo::create(0.0, 0), nullptr));

                    soundEffectButton->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 3)), FadeTo::create(0.0, 0), nullptr));
                    soundEffectButton_border->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 3)), FadeTo::create(0.0, 0), nullptr));

                    exitButton->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 9/2)), FadeTo::create(0.0, 0), nullptr));
                    exitButton_border->runAction(Sequence::create(MoveBy::create(0.25, Point(0, blockHeight * 9/2)), FadeTo::create(0.0, 0), nullptr));
                }
            }
        }

        {   // notice of exit
            if (isNoticeTouch == true) {

                yesButton_pressed->setVisible(false);
                yesButton->setVisible(true);

                noButton_pressed->setVisible(false);
                noButton->setVisible(true);

                if (isAtTouchPos(yesButton, 'b') && isAtTouchPos(yesButton, 'e')) {

                    AudioEngine::end();
                    Director::getInstance()->end();
                }
                else if (isAtTouchPos(noButton, 'b') && isAtTouchPos(noButton, 'e')) {

                    isNoticeTouch = false;

                    removeNotice();
                    noticeBoard->runAction(RemoveSelf::create());
                }
            }
        }

        {
            if (isTouchable == true && isNoticeTouch == false) {

                playButton_border->runAction(ScaleTo::create(0.1, 1.25));
                playLabel->enableOutline(Color4B(0, 255, 255, 255), 5);

                if (isAtTouchPos(playButton, 'b') && isAtTouchPos(playButton, 'e')) {

                    auto scene = MainMenuScene::createScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                    
                    playButton_border->runAction(ScaleTo::create(0.1, 1.25));
                    playLabel->enableOutline(Color4B(0, 255, 255, 255), 5);
                }
            }
        }

        xBegan = -1.0;
        yBegan = -1.0;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void WelcomeScene::showNotice(std::string whatNotice) {

    areYouSure_1 = Label::createWithSystemFont(whatNotice, "Roboto", blockHeight);
    areYouSure_2 = Label::createWithSystemFont("Are you sure?", "Roboto", blockHeight / 2);
    areYouSure_3 = Label::createWithSystemFont("The application", "Roboto", blockHeight / 2);
    areYouSure_4 = Label::createWithSystemFont("will be closed!", "Roboto", blockHeight / 2);

    areYouSure_1->setPosition(Point(noticeBoard->getPosition().x, noticeBoard->getPosition().y + noticeBoard->getContentSize().height / 2 - blockHeight));
    areYouSure_2->setPosition(Point(areYouSure_1->getPosition().x, areYouSure_1->getPosition().y - areYouSure_1->getContentSize().height));
    areYouSure_3->setPosition(Point(areYouSure_2->getPosition().x, areYouSure_2->getPosition().y - areYouSure_1->getContentSize().height));
    areYouSure_4->setPosition(Point(areYouSure_3->getPosition().x, areYouSure_3->getPosition().y - areYouSure_1->getContentSize().height));

    areYouSure_1->enableBold();
    areYouSure_2->enableBold();
    areYouSure_3->enableBold();
    areYouSure_4->enableBold();

    this->addChild(areYouSure_1, 10000 + 1);
    this->addChild(areYouSure_2, 10000 + 1);
    this->addChild(areYouSure_3, 10000 + 1);
    this->addChild(areYouSure_4, 10000 + 1);

    yesButton = Sprite::create("GUI/yes_button.png");
    yesButton->setPosition(Point(noticeBoard->getPosition().x - noticeBoard->getContentSize().width / 2 + yesButton->getContentSize().width * 0.75, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + yesButton->getContentSize().height * 0.75));
    this->addChild(yesButton, 10000 + 1);

    yesButton_pressed = Sprite::create("GUI/yes_button_pressed.png");
    yesButton_pressed->setPosition(Point(noticeBoard->getPosition().x - noticeBoard->getContentSize().width / 2 + yesButton_pressed->getContentSize().width * 0.75, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + yesButton_pressed->getContentSize().height * 0.75));
    yesButton_pressed->setScale(0.9);
    yesButton_pressed->setVisible(false);
    this->addChild(yesButton_pressed, 10000 + 1);

    noButton = Sprite::create("GUI/no_button.png");
    noButton->setPosition(Point(noticeBoard->getPosition().x + noticeBoard->getContentSize().width / 2 - noButton->getContentSize().width * 0.75, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + noButton->getContentSize().height * 0.75));
    this->addChild(noButton, 10000 + 1);

    noButton_pressed = Sprite::create("GUI/no_button_pressed.png");
    noButton_pressed->setPosition(Point(noticeBoard->getPosition().x + noticeBoard->getContentSize().width / 2 - noButton_pressed->getContentSize().width * 0.75, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + noButton_pressed->getContentSize().height * 0.75));
    noButton_pressed->setScale(0.9);
    noButton_pressed->setVisible(false);
    this->addChild(noButton_pressed, 10000 + 1);
}

void WelcomeScene::removeNotice() {

    areYouSure_1->runAction(RemoveSelf::create());
    areYouSure_2->runAction(RemoveSelf::create());
    areYouSure_3->runAction(RemoveSelf::create());
    areYouSure_4->runAction(RemoveSelf::create());
    yesButton->runAction(RemoveSelf::create());
    yesButton_pressed->runAction(RemoveSelf::create());
    noButton->runAction(RemoveSelf::create());
    noButton_pressed->runAction(RemoveSelf::create());
}

inline void WelcomeScene::generateNotice() {

    isNoticeTouch = true;

    noticeBoard = Sprite::create("board.png");
    noticeBoard->setContentSize(Size(blockWidth * 8.5, blockHeight * 7.5));
    noticeBoard->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
    noticeBoard->setColor(Color3B::BLACK);
    noticeBoard->setOpacity(int(255 * 0.6));
    this->addChild(noticeBoard, 10000);

    showNotice("EXIT?");
    if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/notice.mp3", false, 1.0f);
}

void WelcomeScene::set_isTouchable(float dt) {isTouchable = true;}
void WelcomeScene::set_isSettingsOpened(float dt) {isSettingsOpened = true;}

inline void WelcomeScene::audio_selectOn()  {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_on.mp3", false, 1.0f);}
inline void WelcomeScene::audio_selectOff() {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_off.mp3", false, 1.0f);}

// back button ANDROID
void WelcomeScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

        if (isNoticeTouch == false) {

            generateNotice();
            audio_selectOn();
        }
        else {

            isNoticeTouch = false;

            audio_selectOff();
            removeNotice();

            noticeBoard->runAction(RemoveSelf::create());
        }
    }
}

bool WelcomeScene::isAtTouchPos(cocos2d::Sprite *sprite, char touchCondition) {

    bool isDetected = false;
    
    if (touchCondition == 'b') {
        
        if (xBegan >= sprite->getPosition().x - sprite->getContentSize().width / 2 && xBegan <= sprite->getPosition().x + sprite->getContentSize().width / 2
            && yBegan >= sprite->getPosition().y - sprite->getContentSize().height / 2 && yBegan <= sprite->getPosition().y + sprite->getContentSize().height / 2) {

            isDetected = true;
        }
    }
    else if (touchCondition == 'e') {
        
        if (xEnded >= sprite->getPosition().x - sprite->getContentSize().width / 2 && xEnded <= sprite->getPosition().x + sprite->getContentSize().width / 2
            && yEnded >= sprite->getPosition().y - sprite->getContentSize().height / 2 && yEnded <= sprite->getPosition().y + sprite->getContentSize().height / 2) {

            isDetected = true;
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}


