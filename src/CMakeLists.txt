cmake_minimum_required(VERSION 2.8.11)
project(nao_blaar)

find_package(blc_image REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Core)

add_definitions(-Wall ${BL_DEFINITIONS})

include_directories(${BL_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

add_executable(
	${PROJECT_NAME} 
	my_optical_flow.cpp 
	streamcatcher.cpp 
	utilities.cpp 
	mainwindow.cpp 
	moc_mainwindow.cpp 
	videoitem.cpp 
	moc_videoitem.cpp
	)

target_link_libraries(${PROJECT_NAME} ${BL_LIBRARIES} z -lsfml-system -lsfml-graphics -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching Qt5::Widgets)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -fno-inline -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fno-inline -std=c++11")

#set(CMAKE_AUTOMOC ON)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#qt5_wrap_cpp(MAINWINDOW_SRC ${CMAKE_SOURCE_DIR}/mainwindow.h)
#qt5_wrap_ui(MAINWINDOW_UI ${CMAKE_SOURCE_DIR}/mainwindow.ui)

qt5_use_modules(${PROJECT_NAME} Widgets)
