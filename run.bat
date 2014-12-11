cd Debug
start .\My_ReceiverApp1.exe 8000 ..\\ReceiverFolder1\\
start .\My_ReceiverApp2.exe 8001 ..\\ReceiverFolder2\\
start .\My_SenderApp1.exe 5001 localhost 8000 ..\\SenderFileFolder1\\pic.jpg
start .\My_SenderApp2.exe 5002 localhost 8000 ..\\SenderFileFolder2\\1.txt ..\\SendingFolder\\