#include "request.hpp"
#include <iostream>
#include <unistd.h>

Request::Request(std::string headers, std::string body, int fd, Config conf, int port, unsigned long addr)
	{
		_conf = conf;
		m_headers = headers;
		m_body = body;
		m_client = fd;
		m_not_found = DEF_ERR_PAGE;
		m_errorCode = DEFAULT_CODE;
		_head_req.SERVER_PORT = std::to_string(port);
		is_cgi = false;
		s_addr = addr;
		pid_ret = 0;
		m_chunk_size = 0;
		bytes_left = 1;
		first_send = 1;
	};


void		Request::getBody() {
	std::istringstream f(m_body);
	std::string buf;
	std::string total;
	bool flag = 0;
	while (std::getline(f, buf))
	{
		if (!flag)
			m_chunk_size += strtol(buf.c_str(), NULL, 16);
		else
			total += buf.substr(0, buf.size() - 1);
		flag = !flag;
	}
	m_body = total;
	//std::cout << "body_size request" << m_body.size() << std::endl;
	//std::cout << "calcul chunk" << m_chunk_size << std::endl << std::endl;
}

void Request::parse() 
{
	std::istringstream iss(m_headers.c_str());
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	if (parsed[0] == GET || parsed[0] == POST || parsed[0] == HEAD || parsed[0] == PUT || parsed[0] == DELETE)
	{
		m_url = parsed[1];
		_head_req.parse(parsed, m_headers.c_str(), m_url);
		_loc = _conf._locations.get_loc_by_url(m_url);
		m_index = _loc._index;
		if (!_loc._errors.empty())
			m_not_found = _loc._errors;
		//std::cout << "!!!" << _loc._errors << std::endl;

		if (preChecks())
			return;

		if (m_url.find("?") != std::string::npos)
			m_url.replace(m_url.find("?"),m_url.size(), "");

		/*build the right path*/
		_loc._name.pop_back();
		if (m_url == "/" || _loc._name == m_url)
			m_url = m_index;
		else if (strstr(m_url.c_str(), _loc._name.c_str()) != NULL)
			m_url.erase(0, _loc._name.size());
		if (_loc._root != YOUPIBANANE && strstr(m_url.c_str(), UPLOADED) == NULL)
			_loc._root =  _head_req.contentNego(_loc._root);
		m_path = _loc._root + m_url;

		if ((_head_req.REQUEST_METHOD == PUT || _head_req.REQUEST_METHOD == POST) 
			&& _head_req.TRANSFER_ENCODING == CHUNKED_STR)
			getBody();
	}
	else
	{
		badRequest();
		return;
	}
}

void Request::handle() {
	if (m_errorCode >= 400)
		return;
	content_env = _head_req.getStringtoParse(m_url, "?"); // on recup le query string s'il existe
	_head_req.QUERY_STRING = content_env;
	if (_head_req.REQUEST_METHOD == POST && _loc._cgi_type != "" && _head_req.REQUEST_URI.find(_loc._cgi_type) != std::string::npos) // .cgi != NULL A REMPLACER par celui de la config
	{
		is_cgi = true;
		exec_cgi();
		return ;
	}
	else if (_head_req.REQUEST_METHOD == POST)
	{
		is_cgi = false;
		post();
		return;
	}
	else if (_head_req.REQUEST_METHOD == PUT) {
		put();
		return;
	}
	else if (_head_req.REQUEST_METHOD == DELETE)
	{
		delete_m();
		return ;
	}
	else                                                                                                                                                                                                                                                                                                                          
		get(); //also works for HEAD, change is in sendToClient()     

}


int Request::send_to_client() {
	
	if (first_send)
	{
		first_send = !first_send;
		std::ostringstream oss;
		if (pid_ret > 0)
			return internalError();
		if (!is_cgi)
			oss << _head_resp.getBuffer(m_errorCode, m_path.c_str(), _loc._methods, _head_req.REQUEST_METHOD);
		if (_head_req.REQUEST_METHOD != HEAD && _head_req.REQUEST_METHOD != PUT && !is_cgi)
			oss << m_url;
		if (is_cgi)
		{
			m_output = _head_resp.getBuffer_cgi(m_errorCode, m_body, _head_req.X_headers);
			m_output = m_output + m_body;
		}
		else
			m_output = oss.str();
	}

	size_t bytes;
	if (!is_cgi)
	{
		if ((bytes = send(m_client, m_output.c_str(), m_output.size(), 0)) < 0)
			return - 1;
		if (bytes < m_output.size())
			m_output = m_output.substr(bytes);
		else if (bytes == m_output.size())
			bytes_left = !bytes_left;
	}
	else
	{
		std::cout << m_output.size() << std::endl;
		if ((bytes = send(m_client, m_output.c_str(), m_output.size(), 0)) < 0)
			return - 1;
		if (bytes < m_output.size())
		{
			std::cout << "IN" << std::endl;
			m_output = m_output.substr(bytes);
		}
		else if (bytes == m_output.size())
			bytes_left = !bytes_left;
		std::cout << bytes << std::endl;
		std::cout << bytes_left << std::endl << std::endl;
	}
	/*if (_head_req.REQUEST_METHOD == POST)
	{
		std::cout << std::endl << m_output << std::endl;
		std::cout << "- - - - - - - - - - " << std::endl;
	}*/
	return 0;
}