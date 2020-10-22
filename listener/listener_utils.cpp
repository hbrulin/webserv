#include "listener.hpp"

extern fd_set		R_SET; 
extern fd_set		W_SET; 
extern int 			highsock;

int Listener::checkpast(int i)
{
	for (int j = 0; j < i ; j++) {
		if (m_address[j].sin_addr.s_addr == m_address[i].sin_addr.s_addr && m_port[j] == m_port[i] && i != j)
			return(1);		
	}
	return(0);
}

std::string Listener::getHost(const std::string buffer, const std::string toParse)
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
		//std::cout << referer << std::endl;
        return referer;
	}
    return "";
}

std::pair<int, int>	Listener::look_for_sock(int j)
{
	for (int i = 0; i < _size ; i++) {
		if (j == m_sock[i])
			return std::pair<int, int>(j, i);
	}
	return std::pair<int, int>(0, 0);
}

int Listener::getLength(const std::string body, const std::string toParse)
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
		return ft_atoi_base(referer, "0123456789");
        //return strtol(referer.c_str(), NULL, 10);
	}
    return 0;
}

void Listener::clean()
{
	for (int i=0; i <= highsock; ++i)
   	{
    	if (FD_ISSET(i, &R_SET))
        	close(i);
   }
	exit(SIGQUIT);
}