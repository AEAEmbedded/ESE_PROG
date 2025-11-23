#!/bin/bash
# Script to copy source files to all stage directories

BASE_DIR="$(cd "$(dirname "$0")" && pwd)"
SOURCE_DIR="$BASE_DIR/../BiggerLibraryCMake"

echo "Copying source files to all stages..."

for stage in stage1 stage2 stage3 stage4 stage5; do
    echo "  Copying to $stage..."

    # Copy C++ source files
    cp "$SOURCE_DIR/main.cpp" "$BASE_DIR/$stage/"
    cp "$SOURCE_DIR/sht45.cpp" "$BASE_DIR/$stage/"
    cp "$SOURCE_DIR/i2c.cpp" "$BASE_DIR/$stage/"
    cp "$SOURCE_DIR/logger.cpp" "$BASE_DIR/$stage/"
    cp "$SOURCE_DIR/config.cpp" "$BASE_DIR/$stage/"
    cp "$SOURCE_DIR/filter.cpp" "$BASE_DIR/$stage/"

    # Copy include directory
    cp -r "$SOURCE_DIR/include" "$BASE_DIR/$stage/"

    echo "  ✓ Done with $stage"
done

echo ""
echo "All source files copied successfully!"
echo "Each stage is now ready to build independently."
