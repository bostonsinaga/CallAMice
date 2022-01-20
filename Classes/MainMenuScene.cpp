#include "MainMenuScene.h"
#include "GameScene.h"
#include "WelcomeScene.h"
#include "AudioEngine.h"

USING_NS_CC;

extern int id_mice_curious_mp3;

Scene* MainMenuScene::createScene() {

    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);

    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init() {

    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B(0, 200, 0, 255))) {
    
        return false;
    }

    vSize = Director::getInstance()->getVisibleSize();
    vOrigin = Director::getInstance()->getVisibleOrigin();

    uDef = UserDefault::getInstance();

    auto backButton_listener = EventListenerKeyboard::create();
    backButton_listener->onKeyReleased = CC_CALLBACK_2(MainMenuScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backButton_listener, this);

    touching();
    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::set_isTouchable), 0.5f);


    ///////////
    // STUFF //
    ///////////

    auto measureBlock = Sprite::create("BLOCK/block.png");
    blockHeight = measureBlock->getContentSize().height;
    blockWidth  = measureBlock->getContentSize().width;
    measureBlock->runAction(RemoveSelf::create());

    // determine a 'lvpt' size for factor
    auto measureLevelPoint = Sprite::create("GUI/level_point.png");
    lvptWidth = measureLevelPoint->getContentSize().width;
    lvptHeight = measureLevelPoint->getContentSize().height;
    measureLevelPoint->runAction(RemoveSelf::create());

    // 'levelLabel_selected' relocate when a level point selected
    levelLabel_selected = Label::createWithSystemFont("", "Roboto", lvptHeight * 0.75);
    levelLabel_selected->enableOutline(Color4B(200, 200, 200, 255), 5);
    levelLabel_selected->setVisible(false);
    this->addChild(levelLabel_selected, 2);

    // determine each level points position //

    for (int i = 0; i < lvTotal; i++) {

        if (i == 0) {
            
            lvpt_pos[i][0] = vSize.width * 0 + vOrigin.x + lvptWidth;
            lvpt_pos[i][1] = vSize.height * 0.25 + vOrigin.y;

            lvpt_direction[i] = "LEFT";
        }
        else {

            if (lvpt_direction[i - 1] == "LEFT" || lvpt_direction[i - 1] == "RIGHT") {

                lvpt_pos[i][0] = vSize.width / 2 + vOrigin.x;
                lvpt_direction[i] = "MIDDLE";
            }
            else if (lvpt_direction[i - 2] == "LEFT") {

                lvpt_pos[i][0] = vSize.width + vOrigin.x - lvptWidth;
                lvpt_direction[i] = "RIGHT";
            }
            else if (lvpt_direction[i - 2] == "RIGHT") {

                lvpt_pos[i][0] = vSize.width * 0 + vOrigin.x + lvptWidth;
                lvpt_direction[i] = "LEFT";
            }

            lvpt_pos[i][1] = lvpt_pos[i - 1][1] + lvptHeight * 2;

            lvpt_connector_pos[i * 3 - 3][0]     = (lvpt_pos[i - 1][0] + lvpt_pos[i][0]) / 2;
            lvpt_connector_pos[i * 3 + 1 - 3][0] = ((lvpt_pos[i - 1][0] + lvpt_pos[i][0]) / 2 + lvpt_pos[i - 1][0]) / 2;
            lvpt_connector_pos[i * 3 + 2 - 3][0] = ((lvpt_pos[i - 1][0] + lvpt_pos[i][0]) / 2 + lvpt_pos[i][0]) / 2;
            
            lvpt_connector_pos[i * 3 - 3][1]     = (lvpt_pos[i - 1][1] + lvpt_pos[i][1]) / 2;
            lvpt_connector_pos[i * 3 + 1 - 3][1] = ((lvpt_pos[i - 1][1] + lvpt_pos[i][1]) / 2 + lvpt_pos[i - 1][1]) / 2;
            lvpt_connector_pos[i * 3 + 2 - 3][1] = ((lvpt_pos[i - 1][1] + lvpt_pos[i][1]) / 2 + lvpt_pos[i][1]) / 2;
        }

        levelPoint_tag[i] = false;
        
        if (i != lvTotal - 1) {

            lvpt_connector_tag[i * 3]     = false;
            lvpt_connector_tag[i * 3 + 1] = false;
            lvpt_connector_tag[i * 3 + 2] = false;
        }
    }

    int distanceDecrease;
    currentLevel = uDef->getIntegerForKey("CURRENT LEVEL", 1);

    if (currentLevel > 2) {
        
        if      (currentLevel == lvTotal)     distanceDecrease = 5;
        else if (currentLevel == lvTotal - 1) distanceDecrease = 4;
        else if (currentLevel == lvTotal - 2) distanceDecrease = 3;
        else                                  distanceDecrease = 2;

        for (int i = 0; i < lvTotal; i++) {

            lvpt_pos[i][1] -= (currentLevel - distanceDecrease) * lvptHeight * 2;

            if (i != lvTotal - 1) {

                lvpt_connector_pos[i * 3][1]     -= (currentLevel - distanceDecrease) * lvptHeight * 2;
                lvpt_connector_pos[i * 3 + 1][1] -= (currentLevel - distanceDecrease) * lvptHeight * 2;
                lvpt_connector_pos[i * 3 + 2][1] -= (currentLevel - distanceDecrease) * lvptHeight * 2;
            }
        }
    }

    // level points
    this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::constructLevelPoints), 0.0f);

    // level selection tag //

    levelPoint_selected = Sprite::create("GUI/level_point_selected.png");
    levelPoint_selected->setPosition(Point(lvpt_pos[currentLevel - 1][0], lvpt_pos[currentLevel - 1][1]));
    this->addChild(levelPoint_selected, 5);

    isSelectionVerified = true;
    willVerify = currentLevel - 1;

    // settings stuff //

    settingsButton = Sprite::create("GUI/settings_button.png");
    settingsButton->setContentSize(Size(settingsButton->getContentSize().width / 2, settingsButton->getContentSize().height / 2));
    settingsButton->setPosition(Point(vSize.width + vOrigin.x - settingsButton->getContentSize().width * 3/4, vSize.height + vOrigin.y - settingsButton->getContentSize().height * 3/4));
    this->addChild(settingsButton, 10 + 4);

    settingsButton_border = Sprite::create("GUI/settings_button.png");
    settingsButton_border->setContentSize(Size(settingsButton_border->getContentSize().width / 2, settingsButton_border->getContentSize().height / 2));
    settingsButton_border->setPosition(Point(settingsButton->getPosition().x, settingsButton->getPosition().y - blockHeight / 8));
    settingsButton_border->setColor(Color3B::GRAY);
    this->addChild(settingsButton_border, 10 + 3);

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

    this->addChild(musicButton, 10 + 2);
    this->addChild(musicButton_border, 10 + 1);

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

    this->addChild(soundEffectButton, 10 + 2);
    this->addChild(soundEffectButton_border, 10 + 1);

    exitButton = Sprite::create("GUI/exit_button.png");
    exitButton->setPosition(settingsButton->getPosition());
    exitButton->setContentSize(Size(exitButton->getContentSize().width / 2, exitButton->getContentSize().height / 2));
    exitButton->setColor(Color3B::RED);
    exitButton->setOpacity(0);
    this->addChild(exitButton, 10 + 2);

    exitButton_border = Sprite::create("GUI/exit_button.png");
    exitButton_border->setPosition(settingsButton_border->getPosition());
    exitButton_border->setContentSize(Size(exitButton_border->getContentSize().width / 2, exitButton_border->getContentSize().height / 2));
    exitButton_border->setColor(Color3B(127, 0, 0));
    exitButton_border->setOpacity(0);
    this->addChild(exitButton_border, 10 + 1);

    settingsBoard = Sprite::create("board_mini.png");
    settingsBoard->setPosition(settingsButton_border->getPosition());
    settingsBoard->setContentSize(Size(settingsButton->getContentSize().width, blockHeight * 9/2));
    settingsBoard->setColor(Color3B::BLACK);
    settingsBoard->setOpacity(127);
    settingsBoard->setScaleY(0.0);
    this->addChild(settingsBoard, 10);

    // bubble selection
    this->schedule(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::selectionBubbleAnimation), 0.5f);

    return true;
}

