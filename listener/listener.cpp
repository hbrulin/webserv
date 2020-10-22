#include "listener.hpp"

bool		m_run = true;
extern fd_set		R_SET; 
extern fd_set		W_SET; 
extern int 			highsock;

Listener::Listener(std::vector<Config> conf, int size) {

	_size = size;
	_conf = conf;

	//SECU
	m_port = (int *)malloc(sizeof(int) * size + 1);
	m_sock = (int *)malloc(sizeof(int) * size + 1);
	m_address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * size + 1);
	//m_run = true;
	//ft_memset((char *) &m_r_set, 0, sizeof(m_r_set));
	//ft_memset((char *) &m_w_set, 0, sizeof(m_w_set));
	ft_memset((char *) &m_read_set, 0, sizeof(m_read_set));
	ft_memset((char *) &m_write_set, 0, sizeof(m_write_set));

	for (int i = 0; i < size; i++)
	{
		ft_memset((int *) &m_port[i], 0, sizeof(m_port[i]));
		ft_memset((int *) &m_sock[i], 0, sizeof(m_sock[i]));
		ft_memset((char *) &m_address[i], 0, sizeof(m_address[i]));
	}
}

void Listener::exiting(int n) {
	(void)n;

	std::cout << "\n" << "exiting...\n";
	m_run = false;

}

/*prepare fd_set : sock variable for connections coming in + other sockets already accepted*/
void Listener::build_fd_set() {
	FD_ZERO(&R_SET); //clear out so no fd inside
	for (int i = 0; i < _size ; i++) {
		FD_SET(m_sock[i], &R_SET); //adds m_sock to set, so that select() will return if a connection comes in on that socket -> will trigger accept() etc...

		/* Since we start with only one socket, the listening socket,
		it is the highest socket so far. */
		highsock = m_sock[i];
	}

	FD_ZERO(&W_SET); //clear out so no fd inside
	for (int i = 0; i < _size ; i++) {
		FD_SET(m_sock[i], &W_SET); //adds m_sock to set, so that select() will return if a connection comes in on that socket -> will trigger accept() etc...

		/* Since we start with only one socket, the listening socket,
		it is the highest socket so far. */
		highsock = m_sock[i];
	}
}


int Listener::init() {
	signal(SIGINT, &Listener::exiting);

	ft_memset((char *) &R_SET, 0, sizeof(R_SET));
	ft_memset((char *) &W_SET, 0, sizeof(W_SET));
	highsock = 0;

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
		if(!checkpast(i)){
			if (bind(m_sock[i], (struct sockaddr*) &m_address[i], sizeof(m_address[i])) < 0)
			{
				//std::cout << i << std::endl;
				strerror(errno);
				//perror("msg");
				close(m_sock[i]);
				exit(EXIT_FAILURE); //VOIR SI ON EXIT SI UN SERVEUR SUR PLUSIEURS FAIL
			}
		}
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
		//std::cout << req_list.size() << std::endl;
		/* Copy the master fd_set over to the read fd_set.
		Important because the call to select() is destructive. The copy
		only contains the sockets that are accepting inbound connection requests OR messages.
		E.g. You have a server and it's master file descriptor set contains 5 items;
		the listening socket and four clients. When you pass this set into select(),
		only the sockets that are interacting with the server are returned. Let's say
		only one client is sending a message at that time. The contents of 'copy' will
		be one socket. You will have LOST all the other sockets.*/
		//for (int i = 0; i < _size ; i++) {
			ft_memcpy(&m_read_set, &R_SET, sizeof(R_SET));
			ft_memcpy(&m_write_set, &W_SET, sizeof(W_SET));

			/*calling select()*/
			/* The first argument to select is the highest file
				descriptor value plus 1.
			The second argument to select() is the address of
				the fd_set that contains sockets we're waiting
				to be readable (including the listening socket).*/
			sock_count = select(highsock + 1, &m_read_set, &m_write_set, NULL, NULL);
			if (sock_count < 0) {
				strerror(errno);
			}


			/*check if files are open*/
			std::vector<Request*>::iterator it = req_list.begin();
			std::vector<Request*>::iterator ite = req_list.end();
			while (it != ite && (*it)->_status != READ_FILE)
				it++;
			if (it != ite && (*it)->file_fd != -1)
			{
				if (FD_ISSET((*it)->file_fd, &m_read_set))
				{
					if ((*it)->read_file() < 0)
						m_close = true;
				}
				continue;
				//close_conn(j);
			}
			it = req_list.begin();
			ite = req_list.end();
			while (it != ite && (*it)->_status != WRITE_FILE)
				it++;
			if (it != ite && (*it)->file_fd != -1)
			{
				if (FD_ISSET((*it)->file_fd, &m_write_set))
				{
					if ((*it)->write_file() < 0)
						m_close = true;
				}
				continue;
				//close_conn(j);
			}
			/*Descriptors are available*/
			for (int j = 0; j <= highsock && sock_count > 0; j++) {
				std::vector<Request*>::iterator it = req_list.begin();
				std::vector<Request*>::iterator ite = req_list.end();
				while (it != ite && (*it)->m_client != j)
					it++;
				//std::cout << (*it)->m_client << std::endl;
				if (FD_ISSET(j, &m_write_set) && it != ite)
				{
					//std::cout << "!!!!!!!!!!" << std::endl;
					if ((*it)->_status == SEND)
						send_data(it);
					//close_conn(j);
				}
				else if (FD_ISSET(j, &m_read_set)) {//if descriptor is ready, is in read_set
					//Fd is already readable - we have one less to look for. So that we can eventually stop looking
					sock_count -= 1;

					/*Check to see if the FD is the listening socket (m_sock). If it is,
					Accept all incoming connections that are queued up on the listening socket before we
					loop back and call select again.*/
					ret = look_for_sock(j);
					if (ret.first) {
						//std::cout << ret.first << std::endl;
						accept_incoming_connections(ret.first);
						m_nbConf = ret.second;
					}
					else { //if it is not listening socket, then there is a readable connexion that was added in master set and passed into read set
						m_close = false;
						if (buf_list.empty())
							buf_list.push_back(new Buffers(j));
						else
							{
								std::vector<Buffers*>::iterator it = buf_list.begin();
								std::vector<Buffers*>::iterator ite = buf_list.end();
								while (it != ite && (*it)->m_id != j)
									it++;
								if (it == ite)
								{
									it = buf_list.begin();
									while (it != ite && (*it)->m_id != 0)
										it++;
									if (it == ite)
										buf_list.push_back(new Buffers(j));
									else
										(*it)->m_id = j;
								}
							}
						receive_data(j); //receive all incoming data on socket before looping back and calling select again
						//m_close = true;
						//close_conn(j);
					}
				}
				close_conn(j);
			}
	}
	clean();
	return 0;
}

