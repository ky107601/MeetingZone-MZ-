cmake_minimum_required(VERSION 3.16)
project(MediamtxDownloader LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 변수 정의
set(MEDIAMTX_VERSION v1.9.3)
set(MEDIAMTX_TARGZ mediamtx_${MEDIAMTX_VERSION}_linux_arm64v8.tar.gz)
set(MEDIAMTX_DOWNLOAD_URL https://github.com/bluenviron/mediamtx/releases/download/${MEDIAMTX_VERSION}/${MEDIAMTX_TARGZ})

# 실행 파일 추가
add_executable(main main.cpp)

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
