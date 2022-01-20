#include "GameScene.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "AudioEngine.h"

USING_NS_CC;

extern int id_mice_curious_mp3;
unsigned int level;

Scene* GameScene::createScene(int level_in) {

    level = level_in;

    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 200, 0, 255))) {

        return false;
    }

    vSize = Director::getInstance()->getVisibleSize();
    vOrigin = Director::getInstance()->getVisibleOrigin();

    uDef = UserDefault::getInstance();

    auto backButton_listener = EventListenerKeyboard::create();
    backButton_listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButton_listener, this);

    touching();


    ///////////
    // STUFF //
    ///////////

    ////////////
    // LEVELS //
    ////////////

    // random int
    std::srand(time(0));
    seed = std::rand() % 1000;

    blocksMap = new BlocksMap();

    int fkTag, fkVS_w, fkVS_h, randFake;

    if (level == 28) {

        randFake = std::rand() % 4 + 1;

        switch (randFake) {

            case 1:
                fkTag = 21;
                fkVS_w = 0;
                fkVS_h = 1;
                break;

            case 2:
                fkTag = 3;
                fkVS_w = 1;
                fkVS_h = 0;
                break;

            case 3:
                fkTag = 27;
                fkVS_w = 0;
                fkVS_h = -1;
                break;

            case 4:
                fkTag = 45;
                fkVS_w = -1;
                fkVS_h = 0;
                break;

            default :
            fkTag = 21;
            fkVS_w = 0;
            fkVS_h = 1;
        }
    }
    else if (level == 43) {

        randFake = std::rand() % 3 + 1;

        switch (randFake) {

            case 1:
                fkTag = 56;
                fkVS_w = -1;
                fkVS_h = 1;
                break;

            case 2:
                fkTag = 0;
                fkVS_w = 1;
                fkVS_h = 1;
                break;

            case 3:
                fkTag = 7;
                fkVS_w = 1;
                fkVS_h = -1;
                break;

            default :
            fkTag = 56;
            fkVS_w = -1;
            fkVS_h = 1;
        }
    }
    else if (level == 44) {

        randFake = std::rand() % 2 + 1;
        
        switch (randFake) {

            case 1:
                fkTag = 71;
                fkVS_w = -1;
                fkVS_h = -1;
                break;

            case 2:
                fkTag = 63;
                fkVS_w = -1;
                fkVS_h = 1;
                break;

            default :
            fkTag = 71;
            fkVS_w = -1;
            fkVS_h = -1;
        }
    }
    else if (level == 45) {

        randFake = std::rand() % 2 + 1;
        
        switch (randFake) {

            case 1:
                fkTag = 64;
                fkVS_w = -1;
                fkVS_h = 1;
                break;

            case 2:
                fkTag = 0;
                fkVS_w = 1;
                fkVS_h = 1;
                break;

            default :
            fkTag = 64;
            fkVS_w = -1;
            fkVS_h = 1;
        }
    }

    switch (level) {

        // set 1
        case 1 :
            blocksMap->mapValues(4 * blocksMap->getBlockWidth(), 3 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 10, 1, 1, 0); break;

        case 2 :
            blocksMap->mapValues(3 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 7, 4, 0, 1); break;

        case 3 :
            blocksMap->mapValues(3 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 11, 0, 1, 1); break;

        case 4 : // obstacles on
            blocksMap->mapValues(3 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 14, 10, -1, 0); break;

        case 5 : // obstacles on
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 3 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 11, 5, 0, -1); break;

        case 6 :
            blocksMap->mapValues(4 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 3, 12, -1, 0); break;

        case 7 :
            blocksMap->mapValues(4 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 5, 14, 0, -1); break;

        case 8 :
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 2, 17, -1, 0); break;

        case 9 :
            blocksMap->mapValues(4 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 0, 23, -1, -1); break;

        case 10 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 20, 3, 1, -1); break;

        case 11 : // obstacles on
            blocksMap->mapValues(4 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 27, 21, -1, 1); break;

        case 12 : // obstacles on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 4 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 25, 1, 1, 0); break;

        case 13 : // obstacles on
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 9, 5, 0, 1); break;

        case 14 : // obstacles on
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 29, 0, 1, 0); break;

        case 15 : // obstacles on
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 4, 0, 1, 0); break;

        // set 2
        case 16 :
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 14, 10, 0, 1); break;

        case 17 :
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 12, 17, 0, -1); break;

        case 18 :
            blocksMap->mapValues(5 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 20, 14, 0, 1); break;

        case 19 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 27, 2, 1, 0); break;

        case 20 :
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 5 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 2, 32, -1, 0); break;

        case 21 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 30, 5, 1, 0); break;

        case 22 : // gate on
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 41, 0, 1, 1); break;

        case 23 : // gate on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 36, 5, 1, -1); break;

        case 24 : // gate on
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 3, 44, -1, 0); break;

        case 25 : // gate on
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 23, 24, 0, 1); break;

        case 26 : // gate on
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 9 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 0, 53, -1, 0); break;

        case 27 : // gate on
            blocksMap->mapValues(9 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 48, 5, 0, -1); break;

        case 28 : // obstacles on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 24, fkTag, fkVS_w, fkVS_h); break;

        case 29 : // obstacles on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 25, 31, 0, -1); break;

        case 30 : // obstacles on
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 3, 52, -1, 0); break;

        // set 3
        case 31 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 23, 12, 0, 1); break;

        case 32 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 38, 3, 1, 0); break;

        case 33 :
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 18, 23, 0, -1); break;

        case 34 :
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 0, 47, -1, -1); break;

        case 35 :
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 42, 5, 1, -1); break;

        case 36 : // obstacles on
            blocksMap->mapValues(6 * blocksMap->getBlockWidth(), 9 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 49, 4, 1, 0); break;

        case 37 : // obstacles on
            blocksMap->mapValues(9 * blocksMap->getBlockWidth(), 6 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 24, 29, 0, -1); break;

        case 38 : // obstacles on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 21, 27, 0, -1); break;

        case 39 : // gate on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 55, 0, 1, 1); break;

        case 40 : // gate on
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 52, 3, 1, 0); break;

        case 41 : // gate on
            blocksMap->mapValues(7 * blocksMap->getBlockWidth(), 9 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 35, 27, 0, 1); break;

        case 42 : // gate on
            blocksMap->mapValues(9 * blocksMap->getBlockWidth(), 7 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 3, 59, -1, 0); break;

        case 43 : // fake plates
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 63, fkTag, fkVS_w, fkVS_h); break;

        case 44 : // fake plates
            blocksMap->mapValues(8 * blocksMap->getBlockWidth(), 9 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 4, fkTag, fkVS_w, fkVS_h); break;

        case 45 : // fake plates
            blocksMap->mapValues(9 * blocksMap->getBlockWidth(), 8 * blocksMap->getBlockHeight());
            blocksMap->mapGenerate(this, seed, level, 39, fkTag, fkVS_w, fkVS_h); break;

        default :
        blocksMap->mapValues(2 * blocksMap->getBlockWidth(), 2 * blocksMap->getBlockHeight());
        blocksMap->mapGenerate(this, seed, level, 0, 0, 1, 1); break;
    }
    
    blocksMap_miceAnimation_scheduler();

    // checking wheter are color of blocks randomly determined (unblastable)
    for (int i = 0; i < 25; i++) {

        blocksMap->blocksScan();
        blocksMap->blocksBlast(true);
    }

    // GUI
    pauseButton = Sprite::create("GUI/pause_button.png");
    pauseButton->setPosition(Point(vSize.width * 0 + vOrigin.x + pauseButton->getContentSize().width / 2, vSize.height + vOrigin.y - pauseButton->getContentSize().height / 2));
    this->addChild(pauseButton, 10000);

    if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) == 0) pauseButton->setVisible(false);

    fewestMovesOrigin = blocksMap->getFewestMoves();
    fewestMoves = fewestMovesOrigin;

    swipeLabel = Label::createWithSystemFont(std::to_string(fewestMoves), "Roboto", blocksMap->getBlockHeight() * 4/3);
    swipeLabel->setPosition(Point(vSize.width / 2 + vOrigin.x, pauseButton->getPosition().y - pauseButton->getContentSize().height / 2 - swipeLabel->getContentSize().height));
    swipeLabel->setColor(Color3B::WHITE);
    swipeLabel->enableBold();
    this->addChild(swipeLabel, 10000 - 1);

    swipeSign = Sprite::create("GUI/swipe_sign.png");
    swipeSign->setPosition(Point(swipeLabel->getPosition().x - swipeLabel->getContentSize().width / 2 - swipeSign->getContentSize().width / 2, swipeLabel->getPosition().y));
    swipeSign->setColor(Color3B(255, 225, 0));
    this->addChild(swipeSign, 10000 - 1);

    swipeSign_flippedX = Sprite::create("GUI/swipe_sign.png");
    swipeSign_flippedX->setPosition(Point(swipeLabel->getPosition().x + swipeLabel->getContentSize().width / 2 + swipeSign->getContentSize().width / 2, swipeLabel->getPosition().y));
    swipeSign_flippedX->setColor(Color3B(255, 225, 0));
    swipeSign_flippedX->setFlippedX(true);
    this->addChild(swipeSign_flippedX, 10000 - 1);

    swipeIndicator = Sprite::create("GUI/swipe_indicator.png");
    swipeIndicator->setPosition(Point(vSize.width + vOrigin.x - swipeIndicator->getContentSize().width / 2 - swipeIndicator->getContentSize().height / 8, vSize.height + vOrigin.y - swipeIndicator->getContentSize().height * 5/8));
    this->addChild(swipeIndicator, 10000 - 2);

    swipeIndicator_behind = Sprite::create("GUI/swipe_indicator.png");
    swipeIndicator_behind->setPosition(swipeIndicator->getPosition());
    swipeIndicator_behind->setColor(Color3B::BLACK);
    swipeIndicator_behind->setOpacity(127);
    this->addChild(swipeIndicator_behind, 10000 - 3);

    starIndicator_1 = Sprite::create("GUI/star_indicator.png");
    starIndicator_1->setPosition(Point(swipeIndicator->getPosition().x, swipeIndicator->getPosition().y + swipeIndicator->getContentSize().height * 1/6));
    this->addChild(starIndicator_1, 10000 - 1);

    starIndicator_2 = Sprite::create("GUI/star_indicator.png");
    starIndicator_2->setPosition(Point(swipeIndicator->getPosition().x, swipeIndicator->getPosition().y - swipeIndicator->getContentSize().height * 1/6));
    this->addChild(starIndicator_2, 10000 - 1);

    starIndicator_3 = Sprite::create("GUI/star_indicator.png");
    starIndicator_3->setPosition(Point(swipeIndicator->getPosition().x, swipeIndicator->getPosition().y - swipeIndicator->getContentSize().height / 2));
    this->addChild(starIndicator_3, 10000 - 1);

    starIndicator_1_blank = Sprite::create("GUI/star_indicator_blank.png");
    starIndicator_1_blank->setPosition(starIndicator_1->getPosition());
    starIndicator_1_blank->setVisible(false);
    this->addChild(starIndicator_1_blank, 10000 - 1);

    starIndicator_2_blank = Sprite::create("GUI/star_indicator_blank.png");
    starIndicator_2_blank->setPosition(starIndicator_2->getPosition());
    starIndicator_2_blank->setVisible(false);
    this->addChild(starIndicator_2_blank, 10000 - 1);

    starIndicator_3_blank = Sprite::create("GUI/star_indicator_blank.png");
    starIndicator_3_blank->setPosition(starIndicator_3->getPosition());
    starIndicator_3_blank->setVisible(false);
    this->addChild(starIndicator_3_blank, 10000 - 1);

    // notify current level
    char stringBuff[64];
    std::sprintf(stringBuff, "LEVEL %d", level);
    auto noticeLevel = Label::createWithSystemFont(stringBuff, "Roboto", blocksMap->getBlockHeight() * 4/3);
    noticeLevel->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
    noticeLevel->setColor(Color3B(100, 100, 100));
    noticeLevel->enableBold();
    this->addChild(noticeLevel, 10000 - 1);

    noticeLevel->runAction(Sequence::create(DelayTime::create(0.25), TintTo::create(0.5, Color3B::WHITE), DelayTime::create(0.25), FadeOut::create(0.5), nullptr));
    noticeLevel->runAction(ScaleTo::create(1.5, 1.5));

    // touch delay
    if (blocksMap->holesVisible()) this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::set_isTouchable), 2.0f);
    else                           this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::set_isTouchable), 1.5f);

    // show tutorials
    if (level == 1) {

        if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) == 0) {

            blocksMap->tutorialLevel1_blocksSet();
            this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_tutorialLevel1_arrowMove), 1.5f);
            this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_tutorialLevel1_pointingBlocks), 1.0f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::conditionLabel_generate_tutlv1), 1.5f);
        }
    }
    else if (level == 16) {

        if (uDef->getIntegerForKey("LEVEL 16 STARS", -1) == 0) {

            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::conditionLabel_generate_tutlv16_1), 1.5f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::conditionLabel_generate_tutlv16_2), 2.5f);
        }
    }

    return true;
}

