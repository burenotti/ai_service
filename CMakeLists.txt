cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_COMPILER /usr/bin/clang++-12)
set(CMAKE_C_COMPILER /usr/bin/clang-12)

project(ai_service)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_STANDARD 17)

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LC)

include(/ai_service/conan-libs/conanbuildinfo.cmake)

set(CONAN_DISABLE_CHECK_COMPILER True)

conan_basic_setup()

set(HEADERS src/neural/Layer.hpp src/dto/recognize_request.hpp src/controllers/recognize.hpp)
set(SOURCES src/main.cpp src/business/recognize_digit.hpp)

add_executable(ai_service ${HEADERS} ${SOURCES})
target_link_libraries(ai_service ${CONAN_LIBS})
