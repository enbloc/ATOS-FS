/*
 * directory.cpp
 *
 * This file contains the method implementations 
 * and for the Directory class. 
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
#include <vector>
#include <iostream>
#include "directory.h"
using namespace std;

// Constructors
Directory::Directory() { int size = 0; }
Directory::Directory(string name, string path, string parentDirectory){
	this->name = name;
	this->path = path;// TODO HERE
	this->parentDirectory = parentDirectory;
	this->size = 0;
}

// Destructor
Directory::~Directory(){

	// Clean up files in directory.
	for (auto it = files.begin(); it != files.end(); it++){
		delete *it;
	}
	files.clear();

	// Clean up directories contained in the directory
	for (auto it = directories.begin(); it != directories.end(); it++){
		delete *it;
	}
	directories.clear();
	
}

// Getters and Setters
string  Directory::getName() const { return name; }
string  Directory::getPath() const { return path; }
string  Directory::getParentDirectory() const { return parentDirectory; }
int     Directory::getSize()       const { return size; }
int     Directory::incSize(int bytes) { size += bytes; return size; }
int     Directory::decSize(int bytes) { size -= bytes; return size; }
void    Directory::setName(string name) { this->name = name; }
void    Directory::setPath(string path) { this->path = path; }
void    Directory::setParentDirectory(string parentDirectory) { this->parentDirectory = parentDirectory; }

// File and directory Management Methods
vector<File*>      Directory::getFiles()       const { return files; }
vector<Directory*> Directory::getDirectories() const { return directories; }
void Directory::addFile(File* newFile)               { this->files.push_back(newFile); }
void Directory::addDirectory(Directory* newDirectory) { this->directories.push_back(newDirectory); }
void Directory::removeFile(string name){
	
	for (int i = 0; i < files.size(); i++){
		if (name == files[i]->getName()){
			files.erase(files.begin() + i);
			break;
		}
	}
}

void Directory::removeDirectory(string name){
	
	for (int i = 0; i < directories.size(); i++){
		if (name == directories[i]->getName()){
			directories.erase(directories.begin() + i);
			break;
		}
	}
}

// Check to see if this directory contains specified directory.
bool Directory::containsDirectory(string name) const {

	for (int i = 0; i < directories.size(); i++){
		if (directories[i]->getName() == name)
			return true;
	}
	return false;
}

// Check to see if this directory contains specified file.
bool Directory::containsFile(string name) const {

	for (int i = 0; i < files.size(); i++){
		if (files[i]->getName() == name)
			return true;
	}
	return false;
}

// Get a child directory by name if it exists.
Directory* Directory::getDirectory(string name) const {

	for (int i = 0; i < directories.size(); i++){
		if (directories[i]->getName() == name){
			return directories[i];			
		}
	}
	cerr << "Directory not found!" << endl;
	return NULL;
}

// Get a child file by name if it exists.
File* Directory::getFile(string name) const {

	for (int i = 0; i < files.size(); i++){
		if (files[i]->getName() == name)
			return files[i];
	}
	cerr << "File not found!" << endl;
	return NULL;
}
