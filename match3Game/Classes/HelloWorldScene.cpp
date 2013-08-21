#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "Chip.h"
#include "Field.h"
#include "Types.h"

#include "Constants.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    _back = CCSprite::create("back.png");
    _back->setPosition(ccp(kScreenWidth / 2, kScreenHeight / 2));
    this->addChild(_back);
        
    Field *field = Field::create();
    this->addChild(field);
    
    return true;
}

