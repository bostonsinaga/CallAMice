#include "BlocksMap.h"
#include "AudioEngine.h"

USING_NS_CC;

BlocksMap::BlocksMap() {

    Sprite* measureBlock = Sprite::create("BLOCK/block.png");

    blockWidth = measureBlock->getContentSize().width;
    blockHeight = measureBlock->getContentSize().height;

    areaPointsWidth = int(vSize.width / blockWidth) * 4/3;
    areaPointsHeight = int(vSize.height / blockHeight) * 4/3;

    areaCount = areaPointsWidth * areaPointsHeight;

    measureBlock->runAction(RemoveSelf::create());
}

BlocksMap::~BlocksMap() {}

void BlocksMap::blockTag_default(bool only_blockTag) {

    if (only_blockTag == true) for (int i = 0; i < 512; i++) blockTag[i] = false;
    else {
        
        for (int i = 0; i < 512; i++) {

            blockTag[i] = false;
            blockFillTag[i] = false;
        }

        isBlasting = false;
        availablePaths = 0;
    }
}

void BlocksMap::seedRandom_generator() {

    {
        if (seed % 2 == 0) {

            seedRandom = (std::rand() - seed) % 6;
        }
        else {

            seedRandom = (std::rand() + seed) % 6;
        }
    }
}

void BlocksMap::randomColoring(int member) {

    // seed
    seedRandom_generator();

    if (seedRandom == 0) {

        block[member]->setColor(Color3B::YELLOW);
    }
    else if (seedRandom == 1) {

        block[member]->setColor(Color3B::GREEN);
    }
    else if (seedRandom == 2) {

        block[member]->setColor(Color3B::BLUE);
    }
    else if (seedRandom == 3) {

        block[member]->setColor(Color3B::RED);
    }
    else if (seedRandom == 4) {

        block[member]->setColor(Color3B::MAGENTA);
    }
    else if (seedRandom == 5) {

        block[member]->setColor(Color3B::ORANGE);
    }
    else {

        if (seedRandom % 7 == 0) {
            
            block[member]->setColor(Color3B::YELLOW);
        }
        else if (seedRandom % 6 == 0) {

            block[member]->setColor(Color3B::GREEN);
        }
        else if (seedRandom % 5 == 0) {

            block[member]->setColor(Color3B::BLUE);
        }
        else if (seedRandom % 4 == 0) {

            block[member]->setColor(Color3B::RED);
        }
        else if (seedRandom % 3 == 0) {

            block[member]->setColor(Color3B::MAGENTA);
        }
        else if (seedRandom % 2 == 0) {

            block[member]->setColor(Color3B::ORANGE);
        }
        else {

            block[member]->setColor(Color3B::YELLOW);
        }
    }
}

void BlocksMap::mapValues(float widthIn, float heightIn) {

    mapWidth = widthIn;
    mapHeight = heightIn;
    mapCenterPosX = vSize.width / 2 + vOrigin.x;
    mapCenterPosY = vSize.height / 2 + vOrigin.y;

    pointsWidth = (int) mapWidth / blockWidth;
    pointsHeight = (int) mapHeight / blockHeight;

    blockCount = pointsWidth * pointsHeight;
}

