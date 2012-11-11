//
//  main.cpp
//  TankAIRandom
//
//  Created by Richard Sim on 12-11-10.
//  Copyright (c) 2012 Mesophase. All rights reserved.
//

#include <iostream>

#include "TankAICommon/Net.h"

#include "TankAICommon/TankAIGame.h"
#include "TankAICommon/Command.h"


const net::Address kServerAddress(127,0,0,1, kServerPort);


int main(int argc, const char * argv[])
{
    if (!net::InitializeSockets())
    {
        std::cout << "Error: Could not initialize sockets!" << std::endl;
        return 1;
    }
    
    net::Socket socket;
    if (!socket.Open(kTankPort))
    {
        std::cout << "Error: Could not open tank port " << kTankPort << std::endl;
        return 2;
    }
    
    std::cout << "Tank running..." << std::endl;
    
    {
        char serializedData[1];
        serializedData[0] = (char)SERVERCOMMAND_RegisterTank;
        while (!socket.Send(kServerAddress, serializedData, 1))
            /* loop until the registration goes through */;
    }
    
    bool quit = false;
    do
    {
        net::Address sender;
        char buffer[256];
        int bytesRead;
        while ((bytesRead = socket.Receive(sender, buffer, sizeof(buffer))) > 0)
        {
            ServerResponse response = (ServerResponse)buffer[0];
            switch (response)
            {
                case RESPONSE_PingResults:
                {
                    std::cout << "\tPing results received from server (" << (bytesRead-1) << " bytes)." << std::endl;
                    break;
                }
                    
                case RESPONSE_Go:
                {
                    Command cmd;
                    cmd.SetRandom();
                    
                    char serializedData[32];
                    serializedData[0] = (char)SERVERCOMMAND_Command;
                    int cmdSize = 0;
                    cmd.Encode(&(serializedData[1]), &cmdSize);
                    while (!socket.Send(kServerAddress, serializedData, cmdSize+1))
                        /* loop until the command goes through */;
                    break;
                }
                    
                case RESPONSE_Quit:
                {
                    std::cout << "\tQuit received from server. Terminating." << std::endl;
                    quit = true;
                    break;
                }
                    
                default:
                {
                    std::cout << "\tUnknown response from server: " << (int)response << ". Terminating." << std::endl;
                    quit = true;
                    break;
                }
            }
        }
        
        net::wait(0.1f);
    } while (!quit);
    
    std::cout << "Tank finished." << std::endl;
    
    net::ShutdownSockets();
    
    return 0;
}

