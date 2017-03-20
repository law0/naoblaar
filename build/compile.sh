if [ "$1" == "all" ]
then
	moc -o ../src/moc_mainwindow.cpp ../src/mainwindow.h
	moc -o ../src/moc_videoitem.cpp ../src/videoitem.h
	moc -o ../src/moc_qcustomplot.cpp ../src/qcustomplot.h
	moc -o ../src/moc_plotitem.cpp ../src/plotitem.h
fi
cmake ../src
make
