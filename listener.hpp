#ifndef LISTENER_HPP
# define LISTENER_HPP

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
#include "request.hpp"
#include "Data.hpp"

class Listener {

	public:
		Listener(Data conf);
		int init();
		int run();
		void clean();
	
	protected:
		void set_non_blocking();
		void build_fd_set();
		void accept_incoming_connections();
		void receive_data(int fd);
		void close_conn(int fd);

	private:
		/*META VAR*/
		Data _conf;
		std::string SERVER_PROTOCOL;
		std::string SERVER_SOFTWARE;
		std::string GATEWAY_INTERFACE;;

		/*Other var*/
		struct sockaddr_in m_address; // /* bind info structure */ need to have IP defined? see with config
		int			m_port;
		int			m_sock; /* The socket file descriptor for our "listening" socket */
		fd_set		m_set; /* Socket file descriptors we want to wake up for, using select() */
		fd_set		m_working_set;
		bool		m_run;
		int			m_highsock;
		//struct timeval	m_timeout; Is there a need for timeout or should it never end? arg for select()
		bool		m_close;
	
		Listener() {};

		//Mettre ici un objet request??
};

#endif