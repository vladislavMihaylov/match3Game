//
//  Field.cpp
//  match3Game
//
//  Created by Vlad on 21.08.13.
//
//

#include "Field.h"

#include "Constants.h"
#include "HelloWorldScene.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

Field::~Field()
{

}

Field::Field()
{
    
    _isSwapping = false;
    _isDropping = false;
    
    _isGameOver = false;
    
    _chipSelection = nullptr;
    _firstChip = nullptr;
    _game = nullptr;
    
    this->removeAllChildren();
    
    _chipSelection = CCSprite::create("selection.png");
    _chipSelection->setVisible(false);
    
    this->addChild(_chipSelection, 1);
    
    
    _chips.resize(kFieldWidth * kFieldHeight);
    
    setUpGrid();
    
    _fieldAreaWidth = (kFieldWidth * _chipSize.width + (kFieldWidth - 1) * kChipSpacing);
    _fieldAreaHeight = (kFieldHeight * _chipSize.height + (kFieldWidth - 1) * kChipSpacing);
    
    this->scheduleUpdate();
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
        
        if(!getMatchesIfAny().empty()) {
            clear();
            continue;
        }
        
        if(!isItPossibleToPlay()) {
            clear();
            continue;
        }
        
        break;
    }
}

float Field::getFieldAreaWidth()
{
    return _fieldAreaWidth;
}

float Field::getFieldAreaHeight()
{
    return _fieldAreaHeight;
}



void Field::setGameOver(bool isGameOver)
{
    _isGameOver = isGameOver;
}



Chip* Field::addChip(int col, int row)
{
    Chip *chip = Chip::create(CC_Red);
    
    chip->setGridCoords(col, row);
    
    _chipSize = chip->getContentSize();

    chip->setPosition(ccp(col * (kChipSpacing + _chipSize.width) + _chipSize.width / 2, row * -(kChipSpacing + _chipSize.height) - _chipSize.height / 2));

    chip->setType(static_cast<ChipColor>(rand() % kNumOfChipTypes));
    
    _chips[row * kFieldWidth + col] = chip;
    
    this->addChild(chip);
    
    return chip;
    
}

void Field::clear(bool bruteKill)
{
    for(Chip *chip: _chips)
    {
        if(bruteKill)
        {
            this->removeChild(chip, bruteKill);
        }
        else
        {
            chip->die();
        }
    }
    
    _chips.clear();
    _chips.resize(kFieldWidth * kFieldHeight);
}

void Field::shuffle()
{
    clear(false);
    setUpGrid();
}

Chip* Field::getChipAt(int col, int row)
{
    return _chips[row * kFieldWidth + col];
}

Chip* Field::getChipAtXandY(int x, int y)
{
    x -= this->getPosition().x;
    y = this->getPosition().y - y;
    
    int col = (x + kChipSpacing) / (_chipSize.width + kChipSpacing);
    int row = (y + kChipSpacing) / (_chipSize.height + kChipSpacing);
    
    return getChipAt(col, row);
}

bool Field::isItPossibleToPlay()
{
    for(int row = 0; row < kFieldHeight; ++row)
    {
        for(int col = 0; col < kFieldWidth; ++col)
        {
            if(doesChipPatternMatch(col, row, {{1, 0}}, {{-2, 0}, {-1, -1}, {-1, 1}, {2, -1}, {2, 1}, {3, 0}}))
            {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{2, 0}}, {{1, -1}, {1, 1}}))
            {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{0, 1}}, {{0, -2}, {-1, -1}, {1, -1}, {-1, 2}, {1, 2}, {0, 3}}))
            {
                return true;
            }
            
            if(doesChipPatternMatch(col, row, {{0, 2}}, {{-1, 1}, {1, 1}}))
            {
                return true;
            }
        }
    }
    
    return false;
}

bool Field::doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne)
{
    auto typeMatcher = [&](int col, int row, ChipColor type) -> bool
    {
        if(col < 0 || col > (kFieldWidth - 1) || row < 0 || row > (kFieldHeight - 1))
        {
            return false;
        }
        
        return getChipAt(col, row)->getType() == type;
    };
    
    int numOfMustHave = static_cast<int>(mustHave.size());
    Chip *currentChip = getChipAt(col, row);
    
    if(currentChip == nullptr)
    {
        return false;
    }
    
    ChipColor currentColor = currentChip->getType();
    
    for(int i = 0; i < numOfMustHave; ++i)
    {
        if(!typeMatcher(col + mustHave[i].x, row + mustHave[i].y, currentColor))
        {
            return false;
        }
    }
    
    for(int i = 0; i < needOne.size(); ++i)
    {
        if(typeMatcher(col + needOne[i].x, row + needOne[i].y, currentColor))
        {
            return true;
        }
    }
    
    return false;
}

