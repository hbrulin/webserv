#include "listener.hpp"

void Listener::set_non_blocking() {
	//std::cout << m_sock << std::endl;
	if (fcntl(m_sock, F_SETFL, O_NONBLOCK) < 0) {
		strerror(errno);
		exit(EXIT_FAILURE);
	}
}

Listener::Listener() {
	memset((char *) &m_address, 0, sizeof(m_address));
	m_port = 0;
	m_sock = 0;
	memset((char *) &m_connect_list, 0, sizeof(m_connect_list));
	//set = NULL;
	m_highsock = 0;
	m_run = true;
}

int Listener::init() {
	int reuse_addr = 1;  /* Used so we can re-bind to our port
				while a previous connection is still
				in TIME_WAIT state : TIME_WAIT indicates that server
				has closed the connection. The connection is being kept around 
				so that any delayed packets can be matched to the connection 
				and handled appropriately. The connections will be 
				removed when they time out within four minutes.*/
	
	m_port = 8080; //change according to config file

	/* Obtain a file descriptor for listening socket */
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock < 0) {
		strerror(errno);
		exit(EXIT_FAILURE);
	}

	/* Allow socket descriptor to be reuseable */
	/* So that we can re-bind to sock without TIME_WAIT problems, without waiting 4 minutes */
	/*Indique que les règles permettant la validation des adresses 
	fournies dans un appel bind(2) doivent autoriser la réutilisation 
	des adresses locales, même en TIME_WAIT. */
	setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
		sizeof(reuse_addr));

	set_non_blocking();

	// Bind the ip address and port to a socket
	m_address.sin_family = AF_INET;
    m_address.sin_port = m_port; //besoin htons ou atoport??
    m_address.sin_addr.s_addr = inet_addr("0.0.0.0"); //any address
 
    if (bind(m_sock, (struct sockaddr*) &m_address, sizeof(m_address)) < 0)
	{
		strerror(errno);
		close(m_sock);
		exit(EXIT_FAILURE);
	}

	/* This socket will be for listening */
	/*marque la socket référencée par sockfd comme une socket passive, 
	c'est-à-dire comme une socket qui sera utilisée pour accepter les demandes 
	de connexions entrantes en utilisant accept(2)
	L'argument backlog définit une longueur maximale jusqu'à laquelle la file 
	des connexions en attente pour sockfd peut croître. 
	Si une nouvelle connexion arrive alors que la file est pleine, 
	le client reçoit une erreur indiquant ECONNREFUSED. 
	SOMAXCONN defines the maximum number you're allowed to pass to listen(), depends on system*/
	listen(m_sock,SOMAXCONN); 

	/* Since we start with only one socket, the listening socket,
	   it is the highest socket so far. */
	m_highsock = m_sock;


	return 0;
}

int Listener::run() {
	while (m_run) {

	}

	return 0;
}

