//
//  Tank.cpp
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include "TankAICommon/TankAIGame.h"

#include "Tank.h"


int Tank::sUUID = kUUIDMaskTank;


Tank::Tank(char _x, char _y)
{
    UUID = sUUID++;
        
    HitPointsRemaining = kStartingHitPoints;
        
    x = _x;
    y = _y;
}

