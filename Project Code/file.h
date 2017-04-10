/*
 * file.h
 *
 * This file contains the header declarations for the
 * File class. 
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
#ifndef FILE_H
#define FILE_H

#include <list>
#include "diskprocess.h"
using namespace std;


struct Byte {
	int block;
	int byte;
	char* value;
	Byte* next;
	Byte* prev;
};

class File {

	public:

		// Constructors
		File();
		File(string name, string path, string parent);
		~File();

		// Class methods //
		bool open (string n_mode);
		bool close();
		int  read (int numchars, char *buffer, DiskProcessType* disk);
		int  write(int numchars, char* buffer, DiskProcessType* disk, list<Byte*> &freespace);
		void releaseData                      (DiskProcessType* disk, list<Byte*> &freespace);

		// Getters and Setters
		string getName()   const;
		string getPath()   const;
		string getMode()   const;
		string getStatus() const;
		string getParentFolder() const;
		string getLastModified() const;
		int    getSize()  const;
		Byte*  getStart() const;
		Byte*  getEnd()   const;
		void   setName(string name);
		void   setPath(string path);
		void   setLastModified(string lm);
		void   setParentFolder(string pf);
		void   setStart(Byte* start);
		void   setEnd  (Byte* end); 
		void   setSize(int size);
		void   incrementSize();
		void   decrementSize();	
		void   addByte(Byte* newByte); 

	private:

		string name;
		string path;
		string mode;
		string status;
		string parent;
		string lastModified;
		Byte* start;
		Byte* end;
		int size;

};

#endif
