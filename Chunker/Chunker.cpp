////////////////////////////////////////////////////////////////////////////// 
// Chunker.cpp   -  Support sending files across the communication channel		//
// ver 1.0																																  //
// Language:      Visual C++, 2012																					//
// Platform:      Win 8																											//
// Application:   CSE 687 OOD Project 3																			//
// Author:        Xincheng Lai																						  //
////////////////////////////////////////////////////////////////////////////// 

#include "Chunker.h"
using namespace SocketCommunicator;

#include "../Sockets/Sockets.h"
#include <iostream>

/////////////////////////////////////////////////////////////
// Initalize messages in chunker

void chunker::Initalizechunker( const std::string &filename, EndPoint destine,  MsgType filetype ){
	m_filename = filename;
	EndPoint senderEP = getSenderInfo(); 
	m_message.returnAddress() = senderEP;
	m_message.getMsgType() = filetype; 
	m_message.InitializeMes( destine, filename );
}

/////////////////////////////////////////////////////////////
// Get the information of Sender, to set the message's ip and port info 

EndPoint chunker::getSenderInfo(){
	SocketSystem ss;
	std::string host = ss.getHostName();
	std::string ip = ss.getIpFromName(host);
	EndPoint m_send( ip , m_listenport );
	return m_send;
}

/////////////////////////////////////////////////////////////
// Execute the core function slice the file into pieces of messages

void chunker::dochunker(){
	m_contentstring = Base64::encodeFromFile(m_filename);
	const int BlockSize = 1024;
	if (m_contentstring == "")
		return ;
	else {
		std::vector<char> m_contentvchar(m_contentstring.begin(),m_contentstring.end());
		size_t indexBegin, indexEnd , tempEnd;
		indexBegin = 0;
		indexEnd = m_contentvchar.size();
		std::string block;
		while (true){
			block="";
			tempEnd = indexBegin + BlockSize;
			if ( indexBegin < indexEnd ){
				m_message.setEOF() = "NOEnd";
				if ( tempEnd > indexEnd ){
					tempEnd = indexEnd;
					m_message.setEOF() = "EOF";
				}
				// set size of block 
				m_message.sizeOfBlock() = tempEnd - indexBegin; 
				for ( ; indexBegin< tempEnd ; indexBegin++ ){
					block += m_contentvchar[indexBegin]; 
				}
				m_message.body() = block;
				m_Messages.push_back(m_message); 
				indexBegin = tempEnd ;
			}
			else 
				break;
		} 
	}
}


/////////////////////////////////////////////////////////////
// Return message's vector

std::vector<Message> & chunker::returnMyMessages(){
	return m_Messages;
}

/////////////////////////////////////////////////////////////
// Prepare to set message of sender's listening port info

void chunker::setSenderListenInfo( long port ){
	m_listenport = port;
}
 
Message chunker::returnMessage(){
	return m_message;
}

//----< dechunker >---------------------------------
// 
dechunkder::dechunkder(){
}

/////////////////////////////////////////////////////////////
// When the messages have been collected,  it generate file

void dechunkder::generateFile( std::string ofile2 = "debug\\default.txt"){ 
	Base64::decodeToFile(ofile2,m_content);
}


EndPoint dechunkder::returnFrontpoint(){
	EndPoint ep("127.0.0.1",8000);
	return ep;
}

/////////////////////////////////////////////////////////////
// dechunker's core function

void dechunkder::dodechunker( Message getMessage ){
	std::string bodycontent = getMessage.body();
	m_content += bodycontent;
}

/////////////////////////////////////////////////////////////
// When a file has been transfered to string, do clean content 

void dechunkder::clearContent(){
	m_content="";
}

// #define TEST_CHUNKER

#ifdef TEST_CHUNKER

int main (){
	EndPoint dest = EndPoint("192.0.0.5",8000);
	std::string filename("SenderApp.cpp");
	chunker c1;
	c1.Initalizechunker(filename, dest , file);
	c1.dochunker();

	dechunkder d1;
	for ( size_t i =0; i<c1.returnMyMessages().size(); i++ ){
		d1.dodechunker( c1.returnMyMessages()[i] );
	}
	d1.generateFile();

	return 0;
}

#endif
