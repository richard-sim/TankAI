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
    
    bool quit = false;
    while (!quit)
    {
    net::Address sender;
    char buffer[256];
    int bytesRead;
    while ((bytesRead = socket.Receive(sender, buffer, sizeof(buffer))) > 0)
    {
        std::cout << "Tank received packet from " << (int)sender.GetA() << "." << (int)sender.GetB() << "." << (int)sender.GetC() << "." << (int)sender.GetD() << ":" << sender.GetPort() << " (" << bytesRead << " bytes)" << std::endl;
        std::cout << "\tFrom Server: " << buffer << std::endl;
        
        quit = (strcmp(buffer, "quit") == 0);
        
        char* buff = buffer;
        while (*buff)
        {
            if (*buff >= 'a' && *buff <= 'z')
                *buff -= 32;
            else if (*buff >= 'A' && *buff <= 'Z')
                *buff += 32;
            buff++;
        }

        socket.Send(net::Address(127,0,0,1, kServerPort), buffer, bytesRead);
    }
        //net::wait(1.0f);
    }
    
    std::cout << "Tank finished." << std::endl;
    
    net::ShutdownSockets();
    
    return 0;
}

