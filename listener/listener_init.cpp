#include "listener.hpp"

void Listener::build_fd_set() {
	FD_ZERO(&m_r_set);
	for (int i = 0; i < _size ; i++) {
		FD_SET(m_sock[i], &m_r_set);
		m_highsock = m_sock[i];
	}
	FD_ZERO(&m_w_set); 
	for (int i = 0; i < _size ; i++) {
		FD_SET(m_sock[i], &m_w_set); 
		m_highsock = m_sock[i];
	}
}

void Listener::set_non_blocking(int sock) {
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0) {
		strerror(errno);
	}
}

int Listener::init() {
	signal(SIGINT, &Listener::exiting);

	/* Used so we can re-bind to our port while a previous connection is still
	in TIME_WAIT state : TIME_WAIT indicates that server has closed the connection. The connection is being kept around
	so that any delayed packets can be matched to the connection and handled appropriately.*/
	int reuse_addr = 1; 

	for (int i = 0; i < _size ; i++) {
		m_port[i] = _conf[i]._listen;
	}

	/* Obtain a file descriptor for listening socket */
	for (int i = 0; i < _size ; i++) {
		m_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
		if (m_sock[i] < 0) {
			strerror(errno);
			exit(EXIT_FAILURE);
		}
	}

	/* Allow socket descriptor to be reuseable */
	/* So that we can re-bind to sock without TIME_WAIT problems */
	for (int i = 0; i < _size ; i++) {
		setsockopt(m_sock[i], SOL_SOCKET, SO_REUSEADDR, &reuse_addr,
			sizeof(reuse_addr));
	}

	/*All of the sockets for the incoming connections will also be nonblocking since
   	they will inherit that state from the listening socket.  */
	for (int i = 0; i < _size ; i++) {
		set_non_blocking(m_sock[i]);
	}

	int swapped;
	for (int i = 0; i < _size ; i++) {
		m_address[i].sin_family = AF_INET;
		swapped = (m_port[i]>>8) | (m_port[i]<<8);
		m_address[i].sin_port = swapped;
		m_address[i].sin_addr.s_addr = inet_addr(_conf[i]._host.c_str());
	}
	for (int i = 0; i < _size ; i++) {
		if(!checkpast(i)){
			if (bind(m_sock[i], (struct sockaddr*) &m_address[i], sizeof(m_address[i])) < 0)
			{
				strerror(errno);
				close(m_sock[i]);
				exit(EXIT_FAILURE);
			}
		}
	}

	/* This socket will be for listening */
	for (int i = 0; i < _size ; i++) {
		listen(m_sock[i],SOMAXCONN);
	}

	build_fd_set();

	return 0;
}

Listener::Listener(std::vector<Config> conf, int size) {

	_size = size;
	_conf = conf;

	m_port = (int *)malloc(sizeof(int) * size + 1);
	m_sock = (int *)malloc(sizeof(int) * size + 1);
	m_address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in) * size + 1);
	m_highsock = 0;
	ft_memset((char *) &m_r_set, 0, sizeof(m_r_set));
	ft_memset((char *) &m_w_set, 0, sizeof(m_w_set));
	ft_memset((char *) &m_read_set, 0, sizeof(m_read_set));
	ft_memset((char *) &m_write_set, 0, sizeof(m_write_set));

	for (int i = 0; i < size; i++)
	{
		ft_memset((int *) &m_port[i], 0, sizeof(m_port[i]));
		ft_memset((int *) &m_sock[i], 0, sizeof(m_sock[i]));
		ft_memset((char *) &m_address[i], 0, sizeof(m_address[i]));
	}
}