void GameScene::touching() {

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        if (isTouched == false && isTouchable == true) {

            isTouched = true;

            xBegan = touch->getLocation().x;
            yBegan = touch->getLocation().y;

            if (isTouchAtBlocksArea() && isActionTouch == true) {
                
                for (int i = 0; i < blocksMap->getBlockCount(); i++) {

                    if ((xBegan >= blocksMap->getPosX(i) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(i) + blocksMap->getBlockWidth() / 2)
                        && (yBegan >= blocksMap->getPosY(i) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(i) + blocksMap->getBlockHeight() / 2)) {

                        if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) == 0) {
                                
                            if (i == 4 || i == 5) {

                                blockMember = i;
                                break;
                            }
                        }
                        else {

                            blocksMap->unselectBlock(blockMember);
                            blocksMap->selectBlock(i);
                            blockMember = i;
                            break;
                        }
                    }
                }

                if (isBeginningGame == true) {
                
                    isBeginningGame = false;

                    blocksMap->holesInvisible();
                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_holesInvisible_rebuild), 0.5f);
                }
            }

            {
                if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) != 0) {

                    if (isAtTouchPos(pauseButton, 'b') && isActionTouch == true) {

                        audio_selectOn();
                        pauseButton->setColor(Color3B(225, 225, 225));
                        pauseButton->setScale(0.9);
                    }
                }
            }

            {
                if (isActionTouch == false && isNoticeVerifying == true) {

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
                if (isActionTouch == false && isNoticeVerifying == false) {

                    if (isAtTouchPos(resumeButton, 'b')) {
                        
                        audio_selectOn();
                        resumeLabel->setColor(Color3B(225, 225, 225));
                        resumeLabel->setScale(0.9);
                        resumeButton->setScale(0.9);
                    }
                    else if (isAtTouchPos(restartButton, 'b')) {
                        
                        audio_selectOn();
                        restartLabel->setColor(Color3B(225, 225, 225));
                        restartLabel->setScale(0.9);
                        restartButton->setScale(0.9);
                    }
                    else if (isAtTouchPos(quitButton, 'b')) {
                        
                        audio_selectOn();
                        quitLabel->setColor(Color3B(225, 225, 225));
                        quitLabel->setScale(0.9);
                        quitButton->setScale(0.9);
                    }
                    else if (isAtTouchPos(musicButton, 'b')) {
                        
                        audio_selectOn();
                        musicButton->setPosition(Point(musicButton_border->getPosition().x, musicButton_border->getPosition().y));
                    }
                    else if (isAtTouchPos(soundEffectButton, 'b')) {
                        
                        audio_selectOn();
                        soundEffectButton->setPosition(Point(soundEffectButton_border->getPosition().x, soundEffectButton_border->getPosition().y));
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

            if (isTouchAtBlocksArea() && isActionTouch == true) {

                if (uDef->getIntegerForKey("LEVEL 1 STARS", -1) == 0) {
                    
                    if (
                            (xBegan >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yBegan >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        &&
                            (xEnded >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() && xEnded <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth())
                        &&
                            (yEnded < blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2)

                        && blockMember != 0) {

                        blocksMap->moveBlock(blockMember, "DOWN");

                        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_tutorialLevel1_arrowMove));
                        blocksMap->tutorialLevel1_arrowUnmove();

                        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_tutorialLevel1_pointingBlocks));
                        blocksMap->tutorialLevel1_unpointingBlocks();
                    }
                }
                else {
                    
                    if (
                            (xBegan >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yBegan >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        &&
                            (xEnded > blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yEnded >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() && yEnded <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight())
                        ) {

                        blocksMap->moveBlock(blockMember, "RIGHT");
                    }
                    else if (
                            (xBegan >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yBegan >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        &&
                            (xEnded < blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2)
                        &&
                            (yEnded >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() && yEnded <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight())
                        ) {

                        blocksMap->moveBlock(blockMember, "LEFT");
                    }
                    else if (
                            (xBegan >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yBegan >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        &&
                            (xEnded >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() && xEnded <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth())
                        &&
                            (yEnded > blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        ) {

                        blocksMap->moveBlock(blockMember, "UP");
                    }
                    else if (
                            (xBegan >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() / 2 && xBegan <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth() / 2)
                        &&
                            (yBegan >= blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2 && yBegan <= blocksMap->getPosY(blockMember) + blocksMap->getBlockHeight() / 2)
                        &&
                            (xEnded >= blocksMap->getPosX(blockMember) - blocksMap->getBlockWidth() && xEnded <= blocksMap->getPosX(blockMember) + blocksMap->getBlockWidth())
                        &&
                            (yEnded < blocksMap->getPosY(blockMember) - blocksMap->getBlockHeight() / 2)
                        ) {

                        blocksMap->moveBlock(blockMember, "DOWN");
                    }
                }

                retryScan = true;

                blocksMap->unselectBlock(blockMember);

                _eventDispatcher->removeEventListener(touchListener);

                this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_blocksBlast), 0.1f);
            }

            {
                if (isActionTouch == true && uDef->getIntegerForKey("LEVEL 1 STARS", -1) != 0) {

                    if (isAtTouchPos(pauseButton, 'b') && isAtTouchPos(pauseButton, 'e')) {

                        isActionTouch = false;
                        pauseButton->setScale(1.0);
                        generatePauseMenu();
                    }
                    else {

                        pauseButton->setColor(Color3B::WHITE);
                        pauseButton->setScale(1.0);
                    }
                }
            }

            {if (isActionTouch == false && isNoticeVerifying == true) {

                yesButton_pressed->setVisible(false);
                yesButton->setVisible(true);

                noButton_pressed->setVisible(false);
                noButton->setVisible(true);

                if (isAtTouchPos(yesButton, 'b') && isAtTouchPos(yesButton, 'e')) {

                    if (isRestart == true) {

                        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
                        delete blocksMap;

                        auto scene = GameScene::createScene(level);
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                    }
                    else if (isRestart == false) {
                        
                        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
                        
                        delete blocksMap;

                        auto scene = MainMenuScene::createScene();
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                    }
                }
                else if (isAtTouchPos(noButton, 'b') && isAtTouchPos(noButton, 'e')) {

                    isNoticeVerifying = false;
                    removeNotice();

                    resumeLabel->setVisible(true);
                    restartLabel->setVisible(true);
                    quitLabel->setVisible(true);
                    resumeButton->setVisible(true);
                    restartButton->setVisible(true);
                    quitButton->setVisible(true);
                    musicButton->setVisible(true);
                    musicButton_border->setVisible(true);
                    soundEffectButton->setVisible(true);
                    soundEffectButton_border->setVisible(true);
                }
            }}

            {if (isActionTouch == false && isNoticeVerifying == false) {

                resumeLabel->setColor(Color3B::WHITE);
                resumeLabel->setScale(1.0);
                resumeButton->setScale(1.0);

                restartLabel->setColor(Color3B::WHITE);
                restartLabel->setScale(1.0);
                restartButton->setScale(1.0);

                quitLabel->setColor(Color3B::WHITE);
                quitLabel->setScale(1.0);
                quitButton->setScale(1.0);

                musicButton->setPosition(Point(musicButton_border->getPosition().x, musicButton_border->getPosition().y + blocksMap->getBlockHeight() / 8));
                soundEffectButton->setPosition(Point(soundEffectButton_border->getPosition().x, soundEffectButton_border->getPosition().y + blocksMap->getBlockHeight() / 8));

                if (isAtTouchPos(resumeButton, 'b') && isAtTouchPos(resumeButton, 'e')) {

                    isActionTouch = true;

                    pauseButton->setColor(Color3B::WHITE);

                    // remove all pause board stuff
                    noticeBoard->runAction(RemoveSelf::create());
                    resumeLabel->runAction(RemoveSelf::create());
                    restartLabel->runAction(RemoveSelf::create());
                    quitLabel->runAction(RemoveSelf::create());
                    resumeButton->runAction(RemoveSelf::create());
                    restartButton->runAction(RemoveSelf::create());
                    quitButton->runAction(RemoveSelf::create());
                    musicButton->runAction(RemoveSelf::create());
                    musicButton_border->runAction(RemoveSelf::create());
                    soundEffectButton->runAction(RemoveSelf::create());
                    soundEffectButton_border->runAction(RemoveSelf::create());
                }
                else if (isAtTouchPos(restartButton, 'b') && isAtTouchPos(restartButton, 'e')) {

                    isRestart = true;
                    isNoticeVerifying = true;

                    resumeLabel->setVisible(false);
                    restartLabel->setVisible(false);
                    quitLabel->setVisible(false);
                    resumeButton->setVisible(false);
                    restartButton->setVisible(false);
                    quitButton->setVisible(false);
                    musicButton->setVisible(false);
                    musicButton_border->setVisible(false);
                    soundEffectButton->setVisible(false);
                    soundEffectButton_border->setVisible(false);

                    showNotice("RESTART?");
                    audio_notice();
                }
                else if (isAtTouchPos(quitButton, 'b') && isAtTouchPos(quitButton, 'e')) {

                    isRestart = false;
                    isNoticeVerifying = true;

                    resumeLabel->setVisible(false);
                    restartLabel->setVisible(false);
                    quitLabel->setVisible(false);
                    resumeButton->setVisible(false);
                    restartButton->setVisible(false);
                    quitButton->setVisible(false);
                    musicButton->setVisible(false);
                    musicButton_border->setVisible(false);
                    soundEffectButton->setVisible(false);
                    soundEffectButton_border->setVisible(false);

                    showNotice("QUIT?");
                    audio_notice();
                }
                else if (isAtTouchPos(musicButton, 'b') && isAtTouchPos(musicButton, 'e')) {
                    
                    if (uDef->getBoolForKey("MUSIC", true)) {
                        
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
                    
                    if (uDef->getBoolForKey("SOUND EFFECT", true)) {
                        
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
            }}

            xBegan = -1.0;
            yBegan = -1.0;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::swipeCountdownEffect() {

    // swipe decrease effect
    swipeLabel->runAction(Sequence::create(TintTo::create(0.25, Color3B(255, 225, 0)), TintTo::create(0.25, Color3B::WHITE), nullptr));
    swipeSign->runAction(Sequence::create(TintTo::create(0.25, Color3B::WHITE), TintTo::create(0.25, Color3B(255, 225, 0)), nullptr));
    swipeSign_flippedX->runAction(Sequence::create(TintTo::create(0.25, Color3B::WHITE), TintTo::create(0.25, Color3B(255, 225, 0)), nullptr));

    swipeLabel->runAction(Sequence::create(ScaleTo::create(0.25, 1.1), ScaleTo::create(0.25, 1.0), nullptr));
    swipeSign->runAction(Sequence::create(ScaleTo::create(0.25, 1.1), ScaleTo::create(0.25, 1.0), nullptr));
    swipeSign_flippedX->runAction(Sequence::create(ScaleTo::create(0.25, 1.1), ScaleTo::create(0.25, 1.0), nullptr));

    swipeSign->runAction(Sequence::create(MoveBy::create(0.1, Point(-blocksMap->getBlockWidth() / 2, 0)),
                                            MoveBy::create(0.4, Point( blocksMap->getBlockWidth() / 2, 0)),
                                            nullptr));

    swipeSign_flippedX->runAction(Sequence::create(MoveBy::create(0.1, Point( blocksMap->getBlockWidth() / 2, 0)),
                                                    MoveBy::create(0.4, Point(-blocksMap->getBlockWidth() / 2, 0)),
                                                    nullptr));
}

void GameScene::blocksMap_blocksBlast(float dt) {

    if (fewestMoves > 0) {
        
        if (blocksMap->get_isSwiped() == true) {

            blocksMap->set_isSwiped(false);
            swipeCountdownEffect();

            // swipe indicator height decrease
            yDecreased = (float(fewestMoves) - 1.0) / float(fewestMovesOrigin);
            swipeIndicator->runAction(Spawn::create(ScaleTo::create(0.25, 1.0, yDecreased),
                                                    MoveTo::create(0.25, Point(swipeIndicator_behind->getPosition().x,
                                                                               swipeIndicator_behind->getPosition().y - swipeIndicator_behind->getContentSize().height / 2
                                                                               + swipeIndicator->getContentSize().height * yDecreased / 2)),
                                                    nullptr));

            fewestMoves--;
            swipeLabel->setString(std::to_string(fewestMoves));

            if (fewestMoves == fewestMovesOrigin * 2/3 - 1) {

                starIndicator_1->setVisible(false);
                starIndicator_1_blank->setVisible(true);
            }
            else if (fewestMoves == fewestMovesOrigin * 1/3 - 1) {

                starIndicator_2->setVisible(false);
                starIndicator_2_blank->setVisible(true);
            }
            else if (fewestMoves == 0) {

                starIndicator_3->setVisible(false);
                starIndicator_3_blank->setVisible(true);
            }
        }

        blocksMap->noColorUndermice();
        blocksMap->blocksScan();
        blocksMap->blocksBlast();

        if (blocksMap->get_isBlasting() == true) {

            retryScan = true;
            is_miceSearchPath_firstInvoked = true;
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath), 0.5f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_blast), 0.175f);
        }
        else {

            {if (isGameOver == false) touching();}

            if (retryScan == true) {

                retryScan = false;

                _eventDispatcher->removeEventListener(touchListener);

                this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_blocksBlast), 0.0f);
            }
        }
    }
    else {

        isGameOver = true;
        isUnschedule_miceAnimation = true;

        conditionLabel_generate("NO MORE SHIFTING!");
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::goToGameOverScene), 2.0f);
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_failed), 1.0f);
    }
}

