cmake_minimum_required(VERSION 3.20)
project(CarnEvil C)
set(CMAKE_C_STANDARD 11)

# recomp_out/ is generated locally by MidwayRecomp from your own GAME.EXE and
# RTOS image (see README); it is never committed.
file(GLOB GAME_FUNCS CONFIGURE_DEPENDS recomp_out/funcs/funcs_*.c)
file(GLOB RTOS_FUNCS CONFIGURE_DEPENDS recomp_out/rtos_funcs/rtos_funcs_*.c)
if(NOT GAME_FUNCS OR NOT RTOS_FUNCS)
    message(FATAL_ERROR "recomp_out/ is empty: run MidwayRecomp on carnevil_recomp.toml and rtos_recomp.toml first")
endif()

set(RUNTIME
    src/runtime/main.c
    src/runtime/hw.c
    src/runtime/rtos.c
    src/runtime/proc.c
    src/runtime/profile.c
    src/runtime/audio.c
    src/runtime/voodoo_sst1.c
    src/runtime/dcs.c
    src/runtime/adsp2100.c)

add_library(game_recomp STATIC ${GAME_FUNCS})
target_include_directories(game_recomp PRIVATE src/runtime recomp_out/funcs)
add_library(rtos_recomp STATIC ${RTOS_FUNCS})
target_include_directories(rtos_recomp PRIVATE src/runtime recomp_out/rtos_funcs)

add_library(lookup_game OBJECT src/runtime/lookup_game.c)
target_include_directories(lookup_game PRIVATE src/runtime recomp_out/funcs)
add_library(lookup_rtos OBJECT src/runtime/lookup_rtos.c)
target_include_directories(lookup_rtos PRIVATE src/runtime recomp_out/rtos_funcs)

add_executable(carnevil ${RUNTIME} $<TARGET_OBJECTS:lookup_game> $<TARGET_OBJECTS:lookup_rtos>)
target_include_directories(carnevil PRIVATE src/runtime)
target_link_libraries(carnevil PRIVATE game_recomp rtos_recomp winmm)

if(MSVC)
    foreach(t game_recomp rtos_recomp lookup_game lookup_rtos carnevil)
        target_compile_options(${t} PRIVATE /O2 /W3 /wd4244 /wd4267 /wd4101 /wd4189)
        target_compile_definitions(${t} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endforeach()
    target_link_options(carnevil PRIVATE /STACK:16777216 /MAP)   # the map resolves CARNEVIL_PROFILE samples
    set_target_properties(carnevil PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
