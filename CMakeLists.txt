cmake_minimum_required(VERSION 3.10)
project(CarteNapoletane)

set(CMAKE_CXX_STANDARD 17)

# Set source directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(GUI_DIR "${SRC_DIR}/gui")

# Add SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Game logic source files
set(GAME_LOGIC
    ${SRC_DIR}/game_logic.cpp
    ${SRC_DIR}/game_logic.hpp
)

# CLI executable
add_executable(carte_cli
    ${SRC_DIR}/main_cli.cpp
    ${GAME_LOGIC}
)

# GUI executable
add_executable(carte_gui
    ${SRC_DIR}/main_gui.cpp
    ${GUI_DIR}/game_window.cpp
    ${GUI_DIR}/game_window.hpp
    ${GAME_LOGIC}
)

# Link SFML to GUI only
target_link_libraries(carte_gui sfml-graphics sfml-window sfml-system)
