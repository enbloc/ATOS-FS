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
	this->current = this->root;
	initFreeSpace(blocksize, numblocks);
}

// Path utility function that makes hooking up to all of the necessary object pointers easy.
void FileSystem::pathUtil(string path, string &objName, Directory* &parent, bool &found){
	
	found = true;
	parent = root;
	size_t slash_num = path.find("/");	
	vector<string> path_tokens = parsePath(path);

	// If string does not contain a /, then we assume that
	// it is a local call to the current directory.
	if (slash_num == string::npos){ 
		objName = path;
		parent = current;
		cout << "Adding path to arg: " << path << " -> " << current->getPath() << "/" << path << endl;

	// Otherwise, assign the last token for the object name.
	} else objName = path_tokens[path_tokens.size() - 1];
		
	// Traverse through the directory by following the path,
	// breaking at any point at which the path token does not exist.
	for (int i = 1; i < path_tokens.size() - 1; i++){
		if (parent->containsDirectory(path_tokens[i]))
			parent = parent->getDirectory(path_tokens[i]);
		else {
			cerr << "Error: Directory does not exist at specified path!" << endl;
			found = false;
			break;
		}
	}
}

// Create a file under the folder given in the path.
bool FileSystem::createFile(string path){

	bool found;
	string filename;
	Directory* parent;

	// Verify and assign values to variables
	pathUtil(path, filename, parent, found);

	if (found){

	    // Check to ensure file does not exist and is added
	    File* file = new File(filename, path, parent->getName());
	    parent->addFile(file);
	    files.push_back(file);
	    return true;

	} else {
		cerr << "Error: File already exists!" << endl;
		return false;
	}
}

// Delete the file under the folder given in the path.
bool FileSystem::deleteFile(string path){

	bool found;
	string filename;
	Directory* parent;

	// Verify and assign values to variables
	pathUtil(path, filename, parent, found);

	if (found){

	    // Check to ensure file exists and is deleted
	    File* file = getFile(path, filename);
	    file->releaseData(disk, freespace);
	    parent->removeFile(filename);
	    for (int i = 0; i < files.size(); i++){
	     	if (files[i]->getName() == filename){
	       		files.erase(files.begin() + i);                 
	       		break;
	       	}           
	    }

	} else {
		cerr << "Error: File does not exist!" << endl;
		return false;
	}
}

// Open the file under the folder given in the path in read or write mode.
File* FileSystem::openFile(string path, string mode){

	bool found;
	string filename;
	Directory* parent;

	// Verify and assign values to variables
	pathUtil(path, filename, parent, found);

	if (found){

	    File* file = getFile(path, filename);
	    file->open(mode);
	    return file;

	} else {
		cerr << "Error: Attempted to open file that does not exist!" << endl;
		return NULL;
	}

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

// Set the current directory.
Directory* FileSystem::changeDir(string path){

	bool found = true;
	Directory* current_dir = root;
	vector<string> path_tokens = parsePath(path);
	for (int i = 1; i < path_tokens.size(); i++){
		if (current_dir->containsDirectory(path_tokens[i]))
			current_dir = current_dir->getDirectory(path_tokens[i]);
		else {
			cerr << "Error: Directory does not exist at specified path!" << endl;
			found = false;
			break;
		}
	}

	this->current = current_dir;
	if (found) return current_dir;
	else       return NULL;
}

// Create a directory at the specified path.
Directory* FileSystem::makeDir(string path){

	bool found = true;
	Directory* dir;
	Directory* current_dir = root;
	vector<string> path_tokens = parsePath(path);
	for (int i = 1; i < path_tokens.size() - 1; i++){
		if (current_dir->containsDirectory(path_tokens[i]))
			current_dir = current_dir->getDirectory(path_tokens[i]);
		else {
			cerr << "Error: Directory does not exist at specified path!" << endl;
			found = false;
			break;
		}
	}

	if (found){
		dir = new Directory(path_tokens[path_tokens.size() - 1],
					   		current_dir->getPath(),
							current_dir->getName()); //TODO here
		current_dir->addDirectory(dir);
		return dir;
	} else {
		return NULL;
	}

}

// Remove the directory at the specified path.
Directory* FileSystem::removeDir(string path){
	
	bool found = true;
	Directory* current_dir = root;
	vector<string> path_tokens = parsePath(path);
	for (int i = 1; i < path_tokens.size() - 1; i++){
		if (current_dir->containsDirectory(path_tokens[i]))
			current_dir = current_dir->getDirectory(path_tokens[i]);
		else {
			cerr << "Error: Directory does not exist at specified path!" << endl;
			found = false;
			break;
		}
	}

	if (found){
		current_dir->removeDirectory(path_tokens[path_tokens.size() - 1]);
		return current_dir;
	} else {
		return NULL;
	}

}

// Getters and Setters
int FileSystem::getSize() const { return size; }
int FileSystem::getBytesUsed() const { return size - freespace.size(); }
int FileSystem::getBytesOpen() const { return freespace.size(); }
Directory* FileSystem::getRootDir() const { return root; }
Directory* FileSystem::getCurrentDir() const { return current; }
DiskProcessType* FileSystem::getDisk() const { return disk;    }





