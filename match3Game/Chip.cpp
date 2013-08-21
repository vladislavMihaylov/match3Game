//
//  Chip.cpp
//  match3Game
//
//  Created by Vlad on 20.08.13.
//
//

#include "Chip.h"

Chip::Chip(ChipType type)
{
    
    std::ostringstream oss;
    oss << "ct_" << type << ".png";
    std::string str = oss.str();
    
    const char *name = str.c_str();
    
    sprite = CCSprite::create(name);
    this->addChild(sprite);
    
    _state = CS_Init;
    setType(type);
}

Chip::~Chip()
{
    
}

Chip* Chip::create(ChipType type)
{
    Chip *chip = new Chip::Chip(type);
    
    return chip;
}

// setters

void Chip::setType(ChipType type)
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

// getters

ChipType Chip::getType()
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

// other

void Chip::die()
{
    _state = CS_Dying;
}

bool Chip::update()
{
    switch (_state)
    {
        case CS_Init:
        {
            sprite->runAction(CCScaleTo::create(0.2, 1));
        } break;
            
        case CS_Normal:
        {
            
        } break;
            
        case CS_Dying:
        {
            sprite->runAction(CCScaleTo::create(0.2, 1));
        } break;
            
        case CS_Dead:
        {
            this->removeFromParent();
        } break;
            
        default:
            break;
    }
    
    return true;
}