void BlocksMap::mapGenerate(cocos2d::Layer *layer_in, int seed_in, int level_in,
                            int miceTag, int plateTag_in, int valSign_w, int valSign_h) {

    layer = layer_in;
    seed = seed_in;
    level = level_in;
    plateTag = plateTag_in;

    // DEBUG tool
    /*
    aLabel = Label::createWithSystemFont("NOTHING SELECTED", "Roboto", vSize.height * 0.05);
    aLabel->setPosition(Point(vSize.width / 2, vSize.height * 0.8));
    layer->addChild(aLabel + 1);
    // */

    // blaster blocks
    for (int i = 0; i < blockCount; i++) {

        if (i == 0) {

            blockPos[i][0] = mapCenterPosX + mapWidth / 2 - blockWidth / 2;
            blockPos[i][1] = mapCenterPosY + mapHeight / 2 - blockHeight / 2;
        }
        else {

            int j;
            for (j = 0; j < blockCount; j += pointsHeight) {

                if (i - 1 == pointsHeight - 1 + j) {

                    blockPos[i][0] = blockPos[i - pointsHeight][0] - blockWidth;
                    blockPos[i][1] = blockPos[i - pointsHeight][1];
                    break;
                }
            }

            if (j >= blockCount - pointsHeight) {

                blockPos[i][0] = blockPos[i-1][0];
                blockPos[i][1] = blockPos[i-1][1] - blockHeight;
            }
        }

        block[i] = Sprite::create("BLOCK/block.png");
        block[i]->setPosition(Point(blockPos[i][0], blockPos[i][1]));
        layer->addChild(block[i], i + 1 + 1);

        block3D[i] = Sprite::create("BLOCK/block_3d_effect.png");
        block3D[i]->setPosition(block[i]->getPosition());
        block3D[i]->setColor(Color3B::BLACK);
        layer->addChild(block3D[i], i + 1 + 1);

        if (!(i == miceTag)) {

            randomColoring(i);
        }
        else {

            block[i]->setOpacity(0);
            block3D[i]->setOpacity(0);
        }

        grassUnder[i] = Sprite::create("ENVIRONMENT/under_field.png");
        grassUnder[i]->setPosition(Point(blockPos[i][0], blockPos[i][1]));
        layer->addChild(grassUnder[i]);
    }

    // environment 3D effect
    for (int i = 0; i < pointsWidth; i++) {

        if (i == 0) {

            soilDarker[i] = Sprite::create("ENVIRONMENT/soil_darker_corner.png");
            soilShadowHorizontal[i] = Sprite::create("ENVIRONMENT/soil_shadow_corner.png");
        }
        else {

            soilDarker[i] = Sprite::create("ENVIRONMENT/soil_darker.png");
            soilShadowHorizontal[i] = Sprite::create("ENVIRONMENT/soil_shadow.png");
        }

        soilDarker[i]->setPosition(Point(blockPos[pointsHeight * i][0], blockPos[0][1] + blockHeight / 2 - soilDarker[i]->getContentSize().height / 2));
        layer->addChild(soilDarker[i], blockCount + 1);

        soilShadowHorizontal[i]->setPosition(Point(blockPos[(blockCount - 1) - (pointsHeight * i)][0], blockPos[blockCount - 1][1] - blockHeight / 2 + soilShadowHorizontal[i]->getContentSize().height / 2));
        layer->addChild(soilShadowHorizontal[i], blockCount + 1);
    }

    for (int i = 0; i < pointsHeight; i++) {

        if (i == 0) {

            soilLighter[i] = Sprite::create("ENVIRONMENT/soil_lighter_corner.png");
            soilShadowVertical[i] = Sprite::create("ENVIRONMENT/soil_shadow_corner.png");
            soilShadowVertical[i]->setRotation(90);
            soilShadowVertical[i]->setFlippedX(true);
        }
        else {

            soilLighter[i] = Sprite::create("ENVIRONMENT/soil_lighter.png");
            soilShadowVertical[i] = Sprite::create("ENVIRONMENT/soil_shadow.png");
            soilShadowVertical[i]->setRotation(90);
        }

        soilLighter[i]->setPosition(Point(blockPos[0][0] + blockWidth / 2 - soilLighter[i]->getContentSize().width / 2, blockPos[i][1]));
        layer->addChild(soilLighter[i], blockCount + 1);

        soilShadowVertical[i]->setPosition(Point(blockPos[blockCount - 1][0] - blockWidth / 2 + soilShadowVertical[i]->getContentSize().height / 2, blockPos[blockCount - 1 - i][1]));
        layer->addChild(soilShadowVertical[i], blockCount + 1);
    }

    // level stuff
    for (int i = 1; i <= miceFrames_count; i++) {
        
        if      (i == 4) std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", 2);
        else if (i == 5) std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", 1);
        else             std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", i);

        mice[i - 1] = Sprite::create(frameName_buffer);
        mice[i - 1]->setPosition(Point(blockPos[miceTag][0], blockPos[miceTag][1]));
        mice[i - 1]->setAnchorPoint(Vec2(1.0 - blockWidth / 2 / mice[i - 1]->getContentSize().width, 0.5));
        layer->addChild(mice[i - 1], blockCount * 2 - 1 + 1);

        {if (i != 1) mice[i - 1]->setOpacity(0);}
        {if (i >= 4) mice[i - 1]->setFlippedY(true);}
    }

    plate = Sprite::create("LEVEL/plate.png");  // 'valSign_' for accurating plate position exactly at the edge of blocks quad
    plate->setPosition(Point(blockPos[plateTag][0] + valSign_w * blockWidth, blockPos[plateTag][1] + valSign_h * blockHeight));
    layer->addChild(plate, blockCount * 2 - 3 + 1);

    cheese = Sprite::create("LEVEL/cheese.png");
    cheese->setPosition(plate->getPosition());
    layer->addChild(cheese, blockCount * 2 - 2 + 1);

    for (int i = 0; i < miceFrames_count; i++) {

        if (cheese->getPosition().x >= mapCenterPosX + mapWidth / 2) {
            
            if (mice[0]->getPosition().x >= mapCenterPosX) {
                
                if (cheese->getPosition().y >= mapCenterPosY) mice[i]->setRotation(270.0);
                else mice[i]->setRotation(90.0);
            }
            else mice[i]->setRotation(0.0);
        }
        else if (cheese->getPosition().x <= mapCenterPosX - mapWidth / 2) {
            
            if (mice[0]->getPosition().x <= mapCenterPosX) {
                
                if (cheese->getPosition().y >= mapCenterPosY) mice[i]->setRotation(270.0);
                else mice[i]->setRotation(90.0);
            }
            else mice[i]->setRotation(180.0);
        }
        else if (cheese->getPosition().y >= mapCenterPosY + mapHeight / 2) {
            
            if (mice[0]->getPosition().y >= mapCenterPosY) {
                
                if (cheese->getPosition().x >= mapCenterPosX) mice[i]->setRotation(0.0);
                else mice[i]->setRotation(180.0);
            }
            else mice[i]->setRotation(270.0);
        }
        else if (cheese->getPosition().y <= mapCenterPosY - mapHeight / 2) {
            
            if (mice[0]->getPosition().y <= mapCenterPosY) {
                
                if (cheese->getPosition().x >= mapCenterPosX) mice[i]->setRotation(0.0);
                else mice[i]->setRotation(180.0);
            }
            else mice[i]->setRotation(90.0);
        }

        // special rotation
        switch (level) {

            case 43 : mice[i]->setRotation(0.0);   break;
            case 44 : mice[i]->setRotation(180.0); break;
            case 45 : mice[i]->setRotation(270.0); break;
        }
    }

    // environment
    for (int i = 0; i < areaCount; i++) {

        if (i == 0) {

            areaPos[i][0] = vSize.width + vOrigin.x - blockWidth / 2;
            areaPos[i][1] = vSize.height + vOrigin.y - blockHeight / 2;
        }
        else {

            int j;
            for (j = 0; j < areaCount; j += areaPointsHeight) {

                if (i - 1 == areaPointsHeight - 1 + j) {

                    areaPos[i][0] = areaPos[i - areaPointsHeight][0] - blockWidth;
                    areaPos[i][1] = areaPos[i - areaPointsHeight][1];
                    break;
                }
            }

            if (j >= areaCount - areaPointsHeight) {

                areaPos[i][0] = areaPos[i-1][0];
                areaPos[i][1] = areaPos[i-1][1] - blockHeight;
            }
        }

        int k = std::rand() % (areaPointsWidth + areaPointsHeight) + 1;

        if (!(areaPos[i][0] >= mapCenterPosX - mapWidth / 2 - blockWidth && areaPos[i][0] <= mapCenterPosX + mapWidth / 2 + blockWidth
            && areaPos[i][1] >= mapCenterPosY - mapHeight / 2 - blockHeight * 2 && areaPos[i][1] <= mapCenterPosY + mapHeight / 2 + blockHeight)
            && (k == 1 || k == 2 || k == 3 || k == 4 || k == 5 || k == 6)) {

            std::sprintf(frameName_buffer, "ENVIRONMENT/bushes_%d.png", k);

            bushes[i] = Sprite::create(frameName_buffer);
            bushes[i]->setPosition(Point(areaPos[i][0], areaPos[i][1]));
            bushes[i]->setAnchorPoint(Vec2(0.5, 0.0));
            layer->addChild(bushes[i], blockCount * 2 + int((vSize.height - bushes[i]->getPosition().y) / blockHeight) + 1);

            if (k >= 2 && k <= 5) {

                auto leaves = Sprite::create("ENVIRONMENT/bushes_1.png");
                leaves->setPosition(bushes[i]->getPosition());
                leaves->setAnchorPoint(Vec2(0.5, 0.0));
                layer->addChild(leaves, blockCount * 2 + int((vSize.height - bushes[i]->getPosition().y) / blockHeight) + 1);

                bushes[i]->setPosition(Point(leaves->getPosition().x, leaves->getPosition().y + leaves->getContentSize().height * 0.1));
                bushes[i]->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(1.0, 1.0, 0.9), ScaleTo::create(1.0, 1.0, 1.0), nullptr)));
            }
        }
    }

    // surrounding grasses
    int increment, k = 0, currentTag, signW, signH;
    for (int i = 0; i < blockCount; i++) {

        if (i == 0) {

            increment = 1;
            currentTag = 0;
            signW = 1;
            signH = 0;

            grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
            grassSurround[k]->setPosition(Point(blockPos[currentTag][0] + blockWidth, blockPos[currentTag][1] + blockHeight));
            layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

            k++;
        }
        else if (i == pointsHeight - 1) {

            increment = pointsHeight;
            currentTag = pointsHeight - 1;
            signW = 0;
            signH = -1;

            grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
            grassSurround[k]->setPosition(Point(blockPos[currentTag][0] + blockWidth, blockPos[currentTag][1] - blockHeight));
            layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

            k++;
        }
        else if (i == pointsHeight * (pointsWidth - 1)) {

            increment = 1;
            currentTag = pointsHeight * (pointsWidth - 1);
            signW = -1;
            signH = 0;

            grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
            grassSurround[k]->setPosition(Point(blockPos[currentTag][0] - blockWidth, blockPos[currentTag][1] + blockHeight));
            layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

            k++;
        }
        else if (i == blockCount - 1) {

            increment = -pointsHeight;
            currentTag = pointsHeight * (pointsWidth - 1);
            signW = 0;
            signH = 1;

            grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
            grassSurround[k]->setPosition(Point(blockPos[blockCount - 1][0] - blockWidth, blockPos[blockCount - 1][1] - blockHeight));
            layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

            k++;

            for (int j = 1; j <= pointsWidth - 2; j++) {

                currentTag += increment;

                grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
                grassSurround[k]->setPosition(Point(blockPos[currentTag][0] + signW * blockWidth, blockPos[currentTag][1] + signH * blockHeight));
                layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

                k++;
            }
        }
        else if (i == currentTag + increment) {

            currentTag += increment;

            grassSurround[k] = Sprite::create("ENVIRONMENT/field.png");
            grassSurround[k]->setPosition(Point(blockPos[currentTag][0] + signW * blockWidth, blockPos[currentTag][1] + signH * blockHeight));
            layer->addChild(grassSurround[k], blockCount * 2 - 4 + 1);

            k++;
        }
    }

    for (int g = 0; g < 8; g++) {

        grassSurround[k + g] = Sprite::create("ENVIRONMENT/field.png");
        layer->addChild(grassSurround[k + g], blockCount * 2 - 4 + 1);
    }

    grassSurround[k]    ->setPosition(Point(blockPos[0][0],                                             blockPos[0][1] + blockHeight));
    grassSurround[k + 1]->setPosition(Point(blockPos[0][0] + blockWidth,                                blockPos[0][1]));
    grassSurround[k + 2]->setPosition(Point(blockPos[pointsHeight - 1][0] + blockWidth,                 blockPos[pointsHeight - 1][1]));
    grassSurround[k + 3]->setPosition(Point(blockPos[pointsHeight - 1][0],                              blockPos[pointsHeight - 1][1] - blockHeight));
    grassSurround[k + 4]->setPosition(Point(blockPos[pointsHeight * (pointsWidth - 1)][0],              blockPos[pointsHeight * (pointsWidth - 1)][1] + blockHeight));
    grassSurround[k + 5]->setPosition(Point(blockPos[pointsHeight * (pointsWidth - 1)][0] - blockWidth, blockPos[pointsHeight * (pointsWidth - 1)][1]));
    grassSurround[k + 6]->setPosition(Point(blockPos[blockCount - 1][0] - blockWidth,                   blockPos[blockCount - 1][1]));
    grassSurround[k + 7]->setPosition(Point(blockPos[blockCount - 1][0],                                blockPos[blockCount - 1][1] - blockHeight));


    /////////////
    // SPECIAL //
    /////////////

    for (int h = 0; h < obsCount; h++) {

        obstaclePos[h][0] = vSize.width * 3/2 + vOrigin.x;
        obstaclePos[h][1] = vSize.height / 2 + vOrigin.y;
    }

    for (int h = 0; h < gatesCount; h++) {

        gatePos[h][0] = vSize.width * 3/2 + vOrigin.x;
        gatePos[h][1] = vSize.height / 2 + vOrigin.y;
        gateRot[h] = 0.0;
    }

    for (int h = 0; h < holesCount; h++) {
        
        holePos[h][0] = vSize.width * 3/2 + vOrigin.x;
        holePos[h][1] = vSize.height / 2 + vOrigin.y;

        holeTracePos_1[h][0] = vSize.width * 3/2 + vOrigin.x;
        holeTracePos_1[h][1] = vSize.height / 2 + vOrigin.y;

        holeTracePos_2[h][0] = vSize.width * 3/2 + vOrigin.x;
        holeTracePos_2[h][1] = vSize.height / 2 + vOrigin.y;

        holeTracePos_3[h][0] = vSize.width * 3/2 + vOrigin.x;
        holeTracePos_3[h][1] = vSize.height / 2 + vOrigin.y;

        holeMoveDirection[h] = "RIGHT";
    }

    for (int h = 0; h < trapsCount; h++) {
    
        trapPos[h][0] = vSize.width * 3/2 + vOrigin.x;
        trapPos[h][1] = vSize.height / 2 + vOrigin.y;
        trapRot[h] = 0.0;

        fakePlatePos[h][0] = vSize.width * 3/2 + vOrigin.x;
        fakePlatePos[h][1] = vSize.height / 2 + vOrigin.y;
        fakeTrapRotation[h] = 0;
    }

    for (int h = 0; h < 512; h++) {
        
        blockHiding_tag[h][0] = -1;
        gateTag[h] = false;
    }

    // additional levels configuration
    switch (level) {

        case 1 : {

            arrow = Sprite::create("GUI/arrow.png");
            arrow->setPosition(block[4]->getPosition());
            arrow->setOpacity(0);
            layer->addChild(arrow, 10000 - 1 + 1);
            
            break;
        }
        case 4 : {

            int point_arr[] = {12};
            obstaclesConfiguration(1, point_arr);
            break;
        }
        case 5 : {

            int point_arr[] = {8};
            obstaclesConfiguration(1, point_arr);
            break;
        }
        case 11 : {

            int point_arr[] = {24, 17};
            obstaclesConfiguration(2, point_arr);
            break;
        }
        case 12 : {

            int point_arr[] = {13, 12};
            obstaclesConfiguration(2, point_arr);
            break;
        }
        case 13: {

            int point_arr[] = {17, 13, 12, 11, 7};
            obstaclesConfiguration(5, point_arr);
            break;
        }
        case 14 : {

            int point_arr[] = {28, 22, 7, 1};
            obstaclesConfiguration(4, point_arr);
            break;
        }
        case 15 : {

            int point_arr[] = {13, 8, 3, 11, 6, 1};
            obstaclesConfiguration(6, point_arr);
            break;
        }
        case 22 : {

            {
                int point_arr[] = {20, 19, 18, 17, 16, 15, 14};
                obstaclesConfiguration(7, point_arr);

                obstacle[3][0]->runAction(RemoveSelf::create());
                obstacle[3][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {17};
                float rotation_arr[] = {0.0};
                gatesConfiguration(1, point_arr, rotation_arr);
            }

            break;
        }
        case 23 : {

            {
                int point_arr[] = {2, 8, 14, 20, 26, 32, 38};
                obstaclesConfiguration(7, point_arr);

                obstacle[3][0]->runAction(RemoveSelf::create());
                obstacle[3][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {20};
                float rotation_arr[] = {90.0};
                gatesConfiguration(1, point_arr, rotation_arr);
            }

            break;
        }
        case 24 : {

            {
                int point_arr[] = {31, 30, 29, 28, 27, 26, 25, 24};
                obstaclesConfiguration(8, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[7][0]->runAction(RemoveSelf::create());
                obstacle[7][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {31, 24};
                float rotation_arr[] = {180.0, 180.0};
                gatesConfiguration(2, point_arr, rotation_arr);
            }
            
            break;
        }
        case 25 : {

            {
                int point_arr[] = {3, 9, 15, 21, 27, 33, 39, 45};
                obstaclesConfiguration(8, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[7][0]->runAction(RemoveSelf::create());
                obstacle[7][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {3, 45};
                float rotation_arr[] = {270.0, 270.0};
                gatesConfiguration(2, point_arr, rotation_arr);
            }
            
            break;
        }
        case 26 : {

            {
                int point_arr[] = {35, 34, 33, 32, 31, 30, 29, 28, 27};
                obstaclesConfiguration(9, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[4][0]->runAction(RemoveSelf::create());
                obstacle[4][1]->runAction(RemoveSelf::create());

                obstacle[8][0]->runAction(RemoveSelf::create());
                obstacle[8][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {35, 31, 27};
                float rotation_arr[] = {180.0, 180.0, 180.0};
                gatesConfiguration(3, point_arr, rotation_arr);
            }

            break;
        }
        case 27 : {

            {
                int point_arr[] = {2, 8, 14, 20, 26, 32, 38, 44, 50};
                obstaclesConfiguration(9, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[4][0]->runAction(RemoveSelf::create());
                obstacle[4][1]->runAction(RemoveSelf::create());

                obstacle[8][0]->runAction(RemoveSelf::create());
                obstacle[8][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {2, 26, 50};
                float rotation_arr[] = {90.0, 90.0, 90.0};
                gatesConfiguration(3, point_arr, rotation_arr);
            }
            
            break;
        }
        case 28 : {

            int point_arr[] = {0, 6, 48, 42, 22, 15, 26, 33, 10, 11, 38, 37};
            obstaclesConfiguration(12, point_arr);
            break;
        }
        case 29 : {

            int point_arr[] = {7, 6, 14, 15, 55, 54, 46, 47};
            obstaclesConfiguration(8, point_arr);
            break;
        }
        case 30 : {

            int point_arr[] = {17, 24, 40, 39, 37, 36};
            obstaclesConfiguration(6, point_arr);
            break;
        }
        case 31 : {

            int point_arr[] = {21, 14};
            float rotation_arr[] = {270.0, 270.0};
            trapConfiguration(2, point_arr, rotation_arr);
            break;
        }
        case 32 : {

            int point_arr[] = {9, 11};
            float rotation_arr[] = {270.0, 90.0};
            trapConfiguration(2, point_arr, rotation_arr);
            break;
        }
        case 33 : {

            int point_arr[] = {16, 27, 14};
            float rotation_arr[] = {270.0, 180.0, 180.0};
            trapConfiguration(3, point_arr, rotation_arr);
            break;
        }
        case 34 : {

            int point_arr[] = {38, 36, 22};
            float rotation_arr[] = {0.0, 180.0, 270.0};
            trapConfiguration(3, point_arr, rotation_arr);
            break;
        }
        case 35 : {

            int point_arr[] = {23, 10, 2};
            float rotation_arr[] = {90.0, 180.0, 0.0};
            trapConfiguration(3, point_arr, rotation_arr);
            break;
        }
        case 36 : {

            {
                int point_arr[] = {3, 12, 21, 23, 14, 5};
                obstaclesConfiguration(6, point_arr);
            }
            {
                int point_arr[] = {40, 38, 42};
                float rotation_arr[] = {180.0, 270.0, 90.0};
                trapConfiguration(3, point_arr, rotation_arr);
            }

            break;
        }
        case 37 : {

            {
                int point_arr[] = {35, 34, 33, 23, 22, 21};
                obstaclesConfiguration(6, point_arr);
            }
            {
                int point_arr[] = {8, 19, 31, 44};
                float rotation_arr[] = {180.0, 90.0, 90.0, 0.0};
                trapConfiguration(4, point_arr, rotation_arr);
            }

            break;
        }
        case 38 : {

            {
                int point_arr[] = {44, 37, 2, 9, 4, 11, 46, 39, 3, 45};
                obstaclesConfiguration(10, point_arr);
            }
            {
                int point_arr[] = {23, 10, 25, 38};
                float rotation_arr[] = {180.0, 270.0, 0.0, 90.0};
                trapConfiguration(4, point_arr, rotation_arr);
            }
            
            break;
        }
        case 39 : {

            {
                int point_arr[] = {4, 13, 12, 20, 19, 27, 26, 34, 33, 48, 49, 32};
                obstaclesConfiguration(12, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[11][0]->runAction(RemoveSelf::create());
                obstacle[11][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {4, 32};
                float rotation_arr[] = {270.0, 0.0};
                gatesConfiguration(2, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {25, 9, 11};
                float rotation_arr[] = {180.0, 0.0, 90.0};
                trapConfiguration(3, point_arr, rotation_arr);
            }

            break;
        }
        case 40 : {

            {
                int point_arr[] = {14, 15, 16, 17, 18, 19, 20, 21, 23, 25, 27};
                obstaclesConfiguration(11, point_arr);

                obstacle[1][0]->runAction(RemoveSelf::create());
                obstacle[1][1]->runAction(RemoveSelf::create());

                obstacle[3][0]->runAction(RemoveSelf::create());
                obstacle[3][1]->runAction(RemoveSelf::create());

                obstacle[5][0]->runAction(RemoveSelf::create());
                obstacle[5][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {15, 17, 19};
                float rotation_arr[] = {0.0, 0.0, 0.0};
                gatesConfiguration(3, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {7, 9, 11, 13};
                float rotation_arr[] = {0.0, 90.0, 180.0, 270.0};
                trapConfiguration(4, point_arr, rotation_arr);
            }

            break;
        }
        case 41 : {

            {
                int point_arr[] = {2, 11, 20, 29, 38, 47, 56, 6, 15, 24, 33, 42, 51, 60};
                obstaclesConfiguration(14, point_arr);

                obstacle[3][0]->runAction(RemoveSelf::create());
                obstacle[3][1]->runAction(RemoveSelf::create());

                obstacle[8][0]->runAction(RemoveSelf::create());
                obstacle[8][1]->runAction(RemoveSelf::create());

                obstacle[12][0]->runAction(RemoveSelf::create());
                obstacle[12][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {29, 15, 51};
                float rotation_arr[] = {270.0, 270.0, 270.0};
                gatesConfiguration(3, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {21, 23, 41, 39};
                float rotation_arr[] = {270.0, 0.0, 90.0, 180.0};
                trapConfiguration(4, point_arr, rotation_arr);
            }

            break;
        }
        case 42 : {

            {
                int point_arr[] = {14, 15, 16, 17, 18, 19, 20, 35, 36, 37, 38, 39, 40, 41};
                obstaclesConfiguration(14, point_arr);

                obstacle[0][0]->runAction(RemoveSelf::create());
                obstacle[0][1]->runAction(RemoveSelf::create());

                obstacle[6][0]->runAction(RemoveSelf::create());
                obstacle[6][1]->runAction(RemoveSelf::create());
                
                obstacle[10][0]->runAction(RemoveSelf::create());
                obstacle[10][1]->runAction(RemoveSelf::create());
            }
            {
                int point_arr[] = {14, 20, 38};
                float rotation_arr[] = {180.0, 180.0, 180.0};
                gatesConfiguration(3, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {22, 24, 26, 43, 47, 52};
                float rotation_arr[] = {270.0, 0.0, 90.0, 90.0, 270.0, 180.0};
                trapConfiguration(6, point_arr, rotation_arr);
            }

            break;
        }
        case 43 : {

            {
                int point_arr[] = {31, 14, 4, 3, 9, 24, 32, 49, 59};
                float rotation_arr[] = {0.0, 90.0, 180.0, 270.0, 0.0, 90.0, 180.0, 270.0, 0.0};
                trapConfiguration(9, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {7, 0, 56};
                float rotation_arr[] = {0.0, 0.0, 180.0};
                int valSign_w_trap[] = {1, 1, -1};
                int valSign_h_trap[] = {-1, 1, 1};
                fakeCheeseConfiguration(3, point_arr, rotation_arr, valSign_w_trap, valSign_h_trap);
            }

            break;
        }
        case 44 : {

            {
                int point_arr[] = {55, 57, 59, 61, 36, 38, 40, 42, 44};
                float rotation_arr[] = {0.0, 90.0, 180.0, 270.0, 0.0, 90.0, 180.0, 270.0, 0.0};
                trapConfiguration(9, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {63, 71};
                float rotation_arr[] = {180.0, 180.0};
                int valSign_w_trap[] = {-1, -1};
                int valSign_h_trap[] = { 1, -1};
                fakeCheeseConfiguration(2, point_arr, rotation_arr, valSign_w_trap, valSign_h_trap);
            }

            break;
        }
        case 45 : {

            {
                int point_arr[] = {9, 3, 35, 67, 57, 29, 45, 23, 55, 32};
                float rotation_arr[] = {0.0, 180.0, 90.0, 0.0, 180.0, 270.0, 180.0, 0.0, 90.0, 270.0};
                trapConfiguration(10, point_arr, rotation_arr);
            }
            {
                int point_arr[] = {0, 64};
                float rotation_arr[] = {0.0, 180.0};
                int valSign_w_trap[] = {1, -1};
                int valSign_h_trap[] = {1,  1};
                fakeCheeseConfiguration(2, point_arr, rotation_arr, valSign_w_trap, valSign_h_trap);
            }

            break;
        }
    }

    // generate holes
    switch (level) {

        case 16 : holesConfiguration(3); break;
        case 17 : holesConfiguration(3); break;
        case 18 : holesConfiguration(4); break;
        case 19 : holesConfiguration(3); break;
        case 20 : holesConfiguration(4); break;
        case 21 : holesConfiguration(4); break;
        case 22 : holesConfiguration(4); break;
        case 23 : holesConfiguration(4); break;
        case 24 : holesConfiguration(4); break;
        case 25 : holesConfiguration(4); break;
        case 26 : holesConfiguration(5); break;
        case 27 : holesConfiguration(5); break;
        case 28 : holesConfiguration(4); break;
        case 29 : holesConfiguration(5); break;
        case 30 : holesConfiguration(5); break;
        case 38 : holesConfiguration(4); break;
        case 39 : holesConfiguration(4); break;
        case 40 : holesConfiguration(4); break;
        case 41 : holesConfiguration(5); break;
        case 42 : holesConfiguration(4); break;
        case 43 : holesConfiguration(6); break;
        case 44 : holesConfiguration(6); break;
        case 45 : holesConfiguration(6); break;
    }

    // assign 'fewestMoves'
    switch (level) {

        case 1  : fewestMoves = 9;  break;
        case 2  : fewestMoves = 9;  break;
        case 3  : fewestMoves = 15; break;
        case 4  : fewestMoves = 15; break;
        case 5  : fewestMoves = 12; break;
        case 6  : fewestMoves = 15; break;
        case 7  : fewestMoves = 12; break;
        case 8  : fewestMoves = 12; break;
        case 9  : fewestMoves = 24; break;
        case 10 : fewestMoves = 24; break;
        case 11 : fewestMoves = 30; break;
        case 12 : fewestMoves = 21; break;
        case 13 : fewestMoves = 27; break;
        case 14 : fewestMoves = 27; break;
        case 15 : fewestMoves = 30; break;
        case 16 : fewestMoves = 12; break;
        case 17 : fewestMoves = 15; break;
        case 18 : fewestMoves = 18; break;
        case 19 : fewestMoves = 15; break;
        case 20 : fewestMoves = 18; break;
        case 21 : fewestMoves = 27; break;
        case 22 : fewestMoves = 33; break;
        case 23 : fewestMoves = 33; break;
        case 24 : fewestMoves = 33; break;
        case 25 : fewestMoves = 33; break;
        case 26 : fewestMoves = 36; break;
        case 27 : fewestMoves = 36; break;
        case 28 : fewestMoves = 18; break;
        case 29 : fewestMoves = 18; break;
        case 30 : fewestMoves = 27; break;
        case 31 : fewestMoves = 21; break;
        case 32 : fewestMoves = 15; break;
        case 33 : fewestMoves = 15; break;
        case 34 : fewestMoves = 36; break;
        case 35 : fewestMoves = 36; break;
        case 36 : fewestMoves = 36; break;
        case 37 : fewestMoves = 36; break;
        case 38 : fewestMoves = 21; break;
        case 39 : fewestMoves = 39; break;
        case 40 : fewestMoves = 30; break;
        case 41 : fewestMoves = 36; break;
        case 42 : fewestMoves = 45; break;
        case 43 : fewestMoves = 42; break;
        case 44 : fewestMoves = 42; break;
        case 45 : fewestMoves = 42; break;
    }
}

void BlocksMap::holesMove() {

    switch (level) {

        case 16 : for (int i = 0; i < 3; i++) holesMove_way(i); break;
        case 17 : for (int i = 0; i < 3; i++) holesMove_way(i); break;
        case 18 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 19 : for (int i = 0; i < 3; i++) holesMove_way(i); break;
        case 20 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 21 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 22 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 23 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 24 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 25 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 26 : for (int i = 0; i < 5; i++) holesMove_way(i); break;
        case 27 : for (int i = 0; i < 5; i++) holesMove_way(i); break;
        case 28 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 29 : for (int i = 0; i < 5; i++) holesMove_way(i); break;
        case 30 : for (int i = 0; i < 5; i++) holesMove_way(i); break;
        case 38 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 39 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 40 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 41 : for (int i = 0; i < 5; i++) holesMove_way(i); break;
        case 42 : for (int i = 0; i < 4; i++) holesMove_way(i); break;
        case 43 : for (int i = 0; i < 6; i++) holesMove_way(i); break;
        case 44 : for (int i = 0; i < 6; i++) holesMove_way(i); break;
        case 45 : for (int i = 0; i < 6; i++) holesMove_way(i); break;
    }
}

void BlocksMap::holesMove_way(int member) {

    int tryCount = 0;
    bool reachedEnd = false;

    {
        if (holeMoveDirection[member] == "RIGHT") {

            tryCount++;
        
            if (isNotMicePositionTarget(randInt_hole[member] - pointsHeight) == true && (isAtObstaclePos(randInt_hole[member] - pointsHeight) == false || isAtGatePos(randInt_hole[member] - pointsHeight) == true)
                && isAtTrapPos(randInt_hole[member] - pointsHeight) == false && isAtHolePos(randInt_hole[member] - pointsHeight) == false && isAtHoleTrace(randInt_hole[member] - pointsHeight, member) == false
                && holePos[member][0] != blockPos[0][0]) {

                holeTracesAssignment(&member);

                randInt_hole[member] -= pointsHeight;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else holeMoveDirection[member] = "DOWN";
        }
    }
    {
        if (holeMoveDirection[member] == "DOWN") {

            tryCount++;

            if (isNotMicePositionTarget(randInt_hole[member] + 1) == true && (isAtObstaclePos(randInt_hole[member] + 1) == false || isAtGatePos(randInt_hole[member] + 1) == true)
                && isAtTrapPos(randInt_hole[member] + 1) == false && isAtHolePos(randInt_hole[member] + 1) == false && isAtHoleTrace(randInt_hole[member] + 1, member) == false
                && holePos[member][1] != blockPos[blockCount - 1][1]) {

                holeTracesAssignment(&member);
                
                randInt_hole[member] += 1;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else holeMoveDirection[member] = "LEFT";
        }
    }
    {
        if (holeMoveDirection[member] == "LEFT") {

            tryCount++;

            if (isNotMicePositionTarget(randInt_hole[member] + pointsHeight) == true && (isAtObstaclePos(randInt_hole[member] + pointsHeight) == false || isAtGatePos(randInt_hole[member] + pointsHeight) == true)
                && isAtTrapPos(randInt_hole[member] + pointsHeight) == false && isAtHolePos(randInt_hole[member] + pointsHeight) == false && isAtHoleTrace(randInt_hole[member] + pointsHeight, member) == false
                && holePos[member][0] != blockPos[blockCount - 1][0]) {

                holeTracesAssignment(&member);

                randInt_hole[member] += pointsHeight;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else holeMoveDirection[member] = "UP";
        }
    }
    {
        if (holeMoveDirection[member] == "UP") {

            tryCount++;

            if (isNotMicePositionTarget(randInt_hole[member] - 1) == true && (isAtObstaclePos(randInt_hole[member] - 1) == false || isAtGatePos(randInt_hole[member] - 1) == true)
                && isAtTrapPos(randInt_hole[member] - 1) == false && isAtHolePos(randInt_hole[member] - 1) == false && isAtHoleTrace(randInt_hole[member] - 1, member) == false
                && holePos[member][1] != blockPos[0][1]) {

                holeTracesAssignment(&member);

                randInt_hole[member] -= 1;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else {reachedEnd = true; holeMoveDirection[member] = "RIGHT";}
        }
    }

    if (reachedEnd == true) {

        if (tryCount < 4) {
            
            tryCount = 0;
            reachedEnd = false;
            holesMove_way(member);
        }
        else {

            tryCount = 0;
            reachedEnd = false;
                
            if (isNotMicePositionTarget(randInt_hole[member] - 1) == true && (isAtObstaclePos(randInt_hole[member] - 1) == false || isAtGatePos(randInt_hole[member] - 1) == true)
                && isAtTrapPos(randInt_hole[member] - 1) == false && isAtHolePos(randInt_hole[member] - 1) == false && holePos[member][1] != blockPos[0][1]) {

                holeMoveDirection[member] = "UP";
                holeTracesAssignment(&member);

                randInt_hole[member] -= 1;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else if (isNotMicePositionTarget(randInt_hole[member] + pointsHeight) == true && (isAtObstaclePos(randInt_hole[member] + pointsHeight) == false || isAtGatePos(randInt_hole[member] + pointsHeight) == true)
                    && isAtTrapPos(randInt_hole[member] + pointsHeight) == false && isAtHolePos(randInt_hole[member] + pointsHeight) == false && holePos[member][0] != blockPos[blockCount - 1][0]) {

                holeMoveDirection[member] = "LEFT";
                holeTracesAssignment(&member);

                randInt_hole[member] += pointsHeight;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];

                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else if (isNotMicePositionTarget(randInt_hole[member] + 1) == true && (isAtObstaclePos(randInt_hole[member] + 1) == false || isAtGatePos(randInt_hole[member] + 1) == true)
                    && isAtTrapPos(randInt_hole[member] + 1) == false && isAtHolePos(randInt_hole[member] + 1) == false && holePos[member][1] != blockPos[blockCount - 1][1]) {

                holeMoveDirection[member] = "DOWN";
                holeTracesAssignment(&member);

                randInt_hole[member] += 1;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];
                
                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
            else if (isNotMicePositionTarget(randInt_hole[member] - pointsHeight) == true && (isAtObstaclePos(randInt_hole[member] - pointsHeight) == false || isAtGatePos(randInt_hole[member] - pointsHeight) == true)
                    && isAtTrapPos(randInt_hole[member] - pointsHeight) == false && isAtHolePos(randInt_hole[member] - pointsHeight) == false && holePos[member][0] != blockPos[0][0]) {

                holeMoveDirection[member] = "RIGHT";
                holeTracesAssignment(&member);

                randInt_hole[member] -= pointsHeight;
                holePos[member][0] = blockPos[randInt_hole[member]][0];
                holePos[member][1] = blockPos[randInt_hole[member]][1];
                
                hole[member]->runAction(Sequence::create(ScaleTo::create(0.5, 0.0), MoveTo::create(0.0, Point(holePos[member][0], holePos[member][1])), ScaleTo::create(0.5, 1.0), nullptr));
            }
        }
    }
}

void BlocksMap::holeTracesAssignment(int *member) {

    holeTracePos_3[*member][0] = holeTracePos_2[*member][0];
    holeTracePos_3[*member][1] = holeTracePos_2[*member][1];

    holeTracePos_2[*member][0] = holeTracePos_1[*member][0];
    holeTracePos_2[*member][1] = holeTracePos_1[*member][1];

    holeTracePos_1[*member][0] = holePos[*member][0];
    holeTracePos_1[*member][1] = holePos[*member][1];
}

bool BlocksMap::holesVisible() {

    if (holeOn == true) {

        for (int i = 0; i < currentHoles; i++) {

            layer->reorderChild(hole[i], 999);

            hole[i]->setScale(0.0);
            hole[i]->setOpacity(127);
            hole[i]->runAction(ScaleTo::create(0.5, 1.0));
        }

        return true;
    }
    else return false;
}

void BlocksMap::holesInvisible() {
    
    if (holeOn == true) {

        for (int i = 0; i < currentHoles; i++) {
            
            hole[i]->runAction(ScaleTo::create(0.5, 0.0));
        }
    }
}

void BlocksMap::holesInvisible_rebuild() {

    if (holeOn == true) {

        for (int i = 0; i < currentHoles; i++) {

            layer->reorderChild(hole[i], 1);

            hole[i]->setOpacity(255);
            hole[i]->setScale(1.0);
        }
    }
}


/////////////////////
// SPECIAL METHODS //
/////////////////////

void BlocksMap::obstaclesConfiguration(int count, int *point_arr) {

    obstacleOn = true;

    for (int i = 0; i < count; i++) {
        
        // 3D effects
        obstacle[i][0] = Sprite::create("ENVIRONMENT/field_3d_effect.png");
        obstacle[i][0]->setPosition(Point(blockPos[point_arr[i]][0], blockPos[point_arr[i]][1]));
        layer->addChild(obstacle[i][0], blockCount * 2 - 6 + 1);

        // grasses
        obstacle[i][1] = Sprite::create("ENVIRONMENT/field.png");
        obstacle[i][1]->setPosition(Point(blockPos[point_arr[i]][0], blockPos[point_arr[i]][1]));
        layer->addChild(obstacle[i][1], blockCount * 2 - 5 + 1);

        // obstacles position
        obstaclePos[i][0] = blockPos[point_arr[i]][0];
        obstaclePos[i][1] = blockPos[point_arr[i]][1];

        blockHiding_tag[point_arr[i]][0] = point_arr[i];
    }
}

void BlocksMap::gatesConfiguration(int count, int *point_arr, float *rotation_arr) {

    gateOn = true;
    currentGates = count;

    for (int j = 0; j < count; j++) {
        
        for (int i = 1; i <= gateFrames_count; i++) {

            std::sprintf(frameName_buffer, "LEVEL/GATE/gate_%d.png", i);

            gate[j][i - 1] = Sprite::create(frameName_buffer);
            gate[j][i - 1]->setPosition(Point(blockPos[point_arr[j]][0], blockPos[point_arr[j]][1]));
            gate[j][i - 1]->setRotation(rotation_arr[j]);
            if (i != 1) gate[j][i - 1]->setOpacity(0);
            layer->addChild(gate[j][i - 1], blockCount * 2 - 4 + 1);
        }

        gateTag[point_arr[j]] = true;
        gateRot[j] = gate[j][0]->getRotation();
        gatePos[j][0] = gate[j][0]->getPosition().x;
        gatePos[j][1] = gate[j][0]->getPosition().y;
    }
}

void BlocksMap::holesConfiguration(int count) {

    holeOn = true;
    currentHoles = count;

    for (int i = 0; i < count; i++) {

        int randInt = std::rand() % blockCount;
        
        for (int j = 1; j <= 1000; j++) {

            {
                if (i != 0) {

                    for (int k = i; k > 0; k--) {

                        if (randInt == randInt_hole[k - 1]) {

                            if (randInt == blockCount - 1)      randInt -= pointsHeight * std::rand() % 2 + 1;
                            else if (randInt == 0)              randInt += pointsHeight * std::rand() % 2 + 1;
                            else if (randInt == blockCount - 2) randInt--;
                            else if (randInt == 1)              randInt++;
                            else                                randInt -= std::rand() % 2 + 1;
                        }
                    }
                }
            }
            if (isNotMicePositionTarget(randInt) == false || (isAtObstaclePos(randInt) == true && isAtGatePos(randInt) == false) || isAtTrapPos(randInt) == true) {
                
                if (randInt == blockCount - 1)      randInt -= pointsHeight * std::rand() % 2 + 1;
                else if (randInt == 0)              randInt += pointsHeight * std::rand() % 2 + 1;
                else if (randInt == blockCount - 2) randInt--;
                else if (randInt == 1)              randInt++;
                else                                randInt += std::rand() % 2 + 1;
            }
        }

        randInt_hole[i] = randInt;

        hole[i] = Sprite::create("LEVEL/black_hole.png");
        hole[i]->setPosition(Point(blockPos[randInt][0], blockPos[randInt][1]));
        layer->addChild(hole[i], 1);

        holePos[i][0] = blockPos[randInt][0];
        holePos[i][1] = blockPos[randInt][1];
    }
}

void BlocksMap::trapConfiguration(int count, int *point_arr, float *rotation_arr) {

    trapOn = true;
    currentTraps = count;

    for (int i = 0; i < count; i++) {

        trapPos[i][0] = blockPos[point_arr[i]][0];
        trapPos[i][1] = blockPos[point_arr[i]][1];
        trapFrames_construct(i, rotation_arr[i]);

        auto grassUnder_trap = Sprite::create("ENVIRONMENT/under_field.png");
        grassUnder_trap->setPosition(Point(trapPos[i][0], trapPos[i][1]));
        layer->addChild(grassUnder_trap, blockCount * 2 - 4 + 1);

        blockHiding_tag[point_arr[i]][0] = point_arr[i];
    }
}

void BlocksMap::trapFrames_construct(int member, float rotation) {

    for (int j = 1; j <= trapFrames_count; j++) {

        std::sprintf(frameName_buffer, "LEVEL/TRAP/trap_%dd_%d.png", int(rotation), j);

        trap[member][j - 1] = Sprite::create(frameName_buffer);
        trap[member][j - 1]->setPosition(Point(trapPos[member][0], trapPos[member][1]));
        if (j != 1) trap[member][j - 1]->setOpacity(0);
        layer->addChild(trap[member][j - 1], blockCount * 2 - 3 + 1);
    }

    trapRot[member] = rotation;
}

void BlocksMap::trapFrames_destruct(int member) {

    for (int j = 0; j < trapFrames_count; j++) {

        trap[member][j]->runAction(RemoveSelf::create());
    }
}

void BlocksMap::rotateTraps() {

    if (trapOn == true) {

        for (int i = 0; i < currentTraps; i++) {
        
            for (int j = 0; j < trapFrames_count; j++) trap[i][j]->runAction(RotateBy::create(0.5, 90.0));
        }
    }
}

void BlocksMap::regenerateTraps() {

    if (trapOn == true) {

        float rotation;
        
        for (int i = 0; i < currentTraps; i++) {

            rotation = trapRot[i] + 90.0;
            if (rotation > 270.0) rotation = 0.0;

            trapFrames_destruct(i);
            trapFrames_construct(i, rotation);
        }
    }
}

void BlocksMap::fakeCheeseConfiguration(int count, int *point_arr, float *rotation_arr, int *valSign_w_trap, int *valSign_h_trap) {

    fakeOn = true;
    currentFakeCheeses = count;

    for (int i = 0; i < count; i++) {

        if (point_arr[i] != plateTag) {

            fakePlate[i] = Sprite::create("LEVEL/plate.png");  // 'valSign_' for accurating plate position exactly at the edge of blocks quad
            fakePlate[i]->setPosition(Point(blockPos[point_arr[i]][0] + valSign_w_trap[i] * blockWidth, blockPos[point_arr[i]][1] + valSign_h_trap[i] * blockHeight));
            layer->addChild(fakePlate[i], blockCount * 2 - 3 + 1);

            fakeCheese[i] = Sprite::create("LEVEL/cheese.png");
            fakeCheese[i]->setPosition(fakePlate[i]->getPosition());
            layer->addChild(fakeCheese[i], blockCount * 2 - 2 + 1);

            fakePlatePos[i][0] = blockPos[point_arr[i]][0];
            fakePlatePos[i][1] = blockPos[point_arr[i]][1];
            fakeTrapRotation[i] = int(rotation_arr[i]);
        }
        else fakeSkipPos_tag = i;
    }
}

void BlocksMap::gateOpenClose(int member, int blockMember, bool onlyOpen) {

    if (onlyOpen == true) {

        for (int i = 0; i < gateFrames_count; i++) layer->reorderChild(gate[member][i], blockCount * 2);

        gate[member][0]->runAction(Sequence::create(FadeOut::create(0), nullptr));
        gate[member][1]->runAction(Sequence::create(FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
        gate[member][2]->runAction(Sequence::create(DelayTime::create(0.05), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
        gate[member][3]->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), nullptr));

        for (int i = 0; i < blockCount; i++) {
            
            if (isAtBlockPos(i, blockMember) == true) {
                
                block[i]->setOpacity(0);
                block3D[i]->setOpacity(0);
            }
        }
    }
    else {

        gate[member][0]->runAction(Sequence::create(FadeOut::create(0), DelayTime::create(0.7), FadeIn::create(0), nullptr));
        gate[member][1]->runAction(Sequence::create(FadeIn::create(0),  DelayTime::create(0.05), FadeOut::create(0), DelayTime::create(0.6), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
        gate[member][2]->runAction(Sequence::create(DelayTime::create(0.05), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), DelayTime::create(0.5), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
        gate[member][3]->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.5), FadeOut::create(0), nullptr));

        for (int i = 0; i < blockCount; i++) {
            
            if (isAtBlockPos(i, blockMember) == true) {
                
                block[i]->runAction(Sequence::create(FadeOut::create(0), DelayTime::create(1.0), FadeIn::create(0), nullptr));
                block3D[i]->runAction(Sequence::create(FadeOut::create(0), DelayTime::create(1.0), FadeIn::create(0), nullptr));
            }
        }
    }
}


/////////////////////
// REGULAR METHODS //
/////////////////////

void BlocksMap::miceAnimation(bool isThisSubstitute) {

    if (isThisSubstitute == true) {

        mice_substitute[0]->runAction(Sequence::create(DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.7), FadeIn::create(0), nullptr));
        mice_substitute[1]->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.5), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice_substitute[2]->runAction(Sequence::create(DelayTime::create(0.2), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.3), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice_substitute[3]->runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice_substitute[4]->runAction(Sequence::create(DelayTime::create(0.4), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
    }
    else {
        
        mice[0]->runAction(Sequence::create(DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.7), FadeIn::create(0), nullptr));
        mice[1]->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.5), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice[2]->runAction(Sequence::create(DelayTime::create(0.2), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.3), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice[3]->runAction(Sequence::create(DelayTime::create(0.3), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), DelayTime::create(0.1), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
        mice[4]->runAction(Sequence::create(DelayTime::create(0.4), FadeIn::create(0), DelayTime::create(0.1), FadeOut::create(0), nullptr));
    }
}

void BlocksMap::closestToMice() {

    if (trapOn == true && gateOn == true) {
        
        float stuff_arr[currentTraps + currentGates + 1];
        int cmprSuit = 0;

        for (int i = 0; i < currentTraps + currentGates + 1; i++) stuff_arr[i] = 0.0;

        // adding all stuff to container
        for (int i = 0; i < currentTraps + currentGates + 1; i++) {

            if (i == 0)                     stuff_arr[i] = std::abs(plate->getPosition().x - mice[0]->getPosition().x) + std::abs(plate->getPosition().y - mice[0]->getPosition().y);
            else if (i <= currentTraps + 1) stuff_arr[i] = std::abs(trapPos[i - 1][0] - mice[0]->getPosition().x) + std::abs(trapPos[i - 1][1] - mice[0]->getPosition().y);
            else                            stuff_arr[i] = std::abs(gatePos[i - 1][0] - mice[0]->getPosition().x) + std::abs(gatePos[i - 1][1] - mice[0]->getPosition().y);
        }

        // searching the lowest value
        for (int i = 0; i < currentTraps + currentGates + 1; i++) {

            for (int j = 0; j < currentTraps + currentGates + 1; j++) {

                if (stuff_arr[i] <= stuff_arr[j] + blockWidth / 8) cmprSuit++;
                else                                               cmprSuit = 0;
            }

            if (cmprSuit >= currentTraps + currentGates + 1) {
                
                if (i == 0) {
                    
                    closestToMice_x = plate->getPosition().x;
                    closestToMice_y = plate->getPosition().y;
                }
                else if (i <= currentTraps + 1) {

                    closestToMice_x = trapPos[i - 1][0];
                    closestToMice_y = trapPos[i - 1][1];
                }
                else {

                    closestToMice_x = gatePos[i - 1][0];
                    closestToMice_y = gatePos[i - 1][1];
                }

                break;
            }
        }

        // prioritizing position
        if (std::abs(mice[0]->getPosition().x - plate->getPosition().x) <= blockWidth * 3 && std::abs(mice[0]->getPosition().y - plate->getPosition().y) <= blockHeight * 3) {

            closestToMice_x = plate->getPosition().x;
            closestToMice_y = plate->getPosition().y;
        }
        
        for (int i = 0; i < currentTraps; i++) {

            if (std::abs(mice[0]->getPosition().x - trapPos[i][0]) <= blockWidth * 3 && std::abs(mice[0]->getPosition().y - trapPos[i][1]) <= blockHeight * 3) {

                closestToMice_x = trapPos[i][0];
                closestToMice_y = trapPos[i][1];
                break;
            }
        }
    }
    else if (trapOn == true) {
        
        float stuff_arr[currentTraps + 1];
        int cmprSuit = 0;

        for (int i = 0; i < currentTraps + 1; i++) stuff_arr[i] = 0.0;

        // adding all stuff to container
        for (int i = 0; i < currentTraps + 1; i++) {

            if (i == 0) stuff_arr[i] = std::abs(plate->getPosition().x - mice[0]->getPosition().x) + std::abs(plate->getPosition().y - mice[0]->getPosition().y);
            else        stuff_arr[i] = std::abs(trapPos[i - 1][0] - mice[0]->getPosition().x) + std::abs(trapPos[i - 1][1] - mice[0]->getPosition().y);
        }

        // searching the lowest value
        for (int i = 0; i < currentTraps + 1; i++) {

            for (int j = 0; j < currentTraps + 1; j++) {

                if (stuff_arr[i] <= stuff_arr[j] + blockWidth / 8) cmprSuit++;
                else                                               cmprSuit = 0;
            }

            if (cmprSuit >= currentTraps + 1) {
                
                if (i == 0) {
                    
                    closestToMice_x = plate->getPosition().x;
                    closestToMice_y = plate->getPosition().y;
                }
                else {

                    closestToMice_x = trapPos[i - 1][0];
                    closestToMice_y = trapPos[i - 1][1];
                }

                break;
            }
        }

        // prioritizing plate position
        if (std::abs(mice[0]->getPosition().x - plate->getPosition().x) <= blockWidth * 3 && std::abs(mice[0]->getPosition().y - plate->getPosition().y) <= blockHeight * 3) {

            closestToMice_x = plate->getPosition().x;
            closestToMice_y = plate->getPosition().y;
        }

        for (int i = 0; i < currentTraps; i++) {

            if (std::abs(mice[0]->getPosition().x - trapPos[i][0]) <= blockWidth * 3 && std::abs(mice[0]->getPosition().y - trapPos[i][1]) <= blockHeight * 3) {

                closestToMice_x = trapPos[i][0];
                closestToMice_y = trapPos[i][1];
                break;
            }
        }
    }
    else if (gateOn == true) {
        
        float stuff_arr[currentGates + 1];
        int cmprSuit = 0;

        for (int i = 0; i < currentGates + 1; i++) stuff_arr[i] = 0.0;

        // adding all stuff to container
        for (int i = 0; i < currentGates + 1; i++) {

            if (i == 0) stuff_arr[i] = std::abs(plate->getPosition().x - mice[0]->getPosition().x) + std::abs(plate->getPosition().y - mice[0]->getPosition().y);
            else        stuff_arr[i] = std::abs(gatePos[i - 1][0] - mice[0]->getPosition().x) + std::abs(gatePos[i - 1][1] - mice[0]->getPosition().y);
        }

        // searching the lowest value
        for (int i = 0; i < currentGates + 1; i++) {

            for (int j = 0; j < currentGates + 1; j++) {

                if (stuff_arr[i] <= stuff_arr[j] + blockWidth / 8) cmprSuit++;
                else                                               cmprSuit = 0;
            }

            if (cmprSuit >= currentGates + 1) {
                
                if (i == 0) {
                    
                    closestToMice_x = plate->getPosition().x;
                    closestToMice_y = plate->getPosition().y;
                }
                else {

                    closestToMice_x = gatePos[i - 1][0];
                    closestToMice_y = gatePos[i - 1][1];
                }

                break;
            }
        }

        // prioritizing plate position
        if (std::abs(mice[0]->getPosition().x - plate->getPosition().x) <= blockWidth * 3 && std::abs(mice[0]->getPosition().y - plate->getPosition().y) <= blockHeight * 3) {

            closestToMice_x = plate->getPosition().x;
            closestToMice_y = plate->getPosition().y;
        }
    }
    else {

        closestToMice_x = plate->getPosition().x;
        closestToMice_y = plate->getPosition().y;
    }
}

void BlocksMap::miceSearchPath() {

    if (mice[0]->getPosition().x >= mapCenterPosX - mapWidth / 2 && mice[0]->getPosition().x <= mapCenterPosX + mapWidth / 2
        && mice[0]->getPosition().y >= mapCenterPosY - mapHeight / 2 && mice[0]->getPosition().y <= mapCenterPosY + mapHeight / 2) {

        isMiceMove = 1;
        closestToMice();

        for (int k = 0; k < blockCount; k++) {

            if (isNotMicePositionTarget(k) == false) {

                blockTag[k] = false;

                if (blockTag[k - pointsHeight] == true && blockTag[k + pointsHeight] == true && blockPos[k][0] != blockPos[0][0] && blockPos[k][0] != blockPos[blockCount - 1][0]
                    && (isAtObstaclePos(k - pointsHeight) == false || gateTag[k - pointsHeight] == true)
                    && (isAtObstaclePos(k + pointsHeight) == false || gateTag[k + pointsHeight] == true)) {

                    if (std::abs(blockPos[k - pointsHeight][0] - closestToMice_x) <= std::abs(blockPos[k + pointsHeight][0] - closestToMice_x)) {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(blockWidth, 0)), RotateTo::create(0.1, 0.0), nullptr));
                    }
                    else {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(-blockWidth, 0)), RotateTo::create(0.1, 180.0), nullptr));
                    }
                }
                else if (blockTag[k - 1] == true && blockTag[k + 1] == true && blockPos[k][1] != blockPos[0][1] && blockPos[k][1] != blockPos[blockCount - 1][1]
                        && (isAtObstaclePos(k - 1) == false || gateTag[k - 1] == true)
                        && (isAtObstaclePos(k + 1) == false || gateTag[k + 1] == true)) {

                    if (std::abs(blockPos[k - 1][1] - closestToMice_y) <= std::abs(blockPos[k + 1][1] - closestToMice_y)) {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, blockHeight)), RotateTo::create(0.1, 270.0), nullptr));
                    }
                    else {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, -blockHeight)), RotateTo::create(0.1, 90.0), nullptr));
                    }
                }
                else if (blockTag[k - pointsHeight] == true && blockTag[k - 1] == true && blockPos[k][0] != blockPos[0][0] && blockPos[k][1] != blockPos[0][1]
                        && (isAtObstaclePos(k - pointsHeight) == false || gateTag[k - pointsHeight] == true)
                        && (isAtObstaclePos(k - 1) == false || gateTag[k - 1] == true)) {

                    if (std::abs(blockPos[k - pointsHeight][0] - closestToMice_x) + std::abs(blockPos[k - pointsHeight][1] - closestToMice_y)
                    <= std::abs(blockPos[k - 1][0] - closestToMice_x) + std::abs(blockPos[k - 1][1] - closestToMice_y)) {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(blockWidth, 0)), RotateTo::create(0.1, 0.0), nullptr));
                    }
                    else {

                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, blockHeight)), RotateTo::create(0.1, 270.0), nullptr));
                    }
                }
                else if (blockTag[k - pointsHeight] == true && blockTag[k + 1] == true && blockPos[k][0] != blockPos[0][0] && blockPos[k][1] != blockPos[blockCount - 1][1]
                        && (isAtObstaclePos(k - pointsHeight) == false || gateTag[k - pointsHeight] == true)
                        && (isAtObstaclePos(k + 1) == false || gateTag[k + 1] == true)) {

                    if (std::abs(blockPos[k - pointsHeight][0] - closestToMice_x) + std::abs(blockPos[k - pointsHeight][1] - closestToMice_y)
                    <= std::abs(blockPos[k + 1][0] - closestToMice_x) + std::abs(blockPos[k + 1][1] - closestToMice_y)) {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(blockWidth, 0)), RotateTo::create(0.1, 0.0), nullptr));
                    }
                    else {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, -blockHeight)), RotateTo::create(0.1, 90.0), nullptr));
                    }
                }
                else if (blockTag[k + pointsHeight] == true && blockTag[k + 1] == true && blockPos[k][0] != blockPos[blockCount - 1][0] && blockPos[k][1] != blockPos[blockCount - 1][1]
                        && (isAtObstaclePos(k + pointsHeight) == false || gateTag[k + pointsHeight] == true)
                        && (isAtObstaclePos(k + 1) == false || gateTag[k + 1] == true)) {

                    if (std::abs(blockPos[k + pointsHeight][0] - closestToMice_x) + std::abs(blockPos[k + pointsHeight][1] - closestToMice_y)
                    <= std::abs(blockPos[k + 1][0] - closestToMice_x) + std::abs(blockPos[k + 1][1] - closestToMice_y)) {
                    
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(-blockWidth, 0)), RotateTo::create(0.1, 180.0), nullptr));
                    }
                    else {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, -blockHeight)), RotateTo::create(0.1, 90.0), nullptr));
                    }
                }
                else if (blockTag[k + pointsHeight] == true && blockTag[k - 1] == true && blockPos[k][0] != blockPos[blockCount - 1][0] && blockPos[k][1] != blockPos[0][1]
                        && (isAtObstaclePos(k + pointsHeight) == false || gateTag[k + pointsHeight] == true)
                        && (isAtObstaclePos(k - 1) == false || gateTag[k - 1] == true)) {

                    if (std::abs(blockPos[k + pointsHeight][0] - closestToMice_x) + std::abs(blockPos[k + pointsHeight][1] - closestToMice_y)
                    <= std::abs(blockPos[k - 1][0] - closestToMice_x) + std::abs(blockPos[k - 1][1] - closestToMice_y)) {
                            
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(-blockWidth, 0)), RotateTo::create(0.1, 180.0), nullptr));
                    }
                    else {
                        
                        for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, blockHeight)), RotateTo::create(0.1, 270.0), nullptr));
                    }
                }
                else if (blockTag[k - pointsHeight] == true && blockPos[k][0] != blockPos[0][0]
                        && (isAtObstaclePos(k - pointsHeight) == false || gateTag[k - pointsHeight] == true)) {
                    
                    for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(blockWidth, 0)), RotateTo::create(0.1, 0.0), nullptr));
                }
                else if (blockTag[k + pointsHeight] == true && blockPos[k][0] != blockPos[blockCount - 1][0]
                        && (isAtObstaclePos(k + pointsHeight) == false || gateTag[k + pointsHeight] == true)) {
                    
                    for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(-blockWidth, 0)), RotateTo::create(0.1, 180.0), nullptr));
                }
                else if (blockTag[k - 1] == true && blockPos[k][1] != blockPos[0][1]
                        && (isAtObstaclePos(k - 1) == false || gateTag[k - 1] == true)) {
                    
                    for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, blockHeight)), RotateTo::create(0.1, 270.0), nullptr));
                }
                else if (blockTag[k + 1] == true && blockPos[k][1] != blockPos[blockCount - 1][1]
                        && (isAtObstaclePos(k + 1) == false || gateTag[k + 1] == true)) {
                
                    for (int i = 0; i < miceFrames_count; i++) mice[i]->runAction(Spawn::create(MoveBy::create(0.1, Point(0, -blockHeight)), RotateTo::create(0.1, 90.0), nullptr));
                }
                else isMiceMove = 0;
            }
        }
    }
    else isMiceMove = 0;
}

