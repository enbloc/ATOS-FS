/*
 * filesystem.h
 *
 * This file contains the header declarations for the 
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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include "file.h"
#include "directory.h"
#include "diskprocess.h"
using namespace std;

class FileSystem {


	public:

		// Constructors
		FileSystem(int blocksize, int numblocks);
		~FileSystem(){}

		// Required API Methods
		bool  createFile(string path);
		bool  deleteFile(string path);
		File* openFile  (string filename, string path, string mode);
		bool  closeFile(File* handle);
		int   readFile (File* handle, int numchars, char *buffer);
		int   writeFile(File* handle, int humchars, char *buffer);
		int   fileStats(string filename, string path);
		vector<string> listAllFiles();

		// Additional Class Methods
		File*      getFile(string path, string filename);
		Directory* getDirectory(string path);
		vector<string> parsePath(string path);
		void pathSplit(string path, vector<string> &result);
		void initFreeSpace(int blocksize, int numblocks);
		
		// Getters and Setters
		int getSize() const;
		int getBytesUsed() const;
		int getBytesOpen() const;
		Directory* getRootDir() const;
		Directory* getCurrentDir() const;
		DiskProcessType* getDisk() const;

	private:

		DiskProcessType* disk;
		Directory*       root;
		Directory* 		 current;
		vector<File*>    files;
		list<Byte*>      freespace;
		int size;
};

#endif
