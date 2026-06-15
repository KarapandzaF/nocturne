cmake_minimum_required(VERSION 3.20)
project(music_tui C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(NOTCURSES REQUIRED IMPORTED_TARGET notcurses)
pkg_check_modules(TAGLIB REQUIRED IMPORTED_TARGET taglib_c)

include_directories(includes)

add_executable(music_tui
        src/main.c
        src/app.c
        src/audio.c
        src/miniaudio.c
	src/dynamic_array.c
)

target_include_directories(music_tui PRIVATE includes)

target_link_libraries(music_tui PRIVATE
        PkgConfig::NOTCURSES
        PkgConfig::TAGLIB
        m
)

if (APPLE)
    target_link_libraries(music_tui PRIVATE
            "-framework CoreAudio"
            "-framework AudioToolbox"
            "-framework AudioUnit"
            "-framework CoreFoundation"
    )
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(music_tui PRIVATE pthread dl)
endif ()
