//
//  Field.cpp
//  match3Game
//
//  Created by Vlad on 21.08.13.
//
//

#include "Field.h"

#include "Constants.h"

Field::~Field()
{
    
}

Field::Field()
{
    _isSwapping = false;
    _isDropping = false;
    
    _chipSelection = NULL;
    _firstChip = NULL;
    _game = NULL;
    
    this->removeAllChildren();
    
    _chipSelection = CCSprite::create("selection.png");
    _chipSelection->setVisible(true);
    
    this->addChild(_chipSelection, 1);
    this->setPosition(kFieldBasePoint);
    
    _chips.resize(kFieldWidth * kFieldHeight);
    
    setUpGrid();
}

Field* Field::create()
{
    Field *field = new Field();
    
    return field;
}

void Field::setUpGrid()
{
    while(1)
    {
        for(int i = 0; i < kFieldHeight; ++i)
        {
            for(int j = 0; j < kFieldWidth; ++j)
            {
                addChip(i, j);
            }
        }
        
        break;
    }
}

Chip* Field::addChip(int col, int row)
{
    Chip *chip = Chip::create(CT_Red);
    
    chip->setGridCoords(col, row);
    chip->setPosition(ccp(col * (kChipSpacing + kChipWidth) + kChipWidth / 2, row * (kChipSpacing + kChipHeight) + kChipHeight / 2));
    
    chip->setType(static_cast<ChipType>(rand() % kNumOfChipTypes));
    
    _chips[row * kFieldWidth + col] = chip;
    
    this->addChild(chip);
    
    return chip;
    
}

Chip* Field::getChipAt(int col, int row)
{
    return _chips[row * kFieldWidth + col];
}

Chip* Field::getChipAtXandY(int x, int y)
{
    x -= kFieldBasePoint.x;
    y -= kFieldBasePoint.y;
    
    int col = (x + kChipSpacing) / (kChipWidth + kChipSpacing);
    int row = (y + kChipSpacing) / (kChipHeight + kChipSpacing);
    
    return getChipAt(col, row);
}

bool Field::isItPossibleToPlay()
{
    for(int row = 0; row < kFieldHeight; ++row)
    {
        for(int col = 0; col < kFieldWidth; ++col)
        {
            if(doesChipPatternMatch(col, row, {{1, 0}}, {{-2, 0}, {-1, -1}, {-1, 1}, {2, -1}, {2, 1}, {3, 0}})) {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{2, 0}}, {{1, -1}, {1, 1}})) {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{0, 1}}, {{0, -2}, {-1, -1}, {1, -1}, {-1, 2}, {1, 2}, {0, 3}})) {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{0, 2}}, {{-1, 1}, {1, 1}})) {
                return true;
            }
        }
    }
    
    return false;
}

bool Field::doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne)
{
    auto typeMatcher = [&](int col, int row, ChipType type) -> bool
    {
        if(col < 0 || col > (kFieldWidth - 1) || row < 0 || row > (kFieldHeight - 1))
        {
            return false;
        }
        
        return getChipAt(col, row)->getType() == type;
    };
    
    int numOfMustHave = static_cast<int>(mustHave.size());
    Chip *currentChip = getChipAt(col, row);
    
    if(currentChip == NULL)
    {
        return false;
    }
    
    ChipType currentType = currentChip->getType();
    
    for(int i = 0; i < numOfMustHave; ++i)
    {
        if(!typeMatcher(col + mustHave[i].x, row + mustHave[i].y, currentType))
        {
            return false;
        }
    }
    
    for(int i = 0; i < needOne.size(); ++i)
    {
        if(typeMatcher(col + needOne[i].x, row + needOne[i].y, currentType))
        {
            return true;
        }
    }
    
    return false;
}