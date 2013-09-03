//
//  MainMenu.h
//  match3Game
//
//  Created by Vlad on 03.09.13.
//
//

#ifndef __match3Game__MainMenu__
#define __match3Game__MainMenu__

#include "cocos2d.h"

using namespace cocos2d;

class Field;
class GuiLayer;

//class HelloWorld : public cocos2d::CCLayer
class MainMenu : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainMenu);
    
    void play();
    
private:
    
    
};



#endif /* defined(__match3Game__MainMenu__) */
