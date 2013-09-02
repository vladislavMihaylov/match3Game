//
//  Field.cpp
//  match3Game
//
//  Created by Vlad on 21.08.13.
//
//

#include "Field.h"

#include "Config.h"
#include "GameScene.h"

#include "SimpleAudioEngine.h"


using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

extern bool isHorizontalEnable;
extern bool isVerticalEnable;
extern bool isCrossEnable;
extern bool isTimeEnable;
extern bool isRainbowEnable;

bool Field::init() {
    if ( !CCLayer::init() ) {
        return false;
    }
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chipsSprites.plist");
    
    _batchNode = CCSpriteBatchNode::create("chipsSprites.png");
    
    this->addChild(_batchNode);
    
    _isSwapping = false;
    _isDropping = false;
    
    _isGameOver = false;
    
    _chipSelection = nullptr;
    _firstChip = nullptr;
    _game = nullptr;
    
    _chipSelection = CCSprite::createWithSpriteFrameName("selection.png");
    _chipSelection->setVisible(false);
    
    this->addChild(_chipSelection, 1);
    
    _chips.resize(kFieldWidth * kFieldHeight);
    
    setUpGrid();
    
    _fieldAreaWidth = (kFieldWidth * _chipSize.width + (kFieldWidth - 1) * kChipSpacing);
    _fieldAreaHeight = (kFieldHeight * _chipSize.height + (kFieldWidth - 1) * kChipSpacing);
    
    this->scheduleUpdate();

    CCLOG("IsHor: %i IsVer: %i IsCross: %i IsTime: %i", isHorizontalEnable, isVerticalEnable, isCrossEnable, isTimeEnable);

    return true;
}

