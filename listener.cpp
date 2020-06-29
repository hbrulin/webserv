#include "listener.hpp"

Listener::Listener() {
	m_ip = NULL;
	m_port = 0;
	m_sock = 0;
	m_socket = NULL;
	//set = NULL;
}

int Listener::init() {
	m_port = 8080; //change according to config file

	/* Obtain a file descriptor for listening socket */
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock < 0) {
		strerror(errno);
		exit(EXIT_FAILURE);
	}



	return 0;
}

int Listener::run() {

	return 0;
}