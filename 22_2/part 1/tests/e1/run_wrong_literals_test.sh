#!/bin/bash

echo "----------------------------------------"
echo "Running tests for incorrect literals..."

# Path to input and expected output files
input_file="tests/e1/inputs/wrong_literals_test.txt"
expected_output_file="tests/e1/expected_outputs/wrong_literals_test.out"

# Run the program and store the actual output in a variable
actual_output=$(./etapa1 "$input_file")

# Store the expected output in a variable
expected_output=$(cat "$expected_output_file")

# Compare the actual output with the expected output
if [ "$actual_output" == "$expected_output" ]; then
    echo "Test for incorrect literals passed successfully!"
    echo "----------------------------------------"
else
    echo "Test failed!"
    echo "---- Generated Output ----"
    echo "$actual_output"
    echo "--------------------------"
    echo "---- Expected Output ----"
    echo "$expected_output"
    echo "--------------------------"
fi