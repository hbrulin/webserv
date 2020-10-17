#include "listener.hpp"


int Listener::checkpast(int i)
{
	for (int j = 0; j < i ; j++) {
		if (m_address[j].sin_addr.s_addr == m_address[i].sin_addr.s_addr && m_port[j] == m_port[i] && i != j)
			return(1);		
	}
	return(0);
}

std::string Listener::getHost(std::string buffer, std::string toParse)
{
    int n;
    std::string referer;
	n = buffer.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (buffer[i] != '\n' && buffer[i] != '\r') { i++;}
		referer = buffer.substr(n, i - n);
        return referer;
	}
    return "";
}

std::pair<int, int>	Listener::look_for_sock(int j)
{
	//std::cout << "J is " << j << std::endl;
	for (int i = 0; i < _size ; i++) {
		//std::cout << "sock is" <<  m_sock[i] << std::endl;
		if (j == m_sock[i])
			return std::pair<int, int>(j, i);
	}
	return std::pair<int, int>(0, 0);
}

int Listener::getLength(std::string body, std::string toParse)
{
    int n;
    std::string referer;
	n = body.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (body[i] != '\n' && body[i] != '\r') { i++;}
		referer = body.substr(n, i - n);
		//std::cout << referer << std::endl;
        return strtol(referer.c_str(), NULL, 10);
	}
    return 0;
}

void Buffers::clean_buf() {
	m_id = 0;
	track_length = 0;
	body_parse_chunk = 0;
	body_parse_length = 0;
	header_length = 0;
	memset((void *)m_buffer, 0, BUFFER_SIZE + 1);
	headers = "";
	body = "";
}