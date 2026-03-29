cmake_minimum_required(VERSION 3.20)
project(CarnEvil C)

set(CMAKE_C_STANDARD 11)

# Runtime library
add_library(carnevil_runtime STATIC
    src/runtime/seattle_runtime.c
)

target_include_directories(carnevil_runtime PUBLIC
    src/runtime
)

# Recompiled game code + RTOS code
file(GLOB RECOMP_SOURCES recomp_out/funcs/funcs_*.c)
file(GLOB RTOS_SOURCES recomp_out/rtos_funcs/funcs_*.c)

add_library(carnevil_recomp STATIC ${RECOMP_SOURCES})

target_include_directories(carnevil_recomp PRIVATE
    src/runtime
    recomp_out/funcs
)

# Copy recomp.h to the funcs directory so #include "recomp.h" works
configure_file(src/runtime/recomp.h recomp_out/funcs/recomp.h COPYONLY)

# Main executable
# Override functions are listed BEFORE recomp sources so linker uses ours
add_executable(carnevil
    src/runtime/seattle_runtime.c
    src/runtime/seattle_overrides.c
    src/runtime/voodoo.c
    src/runtime/input.c
    src/runtime/dcs_sound.c
    src/runtime/rtos_scheduler.c
    src/runtime/rtos_registration.c
    src/runtime/rtos_stubs.c
    src/runtime/rtos_trampolines.c
    ${RECOMP_SOURCES}
    ${RTOS_SOURCES}
)

target_compile_definitions(carnevil PRIVATE CARNEVIL_STANDALONE)

target_include_directories(carnevil PRIVATE
    src/runtime
    recomp_out/funcs
    recomp_out/rtos_funcs
)

# Copy registration include to runtime directory
configure_file(src/runtime/func_registration.inc recomp_out/funcs/func_registration.inc COPYONLY)

target_include_directories(carnevil_runtime PRIVATE
    recomp_out/funcs
)

if(MSVC)
    target_compile_options(carnevil_recomp PRIVATE /O2 /W3)
    target_compile_options(carnevil_runtime PRIVATE /O2 /W3)
    target_compile_options(carnevil PRIVATE /O2 /W3)
    # Prevent linker from stripping unreferenced recompiled functions
    target_link_options(carnevil PRIVATE /OPT:NOREF /OPT:NOICF)
else()
    target_compile_options(carnevil_recomp PRIVATE -O2 -Wall)
    target_compile_options(carnevil_runtime PRIVATE -O2 -Wall)
endif()
