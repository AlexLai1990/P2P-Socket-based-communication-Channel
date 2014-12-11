#ifndef COMM_H
#define COMM_H
/////////////////////////////////////////////////////////////////////
// Comm.h   -     Defines channel, sender and receiver class	   //
// ver 1.4                                                         //
// Language:      Visual C++, 2008                                 //
// Platform:      Dell Dimension T7400, Win 7 Pro                  //
// Application:   Utility for CSE687 projects                      //
// Author:        Xincheng Lai									   //
//                Part of Class, refered from phD Jim Fawcett,	   //
//                             Syracuse University, CST 4-187      //   
/////////////////////////////////////////////////////////////////////

/*
   Package Operations:
   ===================
   This package provides network communication services, using 
   WinSock2, a nearly Berkley Sockets compliant implementation
   with extensions. After building, it will generate a static 
	 library in the debug file, it's a good way to reuse in the 
	 following projects.
	 
	 Three classes are provided:

	 Channel :
	 --------
	 Function like a real machine, to support sending Message and receiving
	 Message, it compose a Sender and a Receiver Class. It has several public
	 interface for easily use, like setDestinePoint, sendFile(), sendText() etc.

   Revised Message:
   --------
   Supports defining instances that hold type strings - either "text"
   or "file", EndPoints, and a text body.  Messages have the ability to
   serialize themselves into XML strings and to deserialize a properly
   formatted XML string into a Message.

   Revised Sender:
   -------
   Supports connecting and posting messages to a remote EndPoint.

   Revised Receiver:
   ---------
   Starts a listener that waits for connections, then starts a receive
   thread, for each connection, that frames XML Message strings into
   Message instances and enqueues them.  It provides a GetMsg() function
   that retrieves messages from the queue.

   Public Interface:
   =================
   Channel( long port ) - set the listening port as a Receiver
	 Channel( long m_listenport, EndPoint epDest, std::string filepath )
	 - Set local listening port, destine port and file readied to send
	 void setSenderListenPortInfo( long m_listenport )
	 - Set local listening port
	 void setDestPoint( EndPoint epDest )
	 - Set destine point
	 bool sendText( EndPoint epDest, std::string TextContent )
	 - Send Text Message to a Destine point
	 bool sendFile( EndPoint	epDest, std::string filepath )
	 - Send File to a Destine point

*/
/*

   Maintenance History:
   ====================
	ver 1.4
  - Added Channel class to support sending and receiving messages
	- Added dealwithMessage thread and revise the connecthandler class
	  in the comm.cpp
  ver 1.3 : 24 Apr 11
  - Added namespace SocketCommunicator to Comm.h and Message.h which
    will allow you to avoid conflicts with the .Net Message type.
    That means you will need to declare "using namespace SocketCommunicator;"
    in some places in your code.
  - Fixed a bug discovered by Jingyi Ren in the Message type handling.
    She provided a solution, and that worked and has been incorporated.
   ver 1.2 : 01 May 2010
   - added Sender::wait() to wait for sender thread to empty the
     send queue before shutting sender down.
   - added try catch handling around listener to prevent unhandled
     exception when receiver shuts down while listener is listening.
   - added test for both "quit" and "done" messages in Sender's
     SendThreadProc.  The "done" message shuts down send thread.
     The "quit" message shuts down both send thread in Sender and
     receive thread in Receiver.
   ver 1.1 : 17 Apr 2010
   - added Manual and Maintenance pages
   - added other comments in header file, below
   - added delete of socket pointer in ConnectionHandler when thread
     processing completes.
   - fixed bug in ConnectHandler that posted empty messages if client
     broke connection.
   - refactored the code a bit so files using the communicator don't 
     need to include so much.
   ver 1.0 : 13 Apr 2010
   - first release
*/

#include "..\Sockets\Sockets.h"  // must be first
#include "..\Threads\Threads.h"
#include "..\BlockingQueue\BlockingQueue.h"
#include "..\Message\Message.h"
#include "..\Chunker\Chunker.h"

#include "..\FileManagement\FileManagement.h"
#include "..\FileManagement\FileSystem.h"
using namespace FileSystem;

/////////////////////////////////////////////////////////////////////////
// Supports sending Message instances
// - Attempts to connect to EndPoint.  If that fails, returns false.
// - Has send queue so PostMsg(msg) returns quickly

namespace SocketCommunicator
{
class Sender
{
public:
	Sender();
	void startSendThread();
	bool tryToSendFile( EndPoint ep ,std::string sendfile );
	bool tryToSendText( EndPoint ep ,std::string textcontent );
	bool connect(EndPoint pEp);
	void putFileInQueue(std::string	filename); 
	void PostMsg(const Message& pMsg); 
	void wait();
	void setMyListenInfo( long port );

private:
	BQueue<Message> sendQ;
	tthread* pSendThread;
	Socket s_;

	long m_listenport;
	EndPoint ReceiverAddr;
	chunker m_chunker;
};

/////////////////////////////////////////////////////////////////////////
// Supports receiving Message instances
// - listener starts on specified port when constructed.
// - will throw exception if another listener already occuppies the 
//   specified port
// - Will block on GetMsg() if the receive queue is empty.

class Receiver
{
public: 
	Receiver(long port, std::string folder);
	Message GetMsg();
	bool putIntoMessagesQueue();
	bool generateFile();
	EndPoint ReturnReplyToSender();

private:
	SocketListener s_;
	BQueue<Message> recvQ;
	tthread* pListenThread;

	EndPoint ReplyToSender;
	std::string reciFileName;
	std::vector<Message> ReceiverMessages;
	dechunkder m_dechunker;
	std::string m_ReceiverFolder;
};


/////////////////////////////////////////////////////////////////////////
//  Supports receiving and sending Message instances
// - listener starts on specified port when constructed.
// - send a text message to a destine point
// - send a file message to a destine point


class Channel{
private:
	Sender m_cSender;
	Receiver m_cReceiver;
	long m_cPort;
	EndPoint m_cDestine;
	std::string m_ReceiverFolder;
	FileManagement fm;

	bool  CheckFolderExist(const std::string &strPath);
public:
	// set as a Receiver 
	Channel( long port , std::string folder );
	// set as a Sender
	Channel( long m_listenport, EndPoint epDest, std::string filepath );
	void setSenderListenPortInfo( long m_listenport );
	void setDestPoint( EndPoint epDest );
	bool sendText( EndPoint epDest, std::string TextContent );
	bool sendFile( EndPoint	epDest, std::string filepath );
	bool sendFileFolder( EndPoint epDest, std::string filefolder );
};
}


#endif
