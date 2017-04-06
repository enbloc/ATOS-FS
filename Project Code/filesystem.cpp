/*
 * filesystem.cpp
 *
 * This file contains the class method implementations for the 
 * FileSystem class. 
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
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include "filesystem.h"
using namespace std;

// Primary Constructor
FileSystem::FileSystem(int blocksize, int numblocks){
	this->disk = new DiskProcessType(blocksize, numblocks);
	this->root = new Directory("root", "/root", "root");
	this->size = blocksize * numblocks;
	initFreeSpace(blocksize, numblocks);
}

// Create a file under the folder given in the path.
bool FileSystem::createFile(string path){

	string filename;
	size_t found = path.find("/");

	// Use path to retrieve the directory
	vector<string> path_tokens = parsePath(path);
	Directory* current_dir = getDirectory(path);

	if (found == string::npos)
		filename = path;
	else 
		filename = path_tokens[path_tokens.size() - 1];

	// Check to ensure file does not exist and is added
	if (current_dir->containsFile(filename) == false){

		File* file = new File(filename, "root", path_tokens[path_tokens.size() - 1]);
		current_dir->addFile(file);
		files.push_back(file);
		return true;

	} else {
		cerr << "Error: File already exists!" << endl;
		return false;
	}
}

// Delete the file under the folder given in the path.
bool FileSystem::deleteFile(string path){

	string filename;
	size_t found = path.find("/");

	// Use path to retrieve the directory
	vector<string> path_tokens = parsePath(path);
	Directory* current_dir = getDirectory(path);

	if (found == string::npos)
		filename = path;
	else 
		filename = path_tokens[path_tokens.size() - 1];

	File* file = getFile(path, filename);
	Directory* dir = getDirectory(path);
	file->releaseData(disk, freespace);
	dir->removeFile(file);
	for (int i = 0; i < files.size(); i++){
			if (files[i] == file){
				files.erase(files.begin() + i);                 
				break;
		}           
	}
	return true;
}

// Open the file under the folder given in the path in read or write mode.
File* FileSystem::openFile(string filename, string path, string mode){

//	string filename;
	size_t found = path.find("/");

	// Use path to retrieve the directory
	vector<string> path_tokens = parsePath(path);
	Directory* current_dir = getDirectory(path);

//	if (found == string::npos)
//		filename = path;
//	else 
//		filename = path_tokens[path_tokens.size() - 1];

	File* file = getFile(path, filename);
	file->open(mode);
	return file;
}

// Close the file with the given handle.
bool FileSystem::closeFile(File* handle){
	return handle->close();
}

// Read the file with the given handle into the buffer.
int FileSystem::readFile(File* handle, int numchars, char *buffer){

	if (handle->getStatus() == "open"){
		return handle->read(numchars, buffer, disk);
	}
}

// Write to the file with the given handle from the buffer.
int FileSystem::writeFile(File* handle, int numchars, char *buffer){

	if (handle->getStatus() == "open" && 
		handle->getMode()   == "w"){

			return handle->write(numchars, buffer, disk, freespace);
	} else {
		cerr << "Error: Cannot write to file, invalid mode" << endl;
		return -1;
	}
}

// Return the size of the file under the given path.
int FileSystem::fileStats(string filename, string path){

}

// Get a list of all of the files on the disk.
vector<string> FileSystem::listAllFiles(){

	vector<string> file_names;
	for (int i = 0; i < files.size(); i++){
		file_names.push_back(files[i]->getName());
	}

	return file_names;
}

// Get a pointer to a specific file found under the given path.
File* FileSystem::getFile(string path, string filename) {

	// Crawl to find relevant directory
	Directory* current_dir = root;
	vector<string> path_tokens = parsePath(path);
	if (path_tokens.size() > 1){
	    for (int i = 0; i < path_tokens.size(); i++){
	    	current_dir = current_dir->getDirectory(path_tokens[i]);
	    	if (current_dir == NULL){
	    		cerr << "Error retrieving file from directory" << endl;
	    		break;
	    	}
	    }
	}

	// If the file is contained within the directory, return it.
	File* file = current_dir->getFile(filename);
	return file;
}

// Get a pointer to the specific directory found under the given path.
Directory* FileSystem::getDirectory(string path) {

	Directory* current_dir = root;
	vector<string> path_tokens = parsePath(path);
	for (int i = 1; i < path_tokens.size() - 1; i++){
		current_dir = current_dir->getDirectory(path_tokens[i]);
		if (current_dir == NULL){
			cerr << "Error retrieving file from directory" << endl;
			break;
		}
	}

	return current_dir;
}

// Parse the given path into an array of directories
// so that they can be iterated through in order to 
// find a specific file or folder. 
vector<string> FileSystem::parsePath(string path){

	vector<string> path_tokens;
	pathSplit(path, path_tokens);
	return path_tokens;

}

// Split the path into strings using the '/' as a delimiter.
void FileSystem::pathSplit(string path, vector<string> &result){

	char delim = '/';
	stringstream ss;
	ss.str(path);
	string token;

	while (getline(ss, token, delim))
		result.push_back(token);
}

// Initialize the free space list.
void FileSystem::initFreeSpace(int blocksize, int numblocks){

	for (int i = 0; i < numblocks; i++){
		for (int j = 0; j < blocksize; j++){
			Byte* byte = new Byte();
			byte->block = i;
			byte->byte  = j;
			freespace.push_back(byte);
		}
	}
}

// Getters and Setters
int FileSystem::getSize() const { return size; }
int FileSystem::getBytesUsed() const { return size - freespace.size(); }
int FileSystem::getBytesOpen() const { return freespace.size(); }
Directory* FileSystem::getRootDir() const { return root; }
Directory* FileSystem::getCurrentDir() const { return current; }
DiskProcessType* FileSystem::getDisk() const { return disk;    }





