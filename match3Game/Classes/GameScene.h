#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class Field;
class GuiLayer;

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
    
    GuiLayer *_gui;
    
    CCSprite *_back;
    
    Field *_field;
    
    bool _isGameOver;
    bool _canTouch;
    
    int _time;
    int _score;
    
    float _screenWidth;
    float _screenHeight;
    
    CCPoint _touchPosition;
    
    
public:
    
    void setUpBonuses();
    
    void applyBonusTime();
    
    void setGui(GuiLayer *gui);
    
    void setGameOver(bool isGameOver);
    void setCanTouch(bool isCanTouch);
    
    bool getReadyForInput();
    bool getIsGameOver();
    
    void decreaseTime(float ct);
    void applyPoints(int points);
    
    void restart();
    
    void registerWithTouchDispatcher();
    bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    void ccTouchMoved(CCTouch *touch, CCEvent *event);
};

#endif // __HELLOWORLD_SCENE_H__
