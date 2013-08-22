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
    
    this->setTouchEnabled(true);

    _back = CCSprite::create("back.png");
    _back->setPosition(ccp(kScreenWidth / 2, kScreenHeight / 2));
    this->addChild(_back);
        
    _field = Field::create();
    this->addChild(_field);
    
    return true;
}

void GameScene::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    CCPoint touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);
    
    if(!_gameOver)
    {
    
    if((touchLocation.x > kFieldBasePoint.x && touchLocation.x < (kFieldBasePoint.x + kFieldAreaWidth)) &&
       touchLocation.y > kFieldBasePoint.y && touchLocation.y < (kFieldBasePoint.y + kFieldAreaHeight))
    {
        _field->touchOnPos(touchLocation.x, touchLocation.y);
    }
    
    }
    return true;
}