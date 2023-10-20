cmake_minimum_required(VERSION 3.26)
project(ASCII_SSS C)

set(CMAKE_C_STANDARD 11)

add_executable(ASCII_SSS src/main.c
        src/vector.c
        src/vector.h
        src/gui/edit_menu.c
        src/gui/edit_menu.h
        src/graphics/render.c
        src/graphics/render.h
        src/sim/body.c
        src/sim/body.h
        src/graphics/layer.c
        src/graphics/layer.h
        src/lib/econio.c
        src/global.c
        src/global.h
        src/sim/simulator.c
        src/sim/simulator.h
        src/graphics/drawing.c
        src/graphics/drawing.h
        src/gui/overlay.c
        src/gui/overlay.h
        src/lib/debugmalloc.h
)
