#ifndef __BLOCKS_MAP_H__
#define __BLOCKS_MAP_H__

#include "cocos2d.h"

class BlocksMap {
    
    public:

        BlocksMap();
        ~BlocksMap();
        
        void mapValues(float widthIn, float heightIn);                                  // THE FIRST METHODS
        void mapGenerate(cocos2d::Layer *layer_in, int seed_in, int level_in,           // TO INVOKE (LEVELS CONFIGURATION)
                        int miceTag, int plateTag_in, int valSign_w, int valSign_h);
        
        void selectBlock(int member);
        void unselectBlock(int member);
        void moveBlock(int member, std::string direction);
        void blocksBlast(bool isMapChecking = false);
        void blocksFill();
        void blocksScan();
        void miceAnimation(bool isThisSubstitute = false);
        void miceSearchPath();
        void noColorUndermice();

        void rotateTraps();
        void regenerateTraps();
        void miceTrapped_effect();

        void miceEatsCheese_movesForward();
        void miceEatsCheese_movesBackward();

        bool holesVisible();
        void holesInvisible();
        void holesInvisible_rebuild();

        void tutorialLevel1_blocksSet();
        void tutorialLevel1_arrowMove();
        void tutorialLevel1_arrowUnmove();
        void tutorialLevel1_pointingBlocks();
        void tutorialLevel1_unpointingBlocks();

        int getAvailablePaths();
        int getFewestMoves();
        int getBlockCount();
        int get_isMiceMove();

        bool miceEatsCheese();
        bool miceFalls();
        bool miceEntersGate();
        bool miceTrapped();
        bool miceTricked();
        bool get_isBlasting();
        bool get_isTrapped();

        float getBlockWidth();
        float getBlockHeight();
        float getPosX(int member);
        float getPosY(int member);
        float getMapCenterPosX();
        float getMapCenterPosY();
        float getMapWidth();
        float getMapHeight();

        void set_isSwiped(int isSwiped_in);
        bool get_isSwiped();

    private:

        void blockTag_default(bool only_blockTag = false);
        void closestToMice();
        void seedRandom_generator();
        void randomColoring(int member);
        void miceSubstitutes_generate();
        void gateOpenClose(int member, int blockMember, bool onlyOpen = false);
        
        void noBlockUnderObstacle();
        void returnBlockUnderObstacle();
        void blockHiding(int member);
        void holesMove();
        void holesMove_way(int member);
        void holeTracesAssignment(int *member);
        void obstaclesConfiguration(int count, int *point_arr);
        void gatesConfiguration(int count, int *point_arr, float *rotation_arr);
        void holesConfiguration(int count);
        void trapConfiguration(int count, int *point_arr, float *rotation_arr);
        void trapFrames_construct(int member, float rotation);
        void trapFrames_destruct(int member);
        void fakeCheeseConfiguration(int count, int *point_arr, float *rotation_arr, int *valSign_w_trap, int *valSign_h_trap);

        bool isAtBlockPos(int member_try, int member_target);
        bool isNotMicePositionTarget(int member);
        bool isAtObstaclePos(int member);
        bool isAtGatePos(int member);
        bool isAtHolePos(int member);
        bool isAtHoleTrace(int member, int holeTraceMember);
        bool isAtTrapPos(int member);

        static const int obsCount = 32, miceFrames_count = 5, holesCount = 32, gatesCount = 4, gateFrames_count = 4,
                         trapsCount = 32, trapFrames_count = 4, smokeFrames_count = 6;

        cocos2d::Size vSize = cocos2d::Director::getInstance()->getVisibleSize();
        cocos2d::Vec2 vOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();

        cocos2d::UserDefault *uDef = cocos2d::UserDefault::getInstance();

        cocos2d::Layer *layer;

        // blaster blocks stuff
        cocos2d::Sprite* grassUnder[512];
        cocos2d::Sprite* grassSurround[128];
        cocos2d::Sprite* obstacle[obsCount][2];
        cocos2d::Sprite* bushes[512];

        cocos2d::Sprite* soilDarker[32];
        cocos2d::Sprite* soilLighter[32];
        cocos2d::Sprite* soilShadowHorizontal[32];
        cocos2d::Sprite* soilShadowVertical[32];

        cocos2d::Sprite* block[512];
        cocos2d::Sprite* block3D[512];

        cocos2d::Sprite* blast[512][6];
        cocos2d::Sprite* blast3D[512][6];

        cocos2d::Sprite* arrow;

        char frameName_buffer[64];

        bool blockTag[512], blockFillTag[512], isBlasting, isSwiped = false;

        float areaPos[512][2], blockPos[512][2], obstaclePos[obsCount][2]; // static (origin) positions

        int areaPointsWidth, areaPointsHeight, areaCount,
            blockCount, pointsWidth, pointsHeight, blockHiding_tag[512][2],
            seed, seedRandom, level, fewestMoves;

        float blockWidth, blockHeight, mapWidth, mapHeight, mapCenterPosX, mapCenterPosY, deltaTime_1, deltaTime_2;

        // level stuff
        cocos2d::Sprite* mice[miceFrames_count];
        cocos2d::Sprite* mice_substitute[miceFrames_count];
        cocos2d::Sprite* plate;
        cocos2d::Sprite* cheese;
        cocos2d::Sprite* hole[holesCount];
        cocos2d::Sprite* gate[gatesCount][gateFrames_count];
        cocos2d::Sprite* trap[trapsCount][trapFrames_count];
        cocos2d::Sprite* fakePlate[trapsCount];
        cocos2d::Sprite* fakeCheese[trapsCount];
        cocos2d::Sprite* fakeTrap[trapFrames_count];
        cocos2d::Sprite* smoke[smokeFrames_count];

        std::string holeMoveDirection[holesCount];

        int availablePaths, isMiceMove, plateTag, fakeTrapRotation[trapsCount], randInt_hole[holesCount],
            currentGates, currentHoles, currentTraps, currentFakeCheeses = 0, trapTag, fakeSkipPos_tag;

        bool obstacleOn = false, holeOn = false, gateOn = false, trapOn = false, fakeOn = false, isFall = false, gateTag[512], isEat = false, isTrapped = false;

        float micePos_buf[2], miceRot_buf, plateEdgePos[2], holePos[holesCount][2], holeTracePos_1[holesCount][2], holeTracePos_2[holesCount][2], holeTracePos_3[holesCount][2], gatePos[gatesCount][2], gateRot[gatesCount],
              trapPos[trapsCount][2], trapRot[trapsCount], fakePlatePos[trapsCount][2], closestToMice_x, closestToMice_y;

        // DEBUG tool
        // cocos2d::Label* aLabel;
};

#endif // __BLOCKS_MAP_H__


