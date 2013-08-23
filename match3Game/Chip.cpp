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
    
    _coords = CCLabelTTF::create("1,1", "Arial", 16);
    _coords->setColor(ccc3(0, 0, 0));
    //this->addChild(_coords,2);
    
    this->scheduleUpdate();
}

Chip::~Chip()
{
    
}

void Chip::updateLabel(int x, int y)
{
    std::ostringstream oss;
    oss << x <<"," << y;
    std::string str = oss.str();
    
    const char *string = str.c_str();
    
    //_coords->setString(string);
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

void Chip::update(float dt)
{
    switch(_state) {
            
        case CS_Init: {
            sprite->setScale(sprite->getScale() + 5 * dt);
            if(sprite->getScale() >= 1.0) {
                sprite->setScale(1.0);
                _state = CS_Normal;
            }
        } break;
        case CS_Normal: {
            
        } break;
        case CS_Dying: {
            
            setScale(getScale() - 10 * dt);
            
            if(getScale() <= 0) {
                setScale(0);
                setState(CS_Dead);
            }
            
        } break;
        case CS_Dead: {
            this->removeFromParent();
        } break;
    }
    
    /*witch (_state)
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
            
            //CCAction *call = CCCallFunc::create(this, callfunc_selector(Chip::kill()));
            
            CCArray *seqArr = CCArray::create(scaleAction,  NULL);
            
            sprite->runAction(CCSequence::create(seqArr));
            
            //kill();
        } break;
            
        case CS_Dead:
        {
            this->removeFromParent();
        } break;
            
        default:
            break;
    }*/
}

void Chip::kill()
{
    setState(CS_Dead);
}