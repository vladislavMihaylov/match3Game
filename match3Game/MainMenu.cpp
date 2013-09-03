//
//  MainMenu.cpp
//  match3Game
//
//  Created by Vlad on 03.09.13.
//
//

#include "MainMenu.h"
#include "GameScene.h"

using namespace cocos2d;

CCScene* MainMenu::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainMenu *layer = MainMenu::create();
    
    
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool MainMenu::init() {
    
    if ( !CCLayer::init() ) {
        return false;
    }
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *bg = CCSprite::create("mainMenuBg.png");
    bg->setPosition(ccp(size.width/2, size.height/2));
    
    this->addChild(bg);
    
    CCMenuItemImage *playBtn = CCMenuItemImage::create("playBtn.png", "playBtnOn.png", this, menu_selector(MainMenu::play));
    playBtn->setPosition(ccp(size.width/2, size.height*0.65));
    
    CCMenuItemImage *shopBtn = CCMenuItemImage::create("shopBtn.png", "shopBtnOn.png");
    shopBtn->setPosition(ccp(size.width/2, size.height*0.35));
    
    CCMenu *menu = CCMenu::create(playBtn, shopBtn, NULL);
    menu->setPosition(CCPointZero);
    
    this->addChild(menu);
    
    return true;
}

void MainMenu::play()
{
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}