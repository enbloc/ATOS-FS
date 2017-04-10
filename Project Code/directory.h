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
#include "file.h"

using namespace std;

class Directory {


	public:
		
		// Constructors
		Directory();
		Directory(string name, string path, string parentDirectory);
		~Directory();

		// Getters and Setters
		string  getName() const;
		string  getPath() const;
		string  getParentDirectory() const;
		int     getSize() const;
		int     incSize(int bytes);
		int     decSize(int bytes);
		void    setName(string name);
		void    setPath(string path);
		void    setParentDirectory(string parentDirectory);

		// File and directory Management Methods
		vector<File*>      getFiles()       const;
		vector<Directory*> getDirectories() const;
		void addFile(File* newFile);
		void addDirectory(Directory* newDirectory);
		void removeFile(string name);	
		void removeDirectory(string name);
		bool containsDirectory(string name) const;
		bool containsFile(string name) const;
		Directory* getDirectory(string name) const;
		File* getFile(string name) const;

	private:

		int     size;
		string  name;
		string  path;
		string  parentDirectory;
		vector<File*>      files;
		vector<Directory*> directories;

};

#endif
