cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project (moose)

find_library(UWS_LIB uWS)
find_library(M_LIB m)
find_library(SSL_LIB ssl)
find_library(Z_LIB z)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(MY_SRCS
        main.cpp
        handlers.cpp
        heater.cpp
        notify.cpp
        printer.cpp
        connection.cpp
        include)

add_executable(moose ${MY_SRCS})

target_link_libraries(moose uWS)
target_link_libraries(moose m)
target_link_libraries(moose z)
target_link_libraries(moose ssl)

target_compile_definitions(moose PRIVATE RAPIDJSON_HAS_STDSTRING=1)
