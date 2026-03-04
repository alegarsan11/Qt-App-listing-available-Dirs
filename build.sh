#!/bin/bash

# Root folder
ROOT=$(pwd)

# Create main build folder
mkdir -p build

# ---- Build Qt5 ----
echo "Building Qt5..."
mkdir -p build/qt5

# Run qmake from the QT5 folder, but generate Makefile in build/qt5
cd "$ROOT/QT5"
qmake QtMake.pro -o "$ROOT/build/qt5/Makefile"

# Build from the build/qt5 folder
make -C "$ROOT/build/qt5"

cd "$ROOT"

# ---- Build Qt6 ----
echo "Building Qt6..."
mkdir -p build/qt6

# Run qmake from the QT6 folder, generate Makefile in build/qt6
cd "$ROOT/QT6"
qmake QtMake.pro -o "$ROOT/build/qt6/Makefile"

# Build from the build/qt6 folder
make -C "$ROOT/build/qt6"

cd "$ROOT"

# Move or rename final executable if needed
if [ -f build/qt6/build/SimSelector ]; then
    mv build/qt6/build/SimSelector build/SimSelectorQT6
fi
# Move or rename final executable if needed
if [ -f build/qt5/build/SimSelector ]; then
    mv build/qt5/build/SimSelector build/SimSelectorQT5
fi

echo "Build finished!"