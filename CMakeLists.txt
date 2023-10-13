cmake_minimum_required(VERSION 3.26)
project(ASCII_SSS C)

set(CMAKE_C_STANDARD 11)

add_executable(ASCII_SSS src/main.c
        src/sim/vector.c
        src/sim/vector.h
        src/menu/edit_menu.c
        src/menu/edit_menu.h
        src/graphics/renderer.c
        src/graphics/renderer.h
        src/screen.c
        src/screen.h
        src/sim/body.c
        src/sim/body.h
        src/sim/simulator.c
        src/sim/simulator.h
        src/menu/main_menu.c
        src/menu/main_menu.h
        src/graphics/layer.c
        src/graphics/layer.h
        src/econio/econio.c
)
