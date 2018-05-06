cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(Divamia)
add_definitions("-std=c++14")

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_executable(divamia
	src/buffer.cpp
	src/camera.cpp
	src/demo.cpp
	src/framebuffer.cpp
	src/game.cpp
	src/image.cpp
	src/main.cpp
	src/model.cpp
	src/shader.cpp
	src/text.cpp
 	src/texture.cpp
	src/vao.cpp
	src/vertex.cpp
	src/window.cpp)

target_link_libraries(divamia glfw freeimage util rt)

target_include_directories(divamia PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(divamia ${OPENGL_gl_LIBRARY})

target_include_directories(divamia PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(divamia ${GLEW_LIBRARIES})