void MainMenuScene::touching() {

    touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);

    touchListener->onTouchBegan = [=] (Touch *touch, Event *event) {

        xBegan = -1.0;
        yBegan = -1.0;

        if (isTouched == false) {

            isTouched = true;
            
            xBegan = touch->getLocation().x;
            yBegan = touch->getLocation().y;

            yBegan_scroll = touch->getLocation().y;

            if (isTouchable == true && isBoardNotifying == false) {

                if (isSettingsOpened == false) {

                    if (isAtTouchPos(settingsButton, 'b')) {
                        
                        isSettingsPressed = true;
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
                        
                        isSettingsPressed = true;
                        audio_selectOff();
                        settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y));
                    }
                }
                
                if (isSelected == false && isSettingsPressed == false) {

                    for (int i = 0; i < lvTotal; i++) {
                    
                        if (isAtVisible(i, true) == true) {

                            if (isAtTouchPos_levels(i, "B") == true) {

                                AudioEngine::play2d("sounds/select_level.mp3", false, 1.0f);

                                if (willVerify != i) isSelectionVerified = false;

                                isSelected = true;
                                willVerify = i;

                                // save the current level
                                uDef->setIntegerForKey("CURRENT LEVEL", i + 1);
                                uDef->flush();
                                
                                levelLabel_selected->setPosition(levelLabel[i]->getPosition());
                                levelLabel_selected->setString(std::to_string(i + 1));
                                levelLabel_selected->setVisible(true);

                                levelPoint_selected->setPosition(levelPoint[i]->getPosition());

                                break;
                            }
                        }
                    }
                }
            }
            else if (isTouchable == true && isBoardNotifying == true) {

                if (isAtTouchPos(okButton, 'b')) {

                    audio_selectOff();
                    okButton->setScale(0.9);
                    okLabel->setScale(0.9);
                    okLabel->setColor(Color3B(225, 225, 225));
                }
            }
        }

        return true;
    };

    touchListener->onTouchMoved = [=] (Touch *touch, Event *event) {
        
        xMoved = touch->getLocation().x;
        yMoved = touch->getLocation().y;

        yMoved_scroll = touch->getLocation().y;

        if (isTouchable == true && isBoardNotifying == false) {

            if      (yMoved_scroll >= yBegan_scroll + lvptHeight / 4 && lvpt_pos[0][1] <= vSize.height * 0.25 + vOrigin.y)           moveLevelPoints( 1);
            else if (yMoved_scroll <= yBegan_scroll - lvptHeight / 4 && lvpt_pos[lvTotal - 1][1] >= vSize.height * 0.75 + vOrigin.y) moveLevelPoints(-1);
            yBegan_scroll = yMoved_scroll;
        }
    };

    touchListener->onTouchEnded = [=] (Touch *touch, Event *event) {

        isTouched = false;

        xEnded = touch->getLocation().x;
        yEnded = touch->getLocation().y;

        if (isTouchable == true && isBoardNotifying == false) {

            if (isSettingsOpened == false) {

                settingsButton->setPosition(Point(settingsButton_border->getPosition().x, settingsButton_border->getPosition().y + blockHeight / 8));

                if (isAtTouchPos(settingsButton, 'b') && isAtTouchPos(settingsButton, 'e')) {

                    isTouchable = false;

                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::set_isTouchable), 0.25f);
                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::set_isSettingsOpened), 0.25f);
                    
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

                    auto scene = WelcomeScene::createScene();
                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                }
                else if (isAtTouchPos(settingsButton, 'b') && isAtTouchPos(settingsButton, 'e')) {

                    isTouchable = false;
                    isSettingsOpened = false;

                    this->scheduleOnce(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::set_isTouchable), 0.25f);

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

            if (isSelected == true && isSettingsPressed == false) {

                for (int i = 0; i < lvTotal; i++) {
                    
                    if (isAtVisible(i, true) == true) {

                        isSelected = false;
                        levelLabel_selected->setVisible(false);

                        if (isAtTouchPos_levels(i, "BE") == true) {

                            if (isSelectionVerified == true) {

                                sprintf(stringBuffer, "LEVEL %d STARS", i + 1);
                                if (uDef->getIntegerForKey(stringBuffer, -1) == -1) {

                                    audio_notice();

                                    isBoardNotifying = true;
                                    isLocked = true;
                                    levelTarget = i + 1;

                                    // locked board stuff appears
                                    noticeBoard = Sprite::create("board.png");
                                    noticeBoard->setContentSize(Size(blockWidth * 8.5, blockHeight * 7.5));
                                    noticeBoard->setPosition(Point(vSize.width / 2 + vOrigin.x, vSize.height / 2 + vOrigin.y));
                                    noticeBoard->setColor(Color3B::BLACK);
                                    noticeBoard->setOpacity(int(255 * 0.6));
                                    this->addChild(noticeBoard, 10000);

                                    lockedSign_board = Sprite::create("GUI/locked_sign.png");
                                    lockedSign_board->setContentSize(Size(lockedSign_board->getContentSize().width * 1.25, lockedSign_board->getContentSize().height * 1.25));
                                    lockedSign_board->setPosition(Point(noticeBoard->getPosition().x, noticeBoard->getPosition().y + noticeBoard->getContentSize().height / 2 - lockedSign_board->getContentSize().height * 3/4));
                                    this->addChild(lockedSign_board, 10000 + 1);

                                    lockedLabel_board = Label::createWithSystemFont("LEVEL LOCKED!", "Roboto", blockWidth);
                                    lockedLabel_board->setPosition(Point(lockedSign_board->getPosition().x, lockedSign_board->getPosition().y - lockedSign_board->getContentSize().height / 2 - lockedLabel_board->getContentSize().height));
                                    lockedLabel_board->enableBold();
                                    this->addChild(lockedLabel_board, 10000 + 1);

                                    okButton = Sprite::create("GUI/button_board.png");
                                    okButton->setPosition(Point(lockedLabel_board->getPosition().x, lockedLabel_board->getPosition().y - lockedLabel_board->getContentSize().height / 2 - okButton->getContentSize().height));
                                    this->addChild(okButton, 10000 + 1);

                                    okLabel = Label::createWithSystemFont("OK", "Roboto", okButton->getContentSize().height / 2);
                                    okLabel->setPosition(okButton->getPosition());
                                    okLabel->enableBold();
                                    this->addChild(okLabel, 10000 + 2);
                                }
                                else {

                                    isTouchable = false;

                                    this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::constructLevelPoints));
                                    this->unschedule(static_cast<cocos2d::SEL_SCHEDULE>(&MainMenuScene::selectionBubbleAnimation));

                                    auto scene = GameScene::createScene(i + 1);
                                    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
                                }
                            }

                            isSelectionVerified = true;
                            break;
                        }
                    }
                }
            }

            isSettingsPressed = false;
        }
        else if (isTouchable == true && isBoardNotifying == true) {

            okButton->setScale(1.0);
            okLabel->setScale(1.0);
            okLabel->setColor(Color3B::WHITE);

            if (isAtTouchPos(okButton, 'b') && isAtTouchPos(okButton, 'e')) {

                isBoardNotifying = false;

                noticeBoard->runAction(RemoveSelf::create());
                okButton->runAction(RemoveSelf::create());
                okLabel->runAction(RemoveSelf::create());
                
                if(isLocked == true) {

                    isLocked = false;

                    lockedSign_board->runAction(RemoveSelf::create());
                    lockedLabel_board->runAction(RemoveSelf::create());
                }
            }
        }

        xBegan = -1.0;
        yBegan = -1.0;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void MainMenuScene::constructLevelPoints(float dt) {

    for (int i = 0; i < lvTotal; i++) {

        {   // level points
            if (isAtVisible(i, true) == true && levelPoint_tag[i] == false) {

                levelPoint[i] = Sprite::create("GUI/level_point.png");
                levelPoint[i]->setPosition(Point(lvpt_pos[i][0], lvpt_pos[i][1]));
                this->addChild(levelPoint[i]);

                levelLabel[i] = Label::createWithSystemFont(std::to_string(i + 1), "Roboto", lvptHeight * 0.75);
                levelLabel[i]->setPosition(Point(lvpt_pos[i][0], lvpt_pos[i][1]));
                levelLabel[i]->enableOutline(Color4B(100, 100, 100, 255), 5);
                this->addChild(levelLabel[i], 1);

                sprintf(stringBuffer, "LEVEL %d STARS", i + 1);
                if (uDef->getIntegerForKey(stringBuffer, -1) == -1) {

                    lockedSign[i] = Sprite::create("GUI/locked_sign.png");
                    lockedSign[i]->setPosition(Point(levelPoint[i]->getPosition().x, levelPoint[i]->getPosition().y + levelPoint[i]->getContentSize().height / 2 + lockedSign[i]->getContentSize().height * 3/4));
                    this->addChild(lockedSign[i], 2);
                }
                else {

                    sprintf(stringBuffer, "GUI/%d_star.png", uDef->getIntegerForKey(stringBuffer, -1));
                    starsSign[i] = Sprite::create(stringBuffer);
                    starsSign[i]->setPosition(Point(levelPoint[i]->getPosition().x, levelPoint[i]->getPosition().y + levelPoint[i]->getContentSize().height / 2 + starsSign[i]->getContentSize().height * 3/4));
                    this->addChild(starsSign[i], 2);
                }

                levelPoint_tag[i] = true;
            }
        }

        // level points connectors
        if (i != lvTotal - 1) {
        
            {
                if (isAtVisible(i * 3, false) == true && lvpt_connector_tag[i * 3] == false) {
                    
                    lvpt_connector[i * 3] = Sprite::create("BLOCK/block.png");
                    lvpt_connector[i * 3]->setPosition(Point(lvpt_connector_pos[i * 3][0], lvpt_connector_pos[i * 3][1]));
                    lvpt_connector[i * 3]->setScale(0.5);
                    this->addChild(lvpt_connector[i * 3], 3);

                    lvpt_connector_3d[i * 3] = Sprite::create("BLOCK/block_3d_effect.png");
                    lvpt_connector_3d[i * 3]->setPosition(Point(lvpt_connector_pos[i * 3][0], lvpt_connector_pos[i * 3][1]));
                    lvpt_connector_3d[i * 3]->setColor(Color3B::BLACK);
                    lvpt_connector_3d[i * 3]->setScale(0.5);
                    this->addChild(lvpt_connector_3d[i * 3], 4);

                    int random_1 = std::rand() % 6 + 1;
                    if      (random_1 == 1) lvpt_connector[i * 3]->setColor(Color3B::YELLOW);
                    else if (random_1 == 2) lvpt_connector[i * 3]->setColor(Color3B::GREEN);
                    else if (random_1 == 3) lvpt_connector[i * 3]->setColor(Color3B::BLUE);
                    else if (random_1 == 4) lvpt_connector[i * 3]->setColor(Color3B::RED);
                    else if (random_1 == 5) lvpt_connector[i * 3]->setColor(Color3B::MAGENTA);
                    else if (random_1 == 6) lvpt_connector[i * 3]->setColor(Color3B::ORANGE);

                    lvpt_connector_tag[i * 3] = true;
                }
            }
            {
                if (isAtVisible(i * 3 + 1, false) == true && lvpt_connector_tag[i * 3 + 1] == false) {

                    lvpt_connector[i * 3 + 1] = Sprite::create("BLOCK/block.png");
                    lvpt_connector[i * 3 + 1]->setPosition(Point(lvpt_connector_pos[i * 3 + 1][0], lvpt_connector_pos[i * 3 + 1][1]));
                    lvpt_connector[i * 3 + 1]->setScale(0.5);
                    this->addChild(lvpt_connector[i * 3 + 1], 3);

                    lvpt_connector_3d[i * 3 + 1] = Sprite::create("BLOCK/block_3d_effect.png");
                    lvpt_connector_3d[i * 3 + 1]->setPosition(Point(lvpt_connector_pos[i * 3 + 1][0], lvpt_connector_pos[i * 3 + 1][1]));
                    lvpt_connector_3d[i * 3 + 1]->setColor(Color3B::BLACK);
                    lvpt_connector_3d[i * 3 + 1]->setScale(0.5);
                    this->addChild(lvpt_connector_3d[i * 3 + 1], 4);

                    int random_2 = std::rand() % 6 + 1;
                    if      (random_2 == 1) lvpt_connector[i * 3 + 1]->setColor(Color3B::YELLOW);
                    else if (random_2 == 2) lvpt_connector[i * 3 + 1]->setColor(Color3B::GREEN);
                    else if (random_2 == 3) lvpt_connector[i * 3 + 1]->setColor(Color3B::BLUE);
                    else if (random_2 == 4) lvpt_connector[i * 3 + 1]->setColor(Color3B::RED);
                    else if (random_2 == 5) lvpt_connector[i * 3 + 1]->setColor(Color3B::MAGENTA);
                    else if (random_2 == 6) lvpt_connector[i * 3 + 1]->setColor(Color3B::ORANGE);

                    lvpt_connector_tag[i * 3 + 1] = true;
                }
            }
            {
                if (isAtVisible(i * 3 + 2, false) == true && lvpt_connector_tag[i * 3 + 2] == false) {

                    lvpt_connector[i * 3 + 2] = Sprite::create("BLOCK/block.png");
                    lvpt_connector[i * 3 + 2]->setPosition(Point(lvpt_connector_pos[i * 3 + 2][0], lvpt_connector_pos[i * 3 + 2][1]));
                    lvpt_connector[i * 3 + 2]->setScale(0.5);
                    this->addChild(lvpt_connector[i * 3 + 2], 3);

                    lvpt_connector_3d[i * 3 + 2] = Sprite::create("BLOCK/block_3d_effect.png");
                    lvpt_connector_3d[i * 3 + 2]->setPosition(Point(lvpt_connector_pos[i * 3 + 2][0], lvpt_connector_pos[i * 3 + 2][1]));
                    lvpt_connector_3d[i * 3 + 2]->setColor(Color3B::BLACK);
                    lvpt_connector_3d[i * 3 + 2]->setScale(0.5);
                    this->addChild(lvpt_connector_3d[i * 3 + 2], 4);

                    int random_3 = std::rand() % 6 + 1;
                    if      (random_3 == 1) lvpt_connector[i * 3 + 2]->setColor(Color3B::YELLOW);
                    else if (random_3 == 2) lvpt_connector[i * 3 + 2]->setColor(Color3B::GREEN);
                    else if (random_3 == 3) lvpt_connector[i * 3 + 2]->setColor(Color3B::BLUE);
                    else if (random_3 == 4) lvpt_connector[i * 3 + 2]->setColor(Color3B::RED);
                    else if (random_3 == 5) lvpt_connector[i * 3 + 2]->setColor(Color3B::MAGENTA);
                    else if (random_3 == 6) lvpt_connector[i * 3 + 2]->setColor(Color3B::ORANGE);

                    lvpt_connector_tag[i * 3 + 2] = true;
                }
            }
        }
    }

    destructLevelPoints();
}

