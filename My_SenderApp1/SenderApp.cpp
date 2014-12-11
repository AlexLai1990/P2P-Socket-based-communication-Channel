/////////////////////////////////////////////////////////////////////
// SenderApp.cpp - Demonstrates simple use of SocketCommunicator   //
//                     Sends messages to ReceiverApp               //
// ver 2.1                                                         //
// Language:      Visual C++, 2008                                 //
// Platform:      Dell Dimension T7400, Win 7 Pro                  //
// Application:   Utility for CSE687 projects, Spring 2010         //
// Author:        Jim Fawcett, Syracuse University, CST 4-187      //
//                (315) 443-3948, jfawcett@twcny.rr.com            //
/////////////////////////////////////////////////////////////////////
/*
  Note:
  - This end starts sending then when done starts receiving.
  - Since we are running Sender and Receiver on same thread, we must 
    first do all our sending then do all our receiving.
  - There would be no such constraint if we ran sender and receiver on
    separate threads.
  - That is what you should do in Project #4.

  Maintenance History:
  ====================
  ver 2.1 : 24 Apr 11
  - Added namespace SocketCommunicator to Comm.h and Message.h which
    will allow you to avoid conflicts with the .Net Message type.
    That means you will need to declare "using namespace SocketCommunicator;"
    in some places in your code.
  - Fixed a bug discovered by Jingyi Ren in the Message type handling.
    She provided a solution, and that worked and has been incorporated.
  ver 2.0 : 23 Apr 11
  - Thanks to Amit Ahlawat, Himanshu Gupta, Kajal Kapoor, and Jingyi Ren
    for submitting bug reports.
  - added base64 encoding of message bodies when sending files to
    avoid problems with XML markup characters in the file's text.
  - you may wish to encode all messages to avoid switching back and
    forth (as I did below) when sending files.
  ver 1.1 : 17 Apr 11
  - added retry loop while connecting
  - added a Receiver section to demonstrate two-way communication
  ver 1.0 : 16 Apr 10
  - first release
*/

#include "../P2PCommunication/Comm.h"
#include <iostream>
#include <sstream>
#include <conio.h>
#include <fstream>
#include "../Base64Encoding/Base64.h"

using namespace SocketCommunicator;

template <typename T>
std::string Convert(const T& t)
{
  std::ostringstream temp;
  temp << t;
  return temp.str();
}

void main( int argc, char* argv[] )
{
	
	try
	{
		if ( argc != 5 ) {
			return ;
		}

		else {
			std::cout << "\n  Creating My Sending Machine";
			std::cout << "\n ====================\n";
			EndPoint EpDest;
			EpDest.setAddr(argv[2]);
			EpDest.setPort(argv[3]);

			Channel c1 ( std::atol(argv[1]), EpDest, argv[4] ); 
			c1.sendFile( EpDest, argv[4] );
			/*
			Receiver r(5001); 

			std::string ip = "localhost";
			int port1 = 8000;
			int port2 = 9900;
			EndPoint tryPoint1(ip,port1);
			EndPoint tryPoint2(ip,port2);

			Sender sndr4;
			sndr4.tryToSendFile(tryPoint1, "pdffile2.pdf" );

			// Sender sndr;
			// sndr.tryToSendText(tryPoint1, "Test 123" );

			Sender sndr2; 
			sndr2.tryToSendFile(tryPoint1, "pic.jpg" );

			Sender sndr3;
			sndr3.tryToSendFile(tryPoint1, "1.txt" );
			*/
		

		// system("pause");
		
			while(true){
		
			}
			std::cout << "\n\n" << " Closing Sender\n"; 
		}
	}
	 
	catch(std::exception& ex){
    std::cout << "\n\n  " << ex.what();
  }
  std::cout << "\n\n";
}
