cmake_minimum_required(VERSION 3.10)
project(SolitarioNapoletano)

set(CMAKE_CXX_STANDARD 17)

# SFML dependency
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Base source dirs
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(GUI_DIR "${SRC_DIR}/gui")
set(MENU_DIR "${SRC_DIR}/menu")
set(SOLITAIRE_DIR "${SRC_DIR}/solitaires")

# Game logic shared
set(GAME_LOGIC
    ${SRC_DIR}/game_logic.cpp
    ${SRC_DIR}/game_logic.hpp
)

# One-Two-Three solitaire GUI files
set(ONE_TWO_THREE_GUI
    ${SOLITAIRE_DIR}/one_two_three/game_window.cpp
    ${SOLITAIRE_DIR}/one_two_three/game_window.hpp
)

set(LACUNE_GUI 
    ${SOLITAIRE_DIR}/lacune/lacune_window.cpp   
    ${SOLITAIRE_DIR}/lacune/lacune_window.hpp 
)

# CLI executable (just for One-Two-Three)
add_executable(solitaire_cli
    ${SRC_DIR}/main_cli.cpp
    ${GAME_LOGIC}
)

# GUI executable with menu and One-Two-Three game
add_executable(solitaire_gui
    ${SRC_DIR}/main_gui.cpp
    ${MENU_DIR}/menu.cpp
    ${MENU_DIR}/menu.hpp
    ${ONE_TWO_THREE_GUI}
    ${LACUNE_GUI}
    ${GAME_LOGIC}
)

# Link SFML only to GUI
target_link_libraries(solitaire_gui sfml-graphics sfml-window sfml-system)
