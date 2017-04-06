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

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
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
		File() {
			this->size = 0;
			time_t rawtime;
			time(&rawtime);
			this->lastModified = ctime(&rawtime);
		}

		File(string name, string path, string parentFolder){
			this->name = name;
			this->path = path;
			this->mode = "closed";
			this->status = "closed";
			this->start  = NULL;
			this->end    = NULL;
			this->parentFolder = parentFolder;
			this->size = 0;
			time_t rawtime;
			time(&rawtime);
			this->lastModified = ctime(&rawtime);
		}

		~File(){}

		// Class methods //
		
		// Open file in given mode.
		bool open(string n_mode){
			
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
		bool close(){

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
		int read(int numchars, char *buffer, DiskProcessType* disk){
			
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
		int write(int numchars, char* buffer, DiskProcessType* disk, list<Byte*> &freespace){

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
		void releaseData(DiskProcessType* disk, list<Byte*> &freespace){

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
					if (filePtr->next != NULL)
						break;
				}
			}
		}

		// Getters and Setters
		string getName() const { return name; } 
		string getPath() const { return path; }
		string getMode() const { return mode; }
		string getStatus()       const { return status; }
		string getLastModified() const { return lastModified; }
		string getParentFolder() const { return parentFolder; }
		int    getSize()  const { return size;  }
		Byte*  getStart() const { return start; }
		Byte*  getEnd()   const { return end;   }
		void   setName(string name) { this->name = name; }
		void   setPath(string path) { this->path = path; }
		void   setLastModified(string lm) { this->lastModified = lm; }
		void   setParentFolder(string pf) { this->parentFolder = pf; }
		void   setStart(Byte* start) { this->start = start; }
		void   setEnd  (Byte* end)   { this->end   = end;   }
		void   setSize(int size) { this->size = size; }
		void   incrementSize()   { this->size++; }
		void   decrementSize()	 { this->size--; }	
		void   addByte(Byte* newByte) { 
				  newByte->prev = end;
				  this->end->next = newByte;
				  this->end = newByte;
		}

	private:

		string name;
		string path;
		string mode;
		string status;
		string lastModified;
		string parentFolder;
		Byte* start;
		Byte* end;
		int size;

};

#endif
