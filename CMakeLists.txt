cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif()

set(PROJ_NAME fxt)
set(SRC .)

project(${PROJ_NAME})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

if(CMAKE_C_COMPILER_ID STREQUAL MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MT /Oy /Oi /W3 /FAcs /GF /fp:fast /wd4996 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MT /Oy /Oi /W3 /FAcs /GF /fp:fast /wd4996 /D_CRT_SECURE_NO_DEPRECATE /D_CRT_SECURE_NO_WARNINGS")
	add_definitions(-DHAVE_FAST_MATH)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL GNU OR CMAKE_C_COMPILER_ID STREQUAL Clang)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -ffast-math")
	set(MATHLIB m)
endif()

if(MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -static-libstdc++ -static-libgcc")
endif()

include_directories(
	${SRC}
	${SDL2_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
)

add_executable(${PROJ_NAME} WIN32
	${SRC}/main.c
	${SRC}/gl_2_1.c
)

target_link_libraries(${PROJ_NAME}
	${SDL2_LIBRARIES}
	${MATHLIB}
	${OPENGL_LIBRARIES}
)
