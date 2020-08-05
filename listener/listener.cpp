#include "listener.hpp"

Listener::Listener(std::vector<Config> conf, int size) {

	_size = size;
	_conf = conf;

	SERVER_PROTOCOL = "HTTP/1.1";
	SERVER_SOFTWARE = "webserv/1.1";
	GATEWAY_INTERFACE = "CGI/1.1";

	//SECU
	m_port = (int *)malloc(sizeof(int) * size + 1);
	m_sock = (int *)malloc(sizeof(int) * size + 1);
	m_address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * size + 1);
	m_run = true;
	//m_set = (fd_set *)malloc(sizeof(fd_set) * size + 1);
	//m_working_set = (fd_set *)malloc(sizeof(fd_set) * size + 1);
	//m_highsock = (int *)malloc(sizeof(int) * size + 1);
	m_highsock = 0;
	memset((char *) &m_set, 0, sizeof(m_set));
	memset((char *) &m_working_set, 0, sizeof(m_working_set));

	for (int i = 0; i < size; i++)
	{
		memset((int *) &m_port[i], 0, sizeof(m_port[i]));
		memset((int *) &m_sock[i], 0, sizeof(m_sock[i]));
		//memset((int *) &m_highsock[i], 0, sizeof(m_highsock));
		memset((char *) &m_address[i], 0, sizeof(m_address[i]));
		//memset((char *) &m_set[i], 0, sizeof(m_set));
		//memset((char *) &m_working_set[i], 0, sizeof(m_working_set));
	}
}

/*Change things according to META VARIABLES*/
int Listener::init() {
	int reuse_addr = 1;  /* Used so we can re-bind to our port
				while a previous connection is still
				in TIME_WAIT state : TIME_WAIT indicates that server
				has closed the connection. The connection is being kept around
				so that any delayed packets can be matched to the connection
				and handled appropriately. The connections will be
				removed when they time out within four minutes.*/

	for (int i = 0; i < _size ; i++) {
		m_port[i] = _conf[i]._listen;
	}

	/* Obtain a file descriptor for listening socket */
	for (int i = 0; i < _size ; i++) {
		m_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (m_sock[i] < 0) {
			strerror(errno);
			exit(EXIT_FAILURE);
		} //VOIR SI ON EXIT SI UN SERVEUR SUR PLUSIEURS FAIL
	}

	/* Allow socket descriptor to be reuseable */
	/* So that we can re-bind to sock without TIME_WAIT problems, without waiting 4 minutes */
	/*Indique que les règles permettant la validation des adresses
	fournies dans un appel bind(2) doivent autoriser la réutilisation
	des adresses locales, même en TIME_WAIT. */
	for (int i = 0; i < _size ; i++) {
		setsockopt(m_sock[i], SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	}

	/*All of the sockets for the incoming connections will also be nonblocking since
   	they will inherit that state from the listening socket.  */
	//std::cout << _size << std::endl;
	for (int i = 0; i < _size ; i++) {
		set_non_blocking(m_sock[i]);
	}

	int swapped;
	// Bind the ip address and port to a socket
	for (int i = 0; i < _size ; i++) {
		m_address[i].sin_family = AF_INET;
		swapped = (m_port[i]>>8) | (m_port[i]<<8);
		m_address[i].sin_port = swapped; //replaces htons()
		//m_address.sin_addr.s_addr = inet_addr("0.0.0.0"); //any address
		m_address[i].sin_addr.s_addr = inet_addr(_conf[i]._host.c_str());
	}

	for (int i = 0; i < _size ; i++) {
		if (bind(m_sock[i], (struct sockaddr*) &m_address[i], sizeof(m_address[i])) < 0)
		{
			//std::cout << errno << std::endl;
			strerror(errno);
			//perror("msg");
			close(m_sock[i]);
			exit(EXIT_FAILURE); //VOIR SI ON EXIT SI UN SERVEUR SUR PLUSIEURS FAIL
		}
	}
	//std::cout << "test" << std::endl;

	/* This socket will be for listening */
	/*marque la socket référencée par sockfd comme une socket passive,
	c'est-à-dire comme une socket qui sera utilisée pour accepter les demandes
	de connexions entrantes en utilisant accept(2)
	L'argument backlog définit une longueur maximale jusqu'à laquelle la file
	des connexions en attente pour sockfd peut croître.
	Si une nouvelle connexion arrive alors que la file est pleine,
	le client reçoit une erreur indiquant ECONNREFUSED.
	SOMAXCONN defines the maximum number you're allowed to pass to listen(), depends on system*/
	for (int i = 0; i < _size ; i++) {
		listen(m_sock[i],SOMAXCONN);
	}

	/*Build master fd_set*/
	build_fd_set();

	return 0;
}

/*A priori no need for timeout*/
int Listener::run() {
	int sock_count;
	std::pair<int, int> ret;

	while (m_run) {
		/* Copy the master fd_set over to the working fd_set.
		Important because the call to select() is destructive. The copy
		only contains the sockets that are accepting inbound connection requests OR messages.
		E.g. You have a server and it's master file descriptor set contains 5 items;
		the listening socket and four clients. When you pass this set into select(),
		only the sockets that are interacting with the server are returned. Let's say
		only one client is sending a message at that time. The contents of 'copy' will
		be one socket. You will have LOST all the other sockets.*/
		//for (int i = 0; i < _size ; i++) {
			memcpy(&m_working_set, &m_set, sizeof(m_set));

			/*calling select()*/
			/* The first argument to select is the highest file
				descriptor value plus 1.
			The second argument to select() is the address of
				the fd_set that contains sockets we're waiting
				to be readable (including the listening socket).*/
			sock_count = select(m_highsock + 1, &m_working_set, NULL, NULL, NULL);
			if (sock_count < 0) {
				strerror(errno);
				exit(EXIT_FAILURE); //FAUT-IL EXIT SI SELECT FAIL?
			}

			/*Descriptors are available*/
			for (int j = 0; j <= m_highsock && sock_count > 0; j++) {
				if (FD_ISSET(j, &m_working_set)) {//if descriptor is ready, is in working_set
					//std::cout << "test" << std::endl;
					//Fd is already readable - we have one less to look for. So that we can eventually stop looking
					sock_count -= 1;

					/*Check to see if the FD is the listening socket (m_sock). If it is,
					Accept all incoming connections that are queued up on the listening socket before we
					loop back and call select again.*/
					ret = look_for_sock(j);
					//std::cout << ret.first << ret.second << std::endl;
					if (ret.first) {
						//std::cout << "test" << std::endl;
						accept_incoming_connections(ret.first);
					}
					else { //if it is not listening socket, then there is a readable connexion that was added in master set and passed into working set
						m_close = false;
						receive_data(j); //receive all incoming data on socket before looping back and calling select again
						close_conn(j);
					}

				}
			}
		//}
	}

	return 0;
}

std::pair<int, int>	Listener::look_for_sock(int j)
{
	for (int i = 0; i < _size ; i++) {
		//std::cout << m_sock[i] << std::endl;
		if (j == m_sock[i])
			return std::pair<int, int>(j, i);
	}
	return std::pair<int, int>(0, 0);
}

/*void Listener::clean() {
	for (int i=0; i <= m_highsock; ++i)
   	{
    	if (FD_ISSET(i, &m_set))
        close(i);
   }
}*/

void Listener::set_non_blocking(int sock) {
	//std::cout << m_sock << std::endl;
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		//std::cout << errno << std::endl;
		strerror(errno);
		exit(EXIT_FAILURE); //check if exit if only one fails
	}
}