void MainMenuScene::destructLevelPoints() {

    for (int i = 0; i < lvTotal; i++) {

        {   // level points
            if (!(isAtVisible(i, true) == true) && levelPoint_tag[i] == true) {

                levelPoint[i]->runAction(RemoveSelf::create());
                levelLabel[i]->runAction(RemoveSelf::create());

                sprintf(stringBuffer, "LEVEL %d STARS", i + 1);
                if (uDef->getIntegerForKey(stringBuffer, -1) == -1) lockedSign[i]->runAction(RemoveSelf::create());
                else starsSign[i]->runAction(RemoveSelf::create());

                levelPoint_tag[i] = false;
            }
        }

        // level points connectors
        if (i != lvTotal - 1) {

            {
                if (isAtVisible(i * 3, false) == false && lvpt_connector_tag[i * 3] == true) {
                    
                    lvpt_connector[i * 3]->runAction(RemoveSelf::create());
                    lvpt_connector_3d[i * 3]->runAction(RemoveSelf::create());

                    lvpt_connector_tag[i * 3] = false;
                }
            }
            {
                if (isAtVisible(i * 3 + 1, false) == false && lvpt_connector_tag[i * 3 + 1] == true) {
                    
                    lvpt_connector[i * 3 + 1]->runAction(RemoveSelf::create());
                    lvpt_connector_3d[i * 3 + 1]->runAction(RemoveSelf::create());

                    lvpt_connector_tag[i * 3 + 1] = false;
                }
            }
            {
                if (isAtVisible(i * 3 + 2, false) == false && lvpt_connector_tag[i * 3 + 2] == true) {
                
                    lvpt_connector[i * 3 + 2]->runAction(RemoveSelf::create());
                    lvpt_connector_3d[i * 3 + 2]->runAction(RemoveSelf::create());

                    lvpt_connector_tag[i * 3 + 2] = false;
                }
            }
        }
    }
}

