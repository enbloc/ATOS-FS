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
#include "interface.h"
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

int main(int argc, char *argv[]){

	string flag;
	if (argc == 2)
	   flag = argv[1];

		//////////////
		// CLI Mode //
		////////////// 
		if (argc == 1){

			cout << "Running ATOS in CLI mode..." << endl;
			FileSystem* fs = new FileSystem(20, 20);
			Interface*  ui = new Interface(fs);
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

			    if (tokens[0] == "create"){
			    			result = ui->create(tokens[1]); // TODO Screen to see if contains "/"

			    } else if (tokens[0] == "delete"){ 
			    			result = ui->remove(tokens[1]); // TODO Screen to see if contains "/"

			    } else if (tokens[0] == "dir"){ 
			    			result = ui->dir();  

			    } else if (tokens[0] == "edit"){ 
			    			result = ui->edit(tokens[1]); 

			    } else if (tokens[0] == "type"){ 
			    			result = ui->type(tokens[1]); 

			    } else if (tokens[0] == "cd"){ 
			    			result = ui->cd(tokens[1]); 

			    } else if (tokens[0] == "mkdir"){ 
			    			result = ui->mkdir(tokens[1]); 

			    } else if (tokens[0] == "rmdir"){ 
			    			result = ui->rmdir(tokens[1]); 

			    } else if (tokens[0] == "exit"){ 
			    			EXIT = true;

			    } else{ 
			    			// Do nothing.

				}

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