void BlocksMap::blocksScan() {

    blockTag_default();
    noBlockUnderObstacle();

    for (int i = 0; i < blockCount; i++) {

        if (blockPos[i][1] > blockPos[pointsHeight - 2][1]) {

            for (int h = 0; h < blockCount; h++) {

                if (isAtBlockPos(h, i) == true) {

                    for (int g = 0; g < blockCount; g++) {

                        if (isAtBlockPos(g, i + 1) == true) {

                            for (int j = 0; j < blockCount; j++) {

                                if (isAtBlockPos(j, i + 2) == true) {

                                    if (block[h]->getColor() == block[g]->getColor()
                                        && block[h]->getColor() == block[j]->getColor()) {

                                        blockTag[i] = true;
                                        blockTag[i + 1] = true;
                                        blockTag[i + 2] = true;

                                        blockFillTag[i] = true;
                                        blockFillTag[i + 1] = true;
                                        blockFillTag[i + 2] = true;
                                    }

                                    break;
                                }
                            }

                            break;
                        }
                    }

                    break;
                }
            }
        }
        else if (blockPos[i][1] <= blockPos[pointsHeight - 2][1]
                && blockPos[i][1] >= blockPos[pointsHeight - 2][1] - blockHeight / 2) {

            for (int h = 0; h < blockCount; h++) {

                if (isAtBlockPos(h, i) == true) {

                    for (int g = 0; g < blockCount; g++) {

                        if (isAtBlockPos(g, i + 1) == true) {

                            for (int j = 0; j < blockCount; j++) {

                                if (isAtBlockPos(j, i - 1) == true) {

                                    if (block[h]->getColor() == block[g]->getColor()
                                        && block[h]->getColor() == block[j]->getColor()) {

                                        blockTag[i] = true;
                                        blockTag[i + 1] = true;
                                        blockTag[i - 1] = true;

                                        blockFillTag[i] = true;
                                        blockFillTag[i + 1] = true;
                                        blockFillTag[i - 1] = true;
                                    }

                                    break;
                                }
                            }

                            break;
                        }
                    }

                    break;
                }
            }
        }
    }

    for (int i = 0; i < blockCount; i++) {

        if (blockPos[i][0] > blockPos[blockCount - 1 - pointsHeight][0]) {

            for (int h = 0; h < blockCount; h++) {

                if (isAtBlockPos(h, i) == true) {

                    for (int g = 0; g < blockCount; g++) {

                        if (isAtBlockPos(g, i + pointsHeight) == true) {

                            for (int j = 0; j < blockCount; j++) {

                                if (isAtBlockPos(j, i + pointsHeight * 2) == true) {

                                    if (block[h]->getColor() == block[g]->getColor()
                                        && block[h]->getColor() == block[j]->getColor()) {

                                        blockTag[i] = true;
                                        blockTag[i + pointsHeight] = true;
                                        blockTag[i + pointsHeight * 2] = true;

                                        blockFillTag[i] = true;
                                        blockFillTag[i + pointsHeight] = true;
                                        blockFillTag[i + pointsHeight * 2] = true;
                                    }

                                    break;
                                }
                            }

                            break;
                        }
                    }

                    break;
                }
            }
        }
        else if (blockPos[i][0] <= blockPos[blockCount - 1 - pointsHeight][0]
                && blockPos[i][0] >= blockPos[blockCount - 1 - pointsHeight][0] - blockWidth / 2) {

            for (int h = 0; h < blockCount; h++) {

                if (isAtBlockPos(h, i) == true) {

                    for (int g = 0; g < blockCount; g++) {

                        if (isAtBlockPos(g, i + pointsHeight) == true) {

                            for (int j = 0; j < blockCount; j++) {

                                if (isAtBlockPos(j, i - pointsHeight) == true) {

                                    if (block[h]->getColor() == block[g]->getColor()
                                        && block[h]->getColor() == block[j]->getColor()) {

                                        blockTag[i] = true;
                                        blockTag[i + pointsHeight] = true;
                                        blockTag[i - pointsHeight] = true;

                                        blockFillTag[i] = true;
                                        blockFillTag[i + pointsHeight] = true;
                                        blockFillTag[i - pointsHeight] = true;
                                    }

                                    break;
                                }
                            }

                            break;
                        }
                    }

                    break;
                }
            }
        }
    }

    returnBlockUnderObstacle();
}

