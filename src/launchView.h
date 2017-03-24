#ifndef LAUNCHVIEW_H_INCLUDED
#define LAUNCHVIEW_H_INCLUDED

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "utilities.h"
#include "streamcatcher.h"
#include "mainwindow.h"
#include "videoitem.h"
#include "oscillator.h"
#include "plotitem.h"
#include "movie.h"
#include "launchView.h"
#include "oscillator.h"
#include "slideritem.h"

void launchView(int argc, char **argv, StreamCatcher *sc, Oscillator* osc, Oscillator* osc2);

#endif //LAUNCHVIEW_H_INCLUDED
