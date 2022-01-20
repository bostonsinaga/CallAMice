#include "GameOverScene.h"
#include "GameScene.h"
#include "MainMenuScene.h"
#include "AudioEngine.h"

USING_NS_CC;

unsigned int lvOver, starsOver;

Scene* GameOverScene::createScene(int level_in, int stars_in) {

    lvOver = level_in;
    starsOver = stars_in;

    auto scene = Scene::create();
    auto layer = GameOverScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool GameOverScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 200, 0, 255))) {

        return false;
    }

    vSize = Director::getInstance()->getVisibleSize();
    vOrigin = Director::getInstance()->getVisibleOrigin();

    uDef = UserDefault::getInstance();

    auto backButton_listener = EventListenerKeyboard::create();
    backButton_listener->onKeyReleased = CC_CALLBACK_2(GameOverScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButton_listener, this);

    touching();


    ///////////
    // STUFF //
    ///////////

    ////////////////
    // DECORATION //
    ////////////////

    auto measureBlock = Sprite::create("BLOCK/block.png");
    blockWidth = measureBlock->getContentSize().width;
    blockHeight = measureBlock->getContentSize().height;
    measureBlock->runAction(RemoveSelf::create());

    float oriPos[4][2];
    {
        oriPos[0][0] = vSize.width * 0 + vOrigin.x;
        oriPos[0][1] = vSize.height * 0 + vOrigin.y;

        oriPos[1][0] = vSize.width + vOrigin.x;
        oriPos[1][1] = vSize.height * 0 + vOrigin.y;

        oriPos[2][0] = vSize.width + vOrigin.x;
        oriPos[2][1] = vSize.height + vOrigin.y;

        oriPos[3][0] = vSize.width * 0 + vOrigin.x;
        oriPos[3][1] = vSize.height + vOrigin.y;
    }

    Sprite *decBlock[28], *decBlock3D[28];

    for (int i = 0; i < 28; i++) {

        decBlock[i] = Sprite::create("BLOCK/block.png");
        this->addChild(decBlock[i], i + 1);
    }
    
    decBlock[0]->setPosition(Point(oriPos[0][0]  + blockWidth,       oriPos[0][1] + blockHeight));       decBlock[0]->setColor(Color3B::RED);     decBlock[0]->setScale(1.0);
    decBlock[1]->setPosition(Point(oriPos[0][0]  + blockWidth / 2,   oriPos[0][1] + blockHeight / 2));   decBlock[1]->setColor(Color3B::GREEN);   decBlock[1]->setScale(0.5);
    decBlock[2]->setPosition(Point(oriPos[0][0]  + blockWidth * 3/2, oriPos[0][1] + blockHeight * 3/2)); decBlock[2]->setColor(Color3B::BLUE);    decBlock[2]->setScale(0.3);
    decBlock[3]->setPosition(Point(oriPos[0][0]  + blockWidth * 2,   oriPos[0][1] + blockHeight));       decBlock[3]->setColor(Color3B::YELLOW);  decBlock[3]->setScale(0.5);
    decBlock[4]->setPosition(Point(oriPos[0][0]  + blockWidth * 3,   oriPos[0][1] + blockHeight));       decBlock[4]->setColor(Color3B::MAGENTA); decBlock[4]->setScale(0.5);
    decBlock[5]->setPosition(Point(oriPos[0][0]  + blockWidth,       oriPos[0][1] + blockHeight * 2));   decBlock[5]->setColor(Color3B::ORANGE);  decBlock[5]->setScale(0.5);
    decBlock[6]->setPosition(Point(oriPos[0][0]  + blockWidth,       oriPos[0][1] + blockHeight * 3));   decBlock[6]->setColor(Color3B::RED);     decBlock[6]->setScale(0.5);

    decBlock[7]->setPosition(Point(oriPos[1][0]  - blockWidth,       oriPos[1][1] + blockHeight));       decBlock[7]->setColor(Color3B::GREEN);    decBlock[7]->setScale(1.0);
    decBlock[8]->setPosition(Point(oriPos[1][0]  - blockWidth / 2,   oriPos[1][1] + blockHeight / 2));   decBlock[8]->setColor(Color3B::BLUE);     decBlock[8]->setScale(0.5);
    decBlock[9]->setPosition(Point(oriPos[1][0]  - blockWidth * 3/2, oriPos[1][1] + blockHeight * 3/2)); decBlock[9]->setColor(Color3B::YELLOW);   decBlock[9]->setScale(0.3);
    decBlock[10]->setPosition(Point(oriPos[1][0] - blockWidth * 2,   oriPos[1][1] + blockHeight));       decBlock[10]->setColor(Color3B::MAGENTA); decBlock[10]->setScale(0.5);
    decBlock[11]->setPosition(Point(oriPos[1][0] - blockWidth * 3,   oriPos[1][1] + blockHeight));       decBlock[11]->setColor(Color3B::ORANGE);  decBlock[11]->setScale(0.5);
    decBlock[12]->setPosition(Point(oriPos[1][0] - blockWidth,       oriPos[1][1] + blockHeight * 2));   decBlock[12]->setColor(Color3B::RED);     decBlock[12]->setScale(0.5);
    decBlock[13]->setPosition(Point(oriPos[1][0] - blockWidth,       oriPos[1][1] + blockHeight * 3));   decBlock[13]->setColor(Color3B::GREEN);   decBlock[13]->setScale(0.5);

    decBlock[14]->setPosition(Point(oriPos[2][0] - blockWidth,       oriPos[2][1] - blockHeight));       decBlock[14]->setColor(Color3B::BLUE);    decBlock[14]->setScale(1.0);
    decBlock[15]->setPosition(Point(oriPos[2][0] - blockWidth / 2,   oriPos[2][1] - blockHeight / 2));   decBlock[15]->setColor(Color3B::YELLOW);  decBlock[15]->setScale(0.5);
    decBlock[16]->setPosition(Point(oriPos[2][0] - blockWidth * 3/2, oriPos[2][1] - blockHeight * 3/2)); decBlock[16]->setColor(Color3B::MAGENTA); decBlock[16]->setScale(0.3);
    decBlock[17]->setPosition(Point(oriPos[2][0] - blockWidth * 2,   oriPos[2][1] - blockHeight));       decBlock[17]->setColor(Color3B::ORANGE);  decBlock[17]->setScale(0.5);
    decBlock[18]->setPosition(Point(oriPos[2][0] - blockWidth * 3,   oriPos[2][1] - blockHeight));       decBlock[18]->setColor(Color3B::RED);     decBlock[18]->setScale(0.5);
    decBlock[19]->setPosition(Point(oriPos[2][0] - blockWidth,       oriPos[2][1] - blockHeight * 2));   decBlock[19]->setColor(Color3B::GREEN);   decBlock[19]->setScale(0.5);
    decBlock[20]->setPosition(Point(oriPos[2][0] - blockWidth,       oriPos[2][1] - blockHeight * 3));   decBlock[20]->setColor(Color3B::BLUE);    decBlock[20]->setScale(0.5);

    decBlock[21]->setPosition(Point(oriPos[3][0] + blockWidth,       oriPos[3][1] - blockHeight));       decBlock[21]->setColor(Color3B::YELLOW);  decBlock[21]->setScale(1.0);
    decBlock[22]->setPosition(Point(oriPos[3][0] + blockWidth / 2,   oriPos[3][1] - blockHeight / 2));   decBlock[22]->setColor(Color3B::MAGENTA); decBlock[22]->setScale(0.5);
    decBlock[23]->setPosition(Point(oriPos[3][0] + blockWidth * 3/2, oriPos[3][1] - blockHeight * 3/2)); decBlock[23]->setColor(Color3B::ORANGE);  decBlock[23]->setScale(0.3);
    decBlock[24]->setPosition(Point(oriPos[3][0] + blockWidth * 2,   oriPos[3][1] - blockHeight));       decBlock[24]->setColor(Color3B::RED);     decBlock[24]->setScale(0.5);
    decBlock[25]->setPosition(Point(oriPos[3][0] + blockWidth * 3,   oriPos[3][1] - blockHeight));       decBlock[25]->setColor(Color3B::GREEN);   decBlock[25]->setScale(0.5);
    decBlock[26]->setPosition(Point(oriPos[3][0] + blockWidth,       oriPos[3][1] - blockHeight * 2));   decBlock[26]->setColor(Color3B::BLUE);    decBlock[26]->setScale(0.5);
    decBlock[27]->setPosition(Point(oriPos[3][0] + blockWidth,       oriPos[3][1] - blockHeight * 3));   decBlock[27]->setColor(Color3B::YELLOW);  decBlock[27]->setScale(0.5);

    for (int i = 0; i < 28; i++) {

        decBlock3D[i] = Sprite::create("BLOCK/block_3d_effect.png");
        decBlock3D[i]->setPosition(decBlock[i]->getPosition());
        decBlock3D[i]->setScale(decBlock[i]->getScale());
        decBlock3D[i]->setColor(Color3B::BLACK);
        this->addChild(decBlock3D[i], i + 1);
    }


    ///////////
    // LABEL //
    ///////////

    starBlank[0] = Sprite::create("GUI/star_blank_game_over.png");
    starBlank[0]->setPosition(Point(vSize.width / 2 + vOrigin.x - starBlank[0]->getContentSize().width * 3/2, vSize.height * 4/5 + vOrigin.y - starBlank[0]->getContentSize().height / 2));
    this->addChild(starBlank[0], 1);

    starBlank[1] = Sprite::create("GUI/star_blank_game_over.png");
    starBlank[1]->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height * 4/5 + vOrigin.y));
    this->addChild(starBlank[1], 1);

    starBlank[2] = Sprite::create("GUI/star_blank_game_over.png");
    starBlank[2]->setPosition(Point(vSize.width / 2 + vOrigin.x + starBlank[2]->getContentSize().width * 3/2, vSize.height * 4/5 + vOrigin.y - starBlank[2]->getContentSize().height / 2));
    this->addChild(starBlank[2], 1);

    if (starsOver != 0) {

        if (starsOver == 1) {

            star[0] = Sprite::create("GUI/star_game_over.png");
            star[0]->setPosition(starBlank[0]->getPosition());
            star[0]->setScale(0.0);
            this->addChild(star[0], 1);

            star[0]->runAction(ScaleTo::create(0.5, 1.0));
        }
        else if (starsOver == 2) {

            star[0] = Sprite::create("GUI/star_game_over.png");
            star[0]->setPosition(starBlank[0]->getPosition());
            star[0]->setScale(0.0);
            this->addChild(star[0], 2);
            
            star[1] = Sprite::create("GUI/star_game_over.png");
            star[1]->setPosition(starBlank[1]->getPosition());
            star[1]->setScale(0.0);
            this->addChild(star[1], 2);

            star[0]->runAction(ScaleTo::create(0.5, 1.0));
            star[1]->runAction(Sequence::create(DelayTime::create(0.5), ScaleTo::create(0.5, 1.0), nullptr));
        }
        else if (starsOver == 3) {

            star[0] = Sprite::create("GUI/star_game_over.png");
            star[0]->setPosition(starBlank[0]->getPosition());
            star[0]->setScale(0.0);
            this->addChild(star[0], 2);
            
            star[1] = Sprite::create("GUI/star_game_over.png");
            star[1]->setPosition(starBlank[1]->getPosition());
            star[1]->setScale(0.0);
            this->addChild(star[1], 2);

            star[2] = Sprite::create("GUI/star_game_over.png");
            star[2]->setPosition(starBlank[2]->getPosition());
            star[2]->setScale(0.0);
            this->addChild(star[2], 2);

            star[0]->runAction(ScaleTo::create(0.5, 1.0));
            star[1]->runAction(Sequence::create(DelayTime::create(0.5), ScaleTo::create(0.5, 1.0), nullptr));
            star[2]->runAction(Sequence::create(DelayTime::create(1.0), ScaleTo::create(0.5, 1.0), nullptr));
        }

        auto title_1 = Label::createWithSystemFont("LEVEL", "Roboto", blockHeight * 4/3);
        title_1->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height * 2/3 + vOrigin.y - title_1->getContentSize().height));
        title_1->setColor(Color3B(255, 225, 0));
        title_1->enableBold();
        this->addChild(title_1, 1);

        auto title_2 = Label::createWithSystemFont("COMPLETED", "Roboto", blockHeight * 4/3);
        title_2->setPosition(Point(vSize.width / 2 + vOrigin.x, title_1->getPosition().y - title_2->getContentSize().height));
        title_2->setColor(Color3B(255, 225, 0));
        title_2->enableBold();
        this->addChild(title_2, 1);
    }
    else {

        auto title_1 = Label::createWithSystemFont("LEVEL", "Roboto", blockHeight * 4/3);
        title_1->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height * 2/3 + vOrigin.y - title_1->getContentSize().height));
        title_1->setColor(Color3B::RED);
        title_1->enableBold();
        this->addChild(title_1, 1);

        auto title_2 = Label::createWithSystemFont("FAILED", "Roboto", blockHeight * 4/3);
        title_2->setPosition(Point(vSize.width / 2 + vOrigin.x, title_1->getPosition().y - title_2->getContentSize().height));
        title_2->setColor(Color3B::RED);
        title_2->enableBold();
        this->addChild(title_2, 1);
    }


    ////////////
    // BUTTON //
    ////////////

    retryButton = Sprite::create("GUI/retry_button.png");
    retryButton->setPosition(Point(vSize.width / 2 + vOrigin.x - retryButton->getContentSize().width * 3/2, vSize.height * 1/3 + vOrigin.y));
    retryButton->setVisible(false);
    this->addChild(retryButton, 2);

    mainMenuButton = Sprite::create("GUI/main_menu_button.png");
    mainMenuButton->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height * 1/3 + vOrigin.y));
    mainMenuButton->setVisible(false);
    this->addChild(mainMenuButton, 2);

    nextLevelButton = Sprite::create("GUI/next_level_button.png");
    nextLevelButton->setPosition(Point(vSize.width / 2 + vOrigin.x + retryButton->getContentSize().width * 3/2, vSize.height * 1/3 + vOrigin.y));
    nextLevelButton->setVisible(false);
    this->addChild(nextLevelButton, 2);

    retryButton_border = Sprite::create("GUI/retry_button.png");
    retryButton_border->setPosition(Point(retryButton->getPosition().x, retryButton->getPosition().y - blockHeight / 8));
    retryButton_border->setColor(Color3B(0, 100, 0));
    retryButton_border->setVisible(false);
    this->addChild(retryButton_border, 1);

    mainMenuButton_border = Sprite::create("GUI/main_menu_button.png");
    mainMenuButton_border->setPosition(Point(mainMenuButton->getPosition().x, mainMenuButton->getPosition().y - blockHeight / 8));
    mainMenuButton_border->setColor(Color3B(0, 100, 0));
    mainMenuButton_border->setVisible(false);
    this->addChild(mainMenuButton_border, 1);

    nextLevelButton_border = Sprite::create("GUI/next_level_button.png");
    nextLevelButton_border->setPosition(Point(nextLevelButton->getPosition().x, nextLevelButton->getPosition().y - blockHeight / 8));
    nextLevelButton_border->setColor(Color3B(0, 100, 0));
    nextLevelButton_border->setVisible(false);
    this->addChild(nextLevelButton_border, 1);

    // saving stars
    sprintf(stringBuffer_nextLevel, "LEVEL %d STARS", lvOver + 1);
    if (starsOver == 0 && uDef->getIntegerForKey(stringBuffer_nextLevel, -1) == -1) nextLevelButton->setColor(Color3B::GRAY);

    sprintf(stringBuffer, "LEVEL %d STARS", lvOver);
    if (uDef->getIntegerForKey(stringBuffer, -1) < starsOver) this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameOverScene::newStarsCelebration), float(starsOver * 0.5));

    generateLoadingBar_sub();

    // next level unlock
    int intKeyBuffer = uDef->getIntegerForKey(stringBuffer, -1);
    sprintf(stringBuffer, "LEVEL %d STARS", lvOver + 1);
    if (uDef->getIntegerForKey(stringBuffer, -1) == -1 && (starsOver != 0 || intKeyBuffer != 0)) {
        
        uDef->setIntegerForKey(stringBuffer, 0);
        uDef->flush();
    }

    return true;
}

