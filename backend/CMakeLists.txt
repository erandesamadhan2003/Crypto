cmake_minimum_required(VERSION 3.10)

project(Crypto CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(Crypto
    src/main.cpp
    src/utils/Logger.cpp
    src/utils/config.cpp
    src/utils/JSONHelper.cpp
    src/crypto/hash.cpp
)

target_link_libraries(Crypto
    PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
    ZLIB::ZLIB
    nlohmann_json::nlohmann_json
)
