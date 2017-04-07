/*
 * interface.cpp
 *
 * This source file provides the method implementations
 * for the interface class.
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
#include "interface.h"
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

// Constructor
Interface::Interface(FileSystem* fs){
	this->fs = fs;
}

// Destructor
Interface::~Interface(){
	// TODO
}

// Create a file in the specified directory.
bool Interface::create(string path){
	return fs->createFile(path);
}

// Delete a file in the specified directory.
bool Interface::remove(string path){
	return fs->deleteFile(path);
}

// Edit file's contents by appending
// terminal text to the file.
bool Interface::edit(string path){

	// Open the file.
	File* handle = fs->openFile(path, "w");	

	// Get multiline input
	string input = "";
	string line;

	while(getline(cin, line)){
		if (line != "exit") input += line + '\n';
		else break;
	}

	char* buffer = new char[input.length() + 1];
	strcpy(buffer, input.c_str());
	
	fs->writeFile(handle, input.length() + 1, buffer);
	fs->closeFile(handle);

	delete [] buffer;
	return true;
}

// Output the contents of a file.
bool Interface::type(string path){

	// Open the file.
	File* handle = fs->openFile(path, "r");
	char* buffer = new char[handle->getSize()];

	// Read and close the file.
	fs->readFile(handle, handle->getSize(), buffer);
	fs->closeFile(handle);

	for (int i = 0; i < handle->getSize(); i++){
		cout << buffer[i];
	}

	delete [] buffer;
	return true;
}

// Change the current directory to the specified path.
bool Interface::cd(string path){
	Directory* dir = fs->changeDir(path);
	if (dir != NULL) return true;
	else             return false;
}

// Create a directory at the specified path.
bool Interface::mkdir(string path){
	Directory* dir = fs->makeDir(path);
	if (dir != NULL) return true;
	else             return false;
}

// Remove the directory at the specified path.
bool Interface::rmdir(string path){
	Directory* dir = fs->removeDir(path);
	if (dir != NULL) return true;
	else             return false;

}

// Output a report about the current 
// directory and disk information.
bool Interface::dir(){

	int index = 1;
	Directory* dir = fs->getCurrentDir();
	vector<File*> files = dir->getFiles();
	vector<Directory*> dirs = dir->getDirectories();

	// Set up report format
	cout << endl;
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
	cout << "-- FREE SPACE:   " << fs->getBytesOpen() << " bytes" << endl;
    cout << "-- TOTAL SPACE:  " << fs->getSize() << " bytes" << endl;
	cout << "-- PERCENT FREE: " << ((double)fs->getBytesOpen()/(double)fs->getSize()) * (double)100 << "%" << endl;
	cout << endl;
}

