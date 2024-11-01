cmake_minimum_required(VERSION 3.29)
project(app)

set(CMAKE_CXX_STANDARD 20)

set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

include_directories(${VENDOR_DIR}/include)

add_executable(app main.cpp
        authentication.h
        pch.h
        authentication.cpp
        accounts.h
        accounts.cpp
        mainScreen.h
        mainScreen.cpp
        budgetCategories.cpp
        budgetCategories.h
        Analytics.cpp
        Analytics.h
        export.cpp
        export.h
)

target_link_libraries(app "${VENDOR_DIR}/lib/raylib.lib" "${VENDOR_DIR}/lib/xlntd.lib" "${VENDOR_DIR}/lib/hpdf.lib")

add_custom_command(TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${VENDOR_DIR}/lib/raylib.dll"
        "${VENDOR_DIR}/lib/xlntd.dll"
        "${VENDOR_DIR}/lib/hpdf.dll"
        $<TARGET_FILE_DIR:app>
)

if(WIN32)
    target_link_libraries(app opengl32 gdi32 winmm)
    target_compile_options(app PRIVATE /w1493)
endif()
