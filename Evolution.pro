#-------------------------------------------------
#
# Project created by QtCreator 2015-10-28T16:15:19
#
#-------------------------------------------------

QT       += core gui


CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Evolution
TEMPLATE = app

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE += -O3

SOURCES += main.cpp\
        mainwindow.cpp \
    cell.cpp \
    population.cpp \
    game.cpp \
    simwindow.cpp \
    simwidget.cpp \
    data.cpp \
    genotype.cpp \
    labwindow.cpp \
    evolveinfo.cpp

HEADERS  += mainwindow.h \
    cell.h \
    population.h \
    game.h \
    simwindow.h \
    simwidget.h \
    data.h \
    genotype.h \
    labwindow.h \
    evolveinfo.h

FORMS    += mainwindow.ui \
    simwindow.ui \
    simwidget.ui \
    labwindow.ui \
    evolveinfo.ui