void BlocksMap::selectBlock(int member) {

    if (isNotMicePositionTarget(member) == true && isAtObstaclePos(member) == false && isAtTrapPos(member) == false) {

        for (int i = 0; i < blockCount; i++) {

            if (isAtBlockPos(i, member) == true) {

                block3D[i]->setColor(Color3B::WHITE);

                // DEBUG tool 
                /*
                if      (block[i]->getColor() == Color3B::YELLOW)  aLabel->setString("YELLOW");
                else if (block[i]->getColor() == Color3B::GREEN)   aLabel->setString("GREEN");
                else if (block[i]->getColor() == Color3B::BLUE)    aLabel->setString("BLUE");
                else if (block[i]->getColor() == Color3B::RED)     aLabel->setString("RED");
                else if (block[i]->getColor() == Color3B::MAGENTA) aLabel->setString("MAGENTA");
                else if (block[i]->getColor() == Color3B::ORANGE)  aLabel->setString("ORANGE"); 
                // */

                break;
            }
        }
    }
}

void BlocksMap::unselectBlock(int member) {

    if (isNotMicePositionTarget(member) == true && isAtObstaclePos(member) == false && isAtTrapPos(member) == false) {

        for (int i = 0; i < blockCount; i++) {

            if (isAtBlockPos(i, member) == true) {

                block3D[i]->setColor(Color3B::BLACK);
                break;
            }
        }
    }
}

