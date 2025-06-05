#!/bin/bash

# === Configuration ===
SOURCE_DIR="$(cd ./sources && pwd)"
TEST_DIR="$(cd ./tests && pwd)"
BUILD_DIR="./build"
EXE_NAME="tests_debug"
C_LIST="tests_c_files.txt"
OBJ_LIST="tests_obj_files.txt"

# Choose compiler
if [[ "$1" == "gcc" ]]; then
    COMPILER="g++"
else
    COMPILER="clang++"
fi


# Compiler and linker flags for debug build
CFLAGS="-g -Wall -Wextra -std=c++14 -O0 -I$SOURCE_DIR -I$TEST_DIR -c"
LDFLAGS="-g"

# Create build directory
mkdir -p "$BUILD_DIR"
rm -f "$C_LIST" "$OBJ_LIST"

# Clean previous build
echo "Cleaning $BUILD_DIR..."
rm -f "$BUILD_DIR"/*.o "$BUILD_DIR/$EXE_NAME"

# Find all .cpp files
echo "Finding source files..."
find "$TEST_DIR" -name '*.c' > "$C_LIST"

# === Start timer ===
start_time=$(date +%s.%N)

# === Compile ===
echo "Compiling source files..."
while read -r src_file; do
    obj_file="$BUILD_DIR/$(basename "${src_file%.cpp}.o")"
    echo "Compiling $src_file -> $obj_file"
    $COMPILER $CFLAGS "$src_file" -o "$obj_file" || { echo "Compile error in $src_file"; exit 1; }
    echo "$obj_file" >> "$OBJ_LIST"
done < "$C_LIST"

# === Link ===
echo "Linking with $COMPILER..."
$COMPILER $LDFLAGS $(cat "$OBJ_LIST") -o "$BUILD_DIR/$EXE_NAME" || { echo "Link error"; exit 1; }

# === End timer ===
end_time=$(date +%s.%N)
elapsed=$(echo "$end_time - $start_time" | bc)

echo "Release build complete: $BUILD_DIR/$EXE_NAME"
echo "Build time: ${elapsed}s"
