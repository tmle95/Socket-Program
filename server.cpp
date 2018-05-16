
// Server

#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

using namespace std;

struct Table {
	string Major;
	string earlyPay;
	string midPay;
};

int main() {

	Table table[512];
	bool running = true;
	int count = 0;
	string fileName, tempMajor, tempearlyPay, temp, tempmidPay = "";
	string message = "";
	string data, x;
	int sock, portNumber, server;
	int bufferSize = 256;
	char chat[bufferSize];
	char chat2[bufferSize];
	char chat3[bufferSize];

	struct sockaddr_in sa;
	socklen_t size;

	memset(&sa, 0, sizeof(struct sockaddr_in));

	// Prompts user for the name of a file
	cout << "Enter name of file: ";
	cin >> fileName;

	// Receives all the data from the file and stores it into a table
	ifstream ifs;
	stringstream ss;
	ifs.open(fileName.c_str());
	while (getline(ifs, data)) {
		ss << data;
		ss >> x;
		tempMajor = x;
		x.clear();
		while (ss >> x) {
			if (isalpha(x.at(0)) || x.at(0) == '&' || x.at(0) == '(' || x.at(0) == ')') {
				tempMajor = tempMajor + ' ' + x;
				x.clear();
			}
			else {
				tempearlyPay = x;
				x.clear();
				ss >> x;
				tempmidPay = x;
				x.clear();
			}
		}
		ss.clear();
		table[count].Major = tempMajor;
		table[count].earlyPay = tempearlyPay;
		table[count].midPay = tempmidPay;
		count++;
	}
	ifs.close();

	// Prompt user for server's port number
	cout << "Enter server port number: ";
	cin >> portNumber;

	// Create sockets
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << "Failed to create socket" << endl;
		return (-1);
	}

	// Binds an address to sockets
	sa.sin_family = AF_INET;
	sa.sin_port = htons(portNumber);
	if (bind(sock, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
		cout << "Failed to bind socket" << endl;
		return (-1);
	}

	// Set up the buffer size for the socket
	size = sizeof(sa);
	listen(sock, 3);

	// Accepts incoming calls from client
	server = accept(sock, (struct sockaddr*)&sa, &size);
	if (server < 0) {
		cout << "Unable to accept server." << endl;
		return (-1);
	}

	// Receives messages from the client per request
	while (temp != "Terminate") {
		message = "";
		memset(chat, 0, sizeof(chat));
		do {
			recv(server, chat, bufferSize, 0);

			running = false;
			temp = "";
			for (int i = 0; i < strlen(chat); i++) {
				temp += chat[i];
				message += chat[i];
			}
		} while (running);
		running = true;

		if (message != "Terminate") {
			do {
				for (int i = 0; i < 1024; i++) {
					// Sends back to the client with corresponding informations
					if (message == table[i].Major) {
						tempearlyPay = table[i].earlyPay;
						for (int k = 0; k < tempearlyPay.length(); k++) {
							chat3[k] = tempearlyPay[k];
						}
						tempmidPay = table[i].midPay;
						for (int j = 0; j < tempmidPay.length(); j++) {
							chat2[j] = tempmidPay[j];
						}
						running = false;
						send(server, chat3, bufferSize, 0);
						send(server, chat2, bufferSize, 0);
						break;
					}
					if(table[i+1].Major == "") {
						chat3[0] = ' ';
						chat2[0] = ' ';
						running = false;
						send(server, chat3, bufferSize, 0);
						send(server, chat2, bufferSize, 0);
						break;
					}
				}
			} while (running);
		}
		memset(chat2, 0, sizeof(chat2));
		memset(chat3, 0, sizeof(chat3));
		running = true;
		//close server if message recieved from client is Terminate
		if (temp == "Terminate") {
			close(server);
		}
	}

	// Close socket
	close(sock);

	return 0;
}