void BlocksMap::moveBlock(int member, std::string direction) {

    if (isAtObstaclePos(member) == false && isAtTrapPos(member) == false) {

        for (int i = 0; i < blockCount; i++) {

            if (isAtBlockPos(i, member) == true && isNotMicePositionTarget(member) == true) {

                if (direction == "RIGHT") {

                    if (block[i]->getPosition().x < blockPos[0][0] - blockWidth / 2 && isNotMicePositionTarget(member - pointsHeight) == true && isAtObstaclePos(member - pointsHeight) == false && isAtTrapPos(member - pointsHeight) == false) {

                        block[i]->runAction(MoveTo::create(0.1, Point(blockPos[member - pointsHeight][0], blockPos[member - pointsHeight][1])));
                        block3D[i]->runAction(MoveTo::create(0.1, Point(blockPos[member - pointsHeight][0], blockPos[member - pointsHeight][1])));

                        for (int j = 0; j < blockCount; j++) {

                            if (isAtBlockPos(j, member - pointsHeight) == true) {

                                block[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));
                                block3D[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));

                                break;
                            }
                        }

                        isSwiped = true;
                    }
                }
                else if (direction == "LEFT") {

                    if (block[i]->getPosition().x > blockPos[blockCount - 1][0] + blockWidth / 2 && isNotMicePositionTarget(member + pointsHeight) == true && isAtObstaclePos(member + pointsHeight) == false && isAtTrapPos(member + pointsHeight) == false) {

                        block[i]->runAction(MoveTo::create(0.1, Point(blockPos[member + pointsHeight][0], blockPos[member + pointsHeight][1])));
                        block3D[i]->runAction(MoveTo::create(0.1, Point(blockPos[member + pointsHeight][0], blockPos[member + pointsHeight][1])));

                        for (int j = 0; j < blockCount; j++) {

                            if (isAtBlockPos(j, member + pointsHeight) == true) {
                                
                                block[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));
                                block3D[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));

                                break;
                            }
                        }

                        isSwiped = true;
                    }
                }
                else if (direction == "UP") {

                    if (block[i]->getPosition().y < blockPos[0][1] - blockHeight / 2 && isNotMicePositionTarget(member - 1) == true && isAtObstaclePos(member - 1) == false && isAtTrapPos(member - 1) == false) {

                        block[i]->runAction(MoveTo::create(0.1, Point(blockPos[member - 1][0], blockPos[member - 1][1])));
                        block3D[i]->runAction(MoveTo::create(0.1, Point(blockPos[member - 1][0], blockPos[member - 1][1])));

                        for (int j = 0; j < blockCount; j++) {

                            if (isAtBlockPos(j, member - 1) == true) {
                                
                                block[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));
                                block3D[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));

                                break;
                            }
                        }

                        isSwiped = true;
                    }
                }
                else if (direction == "DOWN") {

                    if (block[i]->getPosition().y > blockPos[blockCount - 1][1] + blockHeight / 2 && isNotMicePositionTarget(member + 1) == true && isAtObstaclePos(member + 1) == false && isAtTrapPos(member + 1) == false) {

                        block[i]->runAction(MoveTo::create(0.1, Point(blockPos[member + 1][0], blockPos[member + 1][1])));
                        block3D[i]->runAction(MoveTo::create(0.1, Point(blockPos[member + 1][0], blockPos[member + 1][1])));

                        for (int j = 0; j < blockCount; j++) {

                            if (isAtBlockPos(j, member + 1) == true) {

                                block[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));
                                block3D[j]->runAction(MoveTo::create(0.1, Point(blockPos[member][0], blockPos[member][1])));

                                break;
                            }
                        }

                        isSwiped = true;
                    }
                }
                
                if (isSwiped == true && uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/swipe.mp3", false, 1.0f);
                break;
            }
        }
    }
}

