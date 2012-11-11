//
//  BattleMap.cpp
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include <iostream>

#include "TankAICommon/TankAIGame.h"
#include "TankAICommon/Command.h"

#include "BattleMap.h"
#include "Tank.h"
#include "Projectile.h"


BattleMap::BattleMap()
{
    Tank* t0 = new Tank(-63, -63);
    Tanks[0] = t0;
    
    Tank* t1 = new Tank( 63,  63);
    Tanks[1] = t1;
    
    TimeRemaining = kGameLength;
}

BattleMap::~BattleMap()
{
    delete Tanks[0];
    delete Tanks[1];
    
    std::list<Projectile*>::iterator i = Projectiles.begin();
    for (; i!=Projectiles.end(); /**/)
    {
        delete *i;
        i = Projectiles.erase(i);
    }
}

int BattleMap::GetTimeRemaining() const
{
    return TimeRemaining;
}

GameState BattleMap::Update(const Command& tank0Command, const Command& tank1Command)
{
    Projectile *p0, *p1;
    
    /*if (tank0Command.action == ACTION_Ping)
     Ping(0);
     if (tank1Command.action == ACTION_Ping)
     Ping(1);*/
    
    // Move tanks
    if (tank0Command.action == ACTION_Move)
    {
        Tanks[0]->x += tank0Command.arg0;
        Tanks[0]->y += tank0Command.arg1;
    }
    if (tank1Command.action == ACTION_Move)
    {
        Tanks[1]->x += tank1Command.arg0;
        Tanks[1]->y += tank1Command.arg1;
    }
    
    // Move projectiles 1st unit
    MoveProjectiles();
    CheckForDeaths(&p0, &p1);
    if (p0 && p1)
        return STATE_TieBothDead;
    else if (p0)
        return STATE_Tank0Dead;
    else if (p1)
        return STATE_Tank1Dead;
    
    // Check for projectile deaths
    CheckProjectileDeaths();
    
    // Spawn projectiles (will move 1 unit to start, to get away from the shooter)
    if (tank0Command.action == ACTION_Shoot)
    {
        AddProjectile(Tanks[0], tank0Command.arg0, tank0Command.arg1);
    }
    if (tank1Command.action == ACTION_Shoot)
    {
        AddProjectile(Tanks[1], tank1Command.arg0, tank1Command.arg1);
    }
    CheckForDeaths(&p0, &p1);
    if (p0 && p1)
        return STATE_TieBothDead;
    else if (p0)
        return STATE_Tank0Dead;
    else if (p1)
        return STATE_Tank1Dead;
    
    // Move projectiles 2nd unit
    MoveProjectiles();
    CheckForDeaths(&p0, &p1);
    if (p0 && p1)
        return STATE_TieBothDead;
    else if (p0)
        return STATE_Tank0Dead;
    else if (p1)
        return STATE_Tank1Dead;
    
    // Check for projectile deaths
    CheckProjectileDeaths();
    
    //
    TimeRemaining--;
    if (TimeRemaining <= 0)
        return STATE_TieTimeOver;
    
    return STATE_Continue;
}

Projectile* BattleMap::AddProjectile(Tank* shooter, char dx, char dy)
{
    if (shooter->Projectiles.size() >= kMaxNumProjectilesPerTank)
        return NULL;
    
    Projectile* p = new Projectile(shooter, dx, dy);
    shooter->Projectiles.push_back(p);
    Projectiles.push_back(p);
    
    return p;
}

void BattleMap::Ping(int tank)
{
    Tank* t = Tanks[tank];
    Tank* other = Tanks[1-tank];
    
    char edx = other->x - t->x;
    char edy = other->y - t->y;
    std::cout << "PING" << tank << " Enemy: " << (int)edx << ", " << (int)edy << std::endl;
    
    std::list<Projectile*>::iterator i = Projectiles.begin();
    for (; i!=Projectiles.end(); ++i)
    {
        Projectile* p = *i;
        
        char pdx = p->x - t->x;
        char pdy = p->y - t->y;
        std::cout << "PING" << tank << " Projectile " << p->UUID << ": " << (int)pdx << ", " << (int)pdy << std::endl;
    }
}

void BattleMap::CheckForDeaths(Projectile** ppProjectileTank0, Projectile** ppProjectileTank1)
{
    *ppProjectileTank0 = *ppProjectileTank1 = NULL;
    
    Projectile* p0 = NULL;
    Projectile* p1 = NULL;
    
    std::list<Projectile*>::iterator i = Projectiles.begin();
    for (; i!=Projectiles.end(); ++i)
    {
        Projectile* p = *i;
        if ((p0 == NULL) && ((p->x == Tanks[0]->x) && (p->y == Tanks[0]->y)))
            p0 = p;
        if ((p1 == NULL) && ((p->x == Tanks[1]->x) && (p->y == Tanks[1]->y)))
            p1 = p;
    }
    
    if (p0 != NULL)
    {
        Projectile* p = p0;
        p->Shooter->Projectiles.remove(p);
        Projectiles.remove(p);
        
        Tanks[0]->HitPointsRemaining--;
        if (Tanks[0]->HitPointsRemaining <= 0)
            *ppProjectileTank0 = p0;
    }
    if (p1 != NULL)
    {
        Projectile* p = p1;
        p->Shooter->Projectiles.remove(p);
        Projectiles.remove(p);
        
        Tanks[1]->HitPointsRemaining--;
        if (Tanks[1]->HitPointsRemaining <= 0)
            *ppProjectileTank1 = p1;
    }
}

void BattleMap::MoveProjectiles()
{
    std::list<Projectile*>::iterator i = Projectiles.begin();
    for (; i!=Projectiles.end(); ++i)
    {
        Projectile* p = *i;
        
        p->x += p->dx;
        p->y += p->dy;
        
        p->RemainingDistance--;
    }
}

void BattleMap::CheckProjectileDeaths()
{
    std::list<Projectile*>::iterator i = Projectiles.begin();
    for (; i!=Projectiles.end(); /**/)
    {
        Projectile* p = *i;
        
        if (p->RemainingDistance <= 0)
        {
            p->Shooter->Projectiles.remove(p);
            i = Projectiles.erase(i);
            
            delete p;
            continue;
        }
        
        // Not done in for() as removing items from the list
        ++i;
    }
}

