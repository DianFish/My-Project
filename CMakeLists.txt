
cmake_minimum_required(VERSION 3.20)

project(MyGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SDL2_DIR "G:/SDL/SDL2-2.32.6/install_VS/cmake")

find_package(SDL2 CONFIG REQUIRED)

add_executable(MyGameEngine
    src/main.cpp
    src/Engine.cpp
    src/Player.cpp
)

target_include_directories(MyGameEngine 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(MyGameEngine
    PRIVATE
        SDL2::SDL2
)

if(MSVC)
    # 使用utf-8格式
    target_compile_options(MyGameEngine PRIVATE /utf-8)
endif()

# 添加SDL2动态库
add_custom_command(TARGET MyGameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "G:/SDL/SDL2-2.32.6/install_VS/bin/SDL2.dll"
        "$<TARGET_FILE_DIR:MyGameEngine>"
)
