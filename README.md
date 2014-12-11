P2P-Socket-based-communication-Channel
======================================

Peer To Peer Socket-Based Communication System

Each peer can send text message or files(including binary files) to other peer.

And as a receiver peer, it can handle mulitple connections from others by using asynchronous processing a message Queue structure.

Channel Module:

	Sender Module/ Receiver Module::
	
		Socket Module: Wrapper of using Win32 Socket API to make communication.
		
		Blocking Queue Module: Using std::queue with win32 lock and condiction variable to use like a thread-safe message queue.
		
		Thread Module: It's wrapper of Win32 threads, providing a simple interface similar to the .Net Thread class. 
			Provided by Ph.D Fawcett, Syracuse University, CST 2-187.  
			
		Chunker Module: It provides functions to slice binary file into small blocks and transfer them into a Message Object.
		
		Message Module: This is the transfer media through the channel.
		
		Base64 Module: encode binary data into string and decodes the string back to binary file.
			Provided by Ph.D Fawcett, Syracuse University, CST 2-187. 
			
		XMLWriter Module: Serialize Message Object to string 
		
		XMLReader Module: Deserialize string to Message Object
		
		FileManage Module: Get file, path and directory info of system. 
			Provided by Ph.D Fawcett, Syracuse University, CST 2-187. 
	