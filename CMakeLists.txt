cmake_minimum_required(VERSION 3.2)
project(BlankMat)

# Start project in PROJECT_NAME rather than ALL_BUILD or ZERO_CHECK
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Link .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
include_directories(${CMAKE_BINARY_DIR}/src)

# Group together shaders and config files
file(GLOB SHADER_FILES
    ${CMAKE_SOURCE_DIR}/resources/shaders/*.frag
    ${CMAKE_SOURCE_DIR}/resources/shaders/*.geom
    ${CMAKE_SOURCE_DIR}/resources/shaders/*.vert)
file(GLOB CONFIG_FILES
    ${CMAKE_SOURCE_DIR}/CMakeLists.txt
    ${CMAKE_SOURCE_DIR}/config.txt
    ${CMAKE_SOURCE_DIR}/Readme.md
    ${CMAKE_SOURCE_DIR}/.gitattributes
    ${CMAKE_SOURCE_DIR}/.gitignore)
file(GLOB_RECURSE RESOURCE_FILES
    ${CMAKE_SOURCE_DIR}/resources/models/*)

# Group up files to make IDE look nice
source_group("Sources" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("Shaders" FILES ${SHADER_FILES})
source_group("Configs" FILES ${CONFIG_FILES})
source_group("Resources" FILES ${RESOURCE_FILES})

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES} ${CONFIG_FILES} ${RESOURCE_FILES})

# Define module location
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# find the required packages
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")

# STB_IMAGE
add_library(STB_IMAGE "external/stb_image.cpp")

# GLAD
add_library(GLAD "external/glad.c")

# Combine all libs into one variable
set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} GLAD ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARY} STB_IMAGE)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy dlls
if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()