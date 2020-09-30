#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;


int main(void)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return 1;
	
	int port = 8080;
	string ipAddress = "127.0.0.1";

	sockaddr_in hint;

	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    //	While loop:
    char buf[4096];
    string userInput = "GET / HTTP/1.1\r\n";
	userInput += "Host: localhost:80\r\n";
	userInput += "User-Agent: personal_client/1.1\r\n";
	userInput += "Accept-Encoding: gzip\r\n\r\n";

    //		Send to server
    int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
    if (sendRes == -1)
        cout << "Could not send to server! \r\n";

    //		Wait for response
    memset(buf, 0, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    if (bytesReceived == -1)
        cout << "There was an error getting response from server\r\n";
    else
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";

    //	Close the socket
    close(sock);

    return 0;

}