void GameScene::blocksMap_miceSearchPath(float dt) {

    {
        if (blocksMap->miceFalls() && isGameOver == false) {

            isGameOver = true;
            isSuccess = false;
            conditionLabel_generate("FALL INTO THE HOLE!");

            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::goToGameOverScene), 2.0f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_failed), 1.0f);
        }
    }
    {
        if (blocksMap->miceEntersGate()) {

            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath));
            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath_unscheduler));

            blocksMap->blocksBlast();

            is_miceSearchPath_firstInvoked = true;
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath), 0.5f);
        }
    }
    {
        if (blocksMap->miceTrapped() && isGameOver == false) {

            isGameOver = true;
            isSuccess = false;
            isUnschedule_miceSubstituteAnimation = true;
            conditionLabel_generate("TRAPPED!");

            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::goToGameOverScene), 2.0f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceTrapped_effect), 0.1f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_failed), 1.0f);
        }
    }
    {
        if (blocksMap->miceTricked() && isGameOver == false) {
            
            isGameOver = true;
            isSuccess = false;
            isUnschedule_miceSubstituteAnimation = true;
            
            conditionLabel_generate("TRICKED!");
            blocksMap_miceAnimation_substitute_scheduler();

            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::goToGameOverScene), 2.0f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_failed), 1.0f);
        }
    }
    {
        if (blocksMap->miceEatsCheese() && isGameOver == false) {
            
            isGameOver = true;
            isSuccess = true;

            conditionLabel_generate("YUMMY!");
            blocksMap_miceAnimation_substitute_scheduler();

            this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceEatsCheese_movesForward), 0.1f);
            this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::audio_succeeded), 1.0f);
        }
    }

    blocksMap->miceSearchPath();
    if (blocksMap->get_isMiceMove() == 1 && uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/mice_sound.mp3", false, 1.0f);

    if (is_miceSearchPath_firstInvoked == true) {

        is_miceSearchPath_firstInvoked = false;

        this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath), 0.1f);

        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath_unscheduler),
                            float((0.1 * blocksMap->getAvailablePaths() + 0.1) * blocksMap->get_isMiceMove()));

        swipeCountdownEffect();

        // swipe indicator height increase
        if (fewestMoves == fewestMovesOrigin) yIncreased = 1.0;
        else yIncreased = (float(fewestMoves) + 1.0) / float(fewestMovesOrigin);
        yDecreased = yIncreased;
        swipeIndicator->runAction(Spawn::create(ScaleTo::create(0.25, 1.0, yIncreased),
                                                MoveTo::create(0.25, Point(swipeIndicator_behind->getPosition().x,
                                                                            swipeIndicator_behind->getPosition().y - swipeIndicator_behind->getContentSize().height / 2
                                                                            + swipeIndicator->getContentSize().height * yIncreased / 2)),
                                                nullptr));

        if (fewestMoves < fewestMovesOrigin) fewestMoves++;
        swipeLabel->setString(std::to_string(fewestMoves));

        if (fewestMoves == fewestMovesOrigin * 2/3) {

            starIndicator_1->setVisible(true);
            starIndicator_1_blank->setVisible(false);
        }
        else if (fewestMoves == fewestMovesOrigin * 1/3) {

            starIndicator_2->setVisible(true);
            starIndicator_2_blank->setVisible(false);
        }
        else if (fewestMoves == 1) {

            starIndicator_3->setVisible(true);
            starIndicator_3_blank->setVisible(false);
        }
    }
}