void Field::setUpGrid() {
    while(1) {
        for(int i = 0; i < kFieldHeight; ++i) {
            for(int j = 0; j < kFieldWidth; ++j) {
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

float Field::getFieldAreaWidth() {
    return _fieldAreaWidth;
}

float Field::getFieldAreaHeight() {
    return _fieldAreaHeight;
}

void Field::setGameOver(bool isGameOver) {
    _isGameOver = isGameOver;
}

Chip* Field::addChip(int col, int row) {
    Chip *chip = Chip::create();//(CC_Red);
    
    chip->setGridCoords(col, row);
    
    _chipSize = chip->getContentSize();

    chip->setPosition(ccp(col * (kChipSpacing + _chipSize.width) + _chipSize.width / 2, row * -(kChipSpacing + _chipSize.height) - _chipSize.height / 2));

    chip->setColor(static_cast<ChipColor>(rand() % kNumOfChipTypes));
    
    _chips[row * kFieldWidth + col] = chip;
    
    //this->addChild(chip);
    
    _batchNode->addChild(chip);
    
    return chip;
    
}

void Field::clear(bool bruteKill) {
    for(Chip *chip: _chips) {
        if(bruteKill) {
            //this->removeChild(chip, bruteKill);
            _batchNode->removeChild(chip, bruteKill);
            
            
        } else {
            chip->die();
        }
    }
    
    _chips.clear();
    _chips.resize(kFieldWidth * kFieldHeight);
}

void Field::shuffle() {
    clear(false);
    setUpGrid();
}

Chip* Field::getChipAt(int col, int row) {
    return _chips[row * kFieldWidth + col];
}

Chip* Field::getChipAtXandY(int x, int y) {
    x -= this->getPosition().x;
    y = this->getPosition().y - y;
    
    int col = (x + kChipSpacing) / (_chipSize.width + kChipSpacing);
    int row = (y + kChipSpacing) / (_chipSize.height + kChipSpacing);
    
    return getChipAt(col, row);
}

bool Field::isItPossibleToPlay() {
    for(int row = 0; row < kFieldHeight; ++row) {
        for(int col = 0; col < kFieldWidth; ++col) {
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

bool Field::doesChipPatternMatch(int col, int row, Vec2Collection mustHave, Vec2Collection needOne) {
    auto typeMatcher = [&](int col, int row, ChipColor color) -> bool {
        if(col < 0 || col > (kFieldWidth - 1) || row < 0 || row > (kFieldHeight - 1)) {
            return false;
        }
        
        return getChipAt(col, row)->whichColor() == color;
    };
    
    int numOfMustHave = static_cast<int>(mustHave.size());
    Chip *currentChip = getChipAt(col, row);
    
    if(currentChip == nullptr) {
        return false;
    }
    
    ChipColor currentColor = currentChip->whichColor();
    
    for(int i = 0; i < numOfMustHave; ++i) {
        if(!typeMatcher(col + mustHave[i].x, row + mustHave[i].y, currentColor)) {
            return false;
        }
    }
    
    for(int i = 0; i < needOne.size(); ++i) {
        if(typeMatcher(col + needOne[i].x, row + needOne[i].y, currentColor)) {
            return true;
        }
    }
    
    return false;
}

ChipMatrix Field::getMatchesIfAny() {
    ChipMatrix resultingMatch;
    
    auto checkRows = [&](int row, int col) -> ChipVector {
        Chip *chipToCheck = getChipAt(col, row);
        
        ChipVector result;
        result.push_back(chipToCheck);
        
        if(_firstChip && (_firstChip->whichColor() == CC_Rainbow || _secondChip->whichColor() == CC_Rainbow))
        {
        
            int curCol = col;
            
            while(chipToCheck->whichColor() == CC_Rainbow) {
                
                curCol += 1;
                
                chipToCheck = getChipAt(curCol, row);
                result.push_back(chipToCheck);
            }
        }
        
        
        if(chipToCheck == nullptr) {
            return result;
        }
        
        //todo: add nullptr-chip check
        for(int i = 1; col + i < kFieldWidth; ++i) {
            Chip *currentChip = getChipAt(col + i, row);
            
            if(_firstChip && (_firstChip->whichColor() == CC_Rainbow || _secondChip->whichColor() == CC_Rainbow))
            {
                if(currentChip != chipToCheck) {
                    if((currentChip && chipToCheck->whichColor() == currentChip->whichColor()) || currentChip->whichColor() == CC_Rainbow) {
                        result.push_back(currentChip);
                        
                    } else {
                        return result;
                    }
                }
            }
            else
            {
                if((currentChip && chipToCheck->whichColor() == currentChip->whichColor())) {
                    result.push_back(currentChip);
                } else {
                    return result;
                }
            }
            
        }
        
        return result;
    };
    
    
    //////----/////
    
    auto checkColumns = [&](int row, int col) -> ChipVector {
        Chip *chipToCheck = getChipAt(col, row);
        
        ChipVector result;
        result.push_back(chipToCheck);
        
        if(_firstChip && (_firstChip->whichColor() == CC_Rainbow || _secondChip->whichColor() == CC_Rainbow))
        {
            int curRow = row;
                
            while(chipToCheck->whichColor() == CC_Rainbow) {
                
                curRow += 1;
                
                chipToCheck = getChipAt(col, curRow);
                result.push_back(chipToCheck);
            }
        }
        
        
        if(chipToCheck == nullptr) {
            return result;
        }
        
        for(int i = 1; row + i < kFieldHeight; ++i) {
            Chip *currentChip = getChipAt(col, row + i);

            if(_firstChip && (_firstChip->whichColor() == CC_Rainbow || _secondChip->whichColor() == CC_Rainbow))
            {
                if(currentChip != chipToCheck) {
                    if((currentChip && chipToCheck->whichColor() == currentChip->whichColor()) || currentChip->whichColor() == CC_Rainbow) {
                        result.push_back(currentChip);
                    } else {
                        return result;
                    }
                }
            }
            else
            {
                if((currentChip && chipToCheck->whichColor() == currentChip->whichColor())) {
                    result.push_back(currentChip);
                } else {
                    return result;
                }
            }
        }
        
        return result;
    };
    
    
    
    ////////------////////
    
    for(int row = 0; row < kFieldHeight; ++row) {
        //assume our match is invalid in case it consists of less than 3 elements
        //we should skip 2 last rows since they can not
        //make a match of more than 2 elements
        for(int col = 0; col < kFieldWidth - 2; ++col) {
            auto match = checkRows(row, col);
            if(match.size() > 2) {
                resultingMatch.push_back(match);
                col += (match.size() - 1);
            }
        }
    }
    
    for(int col = 0; col < kFieldWidth; ++col) {
        for(int row = 0; row < kFieldHeight - 2; ++row) {
            auto match = checkColumns(row, col);
            if(match.size() > 2) {
                resultingMatch.push_back(match);
                row += (match.size() - 1);
            }
        }
    }
    
    return resultingMatch;
}

void Field::setGameDelegate(GameScene *game) {
    _game = game;
}

void Field::removeMatchesIfAny() {
    auto matches = getMatchesIfAny();
    
    _firstChip = nullptr;
    _secondChip = nullptr;
        
    int numOfMatches = static_cast<int>(matches.size());
    
    for(int i = 0; i < numOfMatches; ++i) {
        int points = kScorePerChip * ((int)matches[i].size() - 1); //-1 ?
        for(int j = 0; j < matches[i].size(); ++j) {
            
            Chip *chip = matches[i][j];
            auto chipIt = find(_chips.begin(), _chips.end(), chip);
            
            if(chipIt != _chips.end()) {
                //show scores label and apply fade int/scale effect to it
                //these points should be applied once only by the way
                _game->applyPoints(points);
                
                SimpleAudioEngine::sharedEngine()->playEffect("chipBreak.wav");
                
                destroyChip(chip);
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

void Field::destroyChip(Chip *chip)
{
    
    switch (chip->getType())
    {
        case BT_Time:
        {
            chip->setType(BT_None);
            _game->applyBonusTime();
            
            _chips[chip->getGridCoords().y * kFieldWidth + chip->getGridCoords().x] = nullptr;
            displaceChips(chip);
            chip->die();
            
        } break;
        case BT_Horizontal:
        {
            chip->setType(BT_None);
            
            for(int z = 0; z < kFieldWidth; z++) {
                Chip *chipForAdding = getChipAt(z, chip->getGridCoords().y);
                
                if(chipForAdding != nullptr)
                {
                    if(chipForAdding->getType() != BT_None)
                    {
                        destroyChip(chipForAdding);
                    }
                    else
                    {
                        _chips[chipForAdding->getGridCoords().y * kFieldWidth + chipForAdding->getGridCoords().x] = nullptr;
                        displaceChips(chipForAdding);
                        chipForAdding->die();
                    }
                }
            }
            
            
        } break;
            
        case BT_Vertical:
        {
            
            chip->setType(BT_None);
            
            for(int z = 0; z < kFieldHeight; z++) {
                Chip *chipForAdding = getChipAt(chip->getGridCoords().x, z);
                
                if(chipForAdding != nullptr)
                {
                    if(chipForAdding->getType() != BT_None)
                    {
                        destroyChip(chipForAdding);
                    }
                    else
                    {
                        _chips[chipForAdding->getGridCoords().y * kFieldWidth + chipForAdding->getGridCoords().x] = nullptr;
                        displaceChips(chipForAdding);
                        chipForAdding->die();
                    }
                }
            }
            
            
        } break;
            
        case BT_Cross:
        {
            chip->setType(BT_None);
            
            for(int z = 0; z < kFieldWidth; z++) {
                Chip *chipForAdding = getChipAt(z, chip->getGridCoords().y);
                
                if(chipForAdding != nullptr)
                {
                    if(chipForAdding->getType() != BT_None)
                    {
                        destroyChip(chipForAdding);
                    }
                    else
                    {
                        _chips[chipForAdding->getGridCoords().y * kFieldWidth + chipForAdding->getGridCoords().x] = nullptr;
                        displaceChips(chipForAdding);
                        chipForAdding->die();
                    }
                }
            }
            for(int z = 0; z < kFieldHeight; z++) {
                Chip *chipForAdding = getChipAt(chip->getGridCoords().x, z);
                
                if(chipForAdding != nullptr)
                {
                    if(chipForAdding->getType() != BT_None)
                    {
                        destroyChip(chipForAdding);
                    }
                    else
                    {
                        _chips[chipForAdding->getGridCoords().y * kFieldWidth + chipForAdding->getGridCoords().x] = nullptr;
                        displaceChips(chipForAdding);
                        chipForAdding->die();
                    }
                }
            }
            
        } break;
       
            
        default:
            _chips[chip->getGridCoords().y * kFieldWidth + chip->getGridCoords().x] = nullptr;
            displaceChips(chip);
            chip->die();
            break;
    }
    
    
}

void Field::destroyExtraChips(ChipVector extraChips)
{
    int countOfChips = extraChips.size();
    
    for(int i = 0; i < countOfChips; i++)
    {
        Chip *chip = extraChips[i];
        
        _chips[chip->getGridCoords().y * kFieldWidth + chip->getGridCoords().x] = nullptr;
        displaceChips(chip);
        chip->die();
    }
    
    extraChips.clear();
}

void Field::displaceChips(Chip *base)
{
    int baseCol = base->getGridCoords().x;
    int baseRow = base->getGridCoords().y;
    
    for(int row = baseRow - 1; row >= 0; row--) {
        if(getChipAt(baseCol, row) != nullptr) {
            Chip *currentChip = _chips[row * kFieldWidth + baseCol];
            
            currentChip->setGridCoords(currentChip->getGridCoords().x, currentChip->getGridCoords().y + 1);
            
            _chips[(row + 1) * kFieldWidth + baseCol] = currentChip;
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
                newChip->setPosition(ccp(newChip->getPosition().x, (numOfMissingChips++ * (kChipSpacing + kChipHeight) + kChipHeight / 2)));
                
                int willBeBonus = random()%kMaxNumForRandom;
                
                if(willBeBonus == kIsBonus) { // чтобы бонус выпадал довольно таки редко
                    
                    ChipType bonusNum = static_cast<ChipType>(random() % kNumOfBonusTypes);
                    
                    switch (bonusNum)
                    {
                        case BT_None:
                        {
                            newChip->setType(bonusNum);
                        } break;
                            
                        case BT_Horizontal:
                        {
                            if(isHorizontalEnable)
                            {
                                newChip->setType(bonusNum);
                            }
                        }
                        case BT_Vertical:
                        {
                            if(isVerticalEnable)
                            {
                                newChip->setType(bonusNum);
                            }
                        }
                        case BT_Cross:
                        {
                            if(isCrossEnable)
                            {
                                newChip->setType(bonusNum);
                            }
                        }
                        case BT_Time:
                        {
                            if(isTimeEnable)
                            {
                                newChip->setType(bonusNum);
                            }
                        }
                            
                        default:
                            break;
                    }
                    
                }
                
                int willBeRainbow = random()%kMaxNumForRainbow;
                
                if(willBeRainbow == kIsBonus) {
                    if(isRainbowEnable)
                    {
                        newChip->setColor(CC_Rainbow);
                    }
                }
                
                newChip->setVisible(false);
                newChip->setScale(0);
                newChip->runAction(CCScaleTo::create(0.2, 1));
                
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
    
    //swapper(a, b);

    
    if(getMatchesIfAny().empty()) {
        //swap(b, a) ?
        swapper(a, b);
        _firstChip = nullptr;
        _secondChip = nullptr;
        
    } else {
        _isSwapping = true;
    }
    
    
}

CCSize Field::getSizeOfChip()
{
    CCSize chipSize;
    
    if(_firstChip)
    {
        chipSize = _firstChip->getContentSize();
    }
    
    return chipSize;
}



void Field::touchOnPos(int x, int y) {
    Chip *chip = getChipAtXandY(x, y);
    
    if(chip == nullptr) {
        return;
    }
    
    //SoundManager::mngr()->playEffect("btnClick.wav");
    
    //this is the first time we select a chip
    if(_firstChip == nullptr) {
        _chipSelection->setVisible(true);
        _chipSelection->setPosition(chip->getPosition());
        _firstChip = chip;
    } else if(_firstChip == chip) {
        //deselect a previously selected chip
        _chipSelection->setVisible(false);
        _firstChip = nullptr;
    } else {
        //we selected another chip: it is possible it's a neighbour chip
        _chipSelection->setVisible(false);
        
        _secondChip = chip;
        
        CCPoint firstCoords = _firstChip->getGridCoords();
        CCPoint secondCoords = _secondChip->getGridCoords();
        //check if this is the same row and neighbour column
        if(firstCoords.y == secondCoords.y && fabs(firstCoords.x - secondCoords.x) == 1) {
            
            CCPoint tmpCoords = _firstChip->getGridCoords();
            
            _firstChip->setGridCoords(_secondChip->getGridCoords());
            _secondChip->setGridCoords(tmpCoords);
            
            _chips[_firstChip->getGridCoords().y * kFieldWidth + _firstChip->getGridCoords().x] = _firstChip;
            _chips[_secondChip->getGridCoords().y * kFieldWidth + _secondChip->getGridCoords().x] = _secondChip;
            
            CCAction *delayAction = CCDelayTime::create(0.2);
            
            CCAction *call = CCCallFunc::create(this, callfunc_selector(Field::swapAfterTouch));
            
            CCArray *seqArr = CCArray::create(delayAction, call, NULL);
            
            this->runAction(CCSequence::create(seqArr));
            
            //swap(_firstChip, _secondChip);
            //_firstChip = nullptr;
            //_secondChip = nullptr;
        } else if (firstCoords.x == secondCoords.x && fabs(firstCoords.y - secondCoords.y) == 1) {
           
            
            
            CCPoint tmpCoords = _firstChip->getGridCoords();
            
            _firstChip->setGridCoords(_secondChip->getGridCoords());
            _secondChip->setGridCoords(tmpCoords);
            
            _chips[_firstChip->getGridCoords().y * kFieldWidth + _firstChip->getGridCoords().x] = _firstChip;
            _chips[_secondChip->getGridCoords().y * kFieldWidth + _secondChip->getGridCoords().x] = _secondChip;
            
            CCAction *delayAction = CCDelayTime::create(0.2);
            
            CCAction *call = CCCallFunc::create(this, callfunc_selector(Field::swapAfterTouch));
            
            CCArray *seqArr = CCArray::create(delayAction, call, NULL);
            
            this->runAction(CCSequence::create(seqArr));
            
            //swap(_firstChip, _secondChip);
            //_firstChip = nullptr;
            //_secondChip = nullptr;
        } else {
            //this is a distant chip, so select it and deselect the previous one
            _firstChip = chip;
            _chipSelection->setPosition(chip->getPosition());
            _chipSelection->setVisible(true);
            
            _secondChip = nullptr;
        }
    }

}

void Field::swapAfterTouch()
{
    swap(_firstChip, _secondChip);
    
    
}

void Field::update(float dt) {
    if(!_game->getIsGameOver()) {
        moveChips(dt);
    }
}

void Field::moveChips(float dt) {
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
                
                if(chip->getPosition().y < 0) {
                    chip->setVisible(true);
                }
                
                //move to the bottom
                if(currentPos.y < requiredPos.y) {
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(chip->getPosition().x, cut(chip->getPosition().y + kSpeedOfMove * dt, currentPos.y, requiredPos.y));
                    
                    //CCLOG("currentPos: %f, %f requiredPos: %f, %f", currentPos.x, currentPos.y, requiredPos.x, requiredPos.y);
                    
                    //chip->setRotation(20);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.y > requiredPos.y) {
                    //move to the top
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(chip->getPosition().x, cut(chip->getPosition().y - kSpeedOfMove * dt, requiredPos.y, currentPos.y));
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.x < requiredPos.x) {
                    //move to the right
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(cut(chip->getPosition().x + kSpeedOfMove * dt, currentPos.x, requiredPos.x), chip->getPosition().y);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                } else if(currentPos.x > requiredPos.x) {
                    //move to the left
                    //use gravity instead of constant speed
                    CCPoint pos = ccp(cut(chip->getPosition().x - kSpeedOfMove * dt, requiredPos.x, currentPos.x), chip->getPosition().y);
                    
                    chip->setPosition(pos); //apply speed here
                    
                    moved = true;
                }
            }
        }
    }
    
    if(!_isGameOver) {
        if(moved) {
            _game->setCanTouch(false);
        } else {
            _game->setCanTouch(true);
        }
    }
    
    if(_isGameOver) {
        _game->setCanTouch(false);
    }
    
    if(_isDropping && !moved) {
        _isDropping = false;
        removeMatchesIfAny();
    }
    else if(_isSwapping && !moved) {
        _isSwapping = false;
        removeMatchesIfAny();
    }
}
