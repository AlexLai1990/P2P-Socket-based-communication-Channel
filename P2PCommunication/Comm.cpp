/////////////////////////////////////////////////////////////////////
// Comm.cpp   -     Defines channel, sender and receiver class	   //
// ver 1.4                                                         //
// Language:      Visual C++, 2008                                 //
// Platform:      Dell Dimension T7400, Win 7 Pro                  //
// Application:   Utility for CSE687 projects                      //
// Author:        Xincheng Lai								       //
//                Parts of Class, refered from phDJim Fawcett,	   //
//                             Syracuse University, CST 4-187      //   
/////////////////////////////////////////////////////////////////////

#include "Comm.h"
#include <iostream>
#include <sstream> 
#include <conio.h>
#include <fstream>
#include "../Base64Encoding/Base64.h"


using namespace SocketCommunicator;

#ifdef TRACING
  #include "..\threads\locks.h"
  #define TRACE(msg) sout << "\n  " << msg;
#else
  #define TRACE(msg) ;
#endif

//----< send thread deQueues message and pushes into socket >------------ 

class SendThreadProc : public Thread_Processing<SendThreadProc>
{
public:
  SendThreadProc(Socket& sock, BQueue<Message>& sendQ) 
    : sendSock(sock), sendQ_(sendQ) {}
  void run()
  {
    do
    {
      Message msg = sendQ_.deQ(); 
			std::string out = msg.AddHeaderToSenderMessage();
      if(!sendSock.send(out.c_str(), out.size()))
        throw std::exception("\n\n  could not send message\n\n");
      if(msg.body() == "quit" || msg.body() == "done")
      {
        sendSock.disconnect();
        break;
      }
    } while(true);
  }
private:
  Socket& sendSock;
  BQueue<Message>& sendQ_;
};
 
//----< send file messages core code >------------ 

void Sender::putFileInQueue(std::string	filename){
	 
	MsgType m_extention = file;
	m_chunker.Initalizechunker(filename ,  ReceiverAddr, m_extention );
	m_chunker.dochunker();
	std::vector<Message> senderMess = m_chunker.returnMyMessages();
	m_chunker.returnMyMessages().clear();

	for( size_t i = 0; i<senderMess.size(); i++ ){
		this->PostMsg(senderMess[i]);
	}  
	// post a disconnnect message when a file has been sent
	Message DisMes;
	DisMes.returnAddress() = m_chunker.returnMessage().returnAddress();
	DisMes.returnRecevierAddr() = m_chunker.returnMessage().returnRecevierAddr();
	DisMes.body() = "quit" ;
	this->PostMsg(DisMes);
	std::cout << "\n\n  Sending to"<<DisMes.returnRecevierAddr().ToString()<<" ...	\n  "<< filename<<" has been sent successfuly\n";
}

//----< start sending thread >-------------------------------------------

Sender::Sender() {}

//----< set my Sender's listening port >-------------------------------------------

void Sender::setMyListenInfo( long port ){
	m_chunker.setSenderListenInfo( port );
	m_listenport = port;
}

//----< start my sender thread >-------------------------------------------

void Sender::startSendThread(){
	pSendThread = new tthread(*new SendThreadProc(s_, sendQ));
	pSendThread->start();
}

//----< start my sender thread >-------------------------------------------

bool Sender::tryToSendFile( EndPoint ep ,std::string sendfile ){
  int tryCount = 0, MaxTries = 5;
	// try to connect 
  while(++tryCount < MaxTries)
  {
    if(connect( ep ))
      break;
    Sleep(100);
		// std::cout << "\n"<<"	It is "<<tryCount<<" time, I failed attempt to connect to endpoint(" << ep.address().c_str() << ", " << ep.port() << ")";
	}
  if(tryCount < MaxTries)
  { 
		putFileInQueue( sendfile ); 
		return true;
  }
	else{
		std::cout << "\n  I failed "<<MaxTries<<" times to connect to endpoint(" << ep.address().c_str() << ", " << ep.port() << ")";
		return false;
	}
}

//----< try to send Text message >-------------------------------------------

