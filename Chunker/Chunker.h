#ifndef CHUNKER_H
#define CHUNKER_H
////////////////////////////////////////////////////////////////////////////// 
// Chunker.h   -  Support sending files across the communication channel    //
// ver 1.0																	//
// Language:      Visual C++, 2012											//
// Platform:      Win 8														//
// Application:   CSE 687 OOD Project 3										//
// Author:        Xincheng Lai												//
////////////////////////////////////////////////////////////////////////////// 

/*
  Module Operations: 
  ==================
  This package provides a Chunker class.  Its function is to clice the binary file 
	into small blocks and then encode them to send. 

  Build Process:
  ==============
  Required files
    - Message.h, Message.cpp, Base64.h, Base64.cpp, Chunker.h, Chunker.cpp 

  Maintenance History:
  ==================== 
  ver 1.0 : 1 Apr 13
  - first release
*/

#include <string>
#include "../Message/Message.h"
#include "../Base64Encoding/Base64.h"
#include <vector>

namespace SocketCommunicator
{
	class chunker{
	public:
		chunker(){};
		void Initalizechunker( const std::string &content, EndPoint destine, MsgType filetype ); 
		void dochunker();
		EndPoint getSenderInfo();
		Message returnMessage();
		std::vector<Message> & returnMyMessages();
		void setSenderListenInfo( long port );
	
	private: 
		std::string m_filename;
		std::string m_contentstring;
		Message m_message;
		std::vector<Message> m_Messages;
		long m_listenport;
	};

	class dechunkder{
	public:
		dechunkder();
		void clearContent();
		void dodechunker(Message getMessage);
		EndPoint returnFrontpoint();
		void generateFile( std::string ofile2 );
	private: 
		std::string m_content; 
	};

}
#endif