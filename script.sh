#!/bin/bash

filename="$1"

# Compile the C file with the -fsyntax-only flag to check for errors and warnings
output=$(gcc -fsyntax-only "$filename" 2>&1)

# Extract the error and warning lines from the output
errors=$(echo "$output" | grep -i 'error' | wc -l)
warnings=$(echo "$output" | grep -i 'warning' | wc -l)

# Print the number of errors and warnings
echo "Number of errors: $errors"
echo "Number of warnings: $warnings"