bool Sender::tryToSendText( EndPoint ep ,std::string textcontent ){
  int tryCount = 0, MaxTries = 5;
	// try to connect 
  while(++tryCount < MaxTries)
  {
    if(connect( ep ))
      break;
    Sleep(100);
		std::cout << "\n  failed attempt to connect to endpoint(" << ep.address().c_str() << ", " << ep.port() << ")";
	} 
  if(tryCount < MaxTries)
  {  
		Message msg;
		msg.body() = textcontent; 
		msg.getMsgType() = text;
		msg.returnRecevierAddr() = ep;
		SocketSystem ss;
		std::string host = ss.getHostName();
		std::string ip = ss.getIpFromName(host);
		EndPoint m_send( ip , m_listenport );

		msg.returnAddress() = m_send;

		std::cout << "\n\n  Sending To "<< ep.ToString() <<" ...	Text MSG: " << msg.body() <<" has been sent!!\n" << msg.ToString();
		PostMsg(msg);
		
		// post a disconnnect message when a file has been sent
		Message DisMes;
		DisMes.returnAddress() =  m_send;
		DisMes.body() = "quit" ;
		this->PostMsg(DisMes);
		return true;
    // std::cout << "\n\n  Shutting down SenderApp Sender";
  }
	else
		return false;
}


//----< attempt to connect to a listening endpoint >---------------------

bool Sender::connect(EndPoint ep) 
{
  ReceiverAddr = ep;
  std::string address = ep.address();
  long port = ep.port();
  if(!s_.connect(ep.address(), ep.port()))
  {
    return false;
  }
  return true;
}

//----< enqueue message for sending >------------------------------------

void Sender::PostMsg(const Message& msg)
{
  sendQ.enQ(msg);
}

//----< wait for send thread to finish >---------------------------------

void Sender::wait()
{
  pSendThread->wait();
}




//----< receive thread frames messages >---------------------------------
//
//   This class is handling collect message from recvQ
//
class ConnectHandler : public Thread_Processing<ConnectHandler>
{
public:
  ConnectHandler(Socket* pSock, BQueue<Message>& recvQ) 
    : pSock_(pSock), recvQ_(recvQ) {}
  void run(){
    bool ClientActive = true;
    char buffer[1];
    std::string out;
    TRACE("entered ConnectHandler");
    do{
      do{
        if(pSock_->recv(buffer,1)){
          out += (char)buffer[0];
          if(isDone(out))
            break;
        }
        else{
          ClientActive = false;
					delete pSock_;				//break;  // client closed
					return ;
        }
      } while(true);
			// if(!ClientActive)  break;
      Message msg = Message::FromString(out);
      out = "";
			// when the receiver get a acknowledge, it disconnects.
      if(msg.body() == "quit"){
				std::cout<< "\n  I'm Disconnecting from : "<< msg.returnAddress().address()<<"\n\n" ;
        pSock_->disconnect();
        break;
      }
			if(msg.body() == "acknowledge") { 
				std::cout<< "\n  I Got acknowledge !!\n" ; 
      }
      recvQ_.enQ(msg);
    } while(true);
    delete pSock_;  // created in listener for this handler only
  }
	 
private:
  static bool isDone(std::string msg);
  Socket* pSock_;
  BQueue<Message>& recvQ_;
};

//----< did we receive terminating markup for message? >-----------------

bool ConnectHandler::isDone(std::string msg)
{
  static std::string terminal = "</msg>";
  for(size_t i=0; i<terminal.size(); ++i)
    if(terminal[6-i-1] != msg[msg.size()-i-1])
      return false;
  return true;
}


//----< Deal with received messages >---------------------------------
//
class DealWithMessage : public Thread_Processing<DealWithMessage>
{
public:
  DealWithMessage(  BQueue<Message>& recvQ , std::string folder = "..\\Test\\" ) 
    :  recvQ_(recvQ) , ReceiverFolder(folder){}
  void run()
  { 
	do
    {
			Message msg;
			msg = recvQ_.deQ(); 
			ReceiverMessages.push_back(msg);
			if ( msg.getMsgType() == text ){
				if (msg.body() == "acknowledge") 
					std::cout << "  This is ACK Message From :"<< msg.returnAddress().ToString()<<" : "<< msg.ToString() <<"\n"; 
				else 
					std::cout << "\n\n  Receiving Text Message From "<<msg.returnAddress().ToString()<<" : " << msg.ToString(); 
				if (msg.body() != "acknowledge" )
					if ( sendACKMessage( msg.returnAddress(), msg.returnRecevierAddr().port() ) ) { std::cout << "  Acknowledge message sent successfully\n"; }
			}
			if ( msg.getMsgType() == file && msg.returnTagOfEOF() == "EOF" ) {
				reciFileName = msg.returnFileName();
				ReplyToSender = msg.returnAddress();
				generateFile();
			}
		} while(true);
  }

