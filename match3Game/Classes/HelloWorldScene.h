#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Field;

//class HelloWorld : public cocos2d::CCLayer
class GameScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(GameScene);
    
private:
    CCSprite *_back;
    
    Field *_field;
    
    bool _gameOver;
    
    CCLabelTTF *_timeLabel;
    CCLabelTTF *_scoreLabel;
    
    CCMenuItemFont *_restartBtn;
    
    int time;
    int score;
    
    
    
public:
    
    void restart();
    
    void decreaseTime(float ct);
    void applyPoints(int points);
    
    void gameOver();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
};

#endif // __HELLOWORLD_SCENE_H__