ChipMatrix Field::getMatchesIfAny()
{
    ChipMatrix resultingMatch;
    
    auto checkRows = [&](int row, int col) -> ChipVector
    {
        Chip *chipToCheck = getChipAt(col, row);
        
        ChipVector result;
        result.push_back(chipToCheck);
        
        if(chipToCheck == nullptr)
        {
            return result;
        }
        
        //todo: add nullptr-chip check
        for(int i = 1; col + i < kFieldWidth; ++i)
        {
            Chip *currentChip = getChipAt(col + i, row);
            
            if(currentChip && chipToCheck->getType() == currentChip->getType())
            {
                result.push_back(currentChip);
                
            }
            else
            {
                return result;
            }
        }
        
        return result;
    };
    
    auto checkColumns = [&](int row, int col) -> ChipVector {
        Chip *chipToCheck = getChipAt(col, row);
        
        ChipVector result;
        result.push_back(chipToCheck);
        
        if(chipToCheck == nullptr)
        {
            return result;
        }
        
        for(int i = 1; row + i < kFieldHeight; ++i)
        {
            Chip *currentChip = getChipAt(col, row + i);
            
            if(currentChip && chipToCheck->getType() == currentChip->getType())
            {
                result.push_back(currentChip);
                
            }
            else
            {
                return result;
            }
        }
        
        return result;
    };
    
    for(int row = 0; row < kFieldHeight; ++row)
    {
        //assume our match is invalid in case it consists of less than 3 elements
        //we should skip 2 last rows since they can not
        //make a match of more than 2 elements
        for(int col = 0; col < kFieldWidth - 2; ++col)
        {
            auto match = checkRows(row, col);
            if(match.size() > 2)
            {
                resultingMatch.push_back(match);
                
                col += (match.size() - 1);
            }
        }
    }
    
    for(int col = 0; col < kFieldWidth; ++col)
    {
        for(int row = 0; row < kFieldHeight - 2; ++row)
        {
            auto match = checkColumns(row, col);
            if(match.size() > 2)
            {
               
                resultingMatch.push_back(match);
                row += (match.size() - 1);
            }
        }
    }
    
    return resultingMatch;
}

void Field::setGameDelegate(GameScene *game)
{
    _game = game;
}

bool Field::addInBonusesVector(Chip *curChip)
{
    bool isHaveBonus = false;
    
    if(curChip->getBonus() != BT_None)
    {
        isHaveBonus = true;
        
        if(curChip->getBonus() == BT_Horizontal)
        {
            CCLOG("Horizontal");
            
            curChip->setBonus(BT_None);
            
            for(int z = 0; z < kFieldWidth; z++)
            {
                Chip *chipForAdding = getChipAt(z, curChip->getGridCoords().y);
                
                _chipVectorForBonuses.push_back(chipForAdding);
            }
        }
        if(curChip->getBonus() == BT_Vertical)
        {
            CCLOG("Vertical");
            
            curChip->setBonus(BT_None);
            
            for(int z = 0; z < kFieldHeight; z++)
            {
                Chip *chipForAdding = getChipAt(curChip->getGridCoords().x, z);
                
                _chipVectorForBonuses.push_back(chipForAdding);
            }
        }
        if(curChip->getBonus() == BT_Cross)
        {
            CCLOG("Cross");
            
            curChip->setBonus(BT_None);
            
            for(int z = 0; z < kFieldWidth; z++)
            {
                Chip *chipForAdding = getChipAt(z, curChip->getGridCoords().y);
                
                _chipVectorForBonuses.push_back(chipForAdding);
            }
            for(int z = 0; z < kFieldHeight; z++)
            {
                Chip *chipForAdding = getChipAt(curChip->getGridCoords().x, z);
                
                _chipVectorForBonuses.push_back(chipForAdding);
            }
        }
    }

    
    return isHaveBonus;
}

