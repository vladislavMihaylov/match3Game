//
//  GuiLayer.cpp
//  match3Game
//
//  Created by Vlad on 28.08.13.
//
//

#include "cocos2d.h"

using namespace cocos2d;

#include "GuiLayer.h"
#include "Constants.h"

bool GuiLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    _timeLabel = CCLabelTTF::create("Time: 60", "Arial", 18);
    _timeLabel->setPosition(ccp(0.08 * size.width, 0.9 * size.height));
    _timeLabel->setAnchorPoint(ccp(0, 0.5));
    
    _scoreLabel = CCLabelTTF::create("Score: 0", "Arial", 18);
    _scoreLabel->setPosition(ccp(0.08 * size.width, 0.8 * size.height));
    _scoreLabel->setAnchorPoint(ccp(0, 0.5));
    
    this->addChild(_timeLabel);
    this->addChild(_scoreLabel);
    
    _restartBtn = CCMenuItemFont::create("Restart!", this, menu_selector(GuiLayer::restart));
    _restartBtn->setPosition(ccp(0.08 * size.width, 0.65 * size.height));
    _restartBtn->setAnchorPoint(ccp(0, 0.5));
    _restartBtn->setEnabled(false);
    
    CCMenu *menu = CCMenu::create(_restartBtn, NULL);
    menu->setPosition(ccp(0, 0));
    
    this->addChild(menu, 2);
    
    CCLOG("Created GUI");
    
    return true;
}

void GuiLayer::restart()
{
    _gameScene->restart();
}

void GuiLayer::setGameScene(GameScene *gameScene)
{
    _gameScene = gameScene;
}

void GuiLayer::setRestartBtnEnable(bool state)
{
    _restartBtn->setEnabled(state);
}

void GuiLayer::setScoreLabel(int score)
{
    std::ostringstream oss;
    oss << "Score: " << score;
    std::string str = oss.str();
    
    const char *c_score = str.c_str();
    
    _scoreLabel->setString(c_score);
}

void GuiLayer::setTimeLabel(int time)
{
    std::ostringstream oss;
    oss << "Score: " << time;
    std::string str = oss.str();
    
    const char *c_time = str.c_str();
    
    _timeLabel->setString(c_time);
}
