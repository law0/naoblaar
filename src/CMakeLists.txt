# Copyright  ETIS — ENSEA, Université de Cergy-Pontoise, CNRS (2011 - 2016)
# Author: Arnaud Blanchard
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MACOSX_RPATH 0) #avoid warning in MACOSX
#get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME) #The name of the project is the basename of the directory
project(nao_blaar)

find_package(blc_image REQUIRED) 

#Add your properties with the ones of BL_...
add_definitions(-Wall ${BL_DEFINITIONS})
include_directories(${BL_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} my_optical_flow.cpp)
target_link_libraries(${PROJECT_NAME} ${BL_LIBRARIES} z -lsfml-system -lsfml-graphics)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -fno-inline")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fno-inline")
