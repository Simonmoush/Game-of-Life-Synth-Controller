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
	void send(const char* tag, int pitch, int duration, float ctrl1, float ctrl2, float ctrl3);
};
#endif
