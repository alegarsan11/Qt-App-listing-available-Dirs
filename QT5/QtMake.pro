# simselector.pro
QT       += core gui widgets

CONFIG   += c++11

# Input
SOURCES  += main.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h

# For QProcess
QT       += core

TARGET = SimSelector
TEMPLATE = app