void BlocksMap::blocksBlast(bool isMapChecking) {   // 'isMapChecking = true' for randomly blocks color placing (beginning)

    if (isMapChecking == true) {

        for (int i = 0; i < blockCount; i++) {

            if (blockTag[i] == true) {

                randomColoring(i);
            }
        }
    }
    else {

        for (int i = 0; i < blockCount; i++) {

            if (blockTag[i] == true && isAtObstaclePos(i) == false && isAtTrapPos(i) == false) {

                availablePaths++;

                for (int j = 0; j < blockCount; j++) {

                    if (isAtBlockPos(j, i) == true) {
                        
                        block3D[j]->setColor(Color3B::WHITE);
                        break;
                    }
                }

                // blast frames
                for (int k = 1; k <= 6; k++) {

                    std::sprintf(frameName_buffer, "BLOCK/block_blast_%d.png", k);

                    blast[i][k - 1] = Sprite::create(frameName_buffer);
                    blast[i][k - 1]->setPosition(Point(blockPos[i][0], blockPos[i][1]));

                    for (int h = 0; h < blockCount; h++) {

                        if (isAtBlockPos(h, i) == true) {

                            if      (block[h]->getColor() == Color3B::YELLOW)  blast[i][k - 1]->setColor(Color3B::YELLOW);
                            else if (block[h]->getColor() == Color3B::GREEN)   blast[i][k - 1]->setColor(Color3B::GREEN);
                            else if (block[h]->getColor() == Color3B::BLUE)    blast[i][k - 1]->setColor(Color3B::BLUE);
                            else if (block[h]->getColor() == Color3B::RED)     blast[i][k - 1]->setColor(Color3B::RED);
                            else if (block[h]->getColor() == Color3B::MAGENTA) blast[i][k - 1]->setColor(Color3B::MAGENTA);
                            else if (block[h]->getColor() == Color3B::ORANGE)  blast[i][k - 1]->setColor(Color3B::ORANGE);

                            break;
                        }
                    }

                    blast[i][k - 1]->setOpacity(0);
                    layer->addChild(blast[i][k - 1], blockCount * 2 - 1 + 1);

                    std::sprintf(frameName_buffer, "BLOCK/block_blast_3d_%d.png", k);

                    blast3D[i][k - 1] = Sprite::create(frameName_buffer);
                    blast3D[i][k - 1]->setPosition(Point(blockPos[i][0], blockPos[i][1]));
                    blast3D[i][k - 1]->setOpacity(0);
                    layer->addChild(blast3D[i][k - 1], blockCount * 2 - 1 + 1);
                }
            }
        }
        for (int i = 0; i < blockCount; i++) {

            if (blockTag[i] == true && isAtObstaclePos(i) == false && isAtTrapPos(i) == false) {

                for (int j = 0; j < blockCount; j++) {

                    if (isAtBlockPos(j, i) == true) {
                
                        block[j]->runAction(Sequence::create(DelayTime::create(0.1), FadeOut::create(0), nullptr));
                        block3D[j]->runAction(Sequence::create(DelayTime::create(0.1), FadeOut::create(0), nullptr));
                        break;
                    }
                }

                for (int k = 0; k < 6; k++) {

                    deltaTime_1 = 0.1 + 0.05 * k;
                    deltaTime_2 = 0.05;
                    
                    blast[i][k]->runAction(Sequence::create(DelayTime::create(deltaTime_1), FadeIn::create(0), DelayTime::create(deltaTime_2), FadeOut::create(0.1), RemoveSelf::create(), nullptr));
                    blast3D[i][k]->runAction(Sequence::create(DelayTime::create(deltaTime_1), FadeIn::create(0), DelayTime::create(deltaTime_2), FadeOut::create(0.1), RemoveSelf::create(), nullptr));
                }

                isBlasting = true;
            }
        }
    }
}

void BlocksMap::blocksFill() {

    int middleVerticalTag, ismiceOverMidVer, isNotColumnClean,
        filledCount = 0, unfilledCount = 0, peekFilledCount = 0,
        blockLimit_upside, blockLimit_downside,
        avaPat_buf[pointsWidth], cmprSuit = 0;

    availablePaths = 0;
    ismiceOverMidVer = mice[0]->getPosition().y < blockPos[(pointsHeight - 1) / 2][1] ? -1 : 1;
    
    for (int i = 0; i < pointsWidth; i++) {

        avaPat_buf[i] = 0;
        middleVerticalTag = (pointsHeight - 1) / 2 + pointsHeight * i;

        // scans wheter column is clean from mice
        if (!(blockPos[middleVerticalTag][0] >= mice[0]->getPosition().x - blockWidth / 2
            && blockPos[middleVerticalTag][0] <= mice[0]->getPosition().x + blockWidth / 2)) isNotColumnClean = 0;
        else isNotColumnClean = 1;

        blockLimit_upside = middleVerticalTag - ismiceOverMidVer * (std::abs(int((mice[0]->getPosition().y - blockPos[middleVerticalTag][1]) / blockHeight) + 1)) * isNotColumnClean;
        blockLimit_downside = blockLimit_upside + isNotColumnClean + 1;

        // special
        {
            if (isFall == true) {
        
                for (int j = 0; j < holesCount; j++) {

                    if (mice_substitute[0]->getPosition().x >= holePos[j][0] - blockWidth / 2 && mice_substitute[0]->getPosition().x <= holePos[j][0] + blockWidth / 2
                        && mice_substitute[0]->getPosition().y >= holePos[j][1] - blockHeight / 2 && mice_substitute[0]->getPosition().y <= holePos[j][1] + blockHeight / 2
                        && randInt_hole[j] >= pointsHeight * i && randInt_hole[j] <= pointsHeight * i + pointsHeight - 1) {

                        blockLimit_upside = randInt_hole[j] - 1;
                        blockLimit_downside = blockLimit_upside + 2;
                        break;
                    }
                }
            }
        }
        {
            if (isTrapped == true && isFall == false) {
        
                for (int j = 0; j < blockCount; j++) {

                    if (blockPos[j][0] >= trapPos[trapTag][0] - blockWidth / 2 && blockPos[j][0] <= trapPos[trapTag][0] + blockWidth / 2
                        && blockPos[j][1] >= trapPos[trapTag][1] - blockHeight / 2 && blockPos[j][1] <= trapPos[trapTag][1] + blockHeight / 2
                        && j >= pointsHeight * i && j <= pointsHeight * i + pointsHeight - 1) {

                        blockLimit_upside = j - 1;
                        blockLimit_downside = blockLimit_upside + 2;
                        break;
                    }
                }
            }
        }

        for (int g = blockLimit_upside; g >= pointsHeight * i; g--) {

            if (blockFillTag[g] == false) peekFilledCount++;
        }

        for (int j = blockLimit_upside; j >= pointsHeight * i; j--) {

            if (blockFillTag[j] == true) {

                avaPat_buf[i]++;

                for (int k = 0; k < blockCount; k++) {

                    if (isAtBlockPos(k, j) == true) {

                        unfilledCount++;

                        block[k]->runAction(Sequence::create(
                                            MoveTo::create(0, Point(blockPos[pointsHeight * i][0], blockPos[pointsHeight * i][1] + blockHeight / 2)),
                                            MoveTo::create(0.05, Point(blockPos[pointsHeight * i][0], blockPos[pointsHeight * i][1])),
                                            MoveTo::create(float(0.1 * std::abs(blockLimit_upside - peekFilledCount - unfilledCount + 1 - pointsHeight * i)),
                                                            Point(blockPos[blockLimit_upside - peekFilledCount - unfilledCount + 1][0], blockPos[blockLimit_upside - peekFilledCount - unfilledCount + 1][1])),
                                            nullptr));

                        block3D[k]->runAction(Sequence::create(
                                              MoveTo::create(0, Point(blockPos[pointsHeight * i][0], blockPos[pointsHeight * i][1] + blockHeight / 2)),
                                              MoveTo::create(0.05, Point(blockPos[pointsHeight * i][0], blockPos[pointsHeight * i][1])),
                                              MoveTo::create(float(0.1 * std::abs(blockLimit_upside - peekFilledCount - unfilledCount + 1 - pointsHeight * i)),
                                                              Point(blockPos[blockLimit_upside - peekFilledCount - unfilledCount + 1][0], blockPos[blockLimit_upside - peekFilledCount - unfilledCount + 1][1])),
                                              nullptr));

                        randomColoring(k);

                        block[k]->runAction(Spawn::create(ScaleTo::create(0, 1, 0), ScaleTo::create(0.05, 1, 1), FadeIn::create(0.025), nullptr));
                        block3D[k]->runAction(Spawn::create(ScaleTo::create(0, 1, 0), ScaleTo::create(0.05, 1, 1), FadeIn::create(0.025), nullptr));
                        block3D[k]->setColor(Color3B::BLACK);

                        break;
                    }
                }
            }
            else {

                for (int k = 0; k < blockCount; k++) {

                    if (isAtBlockPos(k, j) == true) {
                    
                        filledCount++;

                        block[k]->runAction(MoveTo::create(float(0.1 * std::abs(blockLimit_upside - filledCount + 1 - j)),
                                                            Point(blockPos[blockLimit_upside - filledCount + 1][0], blockPos[blockLimit_upside - filledCount + 1][1])));

                        block3D[k]->runAction(MoveTo::create(float(0.1 * std::abs(blockLimit_upside - filledCount + 1 - j)),
                                                            Point(blockPos[blockLimit_upside - filledCount + 1][0], blockPos[blockLimit_upside - filledCount + 1][1])));

                        break;
                    }
                }
            }
        }

        filledCount = 0;
        unfilledCount = 0;
        peekFilledCount = 0;

        // inverse
        for (int h = blockLimit_downside; h <= pointsHeight - 1 + pointsHeight * i; h++) {

            if (blockFillTag[h] == false) peekFilledCount++;
        }

        for (int j = blockLimit_downside; j <= pointsHeight - 1 + pointsHeight * i; j++) {

            if (blockFillTag[j] == true) {

                avaPat_buf[i]++;

                for (int k = 0; k < blockCount; k++) {

                    if (isAtBlockPos(k, j) == true) {

                        unfilledCount++;

                        block[k]->runAction(Sequence::create(
                                            MoveTo::create(0, Point(blockPos[pointsHeight - 1 + pointsHeight * i][0], blockPos[pointsHeight - 1 + pointsHeight * i][1] - blockHeight / 2)),
                                            MoveTo::create(0.05, Point(blockPos[pointsHeight - 1 + pointsHeight * i][0], blockPos[pointsHeight - 1 + pointsHeight * i][1])),
                                            MoveTo::create(float(0.05 + 0.1 * std::abs((pointsHeight - 1 + pointsHeight * i) - (blockLimit_downside + peekFilledCount + unfilledCount - 1))),
                                                            Point(blockPos[blockLimit_downside + peekFilledCount + unfilledCount - 1][0], blockPos[blockLimit_downside + peekFilledCount + unfilledCount - 1][1])),
                                            nullptr));

                        block3D[k]->runAction(Sequence::create(
                                            MoveTo::create(0, Point(blockPos[pointsHeight - 1 + pointsHeight * i][0], blockPos[pointsHeight - 1 + pointsHeight * i][1] - blockHeight / 2)),
                                            MoveTo::create(0.05, Point(blockPos[pointsHeight - 1 + pointsHeight * i][0], blockPos[pointsHeight - 1 + pointsHeight * i][1])),
                                            MoveTo::create(float(0.05 + 0.1 * std::abs((pointsHeight - 1 + pointsHeight * i) - (blockLimit_downside + peekFilledCount + unfilledCount - 1))),
                                                            Point(blockPos[blockLimit_downside + peekFilledCount + unfilledCount - 1][0], blockPos[blockLimit_downside + peekFilledCount + unfilledCount - 1][1])),
                                            nullptr));

                        randomColoring(k);

                        block[k]->runAction(Spawn::create(ScaleTo::create(0, 1, 0), ScaleTo::create(0.05, 1, 1), FadeIn::create(0.025), nullptr));
                        block3D[k]->runAction(Spawn::create(ScaleTo::create(0, 1, 0), ScaleTo::create(0.05, 1, 1), FadeIn::create(0.025), nullptr));
                        block3D[k]->setColor(Color3B::BLACK);

                        break;
                    }
                }
            }
            else {

                for (int k = 0; k < blockCount; k++) {

                    if (isAtBlockPos(k, j) == true) {
                    
                        filledCount++;

                        block[k]->runAction(MoveTo::create(float(0.1 * std::abs(blockLimit_downside + filledCount - 1 - j)),
                                                            Point(blockPos[blockLimit_downside + filledCount - 1][0], blockPos[blockLimit_downside + filledCount - 1][1])));

                        block3D[k]->runAction(MoveTo::create(float(0.1 * std::abs(blockLimit_downside + filledCount - 1 - j)),
                                                            Point(blockPos[blockLimit_downside + filledCount - 1][0], blockPos[blockLimit_downside + filledCount - 1][1])));

                        break;
                    }
                }
            }
        }

        filledCount = 0;
        unfilledCount = 0;
        peekFilledCount = 0;
    }

    // searching the highest value
    for (int i = 0; i < pointsWidth; i++) {

        for (int j = 0; j < pointsWidth; j++) {

            if (avaPat_buf[i] >= avaPat_buf[j]) cmprSuit++;
            else                                cmprSuit = 0;
        }

        if (cmprSuit >= pointsWidth) availablePaths = avaPat_buf[i];
    }

    // randomize after blocks blasting
    if (holeOn == true && isFall == false && isEat == false) holesMove();
}