void Listener::set_non_blocking(int sock) {
	//std::cout << m_sock << std::endl;
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		//std::cout << errno << std::endl;
		strerror(errno);
		//exit(EXIT_FAILURE); //check if exit if only one fails
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
		FD_SET(new_sock, &R_SET);
		FD_SET(new_sock, &W_SET);
		if (new_sock > highsock)
			highsock = new_sock;
	}
}

/* Receive data on this connection until the
recv fails with EWOULDBLOCK.  If any other failure occurs,
we will close the connection.    */
void Listener::receive_data(int fd) {
	
	int 		bytes;
	int 		ret;
	std::vector<Buffers*>::iterator it = buf_list.begin();
	std::vector<Buffers*>::iterator ite = buf_list.end();
	
	//std::cout << "FD" << fd << std::endl;

	while (it != ite && (*it)->m_id != fd)
		it++;
	int n = it - buf_list.begin();

	//std::cout << "BUFFER ID " << buf_list[n]->m_id << "is n° " << n << " over " << buf_list.size() << " buffers" << std::endl;

	bytes = strlen(buf_list[n]->m_buffer);

	ret = read(fd, buf_list[n]->m_buffer + bytes, BUFFER_SIZE - bytes);

	if (ret < 0) {
		m_close = true; //client will be removed if error
		return;
	}

	/*Check if connection was closed by client*/
	else if (ret == 0)
		return;

	else
	{
		bytes += ret;
		buf_list[n]->m_buffer[bytes] = '\0';
		if (strstr(buf_list[n]->m_buffer, ENDCHARS) != NULL && !buf_list[n]->body_parse_chunk && !buf_list[n]->body_parse_length)
		{
			std::string s(buf_list[n]->m_buffer);
			//std::cout << "M_BUFFER : " << s << std::endl << std::endl;
			size_t npos = s.find(ENDCHARS);
			buf_list[n]->headers = s.substr(0, npos);
			buf_list[n]->body += s.substr(npos + 4, s.size());
			ft_memset((void *)buf_list[n]->m_buffer, 0, BUFFER_SIZE + 1);
			if (strstr(buf_list[n]->headers.c_str(), POST) != NULL || strstr(buf_list[n]->headers.c_str(), PUT) != NULL)
			{
				if (strstr(buf_list[n]->headers.c_str(), CHUNKED_STR) != NULL)
				{
					buf_list[n]->body_parse_chunk = !buf_list[n]->body_parse_chunk;
					if (buf_list[n]->body.empty() == 0 && strstr(buf_list[n]->body.c_str(), ENDCHARS_BOD) != NULL)
					{
						LaunchRequest(n, fd);
						buf_list[n]->clean_buf();
						//delete *it;
						//buf_list.erase(it);
					}
				}
				else if (strstr(buf_list[n]->headers.c_str(), CONTENT_L_STR) != NULL)
				{
					buf_list[n]->body_parse_length = !buf_list[n]->body_parse_length;
					buf_list[n]->header_length = buf_list[n]->headers.size() + 5;
					buf_list[n]->m_content_length = getLength(buf_list[n]->headers, CONTENT_L_STR);
					buf_list[n]->track_length += ret - buf_list[n]->header_length;
					//std::cout << buf_list[n]->track_length << std::endl;
					if (buf_list[n]->track_length >= buf_list[n]->m_content_length - 1)
					{
						LaunchRequest(n, fd);
						buf_list[n]->clean_buf();
						/*delete *it;
						buf_list.erase(it);*/
					}
				}
				else
				{
					LaunchRequest(n, fd);
					buf_list[n]->clean_buf();
					//delete *it;
					//buf_list.erase(it);
				}
			}
			else
			{
				LaunchRequest(n, fd);
				buf_list[n]->clean_buf();
				//delete *it;
				//buf_list.erase(it);
			}
		}
		else if (buf_list[n]->body_parse_chunk || buf_list[n]->body_parse_length)
		{
			buf_list[n]->body += buf_list[n]->m_buffer;
			ft_memset((void *)buf_list[n]->m_buffer, 0, BUFFER_SIZE + 1);
			if (buf_list[n]->body_parse_chunk && strstr(buf_list[n]->body.c_str(), ENDCHARS_BOD) != NULL)
			{
				LaunchRequest(n, fd);
				buf_list[n]->clean_buf();
				//delete *it;
				//buf_list.erase(it);
			}
			else if (buf_list[n]->body_parse_length)
			{
				buf_list[n]->track_length += ret;
				if (buf_list[n]->track_length >= buf_list[n]->m_content_length)
				{
					LaunchRequest(n, fd);
					buf_list[n]->clean_buf();
					//delete *it;
					//buf_list.erase(it);
				}
			}
		}
	}
}

