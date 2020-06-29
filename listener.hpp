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
		void build_fd_set();
		void accept_incoming_connections();

	private:
		struct sockaddr_in m_address; // /* bind info structure */ need to have IP defined? see with config
		int			m_port;
		int			m_sock; /* The socket file descriptor for our "listening" socket */
		fd_set		m_set; /* Socket file descriptors we want to wake up for, using select() */
		fd_set		m_working_set;
		bool		m_run;
		int			m_highsock;
		//struct timeval	m_timeout; Is there a need for timeout or should it never end? arg for select()
		bool		m_close;
	/* 1 listening socket + other socket fds */
};