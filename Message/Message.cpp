/////////////////////////////////////////////////////////////////////
// Message.cpp   -  Defines EndPoints and Messages                   //
// ver 2.5                                                         //
// Language:      Visual C++, 2008                                 //
// Platform:      Dell Dimension T7400, Win 7 Pro                  //
// Application:   Utility for CSE687 projects                      //
// Author:        Jim Fawcett, Syracuse University, CST 4-187      //
// Revise:				Xincheng Lai															       //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////

#include "Message.h"
#include "..\Message\XmlReader.h"
#include "..\Message\XmlWriter.h"
#include <iostream>
#include <sstream>
using namespace SocketCommunicator;


/////////////////////////////////////////////////////////////
// converters to and from string

template <typename T>
std::string ToString(T t)
{
	std::ostringstream temp;
	temp << t;
	return temp.str();
}

template <typename T>
T FromString(const std::string& s)
{
	T t;
	std::istringstream temp(s);
	temp >> t;
	return t;
}
///////////////////////////////////////////////////////////////
// Manage endpoints

EndPoint::EndPoint(const std::string& address, long port)
{
	ep = address + ":" + ::ToString(port); 
}

EndPoint::EndPoint(const std::string& eps) : ep(eps) {}

EndPoint::EndPoint(const EndPoint& epinput){
	ep = epinput.ep ;
}

std::string  EndPoint::address()
{
	size_t pos = ep.find(":");
	return ep.substr(0,pos);
}
long EndPoint::port()
{
	size_t pos = ep.find(":");
	std::string pnum = ep.substr(pos+1);
	return FromString<long>(pnum);
}

std::string  EndPoint::portString(){
	size_t pos = ep.find(":");
	return ep.substr(pos+1);
}


std::string& EndPoint::ToString()
{
	return ep;
}

void EndPoint::setAddr(std::string addr){
	ep = addr; 								 
}

void EndPoint::setPort(std::string port){
	ep += ':';
	ep += port;
}

///////////////////////////////////////////////////////////////
// Build Messages

Message::Message(const std::string& text) 
       : body_(text), returnAddr_(EndPoint("127.0.0.1",8000)), type_(MsgType::text),receiverInfo_(EndPoint("127.0.0.1",8000)) {}

Message::Message(const std::string& body, EndPoint returnAddr, MsgType type)
       : body_(body), returnAddr_(returnAddr), type_(type),receiverInfo_(EndPoint("127.0.0.1",8000))  {}

Message::Message(const Message& msg) 
	: body_(msg.body_), returnAddr_(msg.returnAddr_), type_(msg.type_) ,receiverInfo_(msg.receiverInfo_),
	sendFileName_(msg.sendFileName_), sizeOfBody_(msg.sizeOfBody_), m_endoffile(msg.m_endoffile) {}

std::string& Message::body() { return body_; }
  
EndPoint& Message::returnAddress() { return returnAddr_; }
  
MsgType& Message::getMsgType() { return type_; }

std::string& Message::getEndPoint() { return returnAddr_.ToString(); }

EndPoint& Message::returnRecevierAddr(){
	return receiverInfo_;
}


//----< return string form of Message type >-----------------------------

std::string Message::TypeToString()
{
	switch(type_)
	{
	case text:
		return "text";
	case file:
		return "file";
	}
	return "unknown type";
}
//----< serialize message to string >------------------------------------

std::string Message::ToString()
{
	  XmlWriter wrt, bwrt;
	  wrt.start("msg")
		.addBody(bwrt
		  .element("type",TypeToString())
				.element("destine",receiverInfo_.ToString())
		  .element("senderAddress",returnAddress().ToString())
		  .element("body",body()).xml()
		  ).end();
	  return wrt.xml();
}

//----< add Message sender XML header >-------------------------------
std::string Message::AddHeaderToSenderMessage(){
	XmlWriter wrt, bwrt;
	std::stringstream sb;
	sb << sizeOfBody_; 
  wrt.start("msg")
    .addBody(bwrt
      .element("type",TypeToString())
			.element("filename",sendFileName_)
			.element("destine",receiverInfo_.ToString())
      .element("senderAddress",returnAddress().ToString())
			.element("blockSize",sb.str())
			.element("endofFile",m_endoffile)
			.element("body",body()).xml()
      ).end();
  return wrt.xml();
}