void MainMenuScene::moveLevelPoints(int valSign) {

    /*
        'valSign' = 1  for UP
        'valSign' = -1 for DOWN
    */
    for (int i = 0; i < lvTotal; i++) {

        {   // level points
            if (isAtVisible(i, true) == true && levelPoint_tag[i] == true) {

                levelPoint[i]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                levelLabel[i]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));

                sprintf(stringBuffer, "LEVEL %d STARS", i + 1);
                if (uDef->getIntegerForKey(stringBuffer, -1) == -1) lockedSign[i]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                else                                                starsSign[i]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
            }

            lvpt_pos[i][1] += valSign * lvptHeight / 2;
        }

        // level points connectors
        if (i != lvTotal - 1) {

            {
                if (isAtVisible(i * 3, false) == true && lvpt_connector_tag[i * 3] == true) {
                    
                    lvpt_connector[i * 3]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                    lvpt_connector_3d[i * 3]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                }
            }
            {
                if (isAtVisible(i * 3 + 1, false) == true && lvpt_connector_tag[i * 3 + 1] == true) {
                    
                    lvpt_connector[i * 3 + 1]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                    lvpt_connector_3d[i * 3 + 1]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                }
            }
            {
                if (isAtVisible(i * 3 + 2, false) == true && lvpt_connector_tag[i * 3 + 2] == true) {
                
                    lvpt_connector[i * 3 + 2]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                    lvpt_connector_3d[i * 3 + 2]->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
                }
            }

            lvpt_connector_pos[i * 3][1]     += valSign * lvptHeight / 2;
            lvpt_connector_pos[i * 3 + 1][1] += valSign * lvptHeight / 2;
            lvpt_connector_pos[i * 3 + 2][1] += valSign * lvptHeight / 2;
        }
    }

    levelPoint_selected->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));

    if (isSelected == true) {

        levelLabel_selected->runAction(MoveBy::create(0, Point(0, valSign * lvptHeight / 2)));
    }
}

