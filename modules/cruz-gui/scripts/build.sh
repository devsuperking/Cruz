#!/bin/bash
set -e

mkdir -p out/build
cmake -S . -B out/build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build out/build --config Release

EXECUTABLE=out/build/CruzEngine

if [ -f "$EXECUTABLE" ]; then
    echo "Uruchamiam $EXECUTABLE ..."
    "$EXECUTABLE"
else
    echo "Błąd: nie znaleziono pliku wykonywalnego $EXECUTABLE"
    exit 1
fi
