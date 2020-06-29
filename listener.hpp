#include <iostream>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

class Listener {

	public:
		Listener();
		int init();
		int run();
	
	protected:
		void set_non_blocking();

	private:
		struct sockaddr_in m_address; // /* bind info structure */ need to have IP defined? see with config
		int			m_port;
		int			m_sock; /* The socket file descriptor for our "listening" socket */
		int			*m_connect_list; /* Array of connected sockets so we know who we are talking to */ // A VOIR
		fd_set		set; /* Socket file descriptors we want to wake up for, using select() */
		int			m_highsock;
		bool		m_run;

	/* 1 listening socket + other socket fds */
};