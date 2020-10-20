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
    char buf[8096];
    /*string userInput = "POST /directory/youpi.bla HTTP/1.1\r\n";
	userInput += "Host: localhost:80\r\n";
	userInput += "User-Agent: personal_client/1.1\r\n";
    userInput += "Transfer-Encoding: chunked\r\n";
    userInput += "Content-Type: test/file\r\n";
    userInput += "X-Secret-Header-For-Test: 1\r\n";
	userInput += "Accept-Encoding: gzip\r\n\r\n";
    int k = 0;
    while (k++ < 100000000)
        userInput += "w";
    userInput += "0\r\n\r\n";*/

	/*string userInput = "PUT /put_test/test HTTP/1.1\r\n";
	userInput += "Host: localhost:80\r\n";
	userInput += "User-Agent: personal_client/1.1\r\n";
    //userInput += "Content-Length: 100\r\n";
    userInput += "Content-Type: test/file\r\n";
	userInput += "Accept-Encoding: gzip\r\n\r\n";
    int k = 0;
    while (k++ < 1000)
        userInput += "w";*/

	/*string userInput = " GET www/uploaded/test HTTP/1.1\r\n";
	userInput += "Host: localhost:80\r\n";
	userInput += "User-Agent: personal_client/1.1\r\n\r\n";*/

	string userInput = " GET HTTP/1.1\r\n";
	userInput += "Host: localhost:80\r\n";
	userInput += "User-Agent: personal_client/1.1\r\n\r\n";

	//string userInput = "\r\n\r\n";
	
    //		Send to server
    int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
    if (sendRes == -1)
        cout << "Could not send to server! \r\n";

    //		Wait for response
    memset(buf, 0, 8096);
    int bytesReceived = recv(sock, buf, 8096, 0);
    if (bytesReceived == -1)
        cout << "There was an error getting response from server\r\n";
    else
        cout << "SERVER> " << string(buf, bytesReceived) << "\r\n";

    //	Close the socket
    close(sock);

    return 0;

}