/////////////////////////////////////////////////////////////////////
// SenderApp2.cpp - Demonstrates the P2PCommunication							 //
//                     Sends messages to ReceiverApp               //
// ver 1.1                                                         //
// Language:      Visual C++, 2012                                 //
// Platform:      Win 8																						 //
// Application:   CSE687 OOD  Project 3														 //
// Author:        Xincheng Lai															       //
/////////////////////////////////////////////////////////////////////

/* 
  Maintenance History:
  ====================
  ver 1.1 : 14 Apr 13
	- Revise the output on the console
  ver 1.0 : 01 Apr 13
  - first release, frame the inputs of console and core operation in main function
*/

#include "../P2PCommunication/Comm.h"
#include <iostream>
#include <sstream>
#include <conio.h>
#include <fstream>
#include "../Base64Encoding/Base64.h"

using namespace SocketCommunicator;

void main( int argc, char* argv[] )
{
	try{
		// The application is supposed to receive 4 inputs: This Machine Listening port, Destine IP, Destine Port, Source Filepath.
		if ( argc != 6 ) {
			std::cout<< "\n  Please input the correct arguments!!\n";
			std::cout<<"\n\n  You should input 4 Argument:\n  1.Listenport 2.Destine IP 3. Destine Port 4. Correct Source FilePath 5.Correct Folder path to be sent";
			return ;
		}
		else {
			std::cout << "\n  Creating My Sending Machine2"; 
			DisplayMachine DisMyMachine;
			DisMyMachine.showMachineInfo();
			std::cout<<"  Its listening port is "<<argv[1]<<"\n  ========================================\n";
			std::cout<<"\n  DEMONSTRATE1: Sending file to a single Receiver Machine#1 along with Sender Machine #1\n";

			std::cout<<"\n  DEMONSTRATE2: Sending File including Binary and Text Messages\n"
				<<"		When it sends a file, it will go through chunks\n		so that it can send many kinds of files \n"
				<<"		It also could send files to some running receivers. ";

			EndPoint EpDest;
			EpDest.setAddr(argv[2]);
			EpDest.setPort(argv[3]);  

			std::cout<<"\n  Start sending #1 file\n";
			Channel c2 ( 5002, EpDest, argv[4]); 
			c2.sendFile( EpDest, argv[4] ); 

			EndPoint EpDest2;
			EpDest2.setAddr("localhost");
			EpDest2.setPort("8001");
			c2.sendFile( EpDest2 , "..\\SenderFileFolder2\\2.txt" );

			std::cout<<"\n  Start sending A folder "<< argv[5] <<" to Receiver#2 \n";
			c2.sendFileFolder( EpDest2, argv[5] );

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
