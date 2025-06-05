#!/bin/bash

# === Configuration ===
SOURCE_DIR="$(cd ./sources && pwd)"
BUILD_DIR="./build"
EXE_NAME="app_release"
CPP_LIST="c_files.txt"
OBJ_LIST="obj_files.txt"

# Choose compiler (default: clang++)
if [[ "$1" == "gcc" ]]; then
    COMPILER="g++"
else
    COMPILER="clang++"
fi

# Compiler flags (Release build: no RTTI, no exceptions)
CFLAGS="-O2 -Wall -Wextra -std=c++14 -fno-rtti -fno-exceptions -I$SOURCE_DIR -c "
LDFLAGS="-O2"


if [[ "$1" == "gcc" ]]; then
    CFLAGS="$CFLAGS"
else
    CFLAGS="$CFLAGS -stdlib=libc++"
    LDFLAGS="$LDFLAGS -lc++abi"
fi


# Create build directory
mkdir -p "$BUILD_DIR"
rm -f "$CPP_LIST" "$OBJ_LIST"

# Clean previous build
echo "Cleaning $BUILD_DIR..."
rm -f "$BUILD_DIR"/*.o "$BUILD_DIR/$EXE_NAME"

# Find all .cpp files
echo "Finding source files..."
find "$SOURCE_DIR" -name '*.c' > "$CPP_LIST"

# === Start timer ===
start_time=$(date +%s.%N)

# === Compile ===
echo "Compiling source files..."
while read -r src_file; do
    obj_file="$BUILD_DIR/$(basename "${src_file%.cpp}.o")"
    echo "Compiling $src_file -> $obj_file"
    $COMPILER $CFLAGS "$src_file" -o "$obj_file" || { echo "Compile error in $src_file"; exit 1; }
    echo "$obj_file" >> "$OBJ_LIST"
done < "$CPP_LIST"

# === Link ===
echo "Linking with $COMPILER..."
$COMPILER $LDFLAGS $(cat "$OBJ_LIST") -o "$BUILD_DIR/$EXE_NAME" || { echo "Link error"; exit 1; }

# === End timer ===
end_time=$(date +%s.%N)
elapsed=$(echo "$end_time - $start_time" | bc)

echo "Release build complete: $BUILD_DIR/$EXE_NAME"
echo "Build time: ${elapsed}s"
