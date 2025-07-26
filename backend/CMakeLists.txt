cmake_minimum_required(VERSION 3.10)

project(Crypto CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

target_link_libraries(Crypto PRIVATE nlohmann_json::nlohmann_json)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Crypto src/main.cpp);