/*prepare fd_set : sock variable for connections coming in + other sockets already accepted*/
void Listener::build_fd_set() {
	FD_ZERO(&m_set); //clear out so no fd inside
	for (int i = 0; i < _size ; i++) {
		//FD_ZERO(&m_set);
		FD_SET(m_sock[i], &m_set); //adds m_sock to set, so that select() will return if a connection comes in on that socket -> will trigger accept() etc...

		/* Since we start with only one socket, the listening socket,
		it is the highest socket so far. */
		m_highsock = m_sock[i];
	}
}


/* Accept each incoming connection.  If
accept fails with EWOULDBLOCK, then we
have accepted all of them.  Any other
failure on accept will cause us to end the server */
void Listener::accept_incoming_connections(int i) {
	int	new_sock = 0;
	while (new_sock != -1) {
		new_sock = accept(i, NULL, NULL);
		if (new_sock < 0) {
			/*on ne devait jamais avoir cette erreur si select() marche bien */
			if (errno != EWOULDBLOCK) { //would block if it was non blocking and if we were not handling closing connections. A la place on aurra message erreur servi?
				strerror(errno); //potentiellement ne rien faire, ne pas afficher de message d'erreur
				//m_run = false; //on ne fait rien si renvoie -1, on n'arrête pas le serveur
			}
			break;
		}
		/*Add new incoming connection to master fd_set*/
		FD_SET(new_sock, &m_set);
		if (new_sock > m_highsock)
			m_highsock = new_sock;
	}
}

/* Receive data on this connection until the
recv fails with EWOULDBLOCK.  If any other failure occurs,
we will close the connection.    */
void Listener::receive_data(int fd) {
	int ret;
	char buffer[4096]; //taille buffer??
	memset((char *) &buffer, 0, sizeof(buffer));
	/*This error checking is compliant with correction - check for -1 and 0 */
	while (1)
	{
		memset((char *) &buffer, 0, sizeof(buffer));
		ret = recv(fd, buffer, sizeof(buffer), 0);
	//	std::cout << "Received: " << std::string(buffer, 0, sizeof(buffer));
		if (ret < 0) {
			m_close = true; //client will be removed if error
			break;
		}

		/*Check if connection was closed by client*/
		if (ret == 0) {
			//print something?
			m_close = true; //client will be removed
			break;
		}

		//else data was received
		//len = ret;
		//send(fd, buffer, len, 0);

		int i = look_for_sock(fd).second;
		Request req(buffer, fd, _conf[i], *m_port); //mettre direct dans le Listener
		//std::cout << buffer << std::endl;
		req.parse();
		req.handle();
		//error checking to comply with correction : if error, client will be removed
		if (req.send_to_client() == -1)
		{
			m_close = true;
			break;
		}

	}
}

/*If the m_close flag was turned on, we need
to clean up this active connection.  This
clean up process includes removing the
descriptor from the master set and
determining the new maximum descriptor value
based on the bits that are still turned on in
the master set.  */
void Listener::close_conn(int fd) {
	if (m_close) {
		close(fd);
		FD_CLR(fd, &m_set);
		if (fd == m_highsock) {
			while (!(FD_ISSET(m_highsock, &m_set)))
				m_highsock -= 1;
		}
	}
}
