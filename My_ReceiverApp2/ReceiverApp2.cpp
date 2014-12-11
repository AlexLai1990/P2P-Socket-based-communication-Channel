/////////////////////////////////////////////////////////////////////
// ReceiverApp2.cpp - Demonstrates the P2PCommunication			   //
//                     Get messages from SenderAPP/SenderApp2      //         
// ver 1.1                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Win 8											   //
// Application:   CSE687 OOD  Project 3							   //
// Author:        Xincheng Lai									   //
/////////////////////////////////////////////////////////////////////

/* 
  Maintenance History:
  ====================
  ver 1.1 : 14 Apr 13
	- Revise the output on the console
  ver 1.0 : 01 Apr 13
  - first release, frame the inputs of console and core operation in main function
*/

#include "..\P2PCommunication\comm.h"
#include "..\Base64Encoding\Base64.h"
#include <iostream>
#include <conio.h>

using namespace SocketCommunicator;

void main( int argc, char* argv[] )
{
	if ( argc != 3 ) {
		std::cout<< "\n\n  You should input two argument in Receiver APP\n"
			<<"  1. Listening port in this machine\n  2.Correct Receiver Folder";
			return ;
	}

	std::cout << "\n  Creating My Receiving Machine2"; 
	DisplayMachine DisMyMachine;
	DisMyMachine.showMachineInfo();
	std::cout<<"  Its listening port is "<<argv[1]<<"\n  ========================================\n";

	try {
		Channel c_rcvr( std::atol(argv[1]), argv[2] );
		while(true)	{ }
		std::cout << "\n\n";  
	}
	catch(std::exception& ex) {
		std::cout << "\n\n  " << ex.what();
	}
	std::cout << "\n\n";
}
