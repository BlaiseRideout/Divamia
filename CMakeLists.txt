cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)
project(divamia)

###############
# DEFINITIONS #
###############
set(THIRDPARTY "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty")
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.hpp" "${SRC_DIR}/*.h")


##############
# EXECUTABLE #
##############
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)


#########
# GLFW3 #
#########
set(GLFW_DIR "${THIRDPARTY}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")


########
# GLAD #
########
set(GLAD_DIR "${THIRDPARTY}/glad")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(GLAD_GENERATOR "c-debug")
  add_subdirectory("${GLAD_DIR}")
else()
  set(GLAD_GENERATOR "c")
  add_subdirectory("${GLAD_DIR}")
endif()
target_link_libraries(${PROJECT_NAME} "${GLAD_LIBRARIES}" "${CMAKE_DL_LIBS}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_INCLUDE_DIRS}")


#######
# GLM #
#######
set(GLM_DIR "${THIRDPARTY}/glm")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}")


#############
# FreeImage #
#############
set(FREEIMAGE_DIR "${THIRDPARTY}/FreeImage")
add_subdirectory("${FREEIMAGE_DIR}")
target_link_libraries(${PROJECT_NAME} ${XMAKE_FREEIMAGE_LINK_TARGETS})
target_include_directories(${PROJECT_NAME} PRIVATE ${XMAKE_FREEIMAGE_INCLUDE_PATHS})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${XMAKE_FREEIMAGE_COMPILE_DEFINITIONS})