void BlocksMap::noColorUndermice() {

    for (int g = 0; g < blockCount; g++) {

        if (isNotMicePositionTarget(g) == false) {

            for (int h = 0; h < blockCount; h++) {

                if (isAtBlockPos(h, g) == true) {
                    
                    block[h]->setColor(Color3B::WHITE);
                    
                    block[h]->setOpacity(0);
                    block3D[h]->setOpacity(0);
                }
            }
        }
    }
}

void BlocksMap::noBlockUnderObstacle() {

    for (int g = 0; g < blockCount; g++) {

        {if (isAtObstaclePos(g) == true) blockHiding(g);}
        {if (trapOn == true) if (isAtTrapPos(g) == true) blockHiding(g);}
    }
}

void BlocksMap::returnBlockUnderObstacle() {

    for (int g = 0; g < 512; g++) {

        if (blockHiding_tag[g][0] != -1) {

            block[blockHiding_tag[g][1]]->setPosition(Point(blockPos[blockHiding_tag[g][0]][0], blockPos[blockHiding_tag[g][0]][1]));
            block3D[blockHiding_tag[g][1]]->setPosition(Point(blockPos[blockHiding_tag[g][0]][0], blockPos[blockHiding_tag[g][0]][1]));
        }
    }
}

void BlocksMap::blockHiding(int member) {

    for (int h = 0; h < blockCount; h++) {

        if (isAtBlockPos(h, member) == true) {
            
            block[h]->setPosition(Point(vSize.width * -1.5 + vOrigin.x, vSize.height / 2 + vOrigin.y));
            block3D[h]->setPosition(Point(vSize.width * -1.5 + vOrigin.x, vSize.height / 2 + vOrigin.y));

            blockHiding_tag[member][1] = h;

            break;
        }
    }
}

void BlocksMap::miceEatsCheese_movesForward()  {for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(Spawn::create(MoveTo::create(0.25, Point(plateEdgePos[0], plateEdgePos[1])), ScaleTo::create(0.25, 1.25), nullptr));}
void BlocksMap::miceEatsCheese_movesBackward() {for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(Spawn::create(MoveTo::create(0.25, Point(micePos_buf[0], micePos_buf[1])), ScaleTo::create(0.25, 1.0), nullptr));}

void BlocksMap::miceSubstitutes_generate() {

    micePos_buf[0] = mice[0]->getPosition().x;
    micePos_buf[1] = mice[0]->getPosition().y;
    miceRot_buf    = mice[0]->getRotation();

    for (int i = 0; i < miceFrames_count; i++) {mice[i]->setVisible(false); mice[i]->setPosition(Point(0, 0));}

    // mice substitute stuff
    for (int i = 1; i <= miceFrames_count; i++) {
    
        if      (i == 4) std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", 2);
        else if (i == 5) std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", 1);
        else             std::sprintf(frameName_buffer, "LEVEL/MICE/mice_%d.png", i);

        mice_substitute[i - 1] = Sprite::create(frameName_buffer);
        mice_substitute[i - 1]->setPosition(Point(micePos_buf[0], micePos_buf[1]));
        mice_substitute[i - 1]->setAnchorPoint(Vec2(1.0 - blockWidth / 2 / mice_substitute[i - 1]->getContentSize().width, 0.5));
        mice_substitute[i - 1]->setRotation(miceRot_buf);
        layer->addChild(mice_substitute[i - 1], blockCount * 2 - 1 + 1);

        {if (i != 1) mice_substitute[i - 1]->setOpacity(0);}
        {if (i >= 4) mice_substitute[i - 1]->setFlippedY(true);}
    }
}

void BlocksMap::miceTrapped_effect() {

    miceSubstitutes_generate();

    auto lastTrap = Sprite::create("LEVEL/TRAP/trapped.png");
    lastTrap->setPosition(Point(trap[trapTag][3]->getPosition().x - std::cos(trapRot[trapTag] / 180 * M_PI) * lastTrap->getContentSize().width / 2, trap[trapTag][3]->getPosition().y + std::sin(trapRot[trapTag] / 180 * M_PI) * lastTrap->getContentSize().width / 2));
    lastTrap->setRotation(trapRot[trapTag]);
    lastTrap->setOpacity(0);
    layer->addChild(lastTrap, blockCount * 2 + 1);

    trap[trapTag][0]->runAction(FadeOut::create(0));
    trap[trapTag][1]->runAction(Sequence::create(FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
    trap[trapTag][2]->runAction(Sequence::create(DelayTime::create(0.05), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
    trap[trapTag][3]->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), nullptr));

    lastTrap->runAction(Sequence::create(DelayTime::create(0.1), FadeIn::create(0), nullptr));

    if (uDef->getBoolForKey("SOUND EFFECT", true) == true) AudioEngine::play2d("sounds/mice_trapped.mp3", false, 1.0f);
}

void BlocksMap::tutorialLevel1_blocksSet() {

    block[0]->setColor(Color3B::BLUE);
    block[1]->setColor(Color3B::YELLOW);
    block[2]->setColor(Color3B::GREEN);
    block[3]->setColor(Color3B::BLUE);
    block[4]->setColor(Color3B::RED);
    block[5]->setColor(Color3B::YELLOW);
    block[6]->setColor(Color3B::RED);
    block[7]->setColor(Color3B::YELLOW);
    block[8]->setColor(Color3B::ORANGE);
    block[9]->setColor(Color3B::MAGENTA);
    block[10]->setColor(Color3B::MAGENTA);
    block[11]->setColor(Color3B::GREEN);
}

void BlocksMap::tutorialLevel1_arrowMove() {

    arrow->runAction(Sequence::create(FadeTo::create(0.25, 200), MoveBy::create(1.0, Point(0, -blockHeight)), FadeOut::create(0.25), nullptr));
    arrow->runAction(Sequence::create(DelayTime::create(1.5), MoveBy::create(0, Point(0, blockHeight)), nullptr));
}

void BlocksMap::tutorialLevel1_arrowUnmove() {arrow->setVisible(false);}

void BlocksMap::tutorialLevel1_pointingBlocks() {

    block3D[4]->runAction(Sequence::create(TintTo::create(0.4, Color3B::WHITE), DelayTime::create(0.2), TintTo::create(0.4, Color3B::BLACK), nullptr));
    block3D[5]->runAction(Sequence::create(TintTo::create(0.4, Color3B::WHITE), DelayTime::create(0.2), TintTo::create(0.4, Color3B::BLACK), nullptr));
}

void BlocksMap::tutorialLevel1_unpointingBlocks() {

    block3D[4]->setColor(Color3B::BLACK);
    block3D[5]->setColor(Color3B::BLACK);
}

int BlocksMap::getAvailablePaths() {return availablePaths;}
int BlocksMap::getFewestMoves()    {return fewestMoves;}
int BlocksMap::getBlockCount()     {return blockCount;}
int BlocksMap::get_isMiceMove()    {return isMiceMove;}

bool BlocksMap::miceEatsCheese() {

    if (mice[0]->getPosition().x >= cheese->getPosition().x - blockWidth  * 1.5 && mice[0]->getPosition().x <= cheese->getPosition().x + blockWidth  * 1.5
        && mice[0]->getPosition().y >= cheese->getPosition().y - blockHeight * 1.5 && mice[0]->getPosition().y <= cheese->getPosition().y + blockHeight * 1.5) {
        
        isEat = true;
        miceSubstitutes_generate();

        if (micePos_buf[0] < cheese->getPosition().x
            && micePos_buf[1] >= cheese->getPosition().y - blockHeight / 2 && micePos_buf[1] <= cheese->getPosition().y + blockHeight / 2) {

            plateEdgePos[0] = cheese->getPosition().x - blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 0.0));
        }
        else if (micePos_buf[0] >= cheese->getPosition().x - blockWidth / 2 && micePos_buf[0] <= cheese->getPosition().x + blockWidth / 2
                && micePos_buf[1] > cheese->getPosition().y) {
                    
            plateEdgePos[0] = cheese->getPosition().x;
            plateEdgePos[1] = cheese->getPosition().y + blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 90.0));
        }
        else if (micePos_buf[0] > cheese->getPosition().x
                && micePos_buf[1] >= cheese->getPosition().y - blockHeight / 2 && micePos_buf[1] <= cheese->getPosition().y + blockHeight / 2) {
            
            plateEdgePos[0] = cheese->getPosition().x + blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 180.0));
        }
        else if (micePos_buf[0] >= cheese->getPosition().x - blockWidth / 2 && micePos_buf[0] <= cheese->getPosition().x + blockWidth / 2
                && micePos_buf[1] < cheese->getPosition().y) {
            
            plateEdgePos[0] = cheese->getPosition().x;
            plateEdgePos[1] = cheese->getPosition().y - blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 270.0));
        }
        else if (micePos_buf[0] < cheese->getPosition().x && micePos_buf[1] > cheese->getPosition().y) {
            
            plateEdgePos[0] = cheese->getPosition().x - blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y + blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 45.0));
        }
        else if (micePos_buf[0] > cheese->getPosition().x && micePos_buf[1] > cheese->getPosition().y) {
            
            plateEdgePos[0] = cheese->getPosition().x + blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y + blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 135.0));
        }
        else if (micePos_buf[0] > cheese->getPosition().x && micePos_buf[1] < cheese->getPosition().y) {
            
            plateEdgePos[0] = cheese->getPosition().x + blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y - blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 225.0));
        }
        else if (micePos_buf[0] < cheese->getPosition().x && micePos_buf[1] < cheese->getPosition().y) {
            
            plateEdgePos[0] = cheese->getPosition().x - blockWidth / 2;
            plateEdgePos[1] = cheese->getPosition().y - blockHeight / 2;

            for (int i = 0; i < miceFrames_count; i++) mice_substitute[i]->runAction(RotateTo::create(0.1, 315.0));
        }

        cheese->runAction(ScaleTo::create(2.0, 0));

        return true;
    }
    else {

        return false;
    }
}

