//
//  Command.h
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#ifndef TankAICommon_Command_h
#define TankAICommon_Command_h


#include "TankAIGame.h"


struct Command
{
    GameAction action;
    char arg0, arg1;
    
    void SetRandom();
    
    void Encode(char* buffer, int* bufferSize);
    bool Decode(const char* buffer, const int bufferSize);
};


#endif
