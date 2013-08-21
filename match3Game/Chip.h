//
//  Chip.h
//  match3Game
//
//  Created by Vlad on 20.08.13.
//
//

#ifndef __match3Game__Chip__
#define __match3Game__Chip__

#include "cocos2d.h"

#include "Types.h"

using namespace cocos2d;

class Chip: public cocos2d::CCNode
{
private:
    CCSprite *sprite;
    
    ChipType _type;
    ChipState _state;
    
    CCPoint _gridCoords;
    
public:
    
    Chip(ChipType type);
    ~Chip();
    
    static Chip* create(ChipType type);
    
    void setType(ChipType type);
    void setGridCoords(int col, int row);
    void setGridCoords(CCPoint coords);
    
    CCPoint getGridCoords();
    
    ChipType  getType();
    ChipState getState();
    
    void die();
    
    bool update();
    
private:
    void setState(ChipState state);
    
};

#endif /* defined(__match3Game__Chip__) */
