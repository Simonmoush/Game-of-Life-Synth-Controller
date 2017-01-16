#ifndef TRANSMITTER_H
#define TRANSMITTER_H


#include "osc/OscOutboundPacketStream.h"
#include "ip/UdpSocket.h"
#include <iostream>
#include <cstdlib>
#include <string>


#define OUTPUT_BUFFER_SIZE 1024

class Transmitter{
	private:
	char* addr;
	int port;

	public:
	Transmitter(char* a, int p);
	void sendLife(const char* tag, bool newLife, bool keysquare, int x, int y);
	void sendSetup(const char* tag, int x, int y, float delay);
	void sendControl(const char* tag, float ctl1, float ctl2, float ctl3);
	void sendDelay(const char* tag, float delay);
};
#endif
