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
CXXFLAGS = -I/usr/include/opencv4 -L/usr/lib
LIBS = -lopencv_core -lopencv_videoio -lopencv_imgproc -lopencv_highgui -lavformat -lavcodec -lavutil -lswscale

# Target binary and source file
TARGET = client
SRC = client.cpp

.PHONY: all install install_rpi install_ubuntu run run_rpi run_ubuntu clean detect build build_clean

# Default target
all: install run build

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

# # Run MediaMTX for Raspberry Pi
# run_rpi:
# 	@echo "Running MediaMTX on Raspberry Pi..."
# 	mediamtx mediamtx.yml

# # Run MediaMTX for Ubuntu Desktop
# run_ubuntu:
# 	@echo "Running MediaMTX on Ubuntu Desktop..."
# 	mediamtx mediamtx.yml

# # Run MediaMTX based on detected platform
# run:
# 	@if [ "$(UNAME_S)" = "Linux" ]; then \
# 		if [ "$(IS_RASPBERRY)" -gt 0 ]; then \
# 			$(MAKE) run_rpi; \
# 		else \
# 			$(MAKE) run_ubuntu; \
# 		fi \
# 	else \
# 		echo "Unsupported OS: $(UNAME_S)"; exit 1; \
# 	fi

# Detect platform information
detect:
	@echo "Operating System: $(UNAME_S)"
	@echo "Architecture: $(UNAME_M)"
	@if [ "$(IS_RASPBERRY)" -gt 0 ]; then \
		echo "Platform: Raspberry Pi"; \
	else \
		echo "Platform: Ubuntu Desktop (or other Linux)"; \
	fi

# Build the client binary
build:
	@echo "Building client..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LIBS)
	@echo "Client built successfully."

# Clean rule for client and MediaMTX binaries
clean:
	@echo "Cleaning up..."
	rm -f $(MEDIAMTX_TARGZ_RPI) $(MEDIAMTX_TARGZ_UBUNTU)
	$(MAKE) build_clean
	@echo "Clean up complete."

# Clean client binary
build_clean:
	rm -f $(TARGET)