#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"

class MainMenuScene : public cocos2d::LayerColor {
    
    public:

        // there's no 'id' in cpp, so we recommend returning the class instance pointer
        static cocos2d::Scene* createScene();

        // here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
        virtual bool init();
        
        // implement the "static create()" method manually
        CREATE_FUNC(MainMenuScene);

    private:

        void touching();
        void constructLevelPoints(float dt);
        void destructLevelPoints();
        void moveLevelPoints(int valSign);
        void selectionBubbleAnimation(float dt);

        void set_isTouchable(float dt);
        void set_isSettingsOpened(float dt);

        inline void audio_selectOn();
        inline void audio_selectOff();
        inline void audio_notice();

        void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);

        float nodePoint(cocos2d::Node *node, int valSign, char axis, float range = 0.5);

        bool isAtTouchPos_levels(int member, std::string touchCondition);
        bool isAtVisible(int member, bool isThis_lvpt);

        bool isAtTouchPos(cocos2d::Node *node, char touchCondition);

        static const int lvTotal = 45;
        
        cocos2d::Size vSize;
        cocos2d::Vec2 vOrigin;

        cocos2d::UserDefault* uDef;

        cocos2d::EventListenerTouchOneByOne* touchListener;

        cocos2d::Sprite* levelPoint[lvTotal];
        cocos2d::Sprite* lvpt_connector[lvTotal * 3 - 3];
        cocos2d::Sprite* lvpt_connector_3d[lvTotal * 3 - 3];
        cocos2d::Sprite* levelPoint_selected;

        cocos2d::Label* levelLabel[lvTotal];
        cocos2d::Label* levelLabel_selected;

        cocos2d::Sprite *settingsButton, *settingsButton_border;
        cocos2d::Sprite *musicButton, *musicButton_border;
        cocos2d::Sprite *soundEffectButton, *soundEffectButton_border;
        cocos2d::Sprite *exitButton, *exitButton_border;
        cocos2d::Sprite *settingsBoard;

        cocos2d::Sprite *lockedSign[lvTotal], *starsSign[lvTotal];

        // spawning board stuff
        cocos2d::Sprite *noticeBoard;
        cocos2d::Sprite *okButton;
        cocos2d::Label  *okLabel;

        // locked board stuff
        cocos2d::Sprite *lockedSign_board;
        cocos2d::Label  *lockedLabel_board;

        std::string lvpt_direction[lvTotal];

        char stringBuffer[64];

        int currentLevel, willVerify, levelTarget;

        bool levelPoint_tag[lvTotal], lvpt_connector_tag[lvTotal * 3 - 3], isSelected = false, isSelectionVerified, isTouched = false,
             isTouchable = false, isSettingsOpened = false, isBoardNotifying = false, isLocked = false, isSettingsPressed = false;

        float lvpt_pos[lvTotal][2], lvpt_connector_pos[lvTotal * 3 - 3][2], lvptWidth, lvptHeight, blockWidth, blockHeight;

        float xBegan, yBegan;
        float xMoved, yMoved;
        float xEnded, yEnded;

        float *atVisible_posY, yBegan_scroll, yMoved_scroll;
};

#endif // __MAIN_MENU_SCENE_H__