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
    
    ChipColor _color;
    ChipState _state;
    ChipType _type;
    
    CCPoint _gridCoords;
    
public:
    
    CREATE_FUNC(Chip);
    
    virtual bool init();
    
    void setColor(ChipColor color);
    void setGridCoords(int col, int row);
    void setGridCoords(CCPoint coords);
    void setType(ChipType type);
    
    CCPoint getGridCoords();
    
    ChipColor whichColor();
    ChipState getState();
    ChipType getType();
    
    void die();
    void kill();
    
     void update(float dt);
    
private:
    void setState(ChipState state);
    
};

#endif /* defined(__match3Game__Chip__) */