//----< Return the Tag of end of file >-------------------------
std::string Message::returnTagOfEOF(){
	return m_endoffile;
}

//----< Initialize the Information of Message >-------------------------
void Message::InitializeMes( EndPoint destPoint, std::string filename ){
	receiverInfo_ = destPoint;
	sendFileName_ = filename; 
}

//----< set the size of body in Message >-------------------------------
size_t & Message::sizeOfBlock(){
	return sizeOfBody_;
}

//----< set the EOF tag in Message >-------------------------------
std::string & Message::setEOF(){
	return m_endoffile;
}


//----< remove quotes from quoted string >-------------------------------

std::string Message::trimQuotes(const std::string& s)
{
	if(s.size() > 0 && s[0] == '"')
		return s.substr(1,s.size()-2);
	if(s.size() > 0 && s[0] == '\'')
		return s.substr(1,s.size()-2);
	return s;
}

std::string Message::returnFileName(){
	return sendFileName_;
}
//----< deserialize message string into Message instance >---------------

Message Message::FromString(const std::string& xml)
{
	Message msg("");
	XmlReader rdr(xml);
	while(rdr.next())
	{
		std::string tag = rdr.tag();
		if(tag == "type")
		{
		  if(rdr.body() =="text")
		    msg.getMsgType() = MsgType::text;
		  else
		    msg.getMsgType() = MsgType::file;
		}
		else if(tag == "filename")
		{
			msg.sendFileName_ = rdr.body();
		}
		else if(tag == "senderAddress")
		{
			msg.returnAddress() = rdr.body();
		}
		else if(tag == "destine")
		{
			msg.returnRecevierAddr() = rdr.body();
		}
		else if(tag == "blockSize")
		{
			std::istringstream iss(rdr.body());
			size_t number = size_t(atoi(rdr.body().c_str()));
			msg.sizeOfBody_ = number;
		}
		else if(tag == "endofFile")
		{
			msg.m_endoffile = rdr.body();
		}
		else if(tag == "body")
		{
			msg.body() = rdr.body();
		}
	}
	return msg;
}

//IMessage* IMessage::MakeMessage(const std::string& EndPoint, MsgType type, const std::string& body)
//{
//  return new Message(body, EndPoint, type);
//}

EndPoint testEndPoint()
{ 
	EndPoint ep("127.0.0.1", 8000);
	std::cout << "\n  EndPoint = " << ep.ToString();
	std::cout << "\n  Address  = " << ep.address();
	std::cout << "\n  Port     = " << ep.port();
	return ep;
}

Message testMessage(EndPoint ep)
{
	Message msg("This is a message", ep, MsgType::text);
	std::cout << "\n  Message body = " << msg.body();
	std::cout << "\n  Message return address = " << msg.returnAddress().ToString();
	std::cout << "\n  Message type = " << msg.TypeToString();
	std::cout << "\n";
	std::cout << "\n  Message:";
	std::cout << "\n  " << msg.ToString();
	return msg;
}

void testMessage(Message& msg)
{
	std::cout << "\n  Message body = " << msg.body();
	std::cout << "\n  Message return address = " << msg.returnAddress().ToString();
	std::cout << "\n  Message type = " << msg.TypeToString();
	std::cout << "\n";
}
//----< test stub >------------------------------------------------------

#ifdef TEST_MESSAGE

void main()
{
	std::cout << "\n  Demonstrating EndPoint and Message Operations";
	std::cout << "\n ===============================================\n";
	
	EndPoint ep = testEndPoint();
	std::cout << "\n";
	
	Message msg = testMessage(ep);
	std::cout << std::endl;
	
	Message msg2("// this is a C++ comment");
	msg2.getMsgType() =  MsgType::file;
	std::cout << "\n  " << msg2.ToString();
	testMessage(msg2);
	
	Message msg3("/* this is a C comment */");
	msg3.getMsgType() =  MsgType::text;
	std::cout << "\n  " << msg3.ToString();
	testMessage(msg3);
	
	Message msg4("std::cout << \"a string\"");
	std::cout << "\n  " << msg4.ToString();
	testMessage(msg4);
	
	std::cout << "\n\n";
}

#endif