void GameScene::blocksMap_re_miceTrapped(float dt) {

    if (blocksMap->miceTrapped() == true && isGameOver == false) {

        is_re_miceTrapped = true;
        isGameOver = true;
        isUnschedule_miceSubstituteAnimation = true;
        conditionLabel_generate("TRAPPED!");

        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::goToGameOverScene), 2.0f);
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceTrapped_effect), 0.1f);

        if (reloadMiceSearchPath == true) blocksMap->miceSearchPath();

        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath_unscheduler),
                            float((0.1 * blocksMap->getAvailablePaths() + 0.1) * blocksMap->get_isMiceMove()));
    }

    reloadMiceSearchPath = false;
}

void GameScene::blocksMap_miceSearchPath_unscheduler(float dt) {

    blocksMap_re_miceTrapped(0.0);
    this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceSearchPath));

    if (blocksMap->get_isTrapped() == false) {
        
        reloadMiceSearchPath = true;

        blocksMap->rotateTraps();
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_regenerateTraps), 0.6f);

        if (is_re_miceTrapped == false) this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_re_miceTrapped), 0.6f);
    }

    blocksMap->blocksFill();

    if (isGameOver == false) {
        
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_blocksBlast),
                            float(0.1 * blocksMap->getAvailablePaths() + 0.15));
    }
}

