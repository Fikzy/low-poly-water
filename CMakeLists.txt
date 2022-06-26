cmake_minimum_required(VERSION 3.16)
project(POGL)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(
	${PROJECT_SOURCE_DIR}/src
)

add_subdirectory(glad/)

file(GLOB all_SRCS
	"${PROJECT_SOURCE_DIR}/src/*.hh"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(POGL ${all_SRCS})

target_link_libraries(POGL
	PUBLIC
	glfw

	PRIVATE
	glad
)

file(GLOB SHADERS "shaders/*")
file(COPY ${SHADERS} DESTINATION ${PROJECT_SOURCE_DIR}/build)
