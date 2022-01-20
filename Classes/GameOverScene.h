#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

#include "cocos2d.h"

class GameOverScene : public cocos2d::LayerColor {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene(int level_in, int stars_in);

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(GameOverScene);

    private:

        void touching();
        void newStarsCelebration(float dt);
        void set_isTouchable(float dt);

        void generateLoadingBar(float dt);
        inline void generateLoadingBar_sub();
        void removeLoadingBar(float dt);

        inline void audio_selectOn();
        inline void audio_selectOff();
        inline void audio_notice();

        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

        bool isAtTouchPos(cocos2d::Sprite *sprite, char touchCondition);

        cocos2d::Size vSize;
        cocos2d::Vec2 vOrigin;

        cocos2d::UserDefault* uDef;
        cocos2d::EventListenerTouchOneByOne* touchListener;

        cocos2d::Sprite *starBlank[3];
        cocos2d::Sprite *star[3];

        cocos2d::Sprite *retryButton, *retryButton_border;
        cocos2d::Sprite *mainMenuButton, *mainMenuButton_border;
        cocos2d::Sprite *nextLevelButton, *nextLevelButton_border;

        cocos2d::Sprite *loadingBar;

        int level, stars, blockWidth, blockHeight;

        char stringBuffer[64];
        char stringBuffer_nextLevel[64];

        bool isTouched = false, isTouchable = false;

        float xBegan, yBegan, xEnded, yEnded;
};

#endif // __GAME_OVER_SCENE_H__