	bool generateFile() {
		if ( ReceiverMessages.size() == 0 )
			return false;
		else{
			std::vector<Message> PrepareToStore;
			size_t index = 0;
			do{
				Message msg = ReceiverMessages[index];
				index++;
				if ( msg.returnFileName() == reciFileName && 
					msg.returnAddress().ToString() == ReplyToSender.ToString() )
					PrepareToStore.push_back( msg );
			} while ( index < ReceiverMessages.size() );
			// get all decode string by using dodechunker in dechunker
			for ( size_t i =0; i<PrepareToStore.size(); i++ ){
				m_dechunker.dodechunker( PrepareToStore[i] );
			} 
			// to deal with absolute file path when a sender try to send file
			std::string FileName = reciFileName;
			if ( reciFileName.find("\\") != std::string::npos ){
				std::string FullFileName = reciFileName ;
				FileName = FullFileName.substr(FullFileName.find_last_of("\\")+1 ,FullFileName.length());
			}
			// get File Name to generate file in a certain folder
			std::string Createfile = ReceiverFolder + FileName;
			m_dechunker.generateFile(Createfile);
			m_dechunker.clearContent();
			std::cout <<"\n\n  Receiving...  " << Createfile << " has been created!\n";

			Message getSender = PrepareToStore[PrepareToStore.size() -1 ];
			if ( sendACKMessage( getSender.returnAddress(), getSender.returnRecevierAddr().port() ) ) { std::cout << "\n  Acknowledge message sent successfully\n"; }
			return true;
		}
	}

	bool sendACKMessage( EndPoint replySender , long port ){
		Sender *ackSender = new Sender;
		ackSender->startSendThread();
		ackSender->setMyListenInfo( port );
		if (	ackSender->tryToSendText( replySender , "acknowledge" ) ) {
			return true;
			delete ackSender;
		}
		else {
			return false;
			delete ackSender;
		}
	}
private: 
  BQueue<Message>& recvQ_;
	std::vector<Message> ReceiverMessages;
	std::string reciFileName;
	EndPoint ReplyToSender;
	dechunkder m_dechunker;
	std::string ReceiverFolder;
};
 

//----< define listener's activities >-----------------------------------
//
class ListenProc : public Thread_Processing<ListenProc>
{
public:
  ListenProc(SocketListener& lsock, BQueue<Message>& recvQ, std::string folder ) 
    : lsock_(lsock), recvQ_(recvQ) , m_ReceiverFolder(folder) {}
  void run()
  {
    SOCKET sock;
    Socket* pSock;
    try
    {
		// add a thread for dealing with Messages
		tthread* pDealWithMessage = new tthread(* new DealWithMessage( recvQ_, m_ReceiverFolder) );
		pDealWithMessage->start();
		// listen sender's messages and use pThread to get the messages into the recvQ_
		do {
			sock = lsock_.waitForConnect();
			pSock = new Socket(sock);  // will be deleted in ConnectHandler
			TRACE("\n  connected in listener: ");
			tthread* pThread = new tthread(*new ConnectHandler(pSock,recvQ_));
			pThread->start();
		 } while(true);
    }
    catch(std::exception&)
    {
      TRACE("\n  Receiver's listener shutting down\n\n");
    }
  }
private:
  SocketListener& lsock_;
  BQueue<Message>& recvQ_;
	std::string m_ReceiverFolder;
};


//----< construct listener and start listening for connections >---------

Receiver::Receiver(long port, std::string folder )
        : s_(port), pListenThread(new tthread(*new ListenProc(s_, recvQ, folder))), ReplyToSender(EndPoint("127.0.0.1",8000))
{
	pListenThread->start();
	m_ReceiverFolder = folder;
}

//----< extract message from receive queue >-----------------------------

Message Receiver::GetMsg()
{
  return recvQ.deQ();
}

EndPoint Receiver::ReturnReplyToSender(){
	return ReplyToSender;
}

//----< start my sender thread >-------------------------------------------

bool Receiver::putIntoMessagesQueue(){
	Message msg;
	do {
		msg = GetMsg(); 
		ReceiverMessages.push_back(msg);
	} while(msg.returnTagOfEOF() != "EOF");
		
	reciFileName = msg.returnFileName();
	ReplyToSender = msg.returnAddress();

	if ( ReceiverMessages.size() >0 )
		return true;
	else
		return false;
}

//----< create file after collect all messages >-------------------------------------------

