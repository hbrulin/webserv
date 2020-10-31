#include "listener.hpp"

bool		m_run = true;

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
		FD_CLR(fd, &m_r_set);
		FD_CLR(fd, &m_w_set);
		if (fd == m_highsock) {
			while (!(FD_ISSET(m_highsock, &m_r_set)) && !(FD_ISSET(m_highsock, &m_w_set))) 
				m_highsock -= 1;
		map_ip.erase(fd);
		}
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

	while (it != ite && (*it)->m_id != fd)
		it++;
	int n = it - buf_list.begin();

	bytes = strlen(buf_list[n]->m_buffer);

	ret = read(fd, buf_list[n]->m_buffer + bytes, BUFFER_SIZE - bytes);

	if (ret < 0) {
		m_close = true;
		return;
	}
	/*Check if connection was closed by client*/
	else if (ret == 0)
		return;
	else
	{
		bytes += ret;
		buf_list[n]->m_buffer[bytes] = '\0';
		if (ft_strstr(buf_list[n]->m_buffer, ENDCHARS) != NULL && !buf_list[n]->body_parse_chunk && !buf_list[n]->body_parse_length)
		{
			std::string s(buf_list[n]->m_buffer);
			size_t npos = s.find(ENDCHARS);
			buf_list[n]->headers = s.substr(0, npos);
			buf_list[n]->body += s.substr(npos + 4, s.size());
			ft_memset((void *)buf_list[n]->m_buffer, 0, BUFFER_SIZE + 1);
			if (ft_strstr(buf_list[n]->headers.c_str(), POST) != NULL || ft_strstr(buf_list[n]->headers.c_str(), PUT) != NULL)
			{
				if (ft_strstr(buf_list[n]->headers.c_str(), CHUNKED_STR) != NULL)
				{
					buf_list[n]->body_parse_chunk = !buf_list[n]->body_parse_chunk;
					if (buf_list[n]->body.empty() == 0 && ft_strstr(buf_list[n]->body.c_str(), ENDCHARS_BOD) != NULL)
					{
						LaunchRequest(n, fd);
						buf_list[n]->clean_buf();
					}
				}
				else if (ft_strstr(buf_list[n]->headers.c_str(), CONTENT_L_STR) != NULL)
				{
					buf_list[n]->body_parse_length = !buf_list[n]->body_parse_length;
					buf_list[n]->header_length = buf_list[n]->headers.size() + 5;
					buf_list[n]->m_content_length = getLength(buf_list[n]->headers, CONTENT_L_STR);
					buf_list[n]->track_length += ret - buf_list[n]->header_length;
					if (buf_list[n]->track_length >= buf_list[n]->m_content_length - 1)
					{
						LaunchRequest(n, fd);
						buf_list[n]->clean_buf();
					}
				}
				else
				{
					LaunchRequest(n, fd);
					buf_list[n]->clean_buf();
				}
			}
			else
			{
				LaunchRequest(n, fd);
				buf_list[n]->clean_buf();
			}
		}
		else if (buf_list[n]->body_parse_chunk || buf_list[n]->body_parse_length)
		{
			buf_list[n]->body += buf_list[n]->m_buffer;
			ft_memset((void *)buf_list[n]->m_buffer, 0, BUFFER_SIZE + 1);
			if (buf_list[n]->body_parse_chunk && ft_strstr(buf_list[n]->body.c_str(), ENDCHARS_BOD) != NULL)
			{
				LaunchRequest(n, fd);
				buf_list[n]->clean_buf();
			}
			else if (buf_list[n]->body_parse_length)
			{
				buf_list[n]->track_length += ret;
				if (buf_list[n]->track_length >= buf_list[n]->m_content_length)
				{
					LaunchRequest(n, fd);
					buf_list[n]->clean_buf();
				}
			}
		}
	}
}

