#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

class Listener {

	public:
		Listener();
		int init();
		int run();
	
	private:
		const char *m_ip;
		int			m_port;
		int			m_sock; /* The socket file descriptor for our "listening" socket */
		int			*m_socket; /* Array of connected sockets so we know who we are talking to */ // A VOIR
		fd_set		set; /* Socket file descriptors we want to wake up for, using select() */

	/* 1 listening socket + other socket fds */
};