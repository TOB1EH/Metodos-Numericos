#!/bin/bash

# Script to detect OS and return appropriate compiler path

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    echo "/usr/bin/gcc"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    # Windows (MSYS/MinGW or Windows)
    echo "C:/mingw64/bin/gcc.exe"
else
    # Default fallback
    echo "gcc"
fi
