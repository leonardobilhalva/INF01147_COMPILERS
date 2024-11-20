#!/bin/bash

echo "----------------------------------------"
echo "Running tests for reserved keywords..."

# Path to input and expected output files
input_file="tests/e1/inputs/reserved_keywords_test.txt"
expected_output_file="tests/e1/expected_outputs/reserved_keywords_test.out"

# Run the program and store the actual output in a variable
actual_output=$(./etapa1 "$input_file")

# Store the expected output in a variable
expected_output=$(cat "$expected_output_file")

# Compare the actual output with the expected output
if [ "$actual_output" == "$expected_output" ]; then
    echo "Test for reserved keywords passed successfully!"
    echo "----------------------------------------"
else
    echo "Test failed!"
    echo "---- Generated Output ----"
    echo "$actual_output"
    echo "--------------------------"
    echo "---- Expected Output ----"
    echo "$expected_output"
    echo "--------------------------"
    echo "----------------------------------------"
fi