void Listener::LaunchRequest(int n, int fd)
{
	/*choose config according to server name*/
	std::string host = getHost(buf_list[n]->headers, HOST_STR);
	size_t m = host.find(":");
	host = host.substr(0, m);
	for (int j = 0; j < _size ; j++)
	{
		if (ft_strstr(host.c_str(), _conf[j]._server_name.c_str()) != NULL)
		{
			m_nbConf = j;
			break;
		}
	}

	/*get ip*/
	std::string ip;
	for(std::map<int, std::string>::iterator it = map_ip.begin(); it != map_ip.end(); ++it) {
		if (fd == it->first)
			ip = it->second;
	}

	req_list.push_back(new Request(buf_list[n]->headers, buf_list[n]->body, fd, _conf[m_nbConf], *m_port, ip));
	std::vector<Request*>::iterator it = req_list.begin();
	std::vector<Request*>::iterator ite = req_list.end();
	while (it != ite && (*it)->m_client != fd)
		it++;
	(*it)->parse();
	(*it)->handle();

}

void Listener::send_data(int fd)
{
	std::vector<Request*>::iterator it = req_list.begin();
	std::vector<Request*>::iterator ite = req_list.end();
	while (it != ite && (*it)->m_client != fd)
		it++;
	/*if send error, client will be removed*/
	if ((*it)->send_to_client() == -1)
	{
		m_close = true;
		return;
	}
	if (!(*it)->bytes_left)
	{
		delete *it;
		req_list.erase(it);
	}
}

void Listener::accept_incoming_connections(int i) {
	int	new_sock = 0;
	struct sockaddr_in	info;
	socklen_t			len = sizeof(struct sockaddr);

	memset(&info, 0, sizeof(struct sockaddr));

	while (new_sock != -1) {
		new_sock = accept(i, (struct sockaddr *)&info, &len);
		if (new_sock < 0) {
			if (errno != EWOULDBLOCK) {
				strerror(errno);
			}
			break;
		}
		/*Add new incoming connection to master fd_set*/
		FD_SET(new_sock, &m_r_set);
		FD_SET(new_sock, &m_w_set);
		if (new_sock > m_highsock)
			m_highsock = new_sock;
		map_ip.insert(std::pair<int, std::string>(new_sock, inet_ntoa(info.sin_addr)));
	}
}

int Listener::run() {
	int sock_count;
	std::pair<int, int> ret;

	while (m_run) {
		/* Copy the master fd_set over to the read fd_set.
		Important because the call to select() is destructive. The copy
		only contains the sockets that are accepting inbound connection requests OR messages.
		E.g. You have a server and it's master file descriptor set contains 5 items;
		the listening socket and four clients. When you pass this set into select(),
		only the sockets that are interacting with the server are returned. Let's say
		only one client is sending a message at that time. The contents of 'copy' will
		be one socket. You will have LOST all the other sockets.*/
		ft_memcpy(&m_read_set, &m_r_set, sizeof(m_r_set));
		ft_memcpy(&m_write_set, &m_w_set, sizeof(m_w_set));
		sock_count = select(m_highsock + 1, &m_read_set, &m_write_set, NULL, NULL);
		if (sock_count < 0) {
			strerror(errno);
		}
		for (int j = 0; j <= m_highsock && sock_count > 0; j++) {
			std::vector<Request*>::iterator it = req_list.begin();
			std::vector<Request*>::iterator ite = req_list.end();
			while (it != ite && (*it)->m_client != j)
				it++;
			if (FD_ISSET(j, &m_write_set) && it != ite)
			{
				send_data(j);
				close_conn(j);
			}
			else if (FD_ISSET(j, &m_read_set)) 
			{
				sock_count -= 1;
				/*Check to see if the FD is the listening socket (m_sock). If it is,
				accept all incoming connections that are queued up on the listening socket before we
				loop back and call select again.*/
				ret = look_for_sock(j);
				if (ret.first) {
					accept_incoming_connections(ret.first);
					m_nbConf = ret.second;
				}
				else { /*if it is not listening socket*/
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
					receive_data(j);
					close_conn(j);
				}
			}
		}
	}
	clean();
	return 0;
}

void Listener::exiting(int n) {
	(void)n;

	std::cout << "\n" << "exiting...\n";
	m_run = false;

}