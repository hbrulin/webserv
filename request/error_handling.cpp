#include "request.hpp"

int Request::preChecks()
{
	if (_head_req.SERVER_PROTOCOL != DEF_PROTOCOL)
	{
		m_errorCode = 505;
		m_path = _loc._errors[m_errorCode];
		file_fd = open(m_path.c_str(), O_RDONLY);
		setFileToSet(true);
		_status = READ_FILE;
		return 1;
	}
	if (!_loc.check_allowed_method(_head_req.REQUEST_METHOD, _head_req.REQUEST_URI))
	{	
		m_errorCode = 405;
		m_path = _loc._errors[m_errorCode];
		file_fd = open(m_path.c_str(), O_RDONLY);
		setFileToSet(true);
		_status = READ_FILE;
		return 1;
	}
	return 0;
}

void Request::notFound() {
	m_path = _loc._errors[404];
	file_fd = open(m_path.c_str(), O_RDONLY);
	m_errorCode = 404;
	setFileToSet(true);
	_status = READ_FILE;
}

void Request::badRequest() {
	m_path = _loc._errors[400];
	file_fd = open(m_path.c_str(), O_RDONLY);
	m_errorCode = 400;
	setFileToSet(true);
	_status = READ_FILE;
}

int Request::internalError() {
	//std::ostringstream oss;
	int ret;
	m_output = "HTTP/1.1 500 Internal Server Error\r\n";
	m_output += "Content-Type: text/html\r\n";
	m_output += "Content-Length: 97\r\n\r\n";
	m_output += "<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n";
	if ((ret = send(m_client, m_output.c_str(), m_output.size() + 1, 0)) < 0)
		return -1;
	else if (ret == 0)
		return 0;
	return 0;
}

//int Request::isAllowed(std::string path)
//{
 //   if ((_head_req.REQUEST_METHOD == POST || _head_req.REQUEST_METHOD == DELETE) && path.find(_loc._root) != std::string::npos)
 //   {
   //     _head_resp.ALLOW = GET;
     //   return 0;
    // }
 //   if (_head_req.REQUEST_METHOD == GET && path.find(PHP) != std::string::npos)
  //      return 0;
    //return 1;
//}

int Request::forbiddenChars(std::string s) {
	int i = 0;
	while (s[i])
	{
		if (s[i] == '\\' || s[i] == '{' || s[i] == '}' || s[i] == '|'
			|| s[i] == '^' || s[i] == '~' || s[i] == '(' || s[i] == ')' || s[i] == '`'
			|| s[i] == '<' || s[i] == '>' || s[i] == '"' || s[i] == '#' || s[i] == '%')
			return 1;
		i++;
	}
	return 0;
}



/*int Request::isAuthorized(std::string str)
{
	_head_resp.WWW_AUTHENTICATE = ft_split(_head_req.getStringtoParse((char *)str.c_str(), "WWW-Authenticate: ").c_str(), ' ');
	if (_head_resp.WWW_AUTHENTICATE != NULL && _head_resp.WWW_AUTHENTICATE[0] != NULL)
	{
		if (_head_req.getStringtoParse(m_buffer, "Authorization: ") != "")
			_head_req.AUTHORIZATION = ft_split(_head_req.getStringtoParse(m_buffer, "Authorization: ").c_str(), ' ');
		else
		{
			_head_req.AUTHORIZATION = NULL;
		}
		if (_head_req.AUTHORIZATION == NULL)
		{
			m_errorCode = 401;
			return 0;
		}
		if (strcmp((const char *)_head_req.AUTHORIZATION[0], (const char *)_head_resp.WWW_AUTHENTICATE[0]))
		{
				m_errorCode = 401;
				return 0;
		}
		else if (_head_req.AUTHORIZATION != NULL && _head_req.AUTHORIZATION[1] != NULL)
		{
			if (strncmp(_head_req.AUTHORIZATION[1],"dXNlcjpwYXNzd29yZA==", 11))
			{
				return 0;
			}
		}
	}
	_head_resp.WWW_AUTHENTICATE = NULL;
	return 1;
}*/

/*int Request::isAcceptable()
{
	//Check Langage
	int i = 0;
	if (_head_req.ACCEPT_LANGUAGE == NULL) {}
	{ return 1;}
	while (_head_req.ACCEPT_LANGUAGE[i] != NULL)
	{
		if (strstr(_head_req.ACCEPT_LANGUAGE[i], _head_resp.CONTENT_LANGUAGE.c_str()) != NULL)
			return 1;
		i++;
	}
	return 0;
}*/
