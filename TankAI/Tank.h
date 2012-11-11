//
//  Tank.h
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#ifndef TankAICommon_Tank_h
#define TankAICommon_Tank_h

#include <list>


class Projectile;


class Tank
{
public:
    Tank(char _x, char _y);
    
    int UUID;
    int HitPointsRemaining;
    char x, y;
    
    std::list<Projectile*>  Projectiles;
    
private:
    Tank();
    Tank(const Tank&);
    Tank& operator=(const Tank&);
    
    static int sUUID;
};


#endif