void Listener::LaunchRequest(int n, int fd)
{
	//choose config according to server name
	std::string host = getHost(buf_list[n]->headers, HOST_STR);
	size_t m = host.find(":");
	host = host.substr(0, m);
	//std::cout << "IN" << std::endl;
	for (int j = 0; j < _size ; j++)
	{
		if (strstr(host.c_str(), _conf[j]._server_name.c_str()) != NULL)
		{
			/*std::cout << _conf[j]._server_name << std::endl;
			std::cout << j << std::endl;
			std::cout << _conf.size() << std::endl;*/
			m_nbConf = j;
			break;
		}
	}

	req_list.push_back(new Request(buf_list[n]->headers, buf_list[n]->body, fd, _conf[m_nbConf], *m_port, m_address->sin_addr.s_addr));
	std::vector<Request*>::iterator it = req_list.begin();
	std::vector<Request*>::iterator ite = req_list.end();
	while (it != ite && (*it)->m_client != fd)
		it++;
	(*it)->parse();
	(*it)->handle();

}

void Listener::send_data(std::vector<Request*>::iterator it)
{
	/*std::vector<Request*>::iterator it = req_list.begin();
	std::vector<Request*>::iterator ite = req_list.end();
	while (it != ite && (*it)->m_client != fd)
		it++;*/
	
	//error checking to comply with correction : if error, client will be removed
	if ((*it)->send_to_client() == -1)
	{
		m_close = true; //client removed
		delete *it;
		req_list.erase(it);
		return;
	}
	if (!(*it)->bytes_left)
	{
		/*if ((*it)-> is_cgi)
			std::cout << "DONE" << std::endl;*/
		delete *it;
		req_list.erase(it);
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
	
	//std::vector<Buffers*>::iterator it = buf_list.begin();
	//std::vector<Buffers*>::iterator ite = buf_list.end();
	
	/*while (it != ite && (*it)->m_id != fd)
		it++;
	it--;*/

	if (m_close) {
		close(fd);
		FD_CLR(fd, &R_SET);
		FD_CLR(fd, &W_SET);
		if (fd == highsock) {
			while (!(FD_ISSET(highsock, &R_SET)) && !(FD_ISSET(highsock, &W_SET))) 
				highsock -= 1;
		//delete *it;
		//buf_list.erase(it);
		}
	}
}

