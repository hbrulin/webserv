#include "request.hpp"
#include <iostream>
#include <unistd.h>

Request::Request(char *buffer, int fd, Config conf, int port, unsigned long addr)
	{
		_conf = conf;
		memset((char *) &m_buffer, 0, sizeof(m_buffer));
		m_buffer = buffer;
		m_client = fd;
		m_not_found = "404.html";
		m_not_allowed = "405.html"; // tester avant
		m_not_acceptable = "406.html";
		m_bad_request = "400.html";
		m_unauthorized = "401.html";
		m_not_supported = "505.html"; //tester avant 
		m_length_required = "411.html";
		m_errorCode = 200; //define other error codes
		_head_req.SERVER_PORT = std::to_string(port);
		is_cgi = false;
		s_addr = addr;
		pid_ret = 0;
		//std::cout << "adresse IP" << s_addr << std::endl;

	};

void		Request::getBody(char *m_buffer) {
	int n;
	int i = 0;
	std::string s(m_buffer);
	n = s.find("\n\n");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("\n\n").size();
		i = n;
		while (m_buffer[i] != '\0') { i++;}
		m_body = s.substr(n, i - n);
	}
	//std::cout << n << ' ' << i << std::endl;
}

void Request::parse() {

	//std::cout << m_buffer << std::endl;
	std::string s(m_buffer);
	std::cout << s << std::endl;
	//std::cout << "STOP" << std::endl;
	if (s =="\n\r\n")
	{
		m_errorCode = 411; 
		return;
	}
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "PUT" || parsed[0] == "DELETE")
	{
		m_url = parsed[1];
		_head_req.parse(parsed, m_buffer, m_url);
		_loc = _conf._locations.get_loc_by_url(m_url);
		if (_head_req.SERVER_PROTOCOL != "HTTP/1.1")
		{
			m_errorCode = 505;
			std::ifstream f(_loc._root + m_not_supported);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_not_supported;
			m_url = str;
			f.close();
			return;
		}
		if (!_loc.check_allowed_method(parsed[0])) // la fonction ne fonctionne pas
		{
			m_errorCode = 405; // error for method not allowed
			std::ifstream f(_loc._root + m_not_allowed);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_not_allowed;
			m_url = str;
			f.close();
			return;
		}
		//std::cout << "//////////////////////\n";
		m_index = _loc._index;
		//_loc.print();
		//std::cout << "//////////////////////\n";
		if (m_url.back() == '/') //GET / HTTP/1.1
		{
			m_url = m_index;
		}
		getBody(m_buffer);
		//std::cout << _head_req.BODY << std::endl;
	}
	else
	{
		m_errorCode = 400;
		return;
	}

}

void Request::handle() {
	if (m_errorCode > 400)
		return;
	//changing of root so that it includes the language
	_head_req.REQUEST_URI = m_url;
	if (strstr(m_url.c_str(), _loc._name.c_str()) != NULL)
	{
		m_url.replace(m_url.find(_loc._name.c_str()),_loc._name.size(), _loc._root);
		m_path = m_url;
	}
	else
	{
		_loc._root =  _head_req.contentNego(_loc._root);
		m_path = _loc._root + m_url;
	}
	content_env = _head_req.getStringtoParse((char *)m_url.c_str(), "?"); // on recup le query string s'il existe
	_head_req.QUERY_STRING = content_env;
	if (m_url.find("?") != std::string::npos)
		m_url.replace(m_url.find("?"),m_url.size(), ""); //on retire le query string de l'url
	if ((strstr(m_buffer, "POST") != NULL || strstr(m_buffer, "GET") != NULL) && m_url.find(_loc._cgi_type) != std::string::npos) // .cgi != NULL A REMPLACER par celui de la config
	{
		is_cgi = true;
		exec_cgi();
		return ;
	}
	else if (strstr(m_buffer, "POST") != NULL)
	{
		post();
		return;
	}
	else if (strstr(m_buffer, "PUT") != NULL) {
		put();
		return;
	}
	else if (strstr(m_buffer, "DELETE") != NULL)
	{
		delete_m();
		return ;
	}
	else
	{	//also works for HEAD, change is in sendToClient()                                                                                                                                                                                                                                                                                                                               
		get();
	}

}


int Request::send_to_client() {
	std::ostringstream oss;
	if (!is_cgi)
		oss << _head_resp.getBuffer(m_errorCode, m_path.c_str(), _loc._methods);
	if (_head_req.REQUEST_METHOD != "HEAD" && _head_req.REQUEST_METHOD != "PUT" && !is_cgi)
		oss << m_url;
	//std::cout << "m_output" << m_output << std::endl;
	if (!is_cgi)
		m_output = oss.str();
	//std::cout << "m_output" << m_output << std::endl;
	if (pid_ret > 0)
	{
		std::cout << "error 500" << std::endl;
		oss << "HTTP/1.1 " << 500;
		oss << " Internal Server Error\r\n";
		oss << "Content-Type: text/html" << "\r\n";
		oss << "Content-Length: 97\r\n\r\n";
		oss << "<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n";
		m_output = oss.str();
		if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
			return -1;
		return 0;
	}
	//std::cout << "ici" << std::endl;
	if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
		return - 1;
	return 0;
}

bool Request::check_if_method_is_allowed(std::string method)
{
	for (std::vector<std::string>::size_type i = 0; i < _loc._methods.size(); i++)
	{
		//std::cout << _conf._methods[i] << std::endl;
		if (_loc._methods[i] == method)
			return (true);
	}
	return (false);
}
