cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
    set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()
project(PLantGrowth)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
# Instruct CMake to run moc automatically when needed (Qt projects only)
set(CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(TargetName PLantGrowth)
set(TargetTest PLantGrowthTest)
# use C++ 17
find_package(NGL CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
# find Qt libs first we check for Version 6
find_package(Qt6 COMPONENTS OpenGL Widgets QUIET )
if ( Qt6_FOUND )
    message("Found Qt6 Using that")
else()
    message("Found Qt5 Using that")
    find_package(Qt5 COMPONENTS OpenGL Widgets REQUIRED)
endif()
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_executable(${TargetName})
include_directories(include $ENV{HOME}/NGL/include)
target_include_directories(${TargetName} PRIVATE include)
target_sources(${TargetName} PRIVATE
        include/NGLScene.h
        include/grammar.h
        include/FractalSystem.h
        src/FractalSystem.cpp
        src/grammar.cpp
        src/NGLScene.cpp
        src/NGLSceneMouseControls.cpp
        src/main.cpp


)
##GrammarTest
find_package(GTest CONFIG REQUIRED)
enable_testing()
include(GoogleTest)
#include_directories(include $ENV{HOME}/NGL/include)
add_executable(${TargetTest})
set(CMAKE_CXX_STANDARD 17)
target_include_directories(${TargetTest} PRIVATE include)
target_sources(${TargetTest} PRIVATE
        tests/GrammarTest.cpp
        include/grammar.h
        include/FractalSystem.h
        src/FractalSystem.cpp
        src/grammar.cpp

)
target_link_libraries(${TargetTest} PRIVATE Qt::Widgets Qt::OpenGL NGL)
target_link_libraries(${TargetTest} PRIVATE GTest::gtest GTest::gtest_main)
target_link_libraries(${TargetName} PRIVATE Qt::Widgets Qt::OpenGL NGL)
add_custom_target(${TargetName}CopyFiles ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${TargetName}>/shaders

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/mesh
        $<TARGET_FILE_DIR:${TargetName}>/mesh
)
