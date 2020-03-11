cmake_minimum_required(VERSION 3.1)

project(mario)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.5.1/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_definitions("-DENABLE_DEBUG_MODE -g -Wall -pedantic -O3")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
add_executable(mario main.cpp src/engine/window.cpp src/game.cpp src/engine/manager.cpp
                    src/engine/system.cpp src/entities/entities.cpp src/entities/player.cpp 
                    src/entities/enemies.cpp )
            
target_link_libraries(mario sfml-graphics sfml-window sfml-system sfml-audio sfml-network)
