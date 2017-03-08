# Copyright  ETIS — ENSEA, Université de Cergy-Pontoise, CNRS (2011 - 2016)
# Author: Arnaud Blanchard
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MACOSX_RPATH 0) #avoid warning in MACOSX
#get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME) #The name of the project is the basename of the directory
project(nao_blaar)

find_package(blc_image REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Core)

#Add your properties with the ones of BL_...
add_definitions(-Wall ${BL_DEFINITIONS})
include_directories(${BL_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} my_optical_flow.cpp streamcatcher.cpp utilities.cpp)
target_link_libraries(${PROJECT_NAME} ${BL_LIBRARIES} z -lsfml-system -lsfml-graphics -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching Qt5::Widgets)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -fno-inline -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fno-inline -std=c++11")

qt5_wrap_cpp(${PROJECT_NAME}_SRC ${HW_HEADER_DIR}/MainWindow.h)
qt5_wrap_ui(${PROJECT_NAME}_UI ${HW_GUI_DIR}/MainWindow.ui)

qt5_use_modules(${PROJECT_NAME} Widgets)
