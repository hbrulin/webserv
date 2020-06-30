#ifndef REQUEST_HPP
# define REQUEST_HPP

#include "env.hpp"

/*Faire deux classes : une pour reception, une pour envoi?*/

class Request : public Env {

	private:
	Request();
	char *m_buffer;
	int m_client;
	//HEADERS

	public:
	Request(char *buffer, int sock);

	protected:
	void parse();
	void handle();
	void send_to_client();
};


#endif