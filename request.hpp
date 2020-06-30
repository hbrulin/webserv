#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "env.hpp"
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <fstream>
#include <vector>

/*Faire deux classes : une pour reception, une pour envoi?*/

class Request : public Env {

	private:
	Request();
	char *m_buffer;
	int m_client;
	//HEADERS
	//GET
	//POST
	//FILES

	/*default outputs - à intégrer dans structures ci-dessus?? 
	config selon file .config pour index?*/
	std::string m_content;
	std::string m_not_found;
	std::string m_index;
	int m_errorCode;

	//output
	std::string m_output; //peut-être pas nécessaire, réutiliser m_content?

	//memset m_content et m_output
	public:
	Request(char *buffer, int sock) {
		m_buffer = buffer;
		m_client = sock;
		m_not_found = "/404.html";
		m_index = "/index.html";
		m_errorCode = 404; //define other error codes
	};
	void parse();
	void handle();
	void send_to_client();
};


#endif