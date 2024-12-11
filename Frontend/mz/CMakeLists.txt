cmake_minimum_required(VERSION 3.16)

project(mz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV & FFmpeg 라이브러리 경로 추가
link_directories(${CMAKE_SOURCE_DIR}/lib)

# OpenCV & FFmpeg 헤더 경로 추가
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/opencv4)

# Find Qt and OpenCV packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Concurrent)

# Header & C++ Scripts
set(PROJECT_SOURCES
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    camviewer.h
    camviewer.cpp
    backgroundpicture.h
    backgroundpicture.cpp
    networkmanager.h
    networkmanager.cpp
)

# Define the executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mz
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        image.qrc
    )
else()
    if(ANDROID)
        add_library(mz SHARED ${PROJECT_SOURCES})
    else()
        add_executable(mz ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(mz
    PRIVATE
    # Qt
    Qt${QT_VERSION_MAJOR}::Widgets

    # OpenCV
    opencv_core
    opencv_imgcodecs
    opencv_imgproc
    opencv_highgui
    opencv_videoio
    opencv_video

    # ffmpeg
    iconv
    avcodec       # libavcodec.so
    avformat      # libavformat.so
    avutil        # libavutil.so
    avdevice      # libavdevice.so
    swresample    # libswresample.so
    swscale       # libswscale.so
    x264     

    # etc.
    pthread                   # Required for FFmpeg and OpenCV
    m                         # Math library

    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
)

# Set macOS-specific properties (if applicable)
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mz)
endif()

set_target_properties(mz PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/lib")

# Install rules
install(TARGETS mz
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

## == Media MTX == ##
# 변수 정의
set(MEDIAMTX_VERSION v1.9.3)
set(MEDIAMTX_TARGZ mediamtx_${MEDIAMTX_VERSION}_linux_arm64v8.tar.gz)
set(MEDIAMTX_DOWNLOAD_URL https://github.com/bluenviron/mediamtx/releases/download/${MEDIAMTX_VERSION}/${MEDIAMTX_TARGZ})

# 다운로드 및 압축 해제
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/mediamtx_downloaded
    COMMAND wget ${MEDIAMTX_DOWNLOAD_URL} -O ${CMAKE_BINARY_DIR}/${MEDIAMTX_TARGZ}
    COMMAND tar -xvzf ${CMAKE_BINARY_DIR}/${MEDIAMTX_TARGZ} -C ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/mediamtx_downloaded
    COMMENT "Downloading and extracting mediamtx from ${MEDIAMTX_DOWNLOAD_URL}"
)

# 빌드 타겟 추가
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/mediamtx_built
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/mediamtx_built
    DEPENDS ${CMAKE_BINARY_DIR}/mediamtx_downloaded
    COMMENT "Building mediamtx"
)

# mediamtx 다운로드 및 빌드 타겟
add_custom_target(mediamtx ALL
    DEPENDS ${CMAKE_BINARY_DIR}/mediamtx_built
)
## == Media MTX == ##


# Finalize for Qt6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mz)
endif()