void GameOverScene::touching() {

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);

    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        if (isTouchable == true) {
            
            xBegan = -1.0;
            yBegan = -1.0;

            if (isTouched == false) {

                isTouched = true;

                xBegan = touch->getLocation().x;
                yBegan = touch->getLocation().y;

                if (isAtTouchPos(retryButton, 'b')) {
                    
                    audio_selectOn();
                    retryButton->setPosition(Point(retryButton_border->getPosition().x, retryButton_border->getPosition().y));
                }
                else if (isAtTouchPos(mainMenuButton, 'b')) {
                    
                    audio_selectOn();
                    mainMenuButton->setPosition(Point(mainMenuButton_border->getPosition().x, mainMenuButton_border->getPosition().y));
                }
                else if (isAtTouchPos(nextLevelButton, 'b')) {
                    
                    if (starsOver != 0 || uDef->getIntegerForKey(stringBuffer_nextLevel, -1) != -1) {
                        
                        audio_selectOn();
                        nextLevelButton->setPosition(Point(nextLevelButton_border->getPosition().x, nextLevelButton_border->getPosition().y));
                    }
                }
            }
        }

        return true;
    };

    touchListener->onTouchEnded = [=] (Touch *touch, Event *event) {

        if (isTouchable == true) {
            
            isTouched = false;

            xEnded = touch->getLocation().x;
            yEnded = touch->getLocation().y;
                
            retryButton->setPosition(Point(retryButton_border->getPosition().x, retryButton_border->getPosition().y + blockHeight / 8));
            mainMenuButton->setPosition(Point(mainMenuButton_border->getPosition().x, mainMenuButton_border->getPosition().y + blockHeight / 8));

            if (isAtTouchPos(retryButton, 'b') && isAtTouchPos(retryButton, 'e')) {

                auto scene = GameScene::createScene(lvOver);
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
            }
            else if (isAtTouchPos(mainMenuButton, 'b') && isAtTouchPos(mainMenuButton, 'e')) {

                auto scene = MainMenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
            }

            {
                if (starsOver != 0 || uDef->getIntegerForKey(stringBuffer_nextLevel, -1) != -1) {

                    nextLevelButton->setPosition(Point(nextLevelButton_border->getPosition().x, nextLevelButton_border->getPosition().y + blockHeight / 8));
                    
                    if (isAtTouchPos(nextLevelButton, 'b') && isAtTouchPos(nextLevelButton, 'e')) {

                        // save the current level
                        uDef->setIntegerForKey("CURRENT LEVEL", lvOver + 1);
                        uDef->flush();

                        auto scene = GameScene::createScene(lvOver + 1);
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                    }
                }
            }

            xBegan = -1.0;
            yBegan = -1.0;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameOverScene::newStarsCelebration(float dt) {

    sprintf(stringBuffer, "LEVEL %d STARS", lvOver);
    for (int i = 0; i < starsOver; i++) {
        
        if (uDef->getIntegerForKey(stringBuffer, -1) < i + 1) {
        
            star[i]->runAction(RepeatForever::create(Sequence::create(
                                                                        ScaleTo::create(0.5, starBlank[i]->getContentSize().width / star[i]->getContentSize().width),
                                                                        ScaleTo::create(0.5, (starBlank[i]->getContentSize().width + star[i]->getContentSize().width) / 2 / star[i]->getContentSize().width),
                                                                        nullptr
                                                                    )));
        }
    }

    auto newStarsLabel = Label::createWithSystemFont("NEW STARS !!!", "Roboto", blockWidth * 2/3);
    newStarsLabel->setPosition(Point(starBlank[1]->getPosition().x, starBlank[1]->getPosition().y - starBlank[1]->getContentSize().height));
    newStarsLabel->enableBold();
    this->addChild(newStarsLabel, 1);

    newStarsLabel->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.1), ScaleTo::create(0.5, 1.0), nullptr)));

    uDef->setIntegerForKey(stringBuffer, starsOver);
    uDef->flush();

    if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/new_stars.mp3", false, 1.0f);
}

