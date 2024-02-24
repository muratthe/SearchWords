# psearch - Word Search

## Overview
The `psearch` program is a command-line utility designed to search for a given alphanumeric keyword in a set of input text files. It outputs the matching lines along with the name of the input file and the line number where the match is found.

## Usage
psearch <keyword> <number_of_files> <input_file_1> <input_file_2> ... <input_file_n> <output_file>

- `<keyword>`: Alphanumeric keyword to search for as a separate word.
- `<number_of_files>`: Number of input files to be processed (n).
- `<input_file_1> <input_file_2> ... <input_file_n>`: Names of input text files containing lines to be searched.
- `<output_file>`: Name of the output file to store the matching lines.

## Example
psearch name 3 input1.txt input2.txt input3.txt output.txt

## Output Format
The output file will contain matched lines with the following format:
<input_filename>, <matched_line_number>: <matched_line>

## Example Output
input1.txt, 2: His name is Ali.
input2.txt, 5: Veli's name is not mentioned here.
input3.txt, 1: The name you're looking for is present.

## Sorting
The output is sorted first by `<input_filename>` and then by `<line_number>`.

## Notes
- The keyword is considered a match only if it appears as a separate word in the line.
- The output is sorted to provide a structured view of the matches.