void GameScene::blocksMap_miceEatsCheese_movesForward(float dt) {

    if (eatsCount < 4) {
        
        blocksMap->miceEatsCheese_movesForward();
        this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceEatsCheese_movesBackward), 0.25f);
    }
    else {
        
        this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation_substitute));
        goToGameOverScene(0.0);
    }
}

void GameScene::blocksMap_miceEatsCheese_movesBackward(float dt) {

    blocksMap->miceEatsCheese_movesBackward();
    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceEatsCheese_movesForward), 0.25f);

    eatsCount++;
}

void GameScene::blocksMap_miceAnimation_scheduler() {

    blocksMap->miceAnimation();
    this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation), 0.8f);
}

void GameScene::blocksMap_miceAnimation_substitute_scheduler() {

    blocksMap->miceAnimation(true);
    this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation_substitute), 0.8f);
}

void GameScene::blocksMap_miceAnimation(float dt)            {blocksMap->miceAnimation();}
void GameScene::blocksMap_miceAnimation_substitute(float dt) {blocksMap->miceAnimation(true);}
void GameScene::blocksMap_regenerateTraps(float dt)          {blocksMap->regenerateTraps();}
void GameScene::blocksMap_holesInvisible_rebuild(float dt)   {blocksMap->holesInvisible_rebuild();}

