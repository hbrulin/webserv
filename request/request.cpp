#include "request.hpp"
#include <iostream>
#include <unistd.h>

Request::Request(std::string headers, std::string body, int fd, Config conf, int port, unsigned long addr)
	{
		_conf = conf;
		//memset((char *) &m_buffer, 0, sizeof(m_buffer));
		m_headers = headers;
		m_body = body;
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
		m_chunk_size = 0;
		//std::cout << "adresse IP" << s_addr << std::endl;

	};


void		Request::getBody() {
	/*int n;
	int i = 0;
	std::string s(m_buffer);
	n = s.find("\r\n\r\n");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("\r\n\r\n").size();
		i = n;
		while (m_buffer[i] != '\0') { i++;}
		m_body = s.substr(n, i - n);
	}*/

	std::istringstream f(m_body);
	std::string buf;
	std::string total;
	bool flag = 0;
	while (std::getline(f, buf))
	{
		if (!flag)
		{
			//std::cout << buf << std::endl;
			m_chunk_size += strtol(buf.c_str(), NULL, 16);
		}
		else
			total += buf.substr(0, buf.size() - 1);
		flag = !flag;
	}
	m_body = total;
	//std::cout << "body_size request" << m_body.size() << std::endl;
	std::cout << "calcul chunk" << m_chunk_size << std::endl << std::endl;
}

void Request::parse() 
{
	std::istringstream iss(m_headers.c_str());
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "PUT" || parsed[0] == "DELETE")
	{
		m_url = parsed[1];
		_head_req.parse(parsed, m_headers.c_str(), m_url);
		if (_head_req.REQUEST_METHOD == "PUT")
			std::cout << m_headers << std::endl;
		//m_method = _head_req.REQUEST_METHOD;
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
		if (!_loc.check_allowed_method(parsed[0], _head_req.REQUEST_URI))
		{
			m_errorCode = 405; // error for method not allowed
			std::ifstream f(_loc._root + m_not_allowed);
			std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
			m_path = _loc._root + m_not_allowed;
			m_url = str;
			f.close();
			return;
		}
		m_index = _loc._index;
		//_loc.print();
		if (m_url.find("?") != std::string::npos)
			m_url.replace(m_url.find("?"),m_url.size(), "");

		_loc._name.pop_back();
		if (m_url == "/" || _loc._name == m_url)
		{
			m_url = m_index;
		}
		else if (strstr(m_url.c_str(), _loc._name.c_str()) != NULL)
		{
			m_url.erase(0, _loc._name.size());
		}


		if (_loc._root != "YoupiBanane/")
			_loc._root =  _head_req.contentNego(_loc._root);
		m_path = _loc._root + m_url;
		if ((_head_req.REQUEST_METHOD == "PUT" || _head_req.REQUEST_METHOD == "POST") 
			&& _head_req.TRANSFER_ENCODING == "chunked")
			getBody();
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
	//_head_req.REQUEST_URI = m_url;
	content_env = _head_req.getStringtoParse(m_url, "?"); // on recup le query string s'il existe
	_head_req.QUERY_STRING = content_env;
	/*if (m_url.find("?") != std::string::npos)
		m_url.replace(m_url.find("?"),m_url.size(), "");*/ //on retire le query string de l'url
	/*if (strstr(m_url.c_str(), _loc._name.c_str()) != NULL) //|| !strncmp(m_url.c_str(), _loc._name.c_str(), _loc._name.size() -1)
	{
		m_url.replace(m_url.find(_loc._name.c_str()),_loc._name.size(), _loc._root); // changer 0 par m_url.find(_loc._name.c_str())
		m_path = m_url;
	}
	else
	{
		_loc._root =  _head_req.contentNego(_loc._root);
		m_path = _loc._root + m_url;
	}
	if (m_url.back() == '/' || !strcmp(m_url.c_str(), _loc._name.c_str())) //GET / HTTP/1.1
	{
		m_path = m_path + m_index;
	}*/
	if (_head_req.REQUEST_METHOD == "POST" && _loc._cgi_type != "" && _head_req.REQUEST_URI.find(_loc._cgi_type) != std::string::npos) // .cgi != NULL A REMPLACER par celui de la config
	{
		is_cgi = true;
		exec_cgi();
		return ;
	}
	else if (_head_req.REQUEST_METHOD == "POST")
	{
		is_cgi = false;
		post();
		return;
	}
	else if (_head_req.REQUEST_METHOD == "PUT") {
		put();
		return;
	}
	else if (_head_req.REQUEST_METHOD == "DELETE")
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
	/*if (_head_req.REQUEST_METHOD == "PUT")
		std::cout << m_errorCode << std::endl << std::endl;*/
	std::ostringstream oss;
	if (!is_cgi)
		oss << _head_resp.getBuffer(m_errorCode, m_path.c_str(), _loc._methods, _head_req.REQUEST_METHOD);
	if (_head_req.REQUEST_METHOD != "HEAD" && _head_req.REQUEST_METHOD != "PUT" && !is_cgi)
		oss << m_url;
	if (pid_ret > 0)
	{
		std::cout << "error 500" << std::endl;
		oss << "HTTP/1.1 " << 500;
		oss << " Internal Server Error\r\n";
		oss << "Content-Type: text/html" << "\r\n";
		oss << "Content-Length: 97\r\n\r\n";
		oss << "<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n";
		if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
			return -1;
		return 0;
	}
	if (is_cgi)
	{
		m_output = _head_resp.getBuffer_cgi(m_errorCode, m_body);
	}
	else
		m_output = oss.str();
	int bytes;
	if (!is_cgi)
	{
		if (_head_req.REQUEST_METHOD == "PUT")
			std::cout << std::endl << m_output << std::endl;
		if (send(m_client, m_output.c_str(), m_output.size(), 0) <= 0)
			return - 1;
	}
	else
	{
		if ((bytes = write(m_client, m_output.c_str(), m_output.size())) <= 0)
			return - 1;
		if ((bytes = write(m_client, m_body.c_str(), m_body.size())) <= 0)
			return - 1;
		while (bytes < (int)m_body.size())
		{
			m_body.substr(bytes, m_body.size() - bytes);
			bytes += write(m_client, m_body.c_str(), m_body.size());
		}
	}
	m_output = "";
	m_body.clear();
	m_chunk_size = 0;
	bytes = 0;
	std::cout << "- - - - - - - - - - " << std::endl;
	return 0;
}

bool Request::check_if_method_is_allowed(std::string method)
{
	for (std::vector<std::string>::size_type i = 0; i < _loc._methods.size(); i++)
	{
		if (_loc._methods[i] == method)
			return (true);
	}
	return (false);
}
