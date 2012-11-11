//
//  TankAIGame.h
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#ifndef TankAICommon_TankAIGame_h
#define TankAICommon_TankAIGame_h


const int   kNumGames                   = 32;
const int   kGameLength                 = 2560;
const int   kUUIDMaskTank               = (1 << 30);
const int   kUUIDMaskProjectile         = (1 << 31);
const int   kStartingHitPoints          = 3;
const int   kMaxNumProjectilesPerTank   = 3;
const int   kProjectileRange            = 255;

const int   kServerPort                 = 31337;
const int   kTankPort                   = 40000;


enum GameState
{
    STATE_Continue,
    STATE_TieTimeOver,
    STATE_TieBothDead,
    STATE_Tank0Dead,
    STATE_Tank1Dead,
    
    STATE_COUNT
};
extern const char* kGameStateNames[STATE_COUNT];


enum GameAction
{
    ACTION_Move,
    ACTION_Shoot,
    ACTION_Ping,
    
    ACTION_COUNT
};


#endif
