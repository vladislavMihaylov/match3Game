//
//  match3GameAppDelegate.cpp
//  match3Game
//
//  Created by Vlad on 20.08.13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainMenu.h"

#include "../Platform/CCFileUtils.h"

#include "Config.h"

//#include <vector>

USING_NS_CC;
//using namespace std;
using namespace CocosDenshion;

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource iPhoneResource      =  { cocos2d::CCSizeMake(480, 320),   "iphone" };
static Resource iPhoneHDResource    =  { cocos2d::CCSizeMake(960, 640),   "iphonehd" };
static Resource iPhone5Resource     =  { cocos2d::CCSizeMake(1136, 640),  "iphone5" };
static Resource iPadResource        =  { cocos2d::CCSizeMake(1024, 768),  "ipad"   };
static Resource iPadHDResource      =  { cocos2d::CCSizeMake(2048, 1536), "ipadhd" };

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    //pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEGLView);
    
    ////////
    
    std::vector<std::string>searchPath;
    
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    
    CCSize frameSize = pEGLView->getFrameSize();

    
    CCLOG("width: %f", frameSize.width);
    
        if (frameSize.width == iPhone5Resource.size.width)
            {
                    pEGLView->setDesignResolutionSize(1136, 640, kResolutionNoBorder);
                
                    searchPath.push_back(iPhone5Resource.directory);
                    //pDirector->setContentScaleFactor(iPhone5Resource.size.width/designResolutionSize.width);
            }
        else if (frameSize.width > iPadResource.size.width)
            {
                    searchPath.push_back(iPadHDResource.directory);
                    pDirector->setContentScaleFactor(iPadHDResource.size.width/designResolutionSize.width);
            }
        else if (frameSize.width > iPhoneHDResource.size.width)
            {
                    searchPath.push_back(iPadResource.directory);
                    pDirector->setContentScaleFactor(iPadResource.size.width/designResolutionSize.width);
            }
        else if (frameSize.width > iPhoneResource.size.width)
            {
                    searchPath.push_back(iPhoneHDResource.directory);
                    pDirector->setContentScaleFactor(iPhoneHDResource.size.width/designResolutionSize.width);
            }
        else
            {
                    searchPath.push_back(iPhoneResource.directory);
                    pDirector->setContentScaleFactor(iPhoneResource.size.width/designResolutionSize.width);
            }
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    
    //float scale = pDirector->getContentScaleFactor();
    
    //CCLOG("scale: %f", scale);
    
    ////////

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = MainMenu::scene();

    // run
    pDirector->runWithScene(pScene);
    
    
    
    
    

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
