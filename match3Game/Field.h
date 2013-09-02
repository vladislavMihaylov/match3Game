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
    
    //ChipVector _chipVectorForBonuses;
    
    ChipVector extraChips;
    
    ChipVector _chips;
    
    Chip *_firstChip;
    Chip *_secondChip;
    
    bool _isSwapping;
    bool _isDropping;
    bool _isGameOver;
    
    GameScene *_game;
    CCSprite *_chipSelection;
    
    CCSize _chipSize;
    
    float _fieldAreaWidth;
    float _fieldAreaHeight;
    
    CCSpriteBatchNode *_batchNode;
    
    
public:
    
    CREATE_FUNC(Field);
    
    virtual bool init();
    
    void swapAfterTouch();
    
    void setUpGrid();
    
    void setGameOver(bool isGameOver);
    void setGameDelegate(GameScene *game);
    
    float getFieldAreaWidth();
    float getFieldAreaHeight();
    
    Chip* addChip(int col, int row);
    Chip* getChipAt(int col, int row);
    Chip* getChipAtXandY(int x, int y);
    
    void update(float dt);
    void moveChips(float dt);
    
    void clear(bool bruteKill = true);
    void shuffle();
    
    void swap(Chip *a, Chip *b);
    
    void touchOnPos(int x, int y);
    
    CCSize getSizeOfChip();
    
    void destroyChip(Chip *chip);
    
    void destroyExtraChips(ChipVector extraChips);
    
private:
    
    ChipMatrix getMatchesIfAny();
    
    bool isItPossibleToPlay();
    
    void removeMatchesIfAny();
    void displaceChips(Chip *base);
    void addNewChips();
    
    bool doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne);
    
};



#endif /* defined(__match3Game__Field__) */
