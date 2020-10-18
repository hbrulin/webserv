#ifndef LISTENER_HPP
# define LISTENER_HPP

#include <iostream>
//#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
//#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "../request/request.hpp"
#include "../parser/Config.hpp"
#include <vector>
#include "../utils/definitions.hpp"
#include <signal.h>

class Buffers {
	
	public:
		std::string headers;
		std::string body;
		char *m_buffer;
		int		m_id;
		long int			m_content_length;
		long int			track_length;
		bool				body_parse_chunk;
		bool				body_parse_length;
		unsigned int 		header_length;

		Buffers(int id);
		void clean_buf();
		virtual ~Buffers() {
			free(m_buffer);
		}
};

class Listener {

	public:
		Listener(std::vector<Config> conf, int size);
		int init();
		int run();
		void LaunchRequest(int n, int fd);
		int getLength(const std::string body, const std::string toParse);
		static void exiting(int n);
		void clean();
	
	protected:
		void set_non_blocking(int sock);
		void build_fd_set();
		void accept_incoming_connections(int i);
		void receive_data(int fd);
		void close_conn(int fd);
		std::pair<int, int>	look_for_sock(int j);
		std::string getHost(const std::string buffer, const std::string toParse);
		int checkpast(int i);
		int reparse_body(int n, int fd);

	private:
		std::vector<Config> _conf;

		int _size;
		struct sockaddr_in *m_address; // /* bind info structure */ need to have IP defined? see with config
		int			*m_port;
		int			*m_sock; /* The socket file descriptor for our "listening" socket */
		fd_set		m_set; /* Socket file descriptors we want to wake up for, using select() */
		fd_set		m_working_set;
		int			m_highsock;
		//struct timeval	m_timeout; Is there a need for timeout or should it never end? arg for select()
		bool		m_close;
		int			m_nbConf;
		std::vector<Buffers*> buf_list;
	
		Listener() {};

};

#endif
