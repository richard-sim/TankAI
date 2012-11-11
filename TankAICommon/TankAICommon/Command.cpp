//
//  Command.cpp
//  TankAICommon
//
//  Created by Richard Sim on 12-11-11.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include <stdlib.h>

#include "Command.h"


void Command::SetRandom()
{
    action = (GameAction)(rand() % ACTION_COUNT);
    arg0 = (char)(rand() % 2) * 2 - 1;
    arg1 = (char)(rand() % 2) * 2 - 1;
    
    if ((rand() % 2) == 0)
        arg0 = 0;
    else
        arg1 = 0;
}

void Command::Encode(char* buffer, int* bufferSize)
{
    *bufferSize = 3;
    buffer[0] = (char)action;
    buffer[1] = arg0;
    buffer[2] = arg1;
}

bool Command::Decode(const char* buffer, const int bufferSize)
{
    if (bufferSize != 3)
        return false;
    if ((buffer[0] < 0) || (buffer[0] > ACTION_COUNT))
        return false;
    
    action = (GameAction)buffer[0];
    arg0 = buffer[1];
    arg1 = buffer[2];
    
    return true;
}

