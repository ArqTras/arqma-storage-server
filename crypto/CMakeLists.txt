cmake_minimum_required(VERSION 3.1)

project(crypto)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/arqmad_key.cpp
    include/arqmad_key.h
    include/channel_encryption.hpp
    src/channel_encryption.cpp
    src/signature.cpp
    include/signature.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/crypto-ops.c
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/crypto-ops-data.c
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/crypto-ops.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/keccak.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/keccak.c
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/int-util.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/hash-ops.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendors/arqma/crypto-ops/hash-ops.c
)

add_library(crypto STATIC ${SOURCES})

if (NOT OPENSSL_FOUND)
    find_package(OpenSSL REQUIRED)
endif ()
target_link_libraries(crypto PRIVATE OpenSSL::SSL)

target_include_directories(crypto
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/../vendors
)

arqma_add_subdirectory(../utils utils)

target_link_libraries(crypto PUBLIC utils)

if (NOT Boost_FOUND)
    find_package(Boost REQUIRED system filesystem)
endif()

target_include_directories(crypto PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(crypto PRIVATE ${Boost_LIBRARIES})

add_executable(crypto_test src/test_main.cpp)
target_link_libraries(crypto_test PRIVATE crypto)

# sodium
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../contrib")
set(sodium_USE_STATIC_LIBS ON)
find_package(sodium REQUIRED)
target_link_libraries(crypto PUBLIC sodium)

if (UNIX AND NOT APPLE)
    target_link_libraries(crypto PUBLIC pthread dl)
endif ()