void Field::removeMatchesIfAny()
{
    auto matches = getMatchesIfAny();
    
    //////////
    
    int matchSize = matches.size();
    
    
    if(matchSize > 0)
    {
        for(int i = 0; i < matchSize; i++)
        {
            vector<Chip *> curVector = matches[i];
            
            int curVectorSize = curVector.size();
            
            for(int k = 0; k < curVectorSize; k++)
            {
                Chip *curChip = curVector[k];
                
                addInBonusesVector(curChip);
            }
        }
    }
    
    bool isHaveBonus = true;
    
    //matches.push_back(chipVectorForDie);
    while(isHaveBonus)
    {
        int vectorForBonusesSize = _chipVectorForBonuses.size();
        
        isHaveBonus = false;
        
        for(int i = 0; i < vectorForBonusesSize; i++)
        {
            Chip *curChip = _chipVectorForBonuses[i];
            
            isHaveBonus = addInBonusesVector(curChip);
        }
    }
    
    matches.push_back(_chipVectorForBonuses);
    
    _chipVectorForBonuses.clear();
    ///////////
    
    int numOfMatches = static_cast<int>(matches.size());
    
    //vector<Chip *> chipsForDie;

    
    for(int i = 0; i < numOfMatches; ++i)
    {
        int points = kScorePerChip * ((int)matches[i].size() - 1); //-1 ?
        for(int j = 0; j < matches[i].size(); ++j) {
            
            Chip *chip = matches[i][j];
            auto chipIt = find(_chips.begin(), _chips.end(), chip);
            
            if(chipIt != _chips.end()) {
                //show scores label and apply fade int/scale effect to it
                //these points should be applied once only by the way
                _game->applyPoints(points);
                
                SimpleAudioEngine::sharedEngine()->playEffect("chipBreak.wav");
                
                
                
                _chips[chip->getGridCoords().y * kFieldWidth + chip->getGridCoords().x] = nullptr;
                
                displaceChips(chip);
                //would be nice to add this chip to a special list and
                //apply some basic scale/ fade out effect maybe
                //and use removeChild(chip, false) instead
                //this->removeChild(chip);
                chip->die();
                
                //SoundManager::mngr()->playEffect("chipBreak.wav");
            }
        }
    }
    
    addNewChips();
    
    if(matches.empty()) {
        if(!isItPossibleToPlay()) {
            //onGameOver();
            shuffle();
        }
    }
}

void Field::displaceChips(Chip *base)
{
    int baseCol = base->getGridCoords().x;
    int baseRow = base->getGridCoords().y;
    
    for(int row = baseRow - 1; row >= 0; row--)
    {
        if(getChipAt(baseCol, row) != nullptr)
        {
            Chip *currentChip = _chips[row * kFieldWidth + baseCol];
            
            currentChip->setGridCoords(currentChip->getGridCoords().x, currentChip->getGridCoords().y + 1);
            
            // -> newww currentChip->setGridCoords(currentChip->getGridCoords().x, currentChip->getGridCoords().y + 1);
            _chips[(row + 1) * kFieldWidth + baseCol] = currentChip;
            // -> newww_chips[(row - 1) * kFieldWidth + baseCol] = currentChip;
            _chips[row * kFieldWidth + baseCol] = nullptr;
        }
    }
}

void Field::addNewChips() {
    for(int col = 0; col < kFieldWidth; ++col) {
        int numOfMissingChips = 0;
        for(int row = kFieldHeight - 1; row >= 0; row--) {
            if(getChipAt(col, row) == nullptr) {
                
                Chip *newChip = addChip(col, row);
                //row * (kChipSpacing + kChipHeight) + kChipHeight / 2
                newChip->setPosition(ccp(newChip->getPosition().x, (numOfMissingChips++ * (kChipSpacing + kChipHeight) + kChipHeight / 2)));
                
                int willBeBonus = random()%10;
                
                if(willBeBonus == 5) // чтобы бонус выпадал довольно таки редко
                {
                    newChip->setBonus(static_cast<ChipBonus>(random() % kNumOfBonusTypes));
                }
                
                newChip->setVisible(false);
                newChip->setScale(0);
                newChip->runAction(CCScaleTo::create(0.2, 1));
                // -> newww newChip->setPosition(ccp(newChip->getPosition().x, -(numOfMissingChips++ * (kChipSpacing + kChipHeight) + kChipHeight / 2)));
                
                //CCLOG("newChip.position: %f, %f", newChip->getPosition().x, newChip->getPosition().y);
                
                _isDropping = true;
            }
        }
    }
}