bool BlocksMap::miceFalls() {

    bool isDetected = false;

    {
        if (holeOn == true) {
        
            for (int i = 0; i < holesCount; i++) {

                if (mice[0]->getPosition().x >= holePos[i][0] - blockWidth / 2 && mice[0]->getPosition().x <= holePos[i][0] + blockWidth / 2
                    && mice[0]->getPosition().y >= holePos[i][1] - blockHeight / 2 && mice[0]->getPosition().y <= holePos[i][1] + blockHeight / 2) {

                    isFall = true;
                    isDetected = true;
                    miceSubstitutes_generate();

                    for (int j = 0; j < miceFrames_count; j++) {
                        
                        mice_substitute[j]->runAction(Spawn::create(RotateBy::create(1.0, 360), ScaleTo::create(1.0, 0), MoveTo::create(1.0, Point(holePos[i][0], holePos[i][1])), nullptr));
                        mice_substitute[j]->runAction(Sequence::create(DelayTime::create(0.5), FadeOut::create(0.5), nullptr));
                    }

                    break;
                }
            }
        }
    }
    
    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::miceEntersGate() {

    bool isDetected = false;

    if (gateOn == true) {

        float sinGate, cosGate;
        float enterPosX, enterPosY;
        
        for (int i = 0; i < gatesCount; i++) {

            sinGate = std::sin(gateRot[i] / 180 * M_PI);
            cosGate = std::cos(gateRot[i] / 180 * M_PI);
            enterPosX = -cosGate * blockWidth  + gatePos[i][0];
            enterPosY =  sinGate * blockHeight + gatePos[i][1];

            if (mice[0]->getPosition().x >= enterPosX - blockWidth / 2 && mice[0]->getPosition().x <= enterPosX + blockWidth / 2
                && mice[0]->getPosition().y >= enterPosY - blockHeight / 2 && mice[0]->getPosition().y <= enterPosY + blockHeight / 2) {

                int j_target;
                isDetected = true;

                for (int j = 0; j < blockCount; j++) {

                    if (isNotMicePositionTarget(j) == false) {

                        blockTag_default(true);

                        j_target = j + sinGate * 2 - cosGate * pointsHeight * 2;
                        blockTag[j_target] = true;
                        blockFillTag[j_target] = true;

                        j_target = j + sinGate - cosGate * pointsHeight;
                        blockTag[j_target] = true;

                        break;
                    }
                }

                bool isDetected_atHole = false;
                for (int k = 0; k < holesCount; k++) {
                    
                    if (gatePos[i][0] >= holePos[k][0] - blockWidth / 2 && gatePos[i][0] <= holePos[k][0] + blockWidth / 2
                        && gatePos[i][1] >= holePos[k][1] - blockHeight / 2 && gatePos[i][1] <= holePos[k][1] + blockHeight / 2) {

                        isDetected_atHole = true;
                        gateOpenClose(i, j_target, true);
                        break;
                    }
                }
                
                if (isDetected_atHole == false) gateOpenClose(i, j_target);
                
                break;
            }
        }
    }
    
    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::miceTrapped() {

    bool isDetected = false;

    {
        if (trapOn == true) {

            float sinTrap, cosTrap;
            float enterPosX, enterPosY;
        
            for (int i = 0; i < trapsCount; i++) {

                sinTrap = std::sin(trapRot[i] / 180 * M_PI);
                cosTrap = std::cos(trapRot[i] / 180 * M_PI);
                enterPosX = -cosTrap * blockWidth  + trapPos[i][0];
                enterPosY =  sinTrap * blockHeight + trapPos[i][1];

                if (mice[0]->getPosition().x >= enterPosX - blockWidth / 2 && mice[0]->getPosition().x <= enterPosX + blockWidth / 2
                    && mice[0]->getPosition().y >= enterPosY - blockHeight / 2 && mice[0]->getPosition().y <= enterPosY + blockHeight / 2) {

                    trapTag = i;
                    isTrapped = true;
                    isDetected = true;
                    blockTag_default(true);

                    for (int j = 0; j < blockCount; j++) {

                        if (trapPos[i][0] >= blockPos[j][0] - blockWidth / 2 && trapPos[i][0] <= blockPos[j][0] + blockWidth / 2
                            && trapPos[i][1] >= blockPos[j][1] - blockHeight / 2 && trapPos[i][1] <= blockPos[j][1] + blockHeight / 2) {

                            blockTag[j] = true;
                            break;
                        }
                    }

                    break;
                }
            }
        }
    }
    
    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::miceTricked() {

    bool isDetected = false;
    float trickedMice_rot;

    {
        if (fakeOn == true) {
        
            for (int i = 0; i < currentFakeCheeses; i++) {

                if (mice[0]->getPosition().x >= fakePlatePos[i][0] - blockWidth / 2 && mice[0]->getPosition().x <= fakePlatePos[i][0] + blockWidth / 2
                    && mice[0]->getPosition().y >= fakePlatePos[i][1] - blockHeight / 2 && mice[0]->getPosition().y <= fakePlatePos[i][1] + blockHeight / 2
                    && i != fakeSkipPos_tag) {

                    fakePlate[i]->setVisible(false);
                    fakeCheese[i]->setVisible(false);

                    for (int j = 1; j <= smokeFrames_count; j++) {

                        std::sprintf(frameName_buffer, "LEVEL/SMOKE/smoke_%d.png", j);

                        smoke[j - 1] = Sprite::create(frameName_buffer);
                        smoke[j - 1]->setPosition(fakeCheese[i]->getPosition());
                        smoke[j - 1]->setOpacity(0);
                        layer->addChild(smoke[j - 1], blockCount * 2 - 2 + 1);

                        smoke[j - 1]->runAction(Sequence::create(DelayTime::create(float(0.06 * (j - 1))), FadeIn::create(0), DelayTime::create(0.075), FadeOut::create(0), nullptr));
                    }

                    for (int j = 1; j <= trapFrames_count; j++) {
                        
                        std::sprintf(frameName_buffer, "LEVEL/TRAP/trap_%dd_%d.png", fakeTrapRotation[i], j);

                        fakeTrap[j - 1] = Sprite::create(frameName_buffer);
                        fakeTrap[j - 1]->setPosition(fakeCheese[i]->getPosition());
                        fakeTrap[j - 1]->setOpacity(0);

                        if (fakePlatePos[i][0] <= mapCenterPosX) {

                            if (fakePlatePos[i][1] <= mapCenterPosY) {fakeTrap[j - 1]->setRotation(-45.0); trickedMice_rot = 135.0;}
                            else                                     {fakeTrap[j - 1]->setRotation( 45.0); trickedMice_rot = 225.0;}
                        }
                        else {

                            if (fakePlatePos[i][1] <= mapCenterPosY) {fakeTrap[j - 1]->setRotation( 45.0); trickedMice_rot =  45.0;}
                            else                                     {fakeTrap[j - 1]->setRotation(-45.0); trickedMice_rot = 315.0;}
                        }
                        
                        layer->addChild(fakeTrap[j - 1], blockCount * 2 - 3 + 1);

                        if (j != trapFrames_count) fakeTrap[j - 1]->runAction(Sequence::create(DelayTime::create(float(0.05 * (j - 1) + 0.25)), FadeIn::create(0), DelayTime::create(0.05), FadeOut::create(0), nullptr));
                        else                       fakeTrap[j - 1]->runAction(Sequence::create(DelayTime::create(float(0.05 * (j - 1) + 0.25)), FadeIn::create(0), nullptr));
                    }

                    auto lastTrap = Sprite::create("LEVEL/TRAP/trapped.png");
                    lastTrap->setPosition(fakeCheese[i]->getPosition());
                    lastTrap->setOpacity(0);

                    if (fakePlatePos[i][0] <= mapCenterPosX) {

                        if (fakePlatePos[i][1] <= mapCenterPosY) lastTrap->setRotation( 135);
                        else                                     lastTrap->setRotation(-135);
                    }
                    else {

                        if (fakePlatePos[i][1] <= mapCenterPosY) lastTrap->setRotation( 45);
                        else                                     lastTrap->setRotation(-45);
                    }

                    layer->addChild(lastTrap, blockCount * 2 + 1);
                    lastTrap->runAction(Sequence::create(DelayTime::create(0.4), FadeIn::create(0), nullptr));

                    miceSubstitutes_generate();
                    for (int j = 0; j < miceFrames_count; j++) {
                        
                        mice_substitute[j]->runAction(Spawn::create(MoveTo::create(0.25, fakeCheese[i]->getPosition()), RotateTo::create(0.25, trickedMice_rot), nullptr));
                        mice_substitute[j]->runAction(Sequence::create(ScaleTo::create(0.1, 1.25), ScaleTo::create(0.1, 1.0), nullptr));
                    }

                    isDetected = true;

                    if (uDef->getBoolForKey("SOUND EFFECT", true) == true) {
                        
                        AudioEngine::play2d("sounds/explode.mp3", false, 1.0f);
                        AudioEngine::play2d("sounds/mice_trapped.mp3", false, 1.0f);
                    }

                    break;
                }
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::isAtBlockPos(int member_try, int member_target) {

    // return 'true' wheter block is at targeted block position
    if (block[member_try]->getPosition().x >= blockPos[member_target][0] - blockWidth / 2 && block[member_try]->getPosition().x <= blockPos[member_target][0] + blockWidth / 2
        && block[member_try]->getPosition().y >= blockPos[member_target][1] - blockHeight / 2 && block[member_try]->getPosition().y <= blockPos[member_target][1] + blockHeight / 2) {

        return true;
    }
    else {

        return false;
    }
}

bool BlocksMap::isNotMicePositionTarget(int member) {

    // returns 'true' wheter block is not at mice position
    if (!((blockPos[member][0] >= mice[0]->getPosition().x - blockWidth / 2 && blockPos[member][0] <= mice[0]->getPosition().x + blockWidth / 2)
        && (blockPos[member][1] >= mice[0]->getPosition().y - blockHeight / 2 && blockPos[member][1] <= mice[0]->getPosition().y + blockHeight / 2))) {
        
        return true;
    }
    else {

        blockFillTag[member] = true;
        return false;
    }
}

bool BlocksMap::isAtObstaclePos(int member) {

    bool isDetected = false;

    {
        if (obstacleOn == true) {

            // returns 'true' wheter block is at obstacle position
            for (int i = 0; i < obsCount; i++) {

                if (blockPos[member][0] >= obstaclePos[i][0] - blockWidth / 2 && blockPos[member][0] <= obstaclePos[i][0] + blockWidth / 2
                    && blockPos[member][1] >= obstaclePos[i][1] - blockHeight / 2 && blockPos[member][1] <= obstaclePos[i][1] + blockHeight / 2) {
                    
                    isDetected = true;
                    break;
                }
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::isAtGatePos(int member) {

    bool isDetected = false;

    {
        if (gateOn == true) {

            // returns 'true' wheter block is at gate position
            for (int i = 0; i < gatesCount; i++) {

                if (blockPos[member][0] >= gatePos[i][0] - blockWidth / 2 && blockPos[member][0] <= gatePos[i][0] + blockWidth / 2
                    && blockPos[member][1] >= gatePos[i][1] - blockHeight / 2 && blockPos[member][1] <= gatePos[i][1] + blockHeight / 2) {

                    isDetected = true;
                    break;
                }
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::isAtHolePos(int member) {

    bool isDetected = false;

    {
        if (holeOn == true) {

            // returns 'true' wheter block is at hole position
            for (int i = 0; i < holesCount; i++) {

                if (blockPos[member][0] >= holePos[i][0] - blockWidth / 2 && blockPos[member][0] <= holePos[i][0] + blockWidth / 2
                    && blockPos[member][1] >= holePos[i][1] - blockHeight / 2 && blockPos[member][1] <= holePos[i][1] + blockHeight / 2) {

                    isDetected = true;
                    break;
                }
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::isAtHoleTrace(int member, int holeTraceMember) {

    bool isDetected = false;

    {
        if (holeOn == true) {

            // returns 'true' wheter block is at hole trace position
            if (blockPos[member][0] >= holeTracePos_1[holeTraceMember][0] - blockWidth / 2 && blockPos[member][0] <= holeTracePos_1[holeTraceMember][0] + blockWidth / 2
                && blockPos[member][1] >= holeTracePos_1[holeTraceMember][1] - blockHeight / 2 && blockPos[member][1] <= holeTracePos_1[holeTraceMember][1] + blockHeight / 2) {

                isDetected = true;
            }
            else if (blockPos[member][0] >= holeTracePos_2[holeTraceMember][0] - blockWidth / 2 && blockPos[member][0] <= holeTracePos_2[holeTraceMember][0] + blockWidth / 2
                && blockPos[member][1] >= holeTracePos_2[holeTraceMember][1] - blockHeight / 2 && blockPos[member][1] <= holeTracePos_2[holeTraceMember][1] + blockHeight / 2) {

                isDetected = true;
            }
            else if (blockPos[member][0] >= holeTracePos_3[holeTraceMember][0] - blockWidth / 2 && blockPos[member][0] <= holeTracePos_3[holeTraceMember][0] + blockWidth / 2
                && blockPos[member][1] >= holeTracePos_3[holeTraceMember][1] - blockHeight / 2 && blockPos[member][1] <= holeTracePos_3[holeTraceMember][1] + blockHeight / 2) {

                isDetected = true;
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::isAtTrapPos(int member) {

    bool isDetected = false;

    {
        if (trapOn == true) {
        
            // returns 'true' wheter block is at obstacle position
            for (int i = 0; i < trapsCount; i++) {

                if (blockPos[member][0] >= trapPos[i][0] - blockWidth / 2 && blockPos[member][0] <= trapPos[i][0] + blockWidth / 2
                    && blockPos[member][1] >= trapPos[i][1] - blockHeight / 2 && blockPos[member][1] <= trapPos[i][1] + blockHeight / 2) {
                    
                    isDetected = true;
                    break;
                }
            }
        }
    }

    if (isDetected == true) return true;
    else                    return false;
}

bool BlocksMap::get_isBlasting() {return isBlasting;}
bool BlocksMap::get_isTrapped()  {return isTrapped;}

float BlocksMap::getBlockWidth()     {return blockWidth;}
float BlocksMap::getBlockHeight()    {return blockHeight;}
float BlocksMap::getPosX(int member) {return blockPos[member][0];}
float BlocksMap::getPosY(int member) {return blockPos[member][1];}
float BlocksMap::getMapCenterPosX()  {return mapCenterPosX;}
float BlocksMap::getMapCenterPosY()  {return mapCenterPosY;}
float BlocksMap::getMapWidth()       {return mapWidth;}
float BlocksMap::getMapHeight()      {return mapHeight;}

void BlocksMap::set_isSwiped(int isSwiped_in) {isSwiped = isSwiped_in;}
bool BlocksMap::get_isSwiped()                {return isSwiped;}


