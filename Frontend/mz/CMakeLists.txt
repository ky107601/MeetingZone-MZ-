cmake_minimum_required(VERSION 3.16)

project(mz VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add FFmpeg include directory and library directory
include_directories(${CMAKE_SOURCE_DIR}/include/ffmpeg)  # Include FFmpeg headers
link_directories(${CMAKE_SOURCE_DIR}/lib)               # Link FFmpeg static libraries

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        widget.cpp
        widget.h
        widget.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mz
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        image.qrc
    )
else()
    if(ANDROID)
        add_library(mz SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(mz
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link FFmpeg libraries and Qt Widgets
target_link_libraries(mz
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    avcodec
    avformat
    avutil
    swresample
    swscale
    pthread  # FFmpeg often requires pthread
    m        # Math library
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.mz)
endif()
set_target_properties(mz PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS mz
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mz)
endif()
