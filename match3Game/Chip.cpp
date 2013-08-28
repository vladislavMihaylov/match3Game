//
//  Chip.cpp
//  match3Game
//
//  Created by Vlad on 20.08.13.
//
//

#include "Chip.h"

Chip::Chip(ChipColor type)
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("chipsSprites.plist");
    
    std::ostringstream oss;
    oss << "ct_" << type << ".png";
    std::string str = oss.str();
    
    const char *name = str.c_str();
    
    
    CCSprite::initWithSpriteFrameName(name);
    
    
    //_sprite = CCSprite::create(name);
    //this->addChild(_sprite);
    
    //this->setContentSize(_sprite->getContentSize());
    
    //_bonusSprite = CCSprite::create();
    //this->addChild(_bonusSprite);
    
    setState(CS_Init);
    setType(type);
    setBonus(BT_None);
    
    this->scheduleUpdate();
}

Chip::~Chip()
{
    
}

Chip* Chip::create(ChipColor type)
{
    Chip *chip = new Chip::Chip(type);
    
    return chip;
}

// setters

void Chip::setType(ChipColor type)
{
    _type = type;
    
    std::ostringstream oss;
    oss << "ct_" << type << ".png";
    std::string str = oss.str();
    
    const char *name = str.c_str();
    
    CCSprite::initWithFile(name);
    
    //_sprite = CCSprite::create(name);
    //this->addChild(_sprite);
}

void Chip::setState(ChipState state)
{
    _state = state;
}

void Chip::setGridCoords(int col, int row)
{
    _gridCoords = ccp(col, row);
}

void Chip::setGridCoords(CCPoint coords)
{
    _gridCoords = coords;
}

void Chip::setBonus(ChipBonus bonus)
{
    _bonus = bonus;
    
    std::ostringstream oss;
    oss << "b_" << bonus << ".png";
    std::string str = oss.str();
    
    const char *name = str.c_str();
    
    this->removeChild(_bonusSprite, true);
    if(bonus == BT_None)
    {
        _bonusSprite = CCSprite::create();
    }
    else
    {
        _bonusSprite = CCSprite::create(name);
        _bonusSprite->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
    }
    this->addChild(_bonusSprite,2);
}

// getters

ChipColor Chip::getType()
{
    return _type;
}

ChipState Chip::getState()
{
    return _state;
}

CCPoint Chip::getGridCoords()
{
    return _gridCoords;
}

ChipBonus Chip::getBonus()
{
    return _bonus;
}

// other

void Chip::die()
{
    _state = CS_Dying;
}

void Chip::kill()
{
    setState(CS_Dead);
}

void Chip::update(float dt)
{
    
    switch (_state)
    {
        case CS_Init:
        {
            this->runAction(CCScaleTo::create(0.3, 1));
            _state = CS_Normal;
        } break;
            
        case CS_Normal:
        {
            
        } break;
            
        case CS_Dying:
        {
            CCAction *scaleAction = CCScaleTo::create(0.05, 0);
            
            CCAction *call = CCCallFunc::create(this, callfunc_selector(Chip::kill));
            
            CCArray *seqArr = CCArray::create(scaleAction, call, NULL);
            
            this->runAction(CCSequence::create(seqArr));
            
        } break;
            
        case CS_Dead:
        {
            this->removeFromParent();
        } break;
            
        default:
          break;
    }
}

