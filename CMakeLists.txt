cmake_minimum_required(VERSION 3.20)
project(nocturne C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses)
pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib_c)

include_directories(includes)

add_executable(nocturne
        src/main.c
        src/app.c
        src/audio.c
	src/config.c
        src/miniaudio.c
	src/ui.c
)

target_include_directories(nocturne PRIVATE includes)

target_link_libraries(nocturne PRIVATE
        PkgConfig::NOTCURSES
        PkgConfig::TAGLIB
        m
)

if (APPLE)
    target_link_libraries(nocturne PRIVATE
            "-framework CoreAudio"
            "-framework AudioToolbox"
            "-framework AudioUnit"
            "-framework CoreFoundation"
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(nocturne PRIVATE pthread dl)
endif ()
