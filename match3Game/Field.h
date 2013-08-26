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

class Field: public cocos2d::CCLayer
{
private:
    
    vector<Chip *> _chipVectorForBonuses;
    
    ChipVector _chips;
    
    Chip *_firstChip;
    
    bool _isSwapping;
    bool _isDropping;
    bool _isGameOver;
    
    GameScene *_game;
    CCSprite *_chipSelection;
    
    CCSize _chipSize;
    
    float _fieldAreaWidth;
    float _fieldAreaHeight;
    
    
public:
    Field();
    ~Field();
    
    static Field* create();
    
    void setUpGrid();
    
    void setGameOver(bool isGameOver);
    void setGameDelegate(GameScene *game);
    
    float getFieldAreaWidth();
    float getFieldAreaHeight();
    
    bool addInBonusesVector(Chip *chip);
    
    Chip* addChip(int col, int row);
    Chip* getChipAt(int col, int row);
    Chip* getChipAtXandY(int x, int y);
    
    void update(float dt);
    void moveChips(float dt);
    
    void clear(bool bruteKill = true);
    void shuffle();
    
    void swap(Chip *a, Chip *b);
    
    void touchOnPos(int x, int y);
    
private:
    
    ChipMatrix getMatchesIfAny();
    
    bool isItPossibleToPlay();
    
    void removeMatchesIfAny();
    void displaceChips(Chip *base);
    void addNewChips();
    
    bool doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne);
    
};



#endif /* defined(__match3Game__Field__) */