void GameScene::blocksMap_tutorialLevel1_arrowMove(float dt)      {blocksMap->tutorialLevel1_arrowMove();}
void GameScene::blocksMap_tutorialLevel1_pointingBlocks(float dt) {blocksMap->tutorialLevel1_pointingBlocks();}

void GameScene::blocksMap_miceTrapped_effect(float dt) {
    
    blocksMap->miceTrapped_effect();
    blocksMap_miceAnimation_substitute_scheduler();
}

void GameScene::set_isTouchable(float dt) {isTouchable = true;}

void GameScene::goToGameOverScene(float dt) {

    {if (isUnschedule_miceAnimation == true) this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation));}
    {if (isUnschedule_miceSubstituteAnimation == true) this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&GameScene::blocksMap_miceAnimation_substitute));}

    delete blocksMap;

    int stars;

    if      (fewestMoves >= fewestMovesOrigin * 2/3) stars = 3;
    else if (fewestMoves >= fewestMovesOrigin * 1/3) stars = 2;
    else if (fewestMoves >  fewestMovesOrigin * 0)   stars = 1;
    else                                             stars = 0;

    if (isSuccess == false) stars = 0;

    auto scene = GameOverScene::createScene(level, stars);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameScene::conditionLabel_generate(const std::string &conditionLabel) {

    auto throwLabel = Label::createWithSystemFont(conditionLabel, "Roboto", blocksMap->getBlockWidth() * 4/3);
    if (throwLabel->getContentSize().width >= vSize.width * 0.9) throwLabel->setScale(vSize.width * 0.9 / throwLabel->getContentSize().width);
    throwLabel->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
    throwLabel->enableOutline(Color4B::BLACK, 2);
    throwLabel->setOpacity(0);
    this->addChild(throwLabel, 10000);

    throwLabel->runAction(MoveTo::create(2.0, Point(vSize.width / 2 + vOrigin.x, vSize.height * 3/4 + vOrigin.y)));
    throwLabel->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(0.5), DelayTime::create(0.5), FadeOut::create(0.5), nullptr));
}

