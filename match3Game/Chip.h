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

class Chip: public cocos2d::CCSprite
{
private:
    
    //CCSprite *_sprite;
    CCSprite *_bonusSprite;
    
    ChipColor _type;
    ChipState _state;
    ChipBonus _bonus;
    
    CCPoint _gridCoords;
    
public:
    
    Chip(ChipColor type);
    ~Chip();
    
    static Chip* create(ChipColor type);
    
    void setType(ChipColor type);
    void setGridCoords(int col, int row);
    void setGridCoords(CCPoint coords);
    void setBonus(ChipBonus bonus);
    
    CCPoint getGridCoords();
    
    ChipColor getType();
    ChipState getState();
    ChipBonus getBonus();
    
    void die();
    void kill();
    
     void update(float dt);
    
private:
    void setState(ChipState state);
    
};

#endif /* defined(__match3Game__Chip__) */