void MainMenuScene::selectionBubbleAnimation(float dt) {levelPoint_selected->runAction(Sequence::create(ScaleTo::create(0.25, 1.1), ScaleTo::create(0.25, 1), nullptr));}
void MainMenuScene::set_isTouchable(float dt)          {isTouchable = true;}
void MainMenuScene::set_isSettingsOpened(float dt)     {isSettingsOpened = true;}

inline void MainMenuScene::audio_selectOn()  {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_on.mp3", false, 1.0f);}
inline void MainMenuScene::audio_selectOff() {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/select_off.mp3", false, 1.0f);}
inline void MainMenuScene::audio_notice()    {if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/notice.mp3", false, 1.0f);}

// back button ANDROID
void MainMenuScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {

    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {

        if (isTouchable == true && isBoardNotifying == false) {

            audio_selectOn();
            auto scene = WelcomeScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
        }
        else if (isTouchable == true && isBoardNotifying == true) {

            audio_selectOff();

            isBoardNotifying = false;

            noticeBoard->runAction(RemoveSelf::create());
            okButton->runAction(RemoveSelf::create());
            okLabel->runAction(RemoveSelf::create());

            isLocked = false;

            lockedSign_board->runAction(RemoveSelf::create());
            lockedLabel_board->runAction(RemoveSelf::create());
        }
    }
}

float MainMenuScene::nodePoint(cocos2d::Node *node, int valSign, char axis, float range) {

    if      (axis == 'x') return node->getPosition().x + valSign * node->getContentSize().width  * range;
    else if (axis == 'y') return node->getPosition().y + valSign * node->getContentSize().height * range;

    return 0.0;
}

bool MainMenuScene::isAtTouchPos_levels(int member, std::string touchCondition) {

    /*
        "B"  for Began
        "M"  for Moved
        "E"  for Ended
        "BE" for Began with Ended
    */
    if (touchCondition == "B") {

        if (xBegan >= lvpt_pos[member][0] - lvptWidth / 2 && xBegan <= lvpt_pos[member][0] + lvptWidth / 2
            && yBegan >= lvpt_pos[member][1] - lvptHeight / 2 && yBegan <= lvpt_pos[member][1] + lvptHeight / 2) {

            return true;
        }
        else return false;
    }
    else if (touchCondition == "M") {

        if (xMoved >= lvpt_pos[member][0] - lvptWidth / 2 && xMoved <= lvpt_pos[member][0] + lvptWidth / 2
            && yMoved >= lvpt_pos[member][1] - lvptHeight / 2 && yMoved <= lvpt_pos[member][1] + lvptHeight / 2) {

            return true;
        }
        else return false;
    }
    else if (touchCondition == "E") {

        if (xEnded >= lvpt_pos[member][0] - lvptWidth / 2 && xEnded <= lvpt_pos[member][0] + lvptWidth / 2
            && yEnded >= lvpt_pos[member][1] - lvptHeight / 2 && yEnded <= lvpt_pos[member][1] + lvptHeight / 2) {

            return true;
        }
        else return false;
    }
    else if (touchCondition == "BE") {

        if (xBegan >= lvpt_pos[member][0] - lvptWidth / 2 && xBegan <= lvpt_pos[member][0] + lvptWidth / 2
            && yBegan >= lvpt_pos[member][1] - lvptHeight / 2 && yBegan <= lvpt_pos[member][1] + lvptHeight / 2
            && xEnded >= lvpt_pos[member][0] - lvptWidth / 2 && xEnded <= lvpt_pos[member][0] + lvptWidth / 2
            && yEnded >= lvpt_pos[member][1] - lvptHeight / 2 && yEnded <= lvpt_pos[member][1] + lvptHeight / 2) {

            return true;
        }
        else return false;
    }
    else return false;
}

bool MainMenuScene::isAtVisible(int member, bool isThis_lvpt) {
    
    if (isThis_lvpt == true) atVisible_posY = &lvpt_pos[member][1];
    else                     atVisible_posY = &lvpt_connector_pos[member][1];
        
    if (*atVisible_posY >= -lvptHeight && *atVisible_posY <= vSize.height + vOrigin.y + lvptHeight) {

        return true;
    }
    else return false;
}

bool MainMenuScene::isAtTouchPos(cocos2d::Node *node, char touchCondition) {

    bool isDetected = false;
    
    if (touchCondition == 'b') {
        
        if (xBegan >= nodePoint(node, -1, 'x') && xBegan <= nodePoint(node, 1, 'x')
            && yBegan >= nodePoint(node, -1, 'y') && yBegan <= nodePoint(node, 1, 'y')) {

            isDetected = true;
        }
    }
    else if (touchCondition == 'm') {
        
        if (xMoved >= nodePoint(node, -1, 'x') && xMoved <= nodePoint(node, 1, 'x')
            && yMoved >= nodePoint(node, -1, 'y') && yMoved <= nodePoint(node, 1, 'y')) {

            isDetected = true;
        }
    }
    else if (touchCondition == 'e') {
        
        if (xEnded >= nodePoint(node, -1, 'x') && xEnded <= nodePoint(node, 1, 'x')
            && yEnded >= nodePoint(node, -1, 'y') && yEnded <= nodePoint(node, 1, 'y')) {

            isDetected = true;
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}