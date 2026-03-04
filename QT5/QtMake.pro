QT       += core gui widgets

CONFIG   += c++17

# Tell qmake where to find headers
INCLUDEPATH += include

# Sources
SOURCES += src/main.cpp \
           src/mainWindow.cpp

# Headers
HEADERS += include/mainwindow.h

# Target
TARGET = SimSelector
TEMPLATE = app

# Optional: put the build files into a separate folder
DESTDIR = build

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR  = build/ui