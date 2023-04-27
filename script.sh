#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 file.c"
    exit 1
fi

file=$1
if [ ! -f "$file" ]; then
    echo "$file does not exist or is not a regular file"
    exit 1
fi

if [[ "$file" != *.c ]]; then
    echo "$file is not a C file"
    exit 1
fi

# Compile the file and capture the output of the compilation
output=$(gcc -Wall "${file%.c}" "$file" 2>&1)

# Print the output of the compilation
#echo "$output"

# Count the number of errors and warnings
errors=$(echo "$output" | grep -c "error")
warnings=$(echo "$output" | grep -c "warning")

echo "$file: $errors errors, $warnings warnings"
