ENDIANESS=OSC_DETECT_ENDIANESS #source code will detect using preprocessor
#ENDIANESS=OSC_HOST_LITTLE_ENDIAN

CXX := g++
INCLUDES := -I lib
COPTS  := -Wall -Wextra -O3 -std=c++11
CDEBUG := -Wall -Wextra -g 
CXXFLAGS := $(COPTS) $(INCLUDES) -D$(ENDIANESS)

# Common source groups

SENDSOURCES := lib/osc/OscOutboundPacketStream.cpp
NETSOURCES := lib/ip/posix/UdpSocket.cpp lib/ip/IpEndpointName.cpp lib/ip/posix/NetworkingUtils.cpp
COMMONSOURCES := lib/osc/OscTypes.cpp

SENDOBJECTS := $(SENDSOURCES:.cpp=.o)
NETOBJECTS := $(NETSOURCES:.cpp=.o)
COMMONOBJECTS := $(COMMONSOURCES:.cpp=.o)

all : obj/main.o obj/transmitter.o obj/life.o $(COMMONOBJECTS) $(SENDOBJECTS) $(NETOBJECTS)
	$(CXX) $(CXXFLAGS) -o bin/life $^ -lncurses

obj/main.o : src/main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

obj/life.o : src/life.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^ -lncurses

obj/transmitter.o : src/transmitter.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean :
	rm -rf lib/ip/*.0 lib/ip/posix/*.o lib/osc/*.o obj/*
