cmake_minimum_required(VERSION 3.29)
project(app)

set(CMAKE_CXX_STANDARD 20)

set(RAYLIB_DIR "${CMAKE_SOURCE_DIR}/vendor")

include_directories(${RAYLIB_DIR}/include)

add_executable(app main.cpp
        authentication.h
        pch.h
        authentication.cpp
        accounts.h
        accounts.cpp
        mainScreen.h
        mainScreen.cpp
        budgetCategories.cpp
        budgetCategories.h)

target_link_libraries(app ${RAYLIB_DIR}/lib/raylib.lib)

add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${RAYLIB_DIR}/lib/raylib.dll"
        $<TARGET_FILE_DIR:app>)

if(WIN32)
    target_link_libraries(app opengl32 gdi32 winmm)

    target_compile_options(app PRIVATE /w1493)
endif()
