#include "request.hpp"
#include <iostream>
#include <unistd.h>


void Request::split_resp(char *buffer)
{
	std::string s(buffer);
	int i = 0;
	//SCRIPT_NAME
	int n = s.find("'\n'");
	if (n != (int)std::string::npos)
	{
		m_header.append(buffer, n);
		n = n + 3;
		i = n;
		m_content.append(&buffer[n], strlen(buffer) - n);
	}
	else
	{
		m_content.append(buffer, strlen(buffer));
	}
}



void Request::parse() {

	/*parse word by word*/
	std::istringstream iss(m_buffer);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	// If the GET request is valid, try and get the name
	if (!(parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "TRACE" || parsed[0] == "PATCH" || parsed[0] == "DELETE" || parsed[0] == "OPTION" || parsed[0] == "CONNECT" || parsed[0] == "PUT"))
	{
		m_errorCode = 400;
	}
	if (parsed.size() >= 3 && (parsed[0] == "GET" || parsed[0] == "POST" || parsed[0] == "HEAD" || parsed[0] == "PUT"))
	{
		m_content = parsed[1];
		_head_req.REQUEST_METHOD = parsed[0];
		_head_req.SERVER_PROTOCOL = parsed[2];
		char **tab = ft_split(_head_req.getStringtoParse(m_buffer, "Authorization: ").c_str(), ' ');
		if (tab != NULL && tab[0] != NULL)
				_head_req.AUTH_TYPE = tab[0];
		_head_req.CONTENT_TYPE = _head_req.getStringtoParse(m_buffer, "Content-Type: ");
		_head_req.QUERY_STRING = _head_req.getMetatoParse((char *)m_content.c_str(), "?", (char *)" #");
		_head_req.getScriptName((char *)m_content.c_str());
		_head_req.SERVER_NAME = _head_req.getMetatoParse((char *)m_content.c_str(), "://", ":/?#");
		if (_head_req.getMetatoParse((char*)m_content.c_str(), _head_req.SERVER_NAME + ":", "?/#") != "")
			_head_req.SERVER_PORT = _head_req.getMetatoParse((char*)m_content.c_str(), _head_req.SERVER_NAME + ":", "?/#") != "";
		//_head_req.SERVER_PROTOCOL = _head_req.getMetatoParse(m_content, "", "://");
		if (m_content == "/") //GET / HTTP/1.1
		{
			m_content = m_index;
		}
		//std::cout << m_content << std::endl;
	}
	// HEADERS
	_head_req.REFERER = _head_req.getReferer(m_buffer);
	_head_req.USER_AGENT = _head_req.getUserAgent(m_buffer);
	//parsing languages into vector
	std::string lg = _head_req.getStringtoParse(m_buffer, "Accept-Language: ");
	if (lg != "")
	{
		std::stringstream s(lg);
		std::string segment;
		while(std::getline(s, segment, ','))
		{
			_head_req.ACCEPT_LANGUAGE.push_back(segment);
		}
	}
	//rest of parsing
	if (_head_req.getStringtoParse(m_buffer, "Accept-Charset: ") != "")
		_head_req.ACCEPT_CHARSET = ft_split(_head_req.getStringtoParse(m_buffer, "Accept-Charset: ").c_str(), ',');
	if (_head_req.getStringtoParse(m_buffer, "Transfer-Encoding: ") != "")
		_head_req.TRANSFER_ENCODING = ft_split(_head_req.getStringtoParse(m_buffer, "Transfer-Encoding: ").c_str(), ',');
	_head_req.DATE = _head_req.getStringtoParse(m_buffer, "Date: ");
}

void Request::handle() {

	if (strstr(m_buffer, "POST") != NULL && m_content.find(".php") != std::string::npos) // .cgi != NULL
	{
		post();
		return ;
	}
	else if (strstr(m_buffer, "PUT") != NULL) {
		put();
		return;
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
		oss << m_content;
	m_output = oss.str();

	if (send(m_client, m_output.c_str(), m_output.size() + 1, 0) <= 0)
		return - 1;
	return 0;
}
