cmake_minimum_required(VERSION 3.28)

project(
    tower-main2
    DESCRIPTION "A MMORPG game server for Tower Online"
    LANGUAGES CXX
)

option(TOWER_BUILD_TESTS "Enable the build of tests" ON)

# Dependencies
include(FetchContent)

find_package(Boost 1.86.0 REQUIRED COMPONENTS system)

find_package(OpenSSL REQUIRED)

FetchContent_Declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG v28.3
)
FetchContent_MakeAvailable(protobuf)
option(protobuf_INSTALL "Install protobuf binaries and files" OFF)
option(protobuf_BUILD_TESTS "Build tests" OFF)

# Source
add_library(tower-lib STATIC
    tower/core/numbers.hpp
    tower/core/units.hpp
    tower/net/connection_rudp.cpp
    tower/net/details/rudp.hpp
)

target_compile_features(tower-lib PUBLIC cxx_std_23)
target_compile_options(tower-lib PRIVATE -Wall -Wextra -Wpedantic)

target_include_directories(tower-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/tower)

target_link_libraries(tower-lib
    PRIVATE
    Boost::system
    protobuf::libprotobuf
)

add_executable(tower-server
    tower/main.cpp
)
target_link_libraries(tower-server PRIVATE tower-lib)

# Protobuf compilers
set(proto_outdir ${CMAKE_CURRENT_BINARY_DIR}/tower/tower/message)
file(MAKE_DIRECTORY ${proto_outdir})
file(GLOB_RECURSE protos messages/*.proto)

function(protobuf_compiler infile include outdir)
    get_filename_component(name ${infile} NAME_WE)

    set(outfile
        "${outdir}/${name}pb.cc"
        "${outdir}/${name}pb.h"
    )

    add_custom_command(
        OUTPUT ${outfile}
        COMMAND protoc
        ARGS
            --cpp_out=${outdir}
            -I${include}
            ${infile}
        DEPENDS ${infile} protoc
        COMMENT "[protoc] Compiling message: ${out_file}"
        VERBATIM
    )

    add_custom_target(${name}-proto ALL DEPENDS ${outfile})
    add_dependencies(tower-lib ${name}-proto)
endfunction()

foreach(proto ${protos})
    protobuf_compiler(${proto} ${CMAKE_CURRENT_SOURCE_DIR}/messages ${proto_outdir})
endforeach()

# Tests
if(TOWER_BUILD_TESTS)
    add_subdirectory(tests)
endif()