#include "GameScene.h"
#include "SimpleAudioEngine.h"

#include "Chip.h"
#include "Field.h"
#include "Types.h"

#include "Constants.h"
#include "SimpleAudioEngine.h"

#include "GuiLayer.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    
    
    GuiLayer *gui = GuiLayer::create();
    
    gui->setGameScene(layer);
    layer->setGui(gui);
    
    scene->addChild(layer);
    scene->addChild(gui);
    
    

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
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    _screenWidth = size.width;
    _screenHeight = size.height;
    CCLOG("width: %f height: %f", _screenWidth, _screenHeight);
    
    this->setTouchEnabled(true);
    
    _time = kGameSessionTime;
    
    _back = CCSprite::create("back.png");
    
    _back->setPosition(ccp(_screenWidth / 2, _screenHeight / 2));
    this->addChild(_back);
        
    _field = Field::create();
    _field->setGameDelegate(this);
    _field->setPosition(ccp(0.345 * _screenWidth, 0.96 * _screenHeight));
    this->addChild(_field);
    
    
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("md-1.mp3", true);
    
    this->schedule(schedule_selector(GameScene::decreaseTime), 1);
    
    
    return true;
}

void GameScene::setGui(GuiLayer *gui)
{
    _gui = gui;
}

void GameScene::restart()
{
    this->setTouchEnabled(true);
    
    _gui->setRestartBtnEnable(false);
    
    _time = kGameSessionTime;
    _score = 0;
    
    _gui->setTimeLabel(_time);
    applyPoints(_score);
    
    _field->setGameOver(false);
    
    _field->shuffle();
    
    this->schedule(schedule_selector(GameScene::decreaseTime), 1);
}

void GameScene::decreaseTime(float ct)
{
    if(_time > 0)
    {
        _time--;
        
        if(_time < 10)
        {
            SimpleAudioEngine::sharedEngine()->playEffect("clock.wav");
        }
        
        _gui->setTimeLabel(_time);
    }
    else
    {
        this->unschedule(schedule_selector(GameScene::decreaseTime));
        
        SimpleAudioEngine::sharedEngine()->playEffect("lvlComplete.wav");
        
        this->setTouchEnabled(false);
        
        _field->setGameOver(true);
        
        _gui->setRestartBtnEnable(true);
    }
}

void GameScene::applyPoints(int points)
{
    _score += points;
    
    _gui->setScoreLabel(_score);
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
    
    
    if((touchLocation.x > _field->getPosition().x && touchLocation.x < (_field->getPosition().x + _field->getFieldAreaWidth())) &&
       touchLocation.y < _field->getPosition().y && touchLocation.y > (_field->getPosition().y - _field->getFieldAreaHeight()))
    {
        _field->touchOnPos(touchLocation.x, touchLocation.y);
    }
    
    return true;
}