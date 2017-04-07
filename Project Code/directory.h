/*
 * directory.h
 *
 * This file contains the header declarations and
 * class definitions for the directory class. 
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
#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <iostream>
using namespace std;

class Directory {


	public:
		
		// Constructors
		Directory() { int size = 0; }
		Directory(string name, string path, string parentDirectory){
			this->name = name;
			this->path = path;// TODO HERE
			this->parentDirectory = parentDirectory;
			this->size = 0;
		}
		~Directory(){}

		// Getters and Setters
		string  getName() const { return name; }
		string  getPath() const { return path; }
		string  getParentDirectory() const { return parentDirectory; }
		int     getSize()       const { return size; }
		int     incSize(int bytes) { size += bytes; return size; }
		int     decSize(int bytes) { size -= bytes; return size; }
		void    setName(string name) { this->name = name; }
		void    setPath(string path) { this->path = path; }
		void    setParentDirectory(string parentDirectory) { this->parentDirectory = parentDirectory; }

		// File and directory Management Methods
		vector<File*>      getFiles()       const { return files; }
		vector<Directory*> getDirectories() const { return directories; }
		void addFile(File* newFile)               { this->files.push_back(newFile); }
		void addDirectory(Directory* newDirectory) { this->directories.push_back(newDirectory); }
		void removeFile(string name){
			
			for (int i = 0; i < files.size(); i++){
				if (name == files[i]->getName()){
					files.erase(files.begin() + i);
					break;
				}
			}
		}
		
		void removeDirectory(string name){
			
			for (int i = 0; i < directories.size(); i++){
				if (name == directories[i]->getName()){
					directories.erase(directories.begin() + i);
					break;
				}
			}
		}

		// Check to see if this directory contains specified directory.
		bool containsDirectory(string name) const {

			for (int i = 0; i < directories.size(); i++){
				if (directories[i]->getName() == name)
					return true;
			}
			return false;
		}

		// Check to see if this directory contains specified file.
		bool containsFile(string name) const {

			for (int i = 0; i < files.size(); i++){
				if (files[i]->getName() == name)
					return true;
			}
			return false;
		}

		// Get a child directory by name if it exists.
		Directory* getDirectory(string name) const {

			for (int i = 0; i < directories.size(); i++){
				if (directories[i]->getName() == name){
					return directories[i];			
				}
			}
			cerr << "Directory not found!" << endl;
			return NULL;
		}

		// Get a child file by name if it exists.
		File* getFile(string name) const {

			for (int i = 0; i < files.size(); i++){
				if (files[i]->getName() == name)
					return files[i];
			}
			cerr << "File not found!" << endl;
			return NULL;
		}

	private:

		int     size;
		string  name;
		string  path;
		string  parentDirectory;
		vector<File*>      files;
		vector<Directory*> directories;

};

#endif
