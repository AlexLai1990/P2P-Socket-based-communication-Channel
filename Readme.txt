Xincheng Lai 

Demo: two sending applications and two receiving appication
I will run 4 running projects to demonstrate all the requirments!!!
3 File Folder : SenderFileFolder1,SenderFileFolder2,SendingFolder to prepare for the files to be sent.
2 File FOlder : ReceiverFolder1,ReceiverFolder2 to receive the file from Sender1 and Sender2.

From SenderFileFolder1 ==> To ReceiverFolder
      Sender1	       ==> Receiver1 :  pic.jpg (it's an argument specified binary file) and a Text Message "Haha!!!  This is a Text Message" 
		       ==> Receiver2 :  pdffile.pdf (another binary file, i set with hard code, in application for demonstrating requirment)
      Sender2	       ==> Receiver1 :  1.txt
		       ==> Receiver2 :  2.txt and a filefolder "..\\SendingFolder\\" 
		( You can use your file folder path in SenderApplication2 with 5th argument)

Note:
1. You can run the run.bat to see the default result.
   Please Use the format arguments:
   Receiver App has 2 arguments: listening port, ReceiverFolder.
   Sender App has 4 arguments: Local listening port, Destine IP, Destine Port, Source FilePath.
   Sender App2 has 5 arguments: Local listening port, Destine IP, Destine Port, Source FilePath, File Folder.
   
   following is my run.bat. 		as my default run.
	cd Debug
	start .\My_ReceiverApp1.exe 8000 ..\\ReceiverFolder1\\
	start .\My_ReceiverApp2.exe 8001 ..\\ReceiverFolder2\\
	start .\My_SenderApp1.exe 5001 localhost 8000 ..\\SenderFileFolder1\\pic.jpg
	start .\My_SenderApp2.exe 5002 localhost 8000 ..\\SenderFileFolder2\\1.txt ..\\SendingFolder\\
	
	Notice!!
	In order to meet the requirment#7, demonstrate one sender can send to different receivers.
	I will hard code in application to change the My_SenderApp1's Destine port to 8001 
	after it send 8000 port a pic.jpg.
	So it will send another binary file pdffile.pdf along with pic.jpg to Receiver2 which has port 8002.
	Same for My_SenderApp2, I will also make it to send Receiver2 another file like 2.txt.
	
	In order to demonstrate it can post text message, actually i send a "Haha!!!  This is a Text Message"
	from My_SenderApp1 to Receiver1, it will show in the console of Receiver.
	Actually, the Acknowledge every time i used for sure is also text message.
	
2.  If you want to send your file for testing
	1.	put your test files into SenderFileFolder1 or SenderFileFolder2
	2.	And then Change the Filename in run.bat, like the following 
	start .\My_SenderApp1.exe 5001 localhost 8000 ..\\SenderFileFolder2\\your file
	And you can get the file you just sent in corresponding Receiver folder.
	3.  I can also send a folder. To demonstrate that, you need change the SenderApp2's last argument to your folder
	just like the way i put.