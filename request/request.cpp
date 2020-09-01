#include "request.hpp"
#include <iostream>
#include <unistd.h>

Request::Request(char *buffer, int fd, Config conf, int port)
	{
		_conf = conf;
		memset((char *) &m_buffer, 0, sizeof(m_buffer));
		m_buffer = buffer;
		m_client = fd;
		m_not_found = "404.html";
		m_not_allowed = "405.html";
		m_not_acceptable = "406.html";
		m_bad_request = "400.html";
		m_unauthorized = "401.html";
		m_not_supported = "505.html";
		m_index = "index.html";
		m_errorCode = 404; //define other error codes
		_head_req.SERVER_PORT = std::to_string(port);

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

	/*parse word by word*/
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	//std::cout << m_buffer << std::endl;
	if (parsed.size() >= 3 && (parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "PUT" || parsed[0] == "DELETE"))
	{
		m_url = parsed[1];
		_head_req.parse(parsed, m_buffer, m_url);
		_loc = _conf._locations.get_loc_by_url(m_url);
		//_loc.print();
		if (m_url == "/") //GET / HTTP/1.1
			m_url = m_index;
		getBody(m_buffer);
		//std::cout << _head_req.BODY << std::endl;
	}
	else
	{
		m_errorCode = 400;
		return;
	}
	if (check_if_method_is_allowed(parsed[0]))
	{
		m_errorCode = 405; // error for method not allowed
		return;
	}

}

void Request::handle() {
	if (strstr(m_buffer, "POST") != NULL && m_url.find(".php") != std::string::npos) // .cgi != NULL
	{
		post();
		return ;
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
	oss << _head_resp.getBuffer(m_errorCode, m_path.c_str(), _conf._methods);
	if (_head_req.REQUEST_METHOD != "HEAD" && _head_req.REQUEST_METHOD != "PUT")
		oss << m_url;
	m_output = oss.str();

	if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
		return - 1;
	return 0;
}

bool Request::check_if_method_is_allowed(std::string method)
{
	for (std::vector<std::string>::size_type i = 0; i < _conf._methods.size(); i++)
	{
		//std::cout << _conf._methods[i] << std::endl;
		if (_conf._methods[i] == method)
			return (true);
	}
	return (false);
}
