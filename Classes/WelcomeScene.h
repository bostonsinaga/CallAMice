#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene(bool isFromSplashScene_in = false);

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(WelcomeScene);

    private:

        void touching();
        void showNotice(std::string whatNotice);
        void removeNotice();
        
        void set_isTouchable(float dt);
        void set_isSettingsOpened(float dt);
        
        inline void generateNotice();
        inline void audio_selectOn();
        inline void audio_selectOff();

        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

        bool isAtTouchPos(cocos2d::Sprite *sprite, char touchCondition);

        cocos2d::Size vSize;
        cocos2d::Vec2 vOrigin;

        cocos2d::UserDefault* uDef;

        cocos2d::EventListenerTouchOneByOne* touchListener;

        cocos2d::Sprite *playButton, *playButton_border;
        cocos2d::Sprite *settingsButton, *settingsButton_border;
        cocos2d::Sprite *musicButton, *musicButton_border;
        cocos2d::Sprite *soundEffectButton, *soundEffectButton_border;
        cocos2d::Sprite *exitButton, *exitButton_border;

        cocos2d::Sprite *background;
        cocos2d::Sprite *settingsBoard;

        cocos2d::Label *playLabel;

        // notice stuff
        cocos2d::Sprite *noticeBoard;
        cocos2d::Sprite *yesButton, *yesButton_pressed;
        cocos2d::Sprite *noButton, *noButton_pressed;

        cocos2d::Label *areYouSure_1, *areYouSure_2, *areYouSure_3, *areYouSure_4;

        bool isTouchable = true, isSettingsOpened = false, isNoticeTouch = false, isTouched = false;

        float blockWidth, blockHeight;

        float xBegan, yBegan, xEnded, yEnded;
};

#endif // __WELCOME_SCENE_H__


