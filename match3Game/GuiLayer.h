//
//  GuiLayer.h
//  match3Game
//
//  Created by Vlad on 28.08.13.
//
//

#ifndef __match3Game__GuiLayer__
#define __match3Game__GuiLayer__

#include "cocos2d.h"
#include "GameScene.h"

using namespace cocos2d;

class GuiLayer : public cocos2d::CCLayer
{
private:
    
    GameScene *_gameScene;
    
    CCLabelTTF *_timeLabel;
    CCLabelTTF *_scoreLabel;
    
    CCMenuItemFont *_restartBtn;
    
public:
    
    CREATE_FUNC(GuiLayer);
    
    virtual bool init();
    
    void setGameScene(GameScene *gameScene);
    
    void setRestartBtnEnable(bool state);
    
    void setScoreLabel(int score);
    void setTimeLabel(int time);
    
    void restart();
};

#endif /* defined(__match3Game__GuiLayer__) */
