/*
 * driver.cpp
 *
 * This file serves as the driver for the ATOS file system,
 * which provides two modes of execution:
 * 	
 * 		1) CLI Mode: Run ATOS from the command line.
 * 		2) GUI Mode: Run the ATOS interactive GUI.
 *
 * CLI Mode can be run by calling the executable with no arguments.
 * ATOS can be run in GUI mode by adding a -gui flag after the executable,
 * like so:
 *
 * 		./ATOS -gui
 *
 * Team Members:
 * 		
 * 		Gabriel Miller
 * 		Kyung Song
 * 		Wade Dworaczyk
 *
 * 	Course: Operating Systems
 * 	Date:   4/3/2017
 *
 */
#include "filesystem.h"
#include <iostream>
#include <sstream>
using namespace std;

/*
 * UI Functions
 */

// Output a report about the current 
// directory and disk information.
bool report(FileSystem &fs){

	int index = 1;
	Directory* dir = fs.getRootDir();
	vector<File*> files = dir->getFiles();
	vector<Directory*> dirs = dir->getDirectories();

	// Set up report format
	cout << "#####################################" << endl;
	cout << "#     ATOS-FS Directory Listing     #" << endl;
	cout << "#####################################" << endl;

	// List directories first
	if (!dirs.empty()){
		for (int i = 0; i < dirs.size(); i++){
			cout << index 
				 << ": [DIR] " << dirs[i]->getName() 
				 << ", SIZE: " << dirs[i]->getSize() << endl;
		   index++;	
		}
	}

	// List files within folder
	if (!files.empty()){
		for (int i = 0; i < files.size(); i++){
			cout << index 
				 << ": "       << files[i]->getName() 
				 << ", SIZE: " << files[i]->getSize() << endl; 
			index++;
		}
	}

	// Display disk stats
	cout << endl;
	cout << "-- FREE SPACE:   " << fs.getBytesOpen() << " bytes" << endl;
    	cout << "-- TOTAL SPACE:  " << fs.getSize() << " bytes" << endl;
	cout << "-- PERCENT FREE: " << (fs.getBytesOpen()/fs.getSize()) * 100 << "%" << endl;

}

// Edit file's contents by appending
// terminal text to the file.
bool edit(string filename, FileSystem &fs){

	// Open the file.
	File* handle = fs.openFile(filename, "root", "w");	

	// Get multiline input
	string input = "";
	string line;

	while(getline(cin, line)){

		if (line != "exit")
			input += line + '\n';
		else 
			break;
	}

	cout << "INPUT: " << input << endl;
	return true;
}

// Output the contents of a file.
bool print(string file, FileSystem &fs){

}

int main(int argc, char *argv[]){

	string flag;
	if (argc == 2)
	   flag = argv[1];

		//////////////
		// CLI Mode //
		////////////// 
		if (argc == 1){

			cout << "Running ATOS in CLI mode..." << endl;
			FileSystem fs(20, 20);
			bool EXIT = false;

			// Set up prompt.
			while(!EXIT){

				bool result;
				string token;
				string input;
				stringstream ss;
				vector<string> tokens;

				cout << "$ ";
				getline(cin, input);
				ss.str(input);

				// Split input into tokens
				while (getline(ss, token, ' '))
					tokens.push_back(token);

				for (int i = 0; i < tokens.size(); i++){
				    cout << "Token[" << i << "]: " << tokens[i] << endl;
				}

					if (tokens[0] == "create"){
								// Create file in the current directory, or another path if contains "/"
								result = fs.createFile(tokens[1]); // TODO Screen to see if contains "/"

					} else if (tokens[0] == "delete"){ 
								// Delete file in the current directory, or another path if contains "/"
								result = fs.deleteFile(tokens[1]); // TODO Screen to see if contains "/"

					} else if (tokens[0] == "dir"){ 
								// Print report, including current directory info, free space, and file sizes
								result = report(fs);  // TODO Build function

					} else if (tokens[0] == "edit"){ 
								// Append text entered from command line to the specified file
								result = edit(tokens[1], fs); // TODO Build function

					} else if (tokens[0] == "type"){ 
								// Output the contents of the file to the screen
								result = print(tokens[1], fs); // TODO Build function

					} else if (tokens[0] == "exit"){ 
								// Terminate the program
								EXIT = true;
					} else{ 
								// Do nothing.

				}


				cout << "FLAG" << endl;

			}

		//////////////
		// GUI Mode //
		////////////// 
		} else if (argc == 2 && flag == "-gui"){
			
			// Awesomium implementation goes here.
			cout << "Running ATOS in GUI mode..." << endl;
			
		}

	return 0;	
}
