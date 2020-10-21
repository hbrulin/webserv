#include "request.hpp"

int Request::preChecks()
{
	if (_head_req.SERVER_PROTOCOL != DEF_PROTOCOL)
	{
		m_errorCode = 505;
		//m_path = _loc._root + ERROR_FOLDER + NOT_SUPPORTED;
		m_path = _loc._errors[m_errorCode];
		std::ifstream f(m_path);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_url = str;
		f.close();
		return 1;
	}
	if (!_loc.check_allowed_method(_head_req.REQUEST_METHOD, _head_req.REQUEST_URI))
	{	
		m_errorCode = 405;
		m_path = _loc._errors[m_errorCode];
		//std::cout << m_path;
		//m_path = _loc._root + ERROR_FOLDER + NOT_ALLOWED;
		std::ifstream f(m_path);
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		m_url = str;
		f.close();
		return 1;
	}
	return 0;
}

void Request::notFound() {
	//m_path = _loc._root + ERROR_FOLDER + m_not_found;
	m_path = _loc._errors[404];
	std::ifstream f(m_path);
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	m_url = str;
	m_errorCode = 404;
	f.close();
}

void Request::badRequest() {
	m_path = _loc._errors[400];
	std::ifstream f(m_path);
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	m_url = str;
	m_errorCode = 400;
	f.close();
}

int Request::internalError() {
	std::ostringstream oss;
	int ret;
	oss << "HTTP/1.1 " << 500;
	oss << " Internal Server Error\r\n";
	oss << "Content-Type: text/html" << "\r\n";
	oss << "Content-Length: 97\r\n\r\n";
	oss << "<!doctype html><html><head><title>CGI Error</title></head><body><h1>CGI Error.</h1></body></html>\r\n";
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
