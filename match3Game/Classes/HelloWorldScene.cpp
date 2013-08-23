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
    
    time = 10;//kGameSessionTime;
    
    _timeLabel = CCLabelTTF::create("Time: 60", "Arial", 36);
    _timeLabel->setPosition(ccp(100, 700));
    
    _scoreLabel = CCLabelTTF::create("Score: 0", "Arial", 36);
    _scoreLabel->setPosition(ccp(40, 650));
    _scoreLabel->setAnchorPoint(ccp(0, 0.5));
    
    this->addChild(_timeLabel, 2);
    this->addChild(_scoreLabel, 2);

    _back = CCSprite::create("back.png");
    _back->setPosition(ccp(kScreenWidth / 2, kScreenHeight / 2));
    this->addChild(_back);
        
    _field = Field::create();
    _field->setGameDelegate(this);
    this->addChild(_field);
    
    _restartBtn = CCMenuItemFont::create("Restart!", this, menu_selector(GameScene::restart));
    _restartBtn->setPosition(ccp(100, 350));
    _restartBtn->setEnabled(false);
    
    CCMenu *menu = CCMenu::create(_restartBtn, NULL);
    menu->setPosition(ccp(0, 0));
    
    this->addChild(menu, 2);
    
    this->schedule(schedule_selector(GameScene::decreaseTime), 1);
    
    return true;
}

void GameScene::restart()
{
    this->setTouchEnabled(true);
    
    _restartBtn->setEnabled(false);
    
    time = 10;//kGameSessionTime;
    _timeLabel->setString("Time: 60");
    score = 0;
    _scoreLabel->setString("Score: 0");
    
    
    _field->shuffle();
    
    _gameOver = false;
}

void GameScene::decreaseTime(float ct)
{
    if(time > 0)
    {
        time--;
        
        std::ostringstream oss;
        oss << "Time: " << time;
        std::string str = oss.str();
        
        const char *c_time = str.c_str();
        
        _timeLabel->setString(c_time);
    }
    else
    {
        this->setTouchEnabled(false);
        
        _restartBtn->setEnabled(true);
    }
}

void GameScene::applyPoints(int points)
{
    score += points;
    
    std::ostringstream oss;
    oss << "Score: " << score;
    std::string str = oss.str();
    
    const char *c_score = str.c_str();
    
    _scoreLabel->setString(c_score);
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
           touchLocation.y < kFieldBasePoint.y && touchLocation.y > (kFieldBasePoint.y - kFieldAreaHeight))
        {
            _field->touchOnPos(touchLocation.x, touchLocation.y);
        }
    
    }
    else
    {
        _field->shuffle();
    }
    
    return true;
}