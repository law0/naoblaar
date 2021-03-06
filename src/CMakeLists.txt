cmake_minimum_required(VERSION 2.8.11)
project(nao_app)

find_package(OpenCV REQUIRED)
find_package(Qt5Widgets)
find_package(Qt5Core)

add_definitions(-Wall)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(
	${PROJECT_NAME} 
	main.cpp 
	launchView.cpp
	oscillator.cpp
	streamcatcher.cpp 
	sharedmemory.cpp
	scriptlauncher.cpp
	utilities.cpp 
	moc_videoitem.cpp
	moc_mainwindow.cpp 
	moc_qcustomplot.cpp
	moc_plotitem.cpp
	moc_slideritem.cpp
	moc_filelinebutton.cpp
	mainwindow.cpp 
	filelinebutton.cpp
	videoitem.cpp
	qcustomplot.cpp
	plotitem.cpp
	movie.cpp
	slideritem.cpp
	savedata.cpp
	)

target_link_libraries(${PROJECT_NAME} z -lpthread -lsfml-system -lsfml-graphics -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching Qt5::Widgets)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -fno-inline -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fno-inline -std=c++11")

#set(CMAKE_AUTOMOC ON)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

#qt5_wrap_cpp(MAINWINDOW_SRC ${CMAKE_SOURCE_DIR}/mainwindow.h)
#qt5_wrap_ui(MAINWINDOW_UI ${CMAKE_SOURCE_DIR}/mainwindow.ui)

qt5_use_modules(${PROJECT_NAME} Widgets)
