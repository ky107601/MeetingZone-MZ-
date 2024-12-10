CXX = g++
CXXFLAGS = -I/usr/include -pthread
LDFLAGS = -lboost_system -lboost_thread

all: server client

server: server.cpp
	$(CXX) $(CXXFLAGS) server.cpp -o server $(LDFLAGS)

client: client.cpp
	$(CXX) $(CXXFLAGS) client.cpp -o client $(LDFLAGS)

clean:
	rm -f server client
