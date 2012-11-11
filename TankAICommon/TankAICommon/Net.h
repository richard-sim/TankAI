/*
	Simple Network Library from "Networking for Game Programmers"
	http://www.gaffer.org/networking-for-game-programmers
	Author: Glenn Fiedler <gaffer@gaffer.org>
*/

#ifndef NET_H
#define NET_H

namespace net
{
	// platform independent wait for n seconds
	void wait( float seconds );
    unsigned int GetTimeInMS();

	// internet address
	class Address
	{
	public:
	
		Address();
		Address( unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port );
		Address( unsigned int address, unsigned short port );
	
		unsigned int GetAddress() const;
	
		unsigned char GetA() const;
		unsigned char GetB() const;
		unsigned char GetC() const;
		unsigned char GetD() const;
	
		unsigned short GetPort() const;
	
		bool operator == ( const Address & other ) const;
		bool operator != ( const Address & other ) const;
	
	private:
	
		unsigned int address;
		unsigned short port;
	};

	// sockets
    bool InitializeSockets();
    void ShutdownSockets();

	class Socket
	{
	public:
	
		Socket();
		~Socket();
	
		bool Open( unsigned short port );
        void Close();
	
		bool IsOpen() const;
	
		bool Send( const Address & destination, const void * data, int size );
		int Receive( Address & sender, void * data, int size );
		
	private:
	
		int socket;
	};
}

#endif
