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
    
    
    std::cout << "Server running." << std::endl;

    std::cout << "Waiting for registrations..." << std::endl;
    
    net::Address t0Address;
    net::Address t1Address;
    bool t0Registered = false;
    bool t1Registered = false;
    do
    {
        net::Address sender;
        char buffer[256];
        int bytesRead;
        while ((bytesRead = socket.Receive(sender, buffer, sizeof(buffer))) > 0)
        {
            ServerCommand cmd = (ServerCommand)buffer[0];
            switch (cmd)
            {
                case SERVERCOMMAND_RegisterTank:
                {
                    if (!t0Registered)
                    {
                        t0Address = sender;
                        t0Registered = true;
                    }
                    else
                    {
                        t1Address = sender;
                        t1Registered = true;
                    }
                    break;
                }
                    
                default:
                {
                    std::cout << "Unexpected server command received from " << (int)sender.GetA() << "." << (int)sender.GetB() << "." << (int)sender.GetC() << "." << (int)sender.GetD() << ":" << sender.GetPort() << " while awaiting registrations: " << (int)cmd << "." << std::endl;
                    return 3;
                }
            }
        }
        
        net::wait(0.1f);
    } while (!t0Registered || !t1Registered);
    
    std::cout << "Tank 0 is registered as " << (int)t0Address.GetA() << "." << (int)t0Address.GetB() << "." << (int)t0Address.GetC() << "." << (int)t0Address.GetD() << ":" << t0Address.GetPort() << "." << std::endl;
    std::cout << "Tank 1 is registered as " << (int)t1Address.GetA() << "." << (int)t1Address.GetB() << "." << (int)t1Address.GetC() << "." << (int)t1Address.GetD() << ":" << t1Address.GetPort() << "." << std::endl;
    
    //
    int results[STATE_COUNT] = {0};
    for (int i=0; i<kNumGames; i++)
    {
        BattleMap map;
    
        GameState state;
        do
        {
            char cmdGo[1] = { (char)RESPONSE_Go };
            socket.Send(t0Address, cmdGo, 1);
            socket.Send(t1Address, cmdGo, 1);

            Command c0, c1;
            bool receivedC0 = false;
            bool receivedC1 = false;
            do
            {
                net::Address sender;
                char buffer[256];
                int bytesRead;

                bytesRead = socket.Receive(sender, buffer, sizeof(buffer));
                if (bytesRead > 0)
                {
                    ServerCommand cmd = (ServerCommand)buffer[0];
                    
                    if (!receivedC0 && (sender == t0Address))
                    {
                        if (cmd == SERVERCOMMAND_Command)
                            receivedC0 = c0.Decode(buffer+1, bytesRead-1);
                        
                        if (!receivedC0)
                        {
                            std::cout << "Response received from Tank 0, but it was either not a command, or corrupted." << std::endl;
                        }
                    }
                    else if (!receivedC1 && (sender == t1Address))
                    {
                        if (cmd == SERVERCOMMAND_Command)
                            receivedC1 = c1.Decode(buffer+1, bytesRead-1);
                        
                        if (!receivedC1)
                        {
                            std::cout << "Response received from Tank 1, but it was either not a command, or corrupted." << std::endl;
                        }
                    }
                }
            } while (!receivedC0 && !receivedC1);
            
            //
            state = map.Update(c0, c1);
        } while (state == STATE_Continue);
        
        results[state]++;
    
        std::cout << "GAME END: " << kGameStateNames[state] << " (after " << (kGameLength - map.GetTimeRemaining()) << " ticks)" << std::endl;
    }
    
    std::cout << "Results after " << kNumGames << " battles: Tank 0 with " << results[STATE_Tank1Dead] << " wins, Tank 1 with " << results[STATE_Tank0Dead] << " wins." << std::endl;
    
    std::cout << "There were " << (results[STATE_TieBothDead] + results[STATE_TieTimeOver]) << " draws, " << results[STATE_TieTimeOver] << " of which were time-overs, and " << results[STATE_TieBothDead] << " were doube-deaths." << std::endl;


    //
    char cmdQuit[1] = { (char)RESPONSE_Quit };
    socket.Send(t0Address, cmdQuit, 1);
    socket.Send(t1Address, cmdQuit, 1);

    
    //
    std::cout << "Server finished." << std::endl;
    net::ShutdownSockets();
    
    return 0;
}

