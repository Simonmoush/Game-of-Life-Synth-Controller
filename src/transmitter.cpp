#include "transmitter.h"

// constructor
Transmitter::Transmitter(char* a, int p){
	int len = (a != NULL) ? strlen(a) : 10;
	addr = new char[len];
	(a != NULL) ? strcpy (addr, a) : strcpy (addr, "127.0.0.1"); //localhost is the default sound server

	port = (p != -1) ? p : 7000;
}

void Transmitter::send(const char* tag, int pitch, int duration, float ctrl1, float ctrl2, float ctrl3){
    UdpTransmitSocket transmitSocket(IpEndpointName(addr, port));
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage(tag) << pitch << duration << ctrl1 << ctrl2 << ctrl3 << osc::EndMessage
	<< osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

