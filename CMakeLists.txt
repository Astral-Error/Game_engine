cmake_minimum_required(VERSION 3.15)
project(Game_engine)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib/SDL2/include
    ${PROJECT_SOURCE_DIR}/lib/GLM
    ${PROJECT_SOURCE_DIR}/lib/Box2D/include        # Optional
    ${PROJECT_SOURCE_DIR}/lib/imgui                # Optional
)

# Source files
file(GLOB_RECURSE ENGINE_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Add executable
add_executable(Game_engine ${ENGINE_SOURCES})

# SDL2
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})
    target_link_libraries(Game_engine ${SDL2_LIBRARIES})
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

# Link OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Game_engine ${OPENGL_gl_LIBRARY})

# Optional: Add SDL_mixer, Box2D, ImGui as needed later
# target_link_libraries(Game_engine SDL2_mixer Box2D imgui)