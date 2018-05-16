# Socket-Program

g++ client.cpp -o client
g++ server.cpp -o server

Run server program first, Server will prompt user for a input text file and port number.
./server
Run client program, Client will prompt user for port number and then a name of a career, if career is found in server file, client will recieve career salary information from server.
./client
