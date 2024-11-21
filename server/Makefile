CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I/server/nlohmann -I/server/include 
LDFLAGS = -pthread \
		   ./lib/libavdevice.a \
		   ./lib/libavcodec.a \
		   ./lib/libavfilter.a \
           ./lib/libavformat.a \
           ./lib/libavutil.a \
           ./lib/libswscale.a \
           ./lib/libswresample.a \
        	# -lx264 -lx265 -lvpx -lfdk-aac -lmp3lame -lopus -lvorbis

TARGET = server
SRCS = server.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
