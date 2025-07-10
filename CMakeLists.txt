cmake_minimum_required(VERSION 3.15)
project(Game_engine)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cxx)

# Create executable
add_executable(Game_engine ${ENGINE_SOURCES})

# Platform-specific includes and linking
if (WIN32)
    # Include Directories (Windows only)
    target_include_directories(Game_engine PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/dependencies/SDL2/include
        ${PROJECT_SOURCE_DIR}/dependencies/SDL2_mixer/include
        ${PROJECT_SOURCE_DIR}/dependencies/GLM
        ${PROJECT_SOURCE_DIR}/dependencies/Box2D/include
        ${PROJECT_SOURCE_DIR}/dependencies/glad/include
        ${PROJECT_SOURCE_DIR}/dependencies/imgui
    )

    # GLAD (Windows)
    add_library(glad STATIC dependencies/glad/src/glad.c)
    target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/glad/include)
    target_link_libraries(Game_engine glad)

    # ImGui (Windows)
    file(GLOB IMGUI_SOURCES ${PROJECT_SOURCE_DIR}/dependencies/imgui/*.cpp)
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC ${PROJECT_SOURCE_DIR}/dependencies/imgui)
    target_link_libraries(Game_engine imgui)

    # Link SDL2 statically + Windows system libraries
    target_link_libraries(Game_engine
        ${PROJECT_SOURCE_DIR}/dependencies/SDL2/lib/libSDL2main.a
        ${PROJECT_SOURCE_DIR}/dependencies/SDL2/lib/libSDL2.a
        ${PROJECT_SOURCE_DIR}/dependencies/SDL2_mixer/lib/libSDL2_mixer.a
        ${PROJECT_SOURCE_DIR}/dependencies/Box2D/lib/libbox2d.a

        # Windows system libraries required by SDL2 static linking
        setupapi
        imm32
        version
        winmm
        opengl32
    )

else()
    message(WARNING "Non-Windows builds are not fully configured yet. You must configure Linux paths and linking.")
endif()
