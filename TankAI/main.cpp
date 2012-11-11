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

#include "TankAICommon/Net.h"

#include "TankAICommon/TankAIGame.h"
#include "TankAICommon/Command.h"

#include "Tank.h"
#include "Projectile.h"
#include "BattleMap.h"


//
int main(int argc, const char * argv[])
{
    srand((unsigned int)time(NULL));
    
    if (!net::InitializeSockets())
    {
        std::cout << "Error: Could not initialize sockets!" << std::endl;
        return 1;
    }

    net::Socket socket;
    if (!socket.Open(kServerPort))
    {
        std::cout << "Error: Could not open server port " << kServerPort << std::endl;
        return 2;
    }
    
    std::cout << "Server running..." << std::endl;
    
    for (int i=0; i<10; i++)
    {
    const char data[] = "Test Packet";
    socket.Send(net::Address(127,0,0,1, kTankPort), data, sizeof(data));
    
    net::Address sender;
    char buffer[256];
    int bytesRead;
    while ((bytesRead = socket.Receive(sender, buffer, sizeof(buffer))) > 0)
    {
        std::cout << "Server received packet from " << (int)sender.GetA() << "." << (int)sender.GetB() << "." << (int)sender.GetC() << "." << (int)sender.GetD() << ":" << sender.GetPort() << " (" << bytesRead << " bytes)" << std::endl;
        std::cout << "\tFrom Tank: " << buffer << std::endl;
    }
        net::wait(1.0f);
    }

    {
        const char data[] = "quit";
        socket.Send(net::Address(127,0,0,1, kTankPort), data, sizeof(data));
    }
    
    std::cout << "Server finished." << std::endl;
    
#if 0
    //
    int results[STATE_COUNT] = {0};
    for (int i=0; i<kNumGames; i++)
    {
        BattleMap map;
    
        GameState state;
        do
        {
            Command c0, c1;
            //c0.SetRandom();
            //c1.SetRandom();
            
            state = map.Update(c0, c1);
        } while (state == STATE_Continue);
        
        results[state]++;
    
        std::cout << "GAME END: " << kGameStateNames[state] << " (after " << (kGameLength - map.GetTimeRemaining()) << " ticks)" << std::endl;
    }
    
    std::cout << "Results after " << kNumGames << " battles: Tank 0 with " << results[STATE_Tank1Dead] << " wins, Tank 1 with " << results[STATE_Tank0Dead] << " wins." << std::endl;
    
    std::cout << "There were " << (results[STATE_TieBothDead] + results[STATE_TieTimeOver]) << " draws, " << results[STATE_TieTimeOver] << " of which were time-overs, and " << results[STATE_TieBothDead] << " were doube-deaths." << std::endl;
#endif
    
    //
    net::ShutdownSockets();
    
    return 0;
}

