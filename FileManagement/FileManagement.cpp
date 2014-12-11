///////////////////////////////////////////////////////////////
// FileManagement.cpp - FileManagement Library               //              
// Ver 1.0                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Windows 8																		 //
// Application: CSE687 - OOD, Pr#1, Spring 2013							 //
// Author:      Xincheng Lai																 //
///////////////////////////////////////////////////////////////

#include "FileManagement.h"
#include "FileSystem.h"
using namespace FileSystem;

// Filesystem to get all .h file paths in one specified path
bool FileManagement::getAllHFiles( const std::string & dir, std::vector<std::string>& all_dir_files )
{	
	std::cout<<"\n\n ====== Files for getting types ========== \n\n";
  // std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory()<< "\n";
  Directory::setCurrentDirectory( dir);
  std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory()<< "\n";
  std::vector<std::string> currfiles_h = Directory::getFiles(dir,"*.h");
  std::vector<std::string> currfiles_cpp = Directory::getFiles(dir,"*.cpp");
  
  for (size_t i = 0; i < currfiles_h.size();i++)
  {
	  // std::cout << "\n    " << currfiles_h[i].c_str();
	  all_dir_files.push_back(currfiles_h[i].c_str());
  } 
  std::cout<<"\n";
  return true;

}

// Filesystem to get all the code file paths in one specified path(.cpp/ .h)
bool FileManagement::getAllFilesPath( const std::string & dir, std::vector<std::string>& all_dir_files )
{
	//std::cout<<"\n\n =====  get all files for Parsing Dependency Relationship ===== \n\n"; 
  // Directory::setCurrentDirectory( dir);
  // std::cout << "\n  current directory is:\n    " << Directory::getCurrentDirectory()<< "\n";
	std::vector<std::string> dirs;
	
	 
		std::vector<std::string> currfiles_all = Directory::getFiles(dir,"*.*"); 
		dirs = Directory::getDirectories(dir);
		
		for (size_t i = 0; i < currfiles_all.size();i++)
		{
			currfiles_all[i] = dir + currfiles_all[i] ;
			all_dir_files.push_back(currfiles_all[i].c_str());
		}	
		for (size_t i = 0; i < dirs.size();i++)
		{
			if ( dirs[i] == "." ||  dirs[i] == ".." ) 
				continue;
			dirs[i] = dir+ dirs[i]+"\\";
			if ( dirs.size()>2 )
				getAllFilesPath( dirs[i] , all_dir_files );
			else
				return true;
			// std::cout << "\n    " << dirs[i].c_str();
			// all_dir_files.push_back(dirs[i].c_str());
		} 

	// std::cout << "\n   current all files:" ;	
  std::cout<<"\n";
  return true;
}

//----< test stub >--------------------------------------------
//#define TEST_FILEMANAGEMENT

#ifdef TEST_FILEMANAGEMENT
int main (){
	std::string my_directory = ".\\";
	std::vector<std::string> test_path;
	FileManagement fm;
	// fm.getAllHFiles(my_directory,test_path);
	
	/*
	std::cout <<"  Followings are my h files\n\n    ";
	for ( size_t i=0; i<test_path.size(); i++ ){
		std::cout<<test_path[i]<<"\n    ";
	}
	*/

	fm.getAllFilesPath(my_directory,test_path);
	
	std::cout<<"  ================================================\n\n "
		<<"  Followings are my All files\n\n  ";
	for ( size_t i=0; i<test_path.size(); i++ ){
		std::cout<<"#"<<i+1<<". "<<test_path[i]<<"\n  ";
	}
	
}

#endif
