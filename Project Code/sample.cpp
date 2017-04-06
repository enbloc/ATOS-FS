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
			FileSystem fs(20, 20);

			// Reset the contents of entire disk	
			DiskProcessType* disk = fs.getDisk();
			DiskBlockType* write_buffer = new DiskBlockType(20);
			disk->enableLogging("disklog.txt");
			for (int j = 0; j < 20; j++){
				write_buffer->data[j] = '#';
			}
			for (int i = 0; i < 20; i++){
				disk->write(i, write_buffer);
			}

			// Display the contents of entire disk	
			DiskBlockType* read_buffer = new DiskBlockType(20);
			for (int i = 0; i < 20; i++){
				disk->read(i, read_buffer);
				if (i >= 0 && i <= 9) cout << " ";
				cout << "Block #" << i << ": ";
				for (int j = 0; j < 20; j++){
					cout << read_buffer->data[j] << " ";
				}
				cout << endl;
			}

			// Test API Functions
			fs.createFile("testFile", "root");
			File* handle = fs.openFile("testFile", "root", "w");
			char* write_buff = new char[25];
			char* read_buff  = new char[30];
			
			// Test Values
			write_buff[0] = 'G';
			write_buff[1] = 'A';
			write_buff[2] = 'B';
			write_buff[3] = 'E';
			write_buff[4] = ' ';
			write_buff[5] = 'W';
			write_buff[6] = 'A';
			write_buff[7] = 'S';
			write_buff[8] = ' ';
			write_buff[9] = 'H';
			write_buff[10] = 'E';
			write_buff[11] = 'R';
			write_buff[12] = 'E';
			write_buff[13] = '\0';
			write_buff[14] = '.';
			write_buff[15] = '.';
			write_buff[16] = '.';
			write_buff[17] = '.';
			write_buff[18] = '.';
			write_buff[19] = '.';
			write_buff[20] = 'Y';
			write_buff[21] = 'U';
			write_buff[22] = 'P';
			write_buff[23] = '.';
			write_buff[24] = '.';

			cout << "Writing to disk..." << endl;
			fs.writeFile(handle, 25, write_buff);
			fs.readFile(handle, 25, read_buff);

			cout << "Read buffer: ";
			for (int i = 0; i < 25; i++){
				cout << (char)read_buff[i];
			}
			cout << endl;

			fs.closeFile(handle);

			// Display the contents of entire disk	
			for (int i = 0; i < 20; i++){
				disk->read(i, read_buffer);
				if (i >= 0 && i <= 9) cout << " ";
				cout << "Block #" << i << ": ";
				for (int j = 0; j < 20; j++){
					cout << read_buffer->data[j] << " ";
				}
				cout << endl;
			}

			// Write another file.	
			fs.createFile("testFile2", "root");
			handle = fs.openFile("testFile2", "root", "w");
		
			// Test Values
			write_buff[0] = 'A';
			write_buff[1] = 'L';
			write_buff[2] = 'S';
			write_buff[3] = 'O';
			write_buff[4] = ' ';
			write_buff[5] = 'O';
			write_buff[6] = 'V';
			write_buff[7] = 'E';
			write_buff[8] = 'R';
			write_buff[9] = ' ';
			write_buff[10] = 'H';
			write_buff[11] = 'E';
			write_buff[12] = 'R';
			write_buff[13] = 'E';
			write_buff[14] = '!';
			write_buff[15] = '!';
			write_buff[16] = '!';

			cout << "Writing to disk..." << endl;
			fs.writeFile(handle, 16, write_buff);
			fs.readFile(handle, 16, read_buff);

			cout << "Read buffer: ";
			for (int i = 0; i < 16; i++){
				cout << (char)read_buff[i];
			}
			cout << endl;

			fs.closeFile(handle);

			// Display the contents of entire disk	
			for (int i = 0; i < 20; i++){
				disk->read(i, read_buffer);
				if (i >= 0 && i <= 9) cout << " ";
				cout << "Block #" << i << ": ";
				for (int j = 0; j < 20; j++){
					cout << read_buffer->data[j] << " ";
				}
				cout << endl;
			}

			// Read testFile again.
			handle = fs.openFile("testFile", "root", "r");
			fs.readFile(handle, handle->getSize(), read_buff);
			cout << "Read buffer: ";
			for (int i = 0; i < handle->getSize(); i++){
				cout << (char)read_buff[i];
			}
			cout << endl;

			fs.closeFile(handle);

			
			handle = fs.openFile("testFile", "root", "w");
			char* read_buff1  = new char[50];
			
			// Test Values
			write_buff[0] = 'T';
			write_buff[1] = 'H';
			write_buff[2] = 'I';
			write_buff[3] = 'S';
			write_buff[4] = ' ';
			write_buff[5] = 'I';
			write_buff[6] = 'S';
			write_buff[7] = ' ';
			write_buff[8] = 'J';
			write_buff[9] = 'U';
			write_buff[10] = 'S';
			write_buff[11] = 'T';
			write_buff[12] = ' ';
			write_buff[13] = 'S';
			write_buff[14] = 'O';
			write_buff[15] = 'M';
			write_buff[16] = 'E';
			write_buff[17] = ' ';
			write_buff[18] = 'E';
			write_buff[19] = 'X';
			write_buff[20] = 'T';
			write_buff[21] = 'R';
			write_buff[22] = 'A';
			write_buff[23] = '?';
			write_buff[24] = '!';

			cout << "Writing to disk..." << endl;
			fs.writeFile(handle, 25, write_buff);
			fs.readFile(handle, handle->getSize(), read_buff1);

			cout << "Read buffer: ";
			for (int i = 0; i < handle->getSize(); i++){
				cout << (char)read_buff1[i];
			}
			cout << endl;

			fs.closeFile(handle);

			// Display the contents of entire disk	
			for (int i = 0; i < 20; i++){
				disk->read(i, read_buffer);
				if (i >= 0 && i <= 9) cout << " ";
				cout << "Block #" << i << ": ";
				for (int j = 0; j < 20; j++){
					cout << read_buffer->data[j] << " ";
				}
				cout << endl;
			}

			fs.deleteFile("testFile", "root");
			cout << "Deleting testFile..." << endl;
			// Display the contents of entire disk	
			for (int i = 0; i < 20; i++){
				disk->read(i, read_buffer);
				if (i >= 0 && i <= 9) cout << " ";
				cout << "Block #" << i << ": ";
				for (int j = 0; j < 20; j++){
					cout << read_buffer->data[j] << " ";
				}
				cout << endl;
			}

			// Clean up.
			delete disk;

		//////////////
		// GUI Mode //
		////////////// 
		} else if (argc == 2 && flag == "-gui"){
			
			// Awesomium implementation goes here.
			cout << "Running ATOS in GUI mode..." << endl;
			
		}

	return 0;	
}
