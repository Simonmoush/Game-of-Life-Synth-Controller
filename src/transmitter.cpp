#include "transmitter.h"

// constructor
Transmitter::Transmitter(char* a, int p){
	int len = (a != NULL) ? strlen(a) : 10;
	addr = new char[len];
	(a != NULL) ? strcpy (addr, a) : strcpy (addr, "127.0.0.1"); //localhost is the default sound server

	port = (p != -1) ? p : 7000;
}

//send a udp datagram for each living square or keysquare
void Transmitter::sendLife(const char* tag, bool newLife, bool keysquare, int x, int y){
    UdpTransmitSocket transmitSocket(IpEndpointName(addr, port));
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage(tag) << newLife << keysquare << x << y << osc::EndMessage
	<< osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

//send a setup message that tells the sound server the size of the board and initial time delay
void Transmitter::sendSetup(const char* tag, int x, int y, float delay){
    UdpTransmitSocket transmitSocket(IpEndpointName(addr, port));
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage(tag) << x << y << delay << osc::EndMessage
	<< osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

//send a control message that tells the server general info like current population
void Transmitter::sendControl(const char* tag, float ctl1, float ctl2, float ctl3){
    UdpTransmitSocket transmitSocket(IpEndpointName(addr, port));
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage(tag) << ctl1 << ctl2 << ctl3 << osc::EndMessage
	<< osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}

//tells the server that the delay between iterations has changed, and what the new delay is
void Transmitter::sendDelay(const char* tag, float delay){
    UdpTransmitSocket transmitSocket(IpEndpointName(addr, port));
    
    char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p(buffer, OUTPUT_BUFFER_SIZE);
    
    p << osc::BeginBundleImmediate
        << osc::BeginMessage(tag) << delay << osc::EndMessage
	<< osc::EndBundle;
    
    transmitSocket.Send( p.Data(), p.Size() );
}
