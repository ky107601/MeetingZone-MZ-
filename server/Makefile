# Define variables
MEDIAMTX_VERSION := v1.9.3
MEDIAMTX_TARGZ_RPI := mediamtx_$(MEDIAMTX_VERSION)_linux_arm64v8.tar.gz
MEDIAMTX_TARGZ_UBUNTU := mediamtx_$(MEDIAMTX_VERSION)_linux_amd64.tar.gz
INSTALL_DIR := .
DOWNLOAD_URL_RPI := https://github.com/bluenviron/mediamtx/releases/download/$(MEDIAMTX_VERSION)/$(MEDIAMTX_TARGZ_RPI)
DOWNLOAD_URL_UBUNTU := https://github.com/bluenviron/mediamtx/releases/download/$(MEDIAMTX_VERSION)/$(MEDIAMTX_TARGZ_UBUNTU)

# Detect OS and Architecture
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)
IS_RASPBERRY := $(shell cat /proc/cpuinfo | grep -c "Raspberry Pi")

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -I./nlohmann -I./include `pkg-config --cflags opencv4`
LDFLAGS = -pthread \
	./lib/libavdevice.so \
	./lib/libavcodec.so \
	./lib/libavfilter.so \
	./lib/libavformat.so \
	./lib/libavutil.so \
	./lib/libswscale.so \
	./lib/libswresample.so \
	`pkg-config --libs opencv4`
	
# -lx264 -lx265 -lvpx -lfdk-aac -lmp3lame -lopus -lvorbis

TARGET = server
SRCS = server.cpp main.cpp video.cpp networkmanager.cpp
OBJS = $(SRCS:.cpp=.o)

# Update specific values of YAML file
YAML_FILE = mediamtx.yml
PROTOCOLS_KEY = protocols
PROTOCOLS_VALUE = [tcp]
ENCRYPTION_KEY = encryption
ENCRYPTION_VALUE = "optional"

update_config:
	@echo "Updating $(PROTOCOLS_KEY) to $(PROTOCOLS_VALUE) in $(YAML_FILE)..."
	sed -i 's/\($(PROTOCOLS_KEY):\).*/\1 $(PROTOCOLS_VALUE)/' $(YAML_FILE)
	@echo "Updating $(ENCRYPTION_KEY) to $(ENCRYPTION_VALUE) in $(YAML_FILE)..."
	sed -i 's/\($(ENCRYPTION_KEY):\).*/\1 $(ENCRYPTION_VALUE)/' $(YAML_FILE)
	@echo "Update completed."

# Create key and cert file
# KEY_FILE = server.key
# CRT_FILE = server.crt

# check_certs: $(KEY_FILE) $(CRT_FILE)

# $(KEY_FILE):
#	@echo "Generating $(KEY_FILE)..."
#	openssl genrsa -out $(KEY_FILE) 2048
#	@echo "$(KEY_FILE) generated."

# $(CRT_FILE): $(KEY_FILE)
#	@echo "Generating $(CRT_FILE)..."
#	openssl req -new -x509 -sha256 -key $(KEY_FILE) -out $(CRT_FILE) -days 3650
#	@echo "$(CRT_FILE) generated."

# Install MediaMTX for Raspberry Pi
install_rpi:
	@echo "Installing MediaMTX for Raspberry Pi..."
	wget $(DOWNLOAD_URL_RPI)
	tar zxvf $(MEDIAMTX_TARGZ_RPI)
	rm $(MEDIAMTX_TARGZ_RPI)
	@echo "MediaMTX installed successfully on Raspberry Pi."

# Install MediaMTX for Ubuntu Desktop
install_ubuntu:
	@echo "Installing MediaMTX for Ubuntu Desktop..."
	wget $(DOWNLOAD_URL_UBUNTU)
	tar zxvf $(MEDIAMTX_TARGZ_UBUNTU)
	rm $(MEDIAMTX_TARGZ_UBUNTU)
	@echo "MediaMTX installed successfully on Ubuntu Desktop."

# Install MediaMTX based on detected platform
install:
	@if [ "$(UNAME_S)" = "Linux" ]; then \
		if [ "$(IS_RASPBERRY)" -gt 0 ]; then \
			$(MAKE) install_rpi; \
		else \
			$(MAKE) install_ubuntu; \
		fi \
	else \
		echo "Unsupported OS: $(UNAME_S)"; exit 1; \
	fi
	# $(MAKE) update_config;
	# $(MAKE) check_certs;

all: install $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
