/*
 * interface.h
 *
 * This header file provides the method declarations for
 * the interface class.
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
#ifndef INTERFACE_H
#define INTERFACE_H

#include "filesystem.h"
using namespace std;

class Interface {

	public:
		
		// Constructors
		Interface(FileSystem* fs);
		~Interface();

		// Class Methods
		bool create (string path);
		bool remove (string path);
		bool edit   (string path);
		bool type   (string path);
		bool cd     (string path);
		bool mkdir  (string path);
		bool rmdir  (string path);
		bool dir();

	private:

		FileSystem* fs;
};

#endif
