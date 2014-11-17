#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "ColumnCreator.h"


using namespace std;

// constructor with one param - without saving output into file
ColumnCreator::ColumnCreator(string in_file) {
	file_path = in_file;
	input_file.open(file_path.c_str(), fstream::in); // open file for reading
	readParamsFromFile();
	split2ColumnLine();
	creatColumns();
}

// constructor, reads file; constructor with two params
ColumnCreator::ColumnCreator(string in_file, string out_file) {
	file_path = in_file;
	input_file.open(file_path.c_str(), fstream::in); // open file for reading
	readParamsFromFile();
	split2ColumnLine();
	creatColumns();
	save2File(out_file);
}


// reads first line from file with params and put values into 
// private variables
void ColumnCreator::readParamsFromFile(void) {
	string param_line;
	getline(input_file, param_line); // read 1st line with params
	
	size_t found = 0; // position of found char
	size_t prev_found = 0; // previous found position
	
	int param; // current read param  already converted to integer
	
	int i = 1; // this is only to get know which parameter is reading right now. 1 = cols number, 2 = col width, 3 = space width

	// nice loop which put each number form param_line to each variable
	while (found != string::npos) {
		found = param_line.find_first_not_of("0123456789", found+1); // find first non numerci character (in this case space)
		
		param = atoi(param_line.substr(prev_found, found-prev_found).c_str()); // each variable converted to int
		
		// write param to specyfic variable
		if(i == 1) {
			cols_number = param;
		} else if(i == 2) {
			col_width = param;
		} else if(i == 3) {
			space_width = param;
		}
		i++;
		
		prev_found = found+1; // remember index of last separate char (space) and add one to skip this char
		
	}
	
}


// read rest of file and splited into specyfic column lenght lines;
void ColumnCreator::split2ColumnLine() {
	string whole_file = ""; // Whole file in one string wariable;
	string line; // current line from file
	
	while (getline(input_file, line)) {
		whole_file += line;
		
		if(!input_file.eof()) {
			// all lines except last line
			whole_file += " "; // add space in the ond of line;
		} 
	}
	
	// close file with input data
	input_file.close();
		
	size_t found = 0;
	size_t prev_found = -1;
	
	string temp_line; // temporary variable to make actions easier
	
	while (found != string::npos) {
		found = whole_file.find(' ', found+1);
		
		if (found > col_width) { // if we found the space char which is farer than col_width we need to go to previus space
			temp_line = whole_file.substr(0, prev_found);
			
			temp_line.resize(col_width, ' ');  // fulfil line with spaces up to 25 character
			
			lines.push_back(temp_line);
			whole_file = whole_file.substr(prev_found+1); // delete the begining of whle file and start again
			
			if (whole_file.length() <= col_width) { // if the rest is less the the line just put last line into vector lines
				whole_file.resize(col_width, ' ');
				lines.push_back(whole_file);
				break; // and stop the loop
			}

			// time to zero number variable
			prev_found = 0; 
			found = 0;
		} else {
			prev_found = found;
		}
	}
	
}


void ColumnCreator::creatColumns(void) {
	columns = "";
	string spaces = "";
	spaces.resize(space_width, ' ');
	
	int lines_per_col;
	
	if (lines.size() % cols_number != 0) { // no equal amount of lines in column
		lines_per_col = (int)(lines.size() / cols_number + 1);
		
	} else {
		lines_per_col = (int)(lines.size() / cols_number);
	}
	
	for (int i = 0; i < lines_per_col; i++) {
		for (int j = 0; j < cols_number; j++) {
			
			if (i+lines_per_col*j >= lines.size()) { // sometime last column is shorter than oters
				break;
			}
			
			
			if(j == cols_number) {
				columns += lines[i+lines_per_col*j];			
			} else {
				columns += lines[i+lines_per_col*j] + spaces;
			}
		}
		
		columns += "\n";
	}
	
	cout << columns; // print on screen ready columns
}


// save ready columns to file
void ColumnCreator::save2File(std::string output_filename) {
	output_file.open(output_filename.c_str()); // open file for output data
	output_file << columns; // save to file
}


ColumnCreator::~ColumnCreator(void) {
	// if input file is open than close it
	if(input_file.is_open() == true) {
		input_file.close();
	}
	
	// like above but output file
	if(output_file.is_open() == true) {
		output_file.close();
	}
}