void GameOverScene::set_isTouchable(float dt) {isTouchable = true;}

void GameOverScene::generateLoadingBar(float dt) {

    loadingBar = Sprite::create("GUI/loading_bar.png");
    loadingBar->setPosition(mainMenuButton->getPosition());
    loadingBar->setAnchorPoint(Vec2(0.5, loadingBar->getContentSize().width / 2 / loadingBar->getContentSize().height));
    loadingBar->runAction(RotateBy::create(dt, 360 * dt));
    this->addChild(loadingBar, 100);

    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameOverScene::set_isTouchable), dt);
    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameOverScene::removeLoadingBar), dt);
}

inline void GameOverScene::generateLoadingBar_sub() {

    if (uDef->getIntegerForKey(stringBuffer, -1) < starsOver) generateLoadingBar(float(starsOver * 0.5));
    else {
        
        isTouchable = true;

        retryButton->setVisible(true);
        retryButton_border->setVisible(true);
        mainMenuButton->setVisible(true);
        mainMenuButton_border->setVisible(true);
        nextLevelButton->setVisible(true);
        nextLevelButton_border->setVisible(true);
    }
}

void GameOverScene::removeLoadingBar(float dt) {

    loadingBar->runAction(RemoveSelf::create());

    retryButton->setVisible(true);
    retryButton_border->setVisible(true);
    mainMenuButton->setVisible(true);
    mainMenuButton_border->setVisible(true);
    nextLevelButton->setVisible(true);
    nextLevelButton_border->setVisible(true);
}

