#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////
// Message.h   -  Defines EndPoints and Messages                   //
// ver 2.5                                                         //
// Language:      Visual C++, 2008                                 //
// Platform:      Dell Dimension T7400, Win 7 Pro                  //
// Application:   Utility for CSE687 projects                      //
// Author:        Jim Fawcett, Syracuse University, CST 4-187      //
// Revise:				Xincheng Lai							   //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////

/*
  Module Operations: 
  ==================
  This package provides a Message class.  Its function is to build
  and parse XML messages.  Each message has a type, return address,
  and a text body.

  Build Process:
  ==============
  Required files
    - Message.h, Message.cpp, XmlDocument.h, XmlDocument.cpp,
      XmlElementParts.h, XmlElementParts.cpp, XmlNodes.h, XmlNodes.cpp,
      MTree.h, MTree.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands (either one)
    - devenv XmlProcessing.sln
    - cl /EHsc Message.cpp, XmlDocument.cpp XmlElementParts.cpp XmlNodes.cpp Tokenizer.cpp

  Maintenance History:
  ====================
  ver 2.0 : 04 Jul 13
  - changed xml processing from XmlDocument to XmlReader and XmlWriter.
	- revised EndPoint, Message.
  ver 1.2 : 24 Apr 11
  - Added namespace SocketCommunicator to Comm.h and Message.h which
    will allow you to avoid conflicts with the .Net Message type.
    That means you will need to declare "using namespace SocketCommunicator;"
    in some places in your code.
  - Fixed a bug discovered by Jingyi Ren in the Message type handling.
    She provided a solution, and that worked and has been incorporated.
  ver 1.1 : 06 Apr 11
  - added these comments
  ver 1.0 : 25 Apr 10
  - first release
*/

#include <string>

namespace SocketCommunicator
{
  /////////////////////////////////////////////////////////////////////////
  // Manage addresses in the form 127.0.0.1:8000

  class EndPoint //: public IEndPoint
  {
  public:
	EndPoint(){};
    EndPoint(const std::string& address, long port);
    EndPoint(const std::string& eps);
	EndPoint(const EndPoint& epinput);
    std::string address();
    long port();
	std::string portString();
    std::string& ToString();
	// set Address
	void setAddr( std::string addr );
	// set Port
	void setPort( std::string port );
  private:
    std::string ep;
  };

  /////////////////////////////////////////////////////////////////////////
  // Create, serialize, and deserialize Message instances
	
  enum MsgType { text, file };

  class Message //: public IMessage
  {
  public:
		// enum MsgType { text, file };
    Message(const std::string& text = "");
    Message(const std::string& body, EndPoint ep, MsgType type);
    Message(const Message& msg);
    std::string& getEndPoint();
    MsgType&  getMsgType();
    std::string& body();
    EndPoint& returnAddress();
    std::string TypeToString();
		// add test XML header
    std::string ToString();
		// add Message for sender with XML header
		std::string AddHeaderToSenderMessage();
		void InitializeMes( EndPoint destPoint, std::string filename );
		size_t & sizeOfBlock();
		std::string & setEOF();
		// return information of Receiver
		EndPoint& returnRecevierAddr();
		std::string returnTagOfEOF();
		std::string returnFileName();  

    static Message FromString(const std::string& xml);
  private:
    static std::string trimQuotes(const std::string& s);
    EndPoint returnAddr_;
    std::string body_;
    MsgType type_;

		// add new attribute for sender header 
		EndPoint receiverInfo_;
		std::string sendFileName_;
		// tag the size of body()
		size_t sizeOfBody_;
		// tag like EOF
		std::string m_endoffile;
  };
}
#endif