void Field::swap(Chip *a, Chip *b) {
    auto swapper = [&](Chip *_1, Chip *_2) {
        CCPoint tmpCoords = _1->getGridCoords();
        
        _1->setGridCoords(_2->getGridCoords());
        _2->setGridCoords(tmpCoords);
        
        _chips[_1->getGridCoords().y * kFieldWidth + _1->getGridCoords().x] = _1;
        _chips[_2->getGridCoords().y * kFieldWidth + _2->getGridCoords().x] = _2;
    };
    
    swapper(a, b);
    
    if(getMatchesIfAny().empty())
    {
        //swap(b, a) ?
        swapper(a, b);
    }
    else
    {
        _isSwapping = true;
    }
}

void Field::touchOnPos(int x, int y)
{
    Chip *chip = getChipAtXandY(x, y);
    
    if(chip == nullptr)
    {
        return;
    }
    
    //SoundManager::mngr()->playEffect("btnClick.wav");
    
    //this is the first time we select a chip
    if(_firstChip == nullptr)
    {
        _chipSelection->setVisible(true);
        _chipSelection->setPosition(chip->getPosition());
        _firstChip = chip;
    }
    else if(_firstChip == chip)
    {
        //deselect a previously selected chip
        _chipSelection->setVisible(false);
        _firstChip = nullptr;
    }
    else
    {
        //we selected another chip: it is possible it's a neighbour chip
        _chipSelection->setVisible(false);
        
        CCPoint firstCoords = _firstChip->getGridCoords();
        CCPoint secondCoords = chip->getGridCoords();
        //check if this is the same row and neighbour column
        if(firstCoords.y == secondCoords.y && fabs(firstCoords.x - secondCoords.x) == 1)
        {
            swap(_firstChip, chip);
            _firstChip = nullptr;
        }
        else if(firstCoords.x == secondCoords.x && fabs(firstCoords.y - secondCoords.y) == 1)
        {
            swap(_firstChip, chip);
            _firstChip = nullptr;
        }
        else
        {
            //this is a distant chip, so select it and deselect the previous one
            _firstChip = chip;
            _chipSelection->setPosition(chip->getPosition());
            _chipSelection->setVisible(true);
        }
    }

}

void Field::update(float dt)
{
    moveChips(dt);
}

void Field::moveChips(float dt)
{
    //dt should be kept in mind!
    bool moved = false;
    
    for(int i = 0; i < kFieldHeight; ++i) {
        for(int j = 0; j < kFieldWidth; ++j) {
            
            Chip *chip = _chips[i * kFieldWidth + j];
            if(chip != nullptr) {
                
                CCPoint currentPos = chip->getPosition();
                CCPoint requiredPos = ccp(chip->getGridCoords().x * (kChipSpacing + _chipSize.width) + _chipSize.width / 2,
                                         chip->getGridCoords().y * -(kChipSpacing + _chipSize.height) - _chipSize.height / 2);
                
                //CCLOG("CurrentPos.y: %f", chip->getPosition().y);
                
                if(chip->getPosition().y < 0)
                {
                    chip->setVisible(true);
                }
                
                //move to the bottom
                if(currentPos.y < requiredPos.y)
                {
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(chip->getPosition().x, cut(chip->getPosition().y + kSpeedOfMove * dt, currentPos.y, requiredPos.y));
                    
                    //CCLOG("currentPos: %f, %f requiredPos: %f, %f", currentPos.x, currentPos.y, requiredPos.x, requiredPos.y);
                    
                    //chip->setRotation(20);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.y > requiredPos.y)
                {
                    //move to the top
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(chip->getPosition().x, cut(chip->getPosition().y - kSpeedOfMove * dt, requiredPos.y, currentPos.y));
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.x < requiredPos.x)
                {
                    //move to the right
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(cut(chip->getPosition().x + kSpeedOfMove * dt, currentPos.x, requiredPos.x), chip->getPosition().y);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.x > requiredPos.x)
                {
                    //move to the left
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(cut(chip->getPosition().x - kSpeedOfMove * dt, requiredPos.x, currentPos.x), chip->getPosition().y);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                }
                
                
            }
        }
    }
    
    if(!_isGameOver)
    {
        if(moved)
        {
            _game->setTouchEnabled(false);
        }
        else
        {
            _game->setTouchEnabled(true);
        }
    }
    
    if(_isGameOver)
    {
        _game->setTouchEnabled(false);
    }
    
    if(_isDropping && !moved)
    {
        _isDropping = false;
        removeMatchesIfAny();
    }
    else if(_isSwapping && !moved)
    {
        _isSwapping = false;
        removeMatchesIfAny();
    }
}
