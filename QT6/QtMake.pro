QT       += core gui widgets multimedia multimediawidgets

# Set the target name and specify the application template
TARGET = Launcher
TEMPLATE = app

# Specify the source files
SOURCES += mainWindow.cpp
HEADERS += mainwindow.h

# Include OpenCV settings
CONFIG += c++11

# Set the include path for OpenCV
INCLUDEPATH += /usr/include/opencv4
INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt6/QtMultimedia

# Link against OpenCV libraries
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgcodecs \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_videoio