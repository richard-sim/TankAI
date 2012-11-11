//
//  BattleMap.h
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#ifndef TankAICommon_BattleMap_h
#define TankAICommon_BattleMap_h


#include <list>

#include "TankAICommon/TankAIGame.h"


class Tank;
class Projectile;
struct Command;


class BattleMap
{
public:
    BattleMap();
    ~BattleMap();
    
    int GetTimeRemaining() const;
    
    GameState Update(const Command& tank0Command, const Command& tank1Command);
    
private:
    Projectile* AddProjectile(Tank* shooter, char dx, char dy);
    
    void Ping(int tank);
    void CheckForDeaths(Projectile** ppProjectileTank0, Projectile** ppProjectileTank1);
    void MoveProjectiles();
    void CheckProjectileDeaths();
    
private:
    Tank*                   Tanks[2];
    std::list<Projectile*>  Projectiles;
    
    int                     TimeRemaining;
};


#endif
