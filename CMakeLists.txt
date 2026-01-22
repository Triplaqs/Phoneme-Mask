# ---- Project info

# Determines minimum cmake version required, mandatory (but pretty useless) line
cmake_minimum_required(VERSION 3.14)

# Defines the name of this project, and assert that it will use C++
project("Phoneme mask" C CXX)


# ---- Package imports

# glfw3 should be installed on your system prior to this line for it to work
find_package(glfw3 REQUIRED)



# ---- Compile glad into its own library

# Generate a static glad library. Now you can use the name "glad" further in the code, it will refer to that new library
add_library(glad STATIC glad/src/glad.c)
target_include_directories(glad PUBLIC glad/include)


# ---- Compile your executable

# Add ImGui files
set(IMGUI_SOURCES
    Imgui/imgui.cpp
    Imgui/imgui_draw.cpp
    Imgui/imgui_widgets.cpp
    Imgui/imgui_tables.cpp
    Imgui/imgui_impl_glfw.cpp
    Imgui/imgui_impl_opengl3.cpp
)

# Create an executable named "out" that will depend on the source file src/main.cpp
add_executable(out
    src/vertex.cpp
    src/animation.cpp
    src/main.cpp
    ${IMGUI_SOURCES}
)




# ---- Include directories (headers)
target_include_directories(out PRIVATE
    src
    Imgui
)


# Link "glad" and "glfw" to your "out" executable
target_link_libraries(out glad glfw)