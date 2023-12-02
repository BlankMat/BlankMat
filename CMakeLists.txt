cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 20)
project(BlankMat)

# Start project in PROJECT_NAME rather than ALL_BUILD or ZERO_CHECK
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Link .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)
	
# Add rendering files
file(GLOB RENDERING_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/rendering/*.cpp)
file(GLOB RENDERING_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/rendering/*.h)
	
# Add primitive files
file(GLOB PRIMITIVE_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/primitives/*.cpp)
file(GLOB PRIMITIVE_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/primitives/*.h)

# Add interface files
file(GLOB INTERFACE_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/interfaces/*.cpp)
file(GLOB INTERFACE_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/interfaces/*.h)

# Add interaction files
file(GLOB INTERACTION_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/interaction/*.cpp)
file(GLOB INTERACTION_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/interaction/*.h)

# Add container files
file(GLOB CONTAINER_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/containers/*.cpp)
file(GLOB CONTAINER_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/containers/*.h)

# Add tool files
file(GLOB TOOL_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/tools/*.cpp)
file(GLOB TOOL_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/tools/*.h)

# Add file files
file(GLOB FILE_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/files/*.cpp)
file(GLOB FILE_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/files/*.h)
	
# Add window files
file(GLOB WINDOW_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/windows/*.cpp)
file(GLOB WINDOW_HEADER_FILES ${CMAKE_SOURCE_DIR}/src/windows/*.h)

# Add source files
file(GLOB GLOBAL_SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB GLOBAL_HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

set(SOURCE_FILES ${GLOBAL_SOURCE_FILES} ${RENDERING_SOURCE_FILES} ${FILE_SOURCE_FILES}
	${PRIMITIVE_SOURCE_FILES} ${INTERFACE_SOURCE_FILES} ${INTERACTION_SOURCE_FILES} ${CONTAINER_SOURCE_FILES} ${TOOL_SOURCE_FILES} ${WINDOW_SOURCE_FILES})
	
set(HEADER_FILES ${GLOBAL_HEADER_FILES} ${RENDERING_HEADER_FILES} ${FILE_HEADER_FILES}
	${PRIMITIVE_HEADER_FILES} ${INTERFACE_HEADER_FILES} ${INTERACTION_HEADER_FILES} ${CONTAINER_HEADER_FILES} ${TOOL_HEADER_FILES} ${WINDOW_HEADER_FILES})

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
    ${CMAKE_SOURCE_DIR}/configs/*
    ${CMAKE_SOURCE_DIR}/Readme.md
    ${CMAKE_SOURCE_DIR}/.gitattributes
    ${CMAKE_SOURCE_DIR}/.gitignore)
file(GLOB_RECURSE RESOURCE_FILES
    ${CMAKE_SOURCE_DIR}/resources/models/*)
set_source_files_properties(${RESOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

# Group up files to make IDE look nice
source_group("Sources" FILES ${GLOBAL_SOURCE_FILES})
source_group("Sources/Primitives" FILES ${PRIMITIVE_SOURCE_FILES})
source_group("Sources/Interfaces" FILES ${INTERFACE_SOURCE_FILES})
source_group("Sources/Interaction" FILES ${INTERACTION_SOURCE_FILES})
source_group("Sources/Containers" FILES ${CONTAINER_SOURCE_FILES})
source_group("Sources/Windows" FILES ${WINDOW_SOURCE_FILES})
source_group("Sources/Rendering" FILES ${RENDERING_SOURCE_FILES})
source_group("Sources/Tools" FILES ${TOOL_SOURCE_FILES})
source_group("Sources/Files" FILES ${FILE_SOURCE_FILES})

source_group("Headers" FILES ${GLOBAL_HEADER_FILES})
source_group("Headers/Primitives" FILES ${PRIMITIVE_HEADER_FILES})
source_group("Headers/Interfaces" FILES ${INTERFACE_HEADER_FILES})
source_group("Headers/Interaction" FILES ${INTERACTION_HEADER_FILES})
source_group("Headers/Containers" FILES ${CONTAINER_HEADER_FILES})
source_group("Headers/Windows" FILES ${WINDOW_HEADER_FILES})
source_group("Headers/Rendering" FILES ${RENDERING_HEADER_FILES})
source_group("Headers/Tools" FILES ${TOOL_HEADER_FILES})
source_group("Headers/Files" FILES ${FILE_HEADER_FILES})

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

# JsonCPP
add_library(JSONCPP "external/jsoncpp.cpp")

# Portable File Dialog
add_library(PFD "external/pfd_impl.cpp")

# IMGUI
set(IMGUI_DIR "external/imgui")
add_library(IMGUI "${IMGUI_DIR}/imgui.cpp"
	"${IMGUI_DIR}/imgui_demo.cpp"
	"${IMGUI_DIR}/imgui_draw.cpp"
	"${IMGUI_DIR}/imgui_tables.cpp"
	"${IMGUI_DIR}/imgui_widgets.cpp"
	"${IMGUI_DIR}/imgui_stdlib.cpp"
	"${IMGUI_DIR}/imgui_impl_glfw.cpp"
	"${IMGUI_DIR}/imgui_impl_opengl3.cpp")

# Combine all libs into one variable
set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} GLAD ${CMAKE_DL_LIBS} ${ASSIMP_LIBRARY} STB_IMAGE IMGUI JSONCPP PFD)

# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/include"
	"${CMAKE_SOURCE_DIR}/include/glad"
	"${CMAKE_SOURCE_DIR}/include/glm"
	"${CMAKE_SOURCE_DIR}/include/imgui"
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