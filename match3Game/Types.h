//
//  Types.h
//  match3Game
//
//  Created by Vlad on 20.08.13.
//
//

#ifndef match3Game_Types_h
#define match3Game_Types_h

//#include <vector.h>

#include <vector>
#include <string>
#include <map>

using namespace std;

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

inline float cut(float num, float min, float max)
{
    if(num < min)
    {
        num = min;
    } else if(num > max)
    {
        num = max;
    }
    return num;
}

struct Vector2 {
    float x, y;
    
    bool operator == (const Vector2 &r) {
        return x == r.x && y == r.y;
    }
    
    Vector2 operator +(const Vector2 &r) {
        Vector2 result{x + r.x, y + r.y};
        
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
    
//#define cpp(x, y) (Vector2{(Float)x, (Float)y})

#endif
