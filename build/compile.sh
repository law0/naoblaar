if [ "$1" == "all" ]
then
	moc -o ../src/moc_mainwindow.cpp ../src/mainwindow.h
	moc -o ../src/moc_videoitem.cpp ../src/videoitem.h
	moc -o ../src/moc_qcustomplot.cpp ../src/qcustomplot.h
	moc -o ../src/moc_plotitem.cpp ../src/plotitem.h
	moc -o ../src/moc_slideritem.cpp ../src/slideritem.h
	moc -o ../src/moc_filelinebutton.cpp ../src/filelinebutton.h
fi
cmake ../src
make
