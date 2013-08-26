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
    
    std::ostringstream oss;
    oss << "ct_" << type << ".png";
    std::string str = oss.str();
    
    const char *name = str.c_str();
    
    sprite = CCSprite::create(name);
    this->addChild(sprite);
    
    this->setContentSize(sprite->getContentSize());
    
    bonusSprite = CCSprite::create();
    this->addChild(bonusSprite);
    
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
    
    this->removeChild(sprite, true);
    
    sprite = CCSprite::create(name);
    this->addChild(sprite);
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
    
    this->removeChild(bonusSprite, true);
    if(bonus == BT_None)
    {
        bonusSprite = CCSprite::create();
    }
    else
    {
        bonusSprite = CCSprite::create(name);
    }
    this->addChild(bonusSprite,2);
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

void Chip::update(float dt)
{
    
    switch (_state)
    {
        case CS_Init:
        {
            sprite->runAction(CCScaleTo::create(0.3, 1));
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
            
            sprite->runAction(CCSequence::create(seqArr));
            
        } break;
            
        case CS_Dead:
        {
            this->removeFromParent();
        } break;
            
        default:
            break;
    }
}

void Chip::kill()
{
    setState(CS_Dead);
}