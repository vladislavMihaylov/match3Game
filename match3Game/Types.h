//
//  Types.h
//  match3Game
//
//  Created by Vlad on 20.08.13.
//
//

#ifndef match3Game_Types_h
#define match3Game_Types_h

#include <vector.h>

enum ChipType {
    CT_Red,
    CT_Green,
    CT_Blue,
    CT_Yellow,
    CT_Pink
};

enum ChipState {
    CS_Init,
    CS_Normal,
    CS_Dying,
    CS_Dead
};

struct Vector2 {
    float x, y;
    
    bool operator == (const Vector2 &r) {
        return x == r.x && y == r.y;
    }
    
    Vector2 operator +(const Vector2 &r) {
        Vector2 result;//{x + r.x, y + r.y};
        
        result.x = x + r.x;
        result.y = y + r.y;
        
        return result;
    }
    
    Vector2& operator +=(const Vector2 &r) {
        x = x + r.x;
        y = y + r.y;
        
        return *this;
    }
    
    Vector2 operator -() const {
        Vector2 result = *this;
        result.x = -x;
        result.y = -y;
        
        return result;
    }
    
    Vector2& operator -=(const Vector2 &r) {
        *this += -r;
        return *this;
    }
    };

typedef vector<Vector2> Vec2Collection;

#endif
