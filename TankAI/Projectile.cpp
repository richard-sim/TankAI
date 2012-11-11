//
//  Projectile.cpp
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include "TankAICommon/TankAIGame.h"

#include "Projectile.h"
#include "Tank.h"


int Projectile::sUUID = kUUIDMaskProjectile;


Projectile::Projectile(Tank* _shooter, char _dx, char _dy)
{
    UUID = sUUID++;
    
    x = _shooter->x + _dx;
    y = _shooter->y + _dy;
    
    dx = _dx;
    dy = _dy;
    RemainingDistance = kProjectileRange - 1;   // -1 as always start by moving by dx/dy
    
    Shooter = _shooter;
}