bool Receiver::generateFile(){ 

	if ( ReceiverMessages.size() == 0 )
		return false;
	else{
		for ( size_t i =0; i<ReceiverMessages.size(); i++ ){
			m_dechunker.dodechunker( ReceiverMessages[i] );
		} 
		std::string Createfile = "..\\Test\\"+reciFileName;
		m_dechunker.generateFile(Createfile);
		std::cout <<"\n\n		" << Createfile << " has been created!\n\n";
		return true;
	}
}

//----< Check whether this receiver folder exist or not  >-------------------------------------------

bool  Channel::CheckFolderExist(const std::string &strPath){
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath.c_str(), &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;   
	}
	FindClose(hFind);
	return rValue;
}

//----< set listening port for this channel  >-------------------------------------------

Channel::Channel( long port, std::string folder ): m_cReceiver( port, folder ) ,m_cPort(port), m_ReceiverFolder(folder) {
	if ( !CheckFolderExist( folder ) ){
		CreateDirectory( (LPCSTR)folder.c_str() , NULL );
	}
}

//----< set listening port, Destine port and filepath wanted send for this channel  >-------------------------------------------

Channel::Channel( long m_listenport, EndPoint epDest, std::string filepath ):
	m_cReceiver(m_listenport, ".\\SReceiver\\" ), m_cDestine(epDest), m_cPort(m_listenport)
{
	m_cPort = m_listenport;
	m_cDestine = epDest;
	m_ReceiverFolder = ".\\SReceiver\\";
}

//----< set sender's info in channel >-------------------------------------------

void Channel::setSenderListenPortInfo( long m_listenport ){
	m_cSender.setMyListenInfo( m_listenport );
}

//----< set destine port in channel >-------------------------------------------

void Channel::setDestPoint( EndPoint epDest ){
	m_cDestine = epDest;
}

//----< sendText interface in channle >-------------------------------------------

bool Channel::sendText( EndPoint epDest, std::string TextContent ){
	m_cSender.startSendThread();
	setSenderListenPortInfo( m_cPort );
	m_cSender.tryToSendText( epDest, TextContent );
	return true;
}

//----< sendFile interface in channle >-------------------------------------------

bool Channel::sendFile( EndPoint	epDest, std::string filepath ){
	m_cSender.startSendThread();
	setSenderListenPortInfo( m_cPort );
	m_cSender.tryToSendFile( epDest, filepath );
	return true;
}

//----< send FileFolder interface in channle >-------------------------------------------

bool Channel::sendFileFolder( EndPoint epDest, std::string filefolder ){
	std::vector<std::string> all_file_path;
	fm.getAllFilesPath( filefolder , all_file_path );
	if ( all_file_path.size() == 0 )
		std::cout<< "\n  The file folder is empty!!!\n";
	else {
		for ( size_t i = 0 ; i < all_file_path.size() ; i++ ){
			sendFile( epDest , all_file_path[i] );
		}
	}
	return true;
}
 
#ifdef TEST_COMM

///////////////////////////////////////////////////////////////
// Start testing

template <typename T>
std::string ToString(T t)
{
  std::ostringstream temp;
  temp << t;
  return temp.str();
}

void main()
{
  std::cout << "\n  Demonstrating Communicator Classes";
  std::cout << "\n ====================================\n";
  Receiver rcvr(8000);
  /////////////////////////////////////////////////////////////
  // Start message receiver thread
  class RecvProc : public Thread_Processing<RecvProc>{
  public:
    RecvProc(Receiver& rcvr) : rcvr_(rcvr) {}
    void run(){
      std::string msgBody;
      do{
        Message msg = rcvr_.GetMsg();
        msgBody = msg.body();
        sout << locker << "\n  received " << msgBody << unlocker;
      } while(msgBody != "quit");
    }
  private:
    Receiver& rcvr_;
  };
  tthread* pRt = new tthread(*new RecvProc(rcvr));
  pRt->start(); 
  EndPoint ep("127.0.0.1:8000");
  Message msg("a message",ep,Message::text);
  Sender sndr;
  if(!sndr.connect(ep))
    std::cout << "\n  can't connect to " << ep.ToString();
  else{
    size_t count = 0;
    for(size_t i=0; i<20; ++i)
    {
      msg.body() = "msg #" + ToString(i) + " - some text";
      sout << locker << "\n  Posting  " << msg.body() << unlocker;
      sndr.PostMsg(msg);
    }
    Message end("quit");
    sndr.PostMsg(end);
  }
  pRt->wait();
  std::cout << "\n\n";
}

#endif
