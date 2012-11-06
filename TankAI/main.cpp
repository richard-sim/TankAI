//
//  main.cpp
//  TankAI
//
//  Created by Richard Sim on 12-11-04.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include <stdlib.h>
#include <memory.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <list>


class Tank;
class Projectile;
class BattleMap;


//
const int   kNumGames                   = 32;
const int   kGameLength                 = 2560;
const int   kUUIDMaskTank               = (1 << 30);
const int   kUUIDMaskProjectile         = (1 << 31);
const int   kStartingHitPoints          = 3;
const int   kMaxNumProjectilesPerTank   = 3;
const int   kProjectileRange            = 255;

enum GameState
{
    STATE_Continue,
    STATE_TieTimeOver,
    STATE_TieBothDead,
    STATE_Tank0Dead,
    STATE_Tank1Dead,
    
    STATE_COUNT
};
const char* kGameStateNames[STATE_COUNT] =
{
    "Continue",
    "Tie - Time Over",
    "Tie - Both Tanks Dead",
    "Tank 0 Won",
    "Tank 1 Won"
};

enum GameAction
{
    ACTION_Move,
    ACTION_Shoot,
    ACTION_Ping,
    
    ACTION_COUNT
};

struct Command
{
    GameAction action;
    char arg0, arg1;
    
    Command()
    {
        action = (GameAction)(rand() % ACTION_COUNT);
        arg0 = (char)(rand() % 2) * 2 - 1;
        arg1 = (char)(rand() % 2) * 2 - 1;
        
        if ((rand() % 2) == 0)
            arg0 = 0;
        else
            arg1 = 0;
    }
};

//
class Tank
{
public:
    Tank(char _x, char _y)
    {
        UUID = sUUID++;
        
        HitPointsRemaining = kStartingHitPoints;
        
        x = _x;
        y = _y;
    }
    
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
int Tank::sUUID = kUUIDMaskTank;

//
class Projectile
{
public:
    Projectile(Tank* _shooter, char _dx, char _dy)
    {
        UUID = sUUID++;
        
        x = _shooter->x + _dx;
        y = _shooter->y + _dy;
        
        dx = _dx;
        dy = _dy;
        RemainingDistance = kProjectileRange - 1;   // -1 as always start by moving by dx/dy
        
        Shooter = _shooter;
    }
    
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
int Projectile::sUUID = kUUIDMaskProjectile;

//
class BattleMap
{
public:
    BattleMap()
    {
        Tank* t0 = new Tank(-63, -63);
        Tanks[0] = t0;
        
        Tank* t1 = new Tank( 63,  63);
        Tanks[1] = t1;
        
        TimeRemaining = kGameLength;
    }
    ~BattleMap()
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
    
    int GetTimeRemaining() const
    {
        return TimeRemaining;
    }
    
    GameState Update(const Command& tank0Command, const Command& tank1Command)
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
    
private:
    Projectile* AddProjectile(Tank* shooter, char dx, char dy)
    {
        if (shooter->Projectiles.size() >= kMaxNumProjectilesPerTank)
            return NULL;
        
        Projectile* p = new Projectile(shooter, dx, dy);
        shooter->Projectiles.push_back(p);
        Projectiles.push_back(p);
        
        return p;
    }
    
    void Ping(int tank)
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
    
    void CheckForDeaths(Projectile** ppProjectileTank0, Projectile** ppProjectileTank1)
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
    
    void MoveProjectiles()
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
    
    void CheckProjectileDeaths()
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

private:
    Tank*                   Tanks[2];
    std::list<Projectile*>  Projectiles;
    
    int                     TimeRemaining;
};

//
int main(int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));

    int results[2] = {0, 0};
    for (int i=0; i<kNumGames; i++)
    {
        BattleMap map;
    
        GameState state;
        do
        {
            Command c0, c1;
            state = map.Update(c0, c1);
        } while (state == STATE_Continue);
    
        std::cout << "GAME END: " << kGameStateNames[state] << " (after " << (kGameLength - map.GetTimeRemaining()) << " ticks)" << std::endl;
        
        if (state == STATE_Tank1Dead)
            results[0]++;
        else if (state == STATE_Tank0Dead)
            results[1]++;
    }
    
    std::cout << "Results after " << kNumGames << " battles: Tank 0 with " << results[0] << " wins, Tank 1 with " << results[1] << " wins." << std::endl;
    
    return 0;
}