void GameScene::conditionLabel_generate_tutlv1(float dt)    {conditionLabel_generate("SHIFT THE BLOCKS!");}
void GameScene::conditionLabel_generate_tutlv16_1(float dt) {conditionLabel_generate("AVOID THESE HOLES!");}
void GameScene::conditionLabel_generate_tutlv16_2(float dt) {conditionLabel_generate("THEY MOVE CLOCKWISE");}

// GUI
void GameScene::showNotice(const std::string &whatNotice) {

    areYouSure[0] = Label::createWithSystemFont(whatNotice, "Roboto", blocksMap->getBlockHeight());
    areYouSure[1] = Label::createWithSystemFont("Are you sure?", "Roboto", blocksMap->getBlockHeight() / 2);
    areYouSure[2] = Label::createWithSystemFont("Your current progress", "Roboto", blocksMap->getBlockHeight() / 2);
    areYouSure[3] = Label::createWithSystemFont("will be lost!", "Roboto", blocksMap->getBlockHeight() / 2);

    areYouSure[0]->setPosition(resumeLabel->getPosition());
    areYouSure[1]->setPosition(Point(areYouSure[0]->getPosition().x, areYouSure[0]->getPosition().y - areYouSure[0]->getContentSize().height));
    areYouSure[2]->setPosition(Point(areYouSure[1]->getPosition().x, areYouSure[1]->getPosition().y - areYouSure[0]->getContentSize().height));
    areYouSure[3]->setPosition(Point(areYouSure[2]->getPosition().x, areYouSure[2]->getPosition().y - areYouSure[0]->getContentSize().height));

    areYouSure[0]->enableBold();
    areYouSure[1]->enableBold();
    areYouSure[2]->enableBold();
    areYouSure[3]->enableBold();

    this->addChild(areYouSure[0], 10000 + 1);
    this->addChild(areYouSure[1], 10000 + 1);
    this->addChild(areYouSure[2], 10000 + 1);
    this->addChild(areYouSure[3], 10000 + 1);

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

void GameScene::removeNotice() {

    areYouSure[0]->runAction(RemoveSelf::create());
    areYouSure[1]->runAction(RemoveSelf::create());
    areYouSure[2]->runAction(RemoveSelf::create());
    areYouSure[3]->runAction(RemoveSelf::create());

    yesButton->runAction(RemoveSelf::create());
    yesButton_pressed->runAction(RemoveSelf::create());
    noButton->runAction(RemoveSelf::create());
    noButton_pressed->runAction(RemoveSelf::create());
}

void GameScene::generatePauseMenu() {

    // pause board stuff appears
    noticeBoard = Sprite::create("board.png");
    noticeBoard->setContentSize(Size(blocksMap->getBlockWidth() * 8.5, blocksMap->getBlockHeight() * 7.5));
    noticeBoard->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
    noticeBoard->setColor(Color3B::BLACK);
    noticeBoard->setOpacity(int(255 * 0.6));
    this->addChild(noticeBoard, 10000);

    resumeLabel = Label::createWithSystemFont("RESUME", "Roboto", blocksMap->getBlockHeight());
    resumeLabel->setPosition(Point(noticeBoard->getPosition().x, noticeBoard->getPosition().y + noticeBoard->getContentSize().height / 2 - resumeLabel->getContentSize().height));
    resumeLabel->enableBold();
    this->addChild(resumeLabel, 10000 + 2);

    resumeButton = Sprite::create("button_board_bar.png");
    resumeButton->setContentSize(Size(noticeBoard->getContentSize().width * 0.75, blocksMap->getBlockHeight()));
    resumeButton->setPosition(resumeLabel->getPosition());
    this->addChild(resumeButton, 10000 + 1);

    restartLabel = Label::createWithSystemFont("RESTART", "Roboto", blocksMap->getBlockHeight());
    restartLabel->setPosition(Point(noticeBoard->getPosition().x, resumeLabel->getPosition().y - restartLabel->getContentSize().height * 1.5));
    restartLabel->enableBold();
    this->addChild(restartLabel, 10000 + 2);

    restartButton = Sprite::create("button_board_bar.png");
    restartButton->setContentSize(Size(noticeBoard->getContentSize().width * 0.75, blocksMap->getBlockHeight()));
    restartButton->setPosition(restartLabel->getPosition());
    this->addChild(restartButton, 10000 + 1);

    quitLabel = Label::createWithSystemFont("QUIT", "Roboto", blocksMap->getBlockHeight());
    quitLabel->setPosition(Point(noticeBoard->getPosition().x, restartLabel->getPosition().y - quitLabel->getContentSize().height * 1.5));
    quitLabel->enableBold();
    this->addChild(quitLabel, 10000 + 2);

    quitButton = Sprite::create("button_board_bar.png");
    quitButton->setContentSize(Size(noticeBoard->getContentSize().width * 0.75, blocksMap->getBlockHeight()));
    quitButton->setPosition(quitLabel->getPosition());
    this->addChild(quitButton, 10000 + 1);

    musicButton = Sprite::create("GUI/music_button.png");
    musicButton->setContentSize(Size(musicButton->getContentSize().width * 3/4, musicButton->getContentSize().height * 3/4));
    musicButton->setPosition(Point(noticeBoard->getPosition().x - noticeBoard->getContentSize().width / 2 + musicButton->getContentSize().width * 2, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + musicButton->getContentSize().height + blocksMap->getBlockHeight() / 8));
    
    musicButton_border = Sprite::create("GUI/music_button.png");
    musicButton_border->setContentSize(Size(musicButton_border->getContentSize().width * 3/4, musicButton_border->getContentSize().height * 3/4));
    musicButton_border->setPosition(Point(noticeBoard->getPosition().x - noticeBoard->getContentSize().width / 2 + musicButton_border->getContentSize().width * 2, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + musicButton_border->getContentSize().height));

    if (uDef->getBoolForKey("MUSIC", true)) {
        
        musicButton->setColor(Color3B::YELLOW);
        musicButton_border->setColor(Color3B(127, 127, 0));
    }
    else {

        musicButton->setColor(Color3B::GRAY);
        musicButton_border->setColor(Color3B(63, 63, 63));
    }

    this->addChild(musicButton, 10000 + 2);
    this->addChild(musicButton_border, 10000 + 1);

    soundEffectButton = Sprite::create("GUI/sound_effect_button.png");
    soundEffectButton->setContentSize(Size(soundEffectButton->getContentSize().width * 3/4, soundEffectButton->getContentSize().height * 3/4));
    soundEffectButton->setPosition(Point(noticeBoard->getPosition().x + noticeBoard->getContentSize().width / 2 - soundEffectButton->getContentSize().width * 2, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + soundEffectButton->getContentSize().height + blocksMap->getBlockHeight() / 8));

    soundEffectButton_border = Sprite::create("GUI/sound_effect_button.png");
    soundEffectButton_border->setContentSize(Size(soundEffectButton_border->getContentSize().width * 3/4, soundEffectButton_border->getContentSize().height * 3/4));
    soundEffectButton_border->setPosition(Point(noticeBoard->getPosition().x + noticeBoard->getContentSize().width / 2 - soundEffectButton_border->getContentSize().width * 2, noticeBoard->getPosition().y - noticeBoard->getContentSize().height / 2 + soundEffectButton_border->getContentSize().height));

    if (uDef->getBoolForKey("SOUND EFFECT", true)) {
        
        soundEffectButton->setColor(Color3B(0, 127, 255));
        soundEffectButton_border->setColor(Color3B(0, 63, 127));
    }
    else {

        soundEffectButton->setColor(Color3B::GRAY);
        soundEffectButton_border->setColor(Color3B(63, 63, 63));
    }

    this->addChild(soundEffectButton, 10000 + 2);
    this->addChild(soundEffectButton_border, 10000 + 1);
}

inline void GameScene::audio_selectOn()  {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_on.mp3", false, 1.0f);}
inline void GameScene::audio_selectOff() {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_off.mp3", false, 1.0f);}
inline void GameScene::audio_notice()    {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/notice.mp3", false, 1.0f);}

void GameScene::audio_failed(float dt)    {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/failed.mp3", false, 1.0f);}
void GameScene::audio_succeeded(float dt) {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/succeeded.mp3", false, 1.0f);}
void GameScene::audio_blast(float dt)     {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/blast.mp3", false, 1.0f);}

// back button ANDROID
void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

        if (isTouchable == true) {

            if (isActionTouch == true && uDef->getIntegerForKey("LEVEL 1 STARS", -1) != 0) {

                audio_selectOn();
                
                isActionTouch = false;

                pauseButton->setColor(Color3B(225, 225, 225));
                generatePauseMenu();
            }
            else if (isActionTouch == false && isNoticeVerifying == true) {

                audio_selectOff();

                isNoticeVerifying = false;

                removeNotice();

                resumeLabel->setVisible(true);
                restartLabel->setVisible(true);
                quitLabel->setVisible(true);
                resumeButton->setVisible(true);
                restartButton->setVisible(true);
                quitButton->setVisible(true);
                musicButton->setVisible(true);
                musicButton_border->setVisible(true);
                soundEffectButton->setVisible(true);
                soundEffectButton_border->setVisible(true);
            }
            else if (isActionTouch == false && isNoticeVerifying == false) {

                audio_selectOff();

                isActionTouch = true;

                pauseButton->setColor(Color3B::WHITE);

                // remove all pause board stuff
                noticeBoard->runAction(RemoveSelf::create());
                resumeLabel->runAction(RemoveSelf::create());
                restartLabel->runAction(RemoveSelf::create());
                quitLabel->runAction(RemoveSelf::create());
                resumeButton->runAction(RemoveSelf::create());
                restartButton->runAction(RemoveSelf::create());
                quitButton->runAction(RemoveSelf::create());
                musicButton->runAction(RemoveSelf::create());
                musicButton_border->runAction(RemoveSelf::create());
                soundEffectButton->runAction(RemoveSelf::create());
                soundEffectButton_border->runAction(RemoveSelf::create());
            }
        }
    }
}

bool GameScene::isAtTouchPos(cocos2d::Node *node, char touchCondition) {

    /*
        'b'  for Began
        'e' for Began with Ended
    */
    if (touchCondition == 'b') {

        if (xBegan >= node->getPosition().x - node->getContentSize().width / 2 && xBegan <= node->getPosition().x + node->getContentSize().width / 2
            && yBegan >= node->getPosition().y - node->getContentSize().height / 2 && yBegan <= node->getPosition().y + node->getContentSize().height / 2) {

            return true;
        }
        else return false;
    }
    else if (touchCondition == 'e') {

        if (xEnded >= node->getPosition().x - node->getContentSize().width / 2 && xEnded <= node->getPosition().x + node->getContentSize().width / 2
            && yEnded >= node->getPosition().y - node->getContentSize().height / 2 && yEnded <= node->getPosition().y + node->getContentSize().height / 2) {

            return true;
        }
        else return false;
    }
    else return false;
}

bool GameScene::isTouchAtBlocksArea() {

    if (xBegan >= blocksMap->getMapCenterPosX() - blocksMap->getMapWidth() / 2 && xBegan <= blocksMap->getMapCenterPosX() + blocksMap->getMapWidth() / 2
        && yBegan >= blocksMap->getMapCenterPosY() - blocksMap->getMapHeight() / 2 && yBegan <= blocksMap->getMapCenterPosY() + blocksMap->getMapHeight() / 2) {

        return true;
    }
    else return false;
}


