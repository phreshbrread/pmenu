# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name and language
project(pmenu LANGUAGES C)

# Include ncurses
set(CURSES_NEED_NCURSES TRUE)
find_library(CURSES_MENU_LIBRARY menu)
find_package(Curses REQUIRED)

# Add local library
add_library(pmenu-lib STATIC src/pmenu.c)

# Create executable
add_executable(pmenu src/main.c)

# Dependencies
target_link_libraries(pmenu PRIVATE pmenu-lib)
target_link_libraries(pmenu PRIVATE ${CURSES_LIBRARIES} ${CURSES_MENU_LIBRARY})

# Installation
install(TARGETS pmenu DESTINATION bin)
