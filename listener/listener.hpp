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
#include "../request/request.hpp"
#include "../parser/Config.hpp"
#include <vector>
#include "../headers/returnCodes.hpp"

#define BUFFER_SIZE 100000000

class Buffers {
	
	public:
		std::string headers;
		std::string body;
		char *m_buffer;
		int		m_id;
		long int			m_content_length;
		long int			track_length;
		int					track_recv;
		bool				body_parse_chunk;
		bool				body_parse_length;
		unsigned int 		header_length;

		Buffers(int id);
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
		int getLength(std::string body, std::string toParse);
		//void clean();
	
	protected:
		void set_non_blocking(int sock);
		void build_fd_set();
		void accept_incoming_connections(int i);
		void receive_data(int fd);
		void close_conn(int fd);
		std::pair<int, int>	look_for_sock(int j);
		std::string getHost(std::string buffer, std::string toParse);
		int checkpast(int i);
		int reparse_body(int n, int fd);

	private:
		/*META VAR*/
		std::vector<Config> _conf;
		std::string SERVER_PROTOCOL;
		std::string SERVER_SOFTWARE;
		std::string GATEWAY_INTERFACE;;

		/*Other var*/
		int _size;
		struct sockaddr_in *m_address; // /* bind info structure */ need to have IP defined? see with config
		int			*m_port;
		int			*m_sock; /* The socket file descriptor for our "listening" socket */
		fd_set		m_set; /* Socket file descriptors we want to wake up for, using select() */
		fd_set		m_working_set;
		bool		m_run;
		int			m_highsock;
		//struct timeval	m_timeout; Is there a need for timeout or should it never end? arg for select()
		bool		m_close;
		int			m_nbConf;
		std::vector<Buffers*> buf_list;
	
		Listener() {};

		//Mettre ici un objet request??
};

#endif
