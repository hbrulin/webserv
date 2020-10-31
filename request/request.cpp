#include "request.hpp"

Request::Request(std::string headers, std::string body, int fd, Config conf, int port, std::string ip)
{
		_conf = conf;
		m_headers = headers;
		m_body = body;
		m_client = fd;
		m_errorCode = DEFAULT_CODE;
		_head_req.REMOTE_ADDR = ip;
		_head_req.SERVER_PORT = std::to_string(port);
		is_cgi = false;
		pid_ret = 0;
		_body_size = 0;
		bytes_left = 1;
		first_send = 1;
}

Request &Request::operator=(const Request &copy) {

	_conf = copy._conf;
	m_headers = copy.m_headers;
	m_body = copy.m_body;
	m_client = copy.m_client;
	m_errorCode = copy.m_errorCode;
	_head_req.SERVER_PORT = copy._head_req.SERVER_PORT;
	is_cgi = copy.is_cgi;
	_head_req.REMOTE_ADDR = copy._head_req.REMOTE_ADDR;
	pid_ret = copy.pid_ret;
	_body_size = copy._body_size;
	bytes_left = copy.bytes_left;
	first_send = copy.first_send;
	return *this;
}

void Request::handle() {
	if (m_errorCode >= 400)
		return;
	/*query string*/
	content_env = _head_req.getStringtoParse(m_url, "?");
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
		get();
}


int Request::send_to_client() {
	if (first_send)
	{
		first_send = !first_send;
		if (pid_ret > 0)
			return internalError();
		if (!is_cgi)
		{
			m_output = _head_resp.getBuffer(m_errorCode, m_path.c_str(), _loc._methods, _head_req.REQUEST_METHOD);
			if (_head_req.REQUEST_METHOD != HEAD && _head_req.REQUEST_METHOD != PUT && !is_cgi)
				m_output = m_output + m_url;
		}
		else 
		{
			m_output = _head_resp.getBuffer_cgi(m_errorCode, m_body, _head_req.X_headers);
			m_output = m_output + m_body;
		}
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
	return 0;
}