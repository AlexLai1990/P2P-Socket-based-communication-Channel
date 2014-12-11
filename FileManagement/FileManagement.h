#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H
///////////////////////////////////////////////////////////////
// FileManagement.h - FileManagement Library                 //              
// Ver 1.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Windows 8																		 //
// Application: CSE687 - OOD, Pr#1, Spring 2013							 //
// Author:      Xincheng Lai																 //
///////////////////////////////////////////////////////////////
/*
 *   Package Operations:
     The package contains two interfaces:
		 1. get all h files into the vectors;
		 2. get all files into the vectors.

 *   Maintanence Information:
		 ========================
 *   Required files:
		 ---------------
		 FileManagement.h, FileManagement.cpp

 *   Build Process:
		 --------------
		 Using Visual Studio Command Prompt:

 
 *	 Maintenance History:
	   ====================
  	 ver 1.0 
		 3/10/2013

*/

#include <iostream>
#include <string> 
#include <vector>

class FileManagement{
public:
	bool getAllHFiles( const std::string & dir, std::vector<std::string>& all_dir_files );
	bool getAllFilesPath( const std::string & dir, std::vector<std::string>& all_dir_files );
};


#endif
