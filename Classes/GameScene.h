#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "BlocksMap.h"

class GameScene : public cocos2d::LayerColor {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene(int level_in);

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(GameScene);

    private:

        void touching();
        void blocksMap_blocksBlast(float dt);
        void blocksMap_miceSearchPath(float dt);
        void blocksMap_miceSearchPath_unscheduler(float dt);
        void blocksMap_miceEatsCheese_movesForward(float dt);
        void blocksMap_miceEatsCheese_movesBackward(float dt);

        void blocksMap_miceAnimation_scheduler();
        void blocksMap_miceAnimation_substitute_scheduler();
        void blocksMap_miceAnimation(float dt);
        void blocksMap_miceAnimation_substitute(float dt);

        void blocksMap_regenerateTraps(float dt);
        void blocksMap_miceTrapped_effect(float dt);
        void blocksMap_re_miceTrapped(float dt);
        void blocksMap_holesInvisible_rebuild(float dt);

        void blocksMap_tutorialLevel1_arrowMove(float dt);
        void blocksMap_tutorialLevel1_pointingBlocks(float dt);

        void conditionLabel_generate_tutlv1(float dt);
        void conditionLabel_generate_tutlv16_1(float dt);
        void conditionLabel_generate_tutlv16_2(float dt);

        void goToGameOverScene(float dt);
        void set_isTouchable(float dt);
        void conditionLabel_generate(const std::string &conditionLabel);
        void swipeCountdownEffect();

        void showNotice(const std::string &whatNotice);
        void removeNotice();
        void generatePauseMenu();

        inline void audio_selectOn();
        inline void audio_selectOff();
        inline void audio_notice();

        void audio_failed(float dt);
        void audio_succeeded(float dt);
        void audio_blast(float dt);

        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

        bool isAtTouchPos(cocos2d::Node *node, char touchCondition);
        bool isTouchAtBlocksArea();

        cocos2d::Size vSize;
        cocos2d::Vec2 vOrigin;

        cocos2d::UserDefault* uDef;

        cocos2d::EventListenerTouchOneByOne* touchListener;

        cocos2d::Sprite* pauseButton;
        cocos2d::Sprite* swipeSign;
        cocos2d::Sprite* swipeSign_flippedX;
        cocos2d::Sprite* swipeIndicator;
        cocos2d::Sprite* swipeIndicator_behind;
        cocos2d::Sprite* starIndicator_1;
        cocos2d::Sprite* starIndicator_2;
        cocos2d::Sprite* starIndicator_3;
        cocos2d::Sprite* starIndicator_1_blank;
        cocos2d::Sprite* starIndicator_2_blank;
        cocos2d::Sprite* starIndicator_3_blank;

        cocos2d::Label* swipeLabel;

        // pause board stuff //

        cocos2d::Sprite *noticeBoard;

        cocos2d::Sprite* resumeButton;
        cocos2d::Sprite* restartButton;
        cocos2d::Sprite* quitButton;

        cocos2d::Label* resumeLabel;
        cocos2d::Label* restartLabel;
        cocos2d::Label* quitLabel;

        cocos2d::Sprite *musicButton, *musicButton_border;
        cocos2d::Sprite *soundEffectButton, *soundEffectButton_border;

        // notice stuff //

        cocos2d::Sprite* yesButton;
        cocos2d::Sprite* yesButton_pressed;
        cocos2d::Sprite* noButton;
        cocos2d::Sprite* noButton_pressed;

        cocos2d::Label* areYouSure[4];

        BlocksMap *blocksMap;

        int blockMember = 0, eatsCount = 0, seed, fewestMovesOrigin, fewestMoves;

        bool is_miceSearchPath_firstInvoked, isGameOver = false, retryScan, isNoticeVerifying = false, isActionTouch = true,
             isRestart, isTouched = false, isUnschedule_miceAnimation = false, isUnschedule_miceSubstituteAnimation = false,
             reloadMiceSearchPath = false, is_re_miceTrapped = false, isBeginningGame = true, isTouchable = false, isSuccess;

        float yIncreased, yDecreased;

        float xBegan, yBegan;
        float xEnded, yEnded;
};

#endif // __GAME_SCENE_H__