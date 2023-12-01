cmake_minimum_required(VERSION 3.8)
# プロジェクト名を指定する
project(bp_system VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-10") # Update the compiler version
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # Set the C++ standard
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=gold") # Update the linker version

find_package(Threads)

# インクルードパスを追加
include_directories(_extra/nlohmann)


add_executable(${CMAKE_PROJECT_NAME} 
    main.cpp
    bp_system/_common/node.cpp
    bp_system/_common/node_util.cpp
    bp_system/_common/comm/comm_udp.cpp
    bp_system/b_system/b_hub.cpp
    bp_system/p_system/p_hub.cpp
    # b node
    bp_system/b_system/b_node.cpp
    bp_system/b_system/node/b_simple_node_a.cpp
    bp_system/b_system/node/b_simple_node_b.cpp
    bp_system/b_system/node/b_example_sub_servo.cpp
    bp_system/b_system/node/b_example_pub_control.cpp
    # p node
    bp_system/p_system/p_node.cpp
    bp_system/p_system/node/p_simple_node_a.cpp
)

if(WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()