inline void GameOverScene::audio_selectOn()  {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_on.mp3", false, 1.0f);}
inline void GameOverScene::audio_selectOff() {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_off.mp3", false, 1.0f);}
inline void GameOverScene::audio_notice()    {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/notice.mp3", false, 1.0f);}

// back button ANDROID
void GameOverScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

        if (isTouchable == true) {

            audio_selectOn();

            auto scene = MainMenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
    }
}

bool GameOverScene::isAtTouchPos(cocos2d::Sprite *sprite, char touchCondition) {

    bool isDetected = false;
    
    if (touchCondition == 'b') {
        
        if (xBegan >= sprite->getPosition().x - sprite->getContentSize().width / 2 && xBegan <= sprite->getPosition().x + sprite->getContentSize().width / 2
            && yBegan >= sprite->getPosition().y - sprite->getContentSize().height / 2 && yBegan <= sprite->getPosition().y + sprite->getContentSize().height / 2) {

            isDetected = true;
        }
    }
    else {
        
        if (xEnded >= sprite->getPosition().x - sprite->getContentSize().width / 2 && xEnded <= sprite->getPosition().x + sprite->getContentSize().width / 2
            && yEnded >= sprite->getPosition().y - sprite->getContentSize().height / 2 && yEnded <= sprite->getPosition().y + sprite->getContentSize().height / 2) {

            isDetected = true;
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}


