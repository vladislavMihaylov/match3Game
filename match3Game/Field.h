//
//  Field.h
//  match3Game
//
//  Created by Vlad on 21.08.13.
//
//

#ifndef __match3Game__Field__
#define __match3Game__Field__

#include "cocos2d.h"

#include "Chip.h"

#include <vector>

using namespace cocos2d;
using namespace std;

typedef vector<Chip *> ChipVector;
typedef vector<ChipVector> ChipMatrix;

class GameScene;

class Field: public cocos2d::CCNode
{
private:
    ChipVector _chips;
    
    Chip *_firstChip;
    
    bool _isSwapping;
    bool _isDropping;
    
    GameScene *_game;
    CCSprite *_chipSelection;
    
public:
    Field();
    ~Field();
    
    static Field* create();
    void setUpGrid();
    
    Chip* addChip(int col, int row);
    Chip* getChipAt(int col, int row);
    Chip* getChipAtXandY(int x, int y);
    
    
    //void visit();
    //void render();
private:
    
    bool isItPossibleToPlay();
    
    bool doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne);
    
};



#endif /* defined(__match3Game__Field__) */
