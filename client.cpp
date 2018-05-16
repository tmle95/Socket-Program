
// Client

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

using namespace std;

int main() {

	const int MAXHOSTNAME = 9;
	bool running = true;
	string tempName, message, message2 = "";
	string temp = "";
	string name;
	int portNumber, sock;
	int bufferSize = 256;
	char chat[bufferSize];
	char chat2[bufferSize];
	char chat3[bufferSize];
	char servName[MAXHOSTNAME + 1];

	struct sockaddr_in sa;
	struct hostent *hp;
	memset(&sa, 0, sizeof(struct sockaddr_in));
	while (tempName != "localhost") {
		// Prompt user for server's user name
		tempName = "";
		cout << "Enter server host name: ";
		cin >> servName;
		hp = gethostbyname(servName);
		for (int i = 0; i < strlen(servName); i++) {
			tempName += servName[i];
		}
		if (tempName == "exit") {
			cout << "Failed to get host name" << endl;
			return (-1);
		}
	}

	// Prompt user for server's port number
	cout << "Enter server port number: ";
	cin >> portNumber;

	// Create a socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cout << "Unable to create socket" << endl;
		return (-1);
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(portNumber);
	cin.ignore();
	cout << "\nType \"Terminate\" to exit the server \n" << endl;
	// Connects it to server
	if (connect(sock, (struct sockaddr*)&sa, sizeof(sa)) == 0) {
		// Prompt user for a major to send to the server
		while (temp != "Terminate") {

			memset(chat, 0, sizeof(chat));
			do {
				cout << "Enter a college major: ";
				cin.getline(chat, bufferSize);
				send(sock, chat, bufferSize, 0);
				running = false;
				temp = "";
				for (int i = 0; i < strlen(chat); i++) {
					temp += chat[i];
				}

			} while (running);
			running = true;
			//If user inputs enter exit the loop
			if (temp == "") {
				cout << endl << "Empty string... loop ending" << endl << endl;
				break;
			}
			memset(chat, 0, sizeof(chat));
			if (temp != "Terminate") {
				do {
					// Receives the information(s) requested from server
					recv(sock, chat3, bufferSize, 0);
					for (int a = 0; a < strlen(chat3); a++) {
						message += chat3[a];
					}
					recv(sock, chat2, bufferSize, 0);
					for (int b = 0; b < strlen(chat2); b++) {
						message2 += chat2[b];
					}
					if (chat3[0] == ' ' && chat2[0] == ' ') {
						cout << endl << "That major is not in the table" << endl << endl;
						running = false;
					}
					else {
						// Prints out the average pays of the major it got from the server
						cout << endl << "The average early career pay for a " << temp << " major is $" << message << endl;
						cout << "The corresponding mid-career pay is $" << message2 << endl << endl;
						running = false;
					}
				} while (running);
			}
			message = "";
			message2 = "";
			running = true;
		}
	}
	else {
		//failed to connect exit
		cout << "Failed to connect to server" << endl;
		return (-1);
	}

	// Close the socket
	close(sock);

	return 0;
}