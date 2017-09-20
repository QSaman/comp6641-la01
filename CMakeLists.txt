project(httpc)
set(CMAKE_MINIMUM_VERSION 3.1)
cmake_minimum_required(VERSION ${CMAKE_MINIMUM_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ucm)
include(DefaultOptions)

include(DownloadAsio)
include_directories(${ASIO_INCLUDE_DIR})

include(DownloadCxxopts)
include_directories(${CXXOPTS_INCLUDE_DIR})

#The following variable requires CMake 3.1
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

ucm_add_dirs(src TO SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Threads::Threads)