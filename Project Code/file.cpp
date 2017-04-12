/*
 * file.cpp
 *
 * This file contains the method implementations for 
 * the File class. 
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
#include <ctime>
#include <chrono>
#include <list>
#include "file.h"
using namespace std;

// Constructors
File::File() {
	this->size = 0;
	time_t rawtime;
	time(&rawtime);
	this->lastModified = ctime(&rawtime);
}

File::File(string name, string path, string parent, DiskProcessType* &disk, list<Byte*> &freespace){
	this->name   = name;
	this->path   = path;
	this->mode   = "closed";
	this->status = "closed";
	this->start  = NULL;
	this->end    = NULL;
	this->parent = parent;
	this->disk   = disk;
	this->size = 0;
	time_t rawtime;
	time(&rawtime);
	this->freespace = &freespace;
	this->lastModified = ctime(&rawtime);
}

File::~File(){}

/*
 * Class Methods
 */

// Open file in given mode.
bool File::open(string n_mode){
	
	if (status == "closed"){
		status = "open";
		this->mode = n_mode;
		return true;

	} else {
		if (n_mode == "w" && this->mode == "w"){
			cerr << "Error: File is already being written to!" << endl;
			return false;
		} else if (n_mode == "w" && this->mode == "r"){
			status = "open";
			this->mode = n_mode;
			return true;
		} else {
			status = "open";
			return true;
		}	
	}
}

// Close the file.
bool File::close(){

	if (status == "open"){
		status = "closed";
		this->mode = "closed";
		return true;

	} else if (status == "closed"){
		cerr << "Error: File is already closed!" << endl;
		return false;
	} else {
		cerr << "Error: Undefined file status" << endl;
		return false;
	}
}

// Read a number of characters from file into buffer.
int File::read(int numchars, char *buffer, DiskProcessType* disk){
	
	int  bytes_read = 0;
	int  current_block;
	bool eof = false;
	DiskBlockType *read_buff = new DiskBlockType(disk->getBlockSize());
	Byte *filePtr; 

	if (status == "open"){
		if (size > 0){
			if (start != NULL && 
				  end != NULL){
					
				filePtr = start;
				while (bytes_read < numchars && !eof){
					current_block = filePtr->block;
					disk->read(current_block, read_buff);

					while (filePtr->block == current_block && !eof){
						buffer[bytes_read] = read_buff->data[filePtr->byte];
						bytes_read++;
						if (filePtr->next == NULL)
							eof = true;
						else
							filePtr = filePtr->next;
					}
				}
			}
		}
	}
	
	return bytes_read;	
}

// Write a number of characters from the buffer to the disk.
int File::write(int numchars, char* buffer, DiskProcessType* disk, list<Byte*> &freespace){

	int  bytes_written   = 0;
	int  bytes_allocated = 0;
	int  current_block;
	DiskBlockType* write_buff = new DiskBlockType(disk->getBlockSize());
	Byte* filePtr;
	Byte* entryPtr;
	if (status == "open" && mode == "w"){

		// Add first byte address if none
		if (start == NULL || end == NULL){
			Byte* byte = freespace.front();
			freespace.pop_front();
			byte->next = NULL;
			byte->prev = NULL;
			start = byte;
			end   = byte;
			entryPtr = start;
			bytes_allocated++;
			size++;
		}

		// Continue allocating bytes as needed
		while (bytes_allocated < numchars){

			if (bytes_allocated == 0)
				entryPtr = end;

			filePtr = end;
			Byte* byte = freespace.front();
			freespace.pop_front();
			byte->prev = end;
			end->next = byte;
			end = byte;
			bytes_allocated++;
			size++;
		}

		// Begin writing to disk
		if (bytes_allocated == size)
			filePtr = entryPtr;	
		else 
			filePtr = entryPtr->next;

		while (bytes_written < numchars){
			current_block = filePtr->block;
			disk->read(current_block, write_buff);

			while (filePtr->block == current_block && 
				   bytes_written < numchars){

						write_buff->data[filePtr->byte] = buffer[bytes_written];
						bytes_written++;
						if (filePtr->next != NULL)
							filePtr = filePtr->next;
						else
							break;
			}

			disk->write(current_block, write_buff);
		}
	}

	return bytes_written;
}

// Function to release all of the data back to free space.
void File::releaseData(DiskProcessType* disk, list<Byte*> &freespace){

	// Initialize write buffer to clear disk (optional).
	DiskBlockType* nbuffer = new DiskBlockType(disk->getBlockSize());
	int bytes_overwritten = 0;
	int current_block     = 0;
	bool eof = false;
	Byte* filePtr;

	if (size > 0){
		if (start != NULL && 
			  end != NULL){
					
			filePtr = start;
			while (bytes_overwritten < size && !eof){
				current_block = filePtr->block;
				disk->read(current_block, nbuffer);

				while (filePtr->block == current_block && !eof){
					nbuffer->data[filePtr->byte] = 'X';
					bytes_overwritten++;
					if (filePtr->next == NULL)
						eof = true;
					else
						filePtr = filePtr->next;
				}
				disk->write(current_block, nbuffer);
			}
		}
		
		// Release byte addresses to freespace.
		Byte* filePtr = start;
		while(filePtr){
			freespace.push_back(filePtr);
			if (filePtr->next == NULL) break;
			else filePtr = filePtr->next;
		}
	}
}

// Getters and Setters
string File::getName() const { return name; } 
string File::getPath() const { return path; }
string File::getMode() const { return mode; }
string File::getStatus()       const { return status; }
string File::getParentFolder() const { return parent; }
string File::getLastModified() const { return lastModified; }
int    File::getSize()  const { return size;  }
Byte*  File::getStart() const { return start; }
Byte*  File::getEnd()   const { return end;   }
void   File::setName(string name) { this->name = name; }
void   File::setPath(string path) { this->path = path; }
void   File::setLastModified(string lm) { this->lastModified = lm; }
void   File::setParentFolder(string pf) { this->parent = pf; }
void   File::setStart(Byte* start) { this->start = start; }
void   File::setEnd  (Byte* end)   { this->end   = end;   }
void   File::setSize(int size) { this->size = size; }
void   File::incrementSize()   { this->size++; }
void   File::decrementSize()	 { this->size--; }	
void   File::addByte(Byte* newByte) { 
		  newByte->prev = end;
		  this->end->next = newByte;
		  this->end = newByte;
}
