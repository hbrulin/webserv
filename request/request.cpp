#include "request.hpp"
#include <iostream>
#include <unistd.h>

Request::Request(std::string headers, std::string body, int fd, Config conf, int port, unsigned long addr)
	{
		_conf = conf;
		m_headers = headers;
		m_body = body;
		m_client = fd;
		m_errorCode = DEFAULT_CODE;
		_head_req.SERVER_PORT = std::to_string(port);
		is_cgi = false;
		s_addr = addr;
		pid_ret = 0;
		_body_size = 0;
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
			_body_size += ft_atoi_base(buf, "0123456789abcdef");
			//_body_size += strtol(buf.c_str(), NULL, 16);
		else
			total += buf.substr(0, buf.size() - 1);
		flag = !flag;
	}
	m_body = total;
}

int Request::isGoodRequest()
{
	//std::cout << m_headers << std::endl;
	std::string buf;
	int line = 0;
	if (m_headers.empty())
		return 1;

	std::istringstream iss(m_headers.c_str());

	while (std::getline(iss, buf))
	{
		std::istringstream s(buf.c_str());
		std::vector<std::string> parsed((std::istream_iterator<std::string>(s)), std::istream_iterator<std::string>());
		if (line == 0)
		{
			if (parsed.size() != 3)
				return 1;
			if (parsed[0] != GET && parsed[0] != POST && parsed[0] != HEAD && parsed[0] != PUT && parsed[0] != DELETE)
				return 1;
			if (strstr(parsed[1].c_str(), "/") == NULL || forbiddenChars(parsed[1]))
				return 1;
			line++;
			m_url = parsed[1];
			_head_req.REQUEST_METHOD = parsed[0];
			_head_req.REQUEST_URI = parsed[1];
			_head_req.SERVER_PROTOCOL = parsed[2];
		}
		else
		{
			if (parsed.size() < 2)
				return 1;
		}
	}
	return 0;
}

void Request::parse()
{
	if (!isGoodRequest())
	{
		_head_req.parse(m_headers.c_str(), m_url);
		_loc = _conf._locations.get_loc_by_url(m_url);
		m_index = _loc._index;
		/*if (!_loc._errors.empty())
			m_not_found = _loc._errors[404];*/
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

		if (!_loc._uploaded_files_root.empty() && strstr(m_url.c_str(), _loc._uploaded_files_root.c_str()) != NULL)
		{
			m_path = m_url;
			return;
		}

		if (strstr(CONTENT_NEGO_AVAILABLE, _loc._root.c_str()) != NULL)
		{
			_loc._root = _head_req.contentNego(_loc._root);
		}
		m_path = _loc._root + m_url;

		/*parse body*/
		if (_head_req.REQUEST_METHOD == PUT || _head_req.REQUEST_METHOD == POST)
		{
			if (_head_req.TRANSFER_ENCODING == CHUNKED_STR)
				getBody();
			else if (_head_req.CONTENT_LENGTH.empty() == 0)
				_body_size = (unsigned int)stoi(_head_req.CONTENT_LENGTH);
			else
			{
				m_errorCode = 411;
				return;
			}
		}
	}
	else
	{
		_loc = _conf._locations._blank;
		//_loc.print();
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
	if ((bytes = send(m_client, m_output.c_str(), m_output.size(), 0)) < 0)
		return - 1;
	else if (bytes == 0)
		return 0;
	if (bytes < m_output.size())
		m_output = m_output.substr(bytes);
	else if (bytes == m_output.size())
		bytes_left = !bytes_left;
	
	if (!is_cgi)
	{
		std::cout << std::endl << m_output.substr(0, 500) << std::endl;
		std::cout << "- - - - - - - - - - " << std::endl;
	}
	return 0;
}

Request::~Request(){}

//Request &Request::operator=(const Request &copy)
//{
	//this->m_body = copy.m_body;
//	this->is_cgi = copy.is_cgi;
//	return *this;
//}

Request::Request(const Request &copy)
{
	this->is_cgi = copy.is_cgi;
	//this->m_body = copy.m_body;
}
