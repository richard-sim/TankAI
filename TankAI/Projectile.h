//
//  Projectile.h
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#ifndef TankAICommon_Projectile_h
#define TankAICommon_Projectile_h


class Tank;

class Projectile
{
public:
    Projectile(Tank* _shooter, char _dx, char _dy);
    
    int UUID;
    char x, y;
    char dx, dy;
    int RemainingDistance;
    
    Tank* Shooter;
    
private:
    Projectile();
    Projectile(const Projectile&);
    Projectile& operator=(const Projectile&);
    
    static int sUUID;
};


#endif
