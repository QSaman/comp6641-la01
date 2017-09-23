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

include(DownloadLUrlParser)
include_directories(${LURLPARSER_INCLUDE_DIR})
set(LIBRARIES ${LURLPARSER_LIBRARIES})

#The following variable requires CMake 3.1
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

ucm_add_dirs(src TO SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
add_dependencies(${PROJECT_NAME} external_asio external_cxxopts)
target_link_libraries(${PROJECT_NAME} Threads::Threads ${LURLPARSER_LIBRARIES})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/sample_files" "${PROJECT_BINARY_DIR}/sample_files")