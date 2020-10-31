#include "request.hpp"

int Request::preChecks()
{
	if (_head_req.SERVER_PROTOCOL != DEF_PROTOCOL)
	{
		m_errorCode = 505;
		m_path = _loc._errors[m_errorCode];
		if (read_file() == -1)
			internalError();
		return 1;
	}
	if (!_loc.check_allowed_method(_head_req.REQUEST_METHOD, _head_req.REQUEST_URI))
	{
		m_errorCode = 405;
		m_path = _loc._errors[m_errorCode];
		if (read_file() == -1)
			internalError();
		return 1;
	}
	if (!isAuthorized(m_headers))
	{
		m_errorCode = 401;
		m_path = _loc._errors[m_errorCode];
		if (read_file() == -1)
			internalError();
		return 1;
	}
	return 0;
}

void Request::notFound() {
	m_path = _loc._errors[404];
	m_errorCode = 404;
	if (read_file() == -1)
		internalError();
}

void Request::badRequest() {
	m_path = _loc._errors[400];
	m_errorCode = 400;
	if (read_file() == -1)
		internalError();
}

int Request::internalError() {
	m_url = "HTTP/1.1 500 Internal Server Error\r\n";
	m_url += "Content-Type: text/html\r\n";
	m_url += "Content-Length: 97\r\n\r\n";
	m_url += "<!doctype html><html><head><title>Internal Error</title></head><body><h1>Internal Error.</h1></body></html>\r\n";
	return 0;
}

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

void Request::free_authorization()
{
	if (_head_req.AUTHORIZATION != NULL)
	{
		int i = 0;
		while (_head_req.AUTHORIZATION[i])
		{
			free(_head_req.AUTHORIZATION[i]);
			_head_req.AUTHORIZATION[i] = NULL;
			i++;
		}
		free(_head_req.AUTHORIZATION);
	}
	if (_head_resp.WWW_AUTHENTICATE != NULL)
	{
		int i = 0;
		while (_head_resp.WWW_AUTHENTICATE[i])
		{
			free(_head_resp.WWW_AUTHENTICATE[i]);
			_head_resp.WWW_AUTHENTICATE[i] =NULL;
			i++;
		}
		free(_head_resp.WWW_AUTHENTICATE);
	}
	_head_resp.WWW_AUTHENTICATE = NULL;
	_head_req.AUTHORIZATION = NULL;
}

int Request::isAuthorized(std::string str)
{
	_head_req.AUTHORIZATION = NULL;
	_head_resp.WWW_AUTHENTICATE = ft_split(_head_req.getStringtoParse((char *)str.c_str(), "WWW-Authenticate: ").c_str(), ' ');
	if (_head_resp.WWW_AUTHENTICATE != NULL && _head_resp.WWW_AUTHENTICATE[0] != NULL)
	{
		if (_head_req.getStringtoParse(m_headers, "Authorization: ") != "")
			_head_req.AUTHORIZATION = ft_split(_head_req.getStringtoParse(m_headers, "Authorization: ").c_str(), ' ');
		else
		{
			_head_req.AUTHORIZATION = NULL;
		}
		if (_head_req.AUTHORIZATION == NULL)
		{
			m_errorCode = 401;
			free_authorization();
			return 0;
		}
		if (strcmp((const char *)_head_req.AUTHORIZATION[0], (const char *)_head_resp.WWW_AUTHENTICATE[0]))
		{
				m_errorCode = 401;
				free_authorization();
				return 0;
		}
		else if (_head_req.AUTHORIZATION != NULL && _head_req.AUTHORIZATION[1] != NULL)
		{
			if (strncmp(_head_req.AUTHORIZATION[1],"dXNlcjpwYXNzd29yZA==", 11))
			{
				free_authorization();
				return 0;
			}
		}
	}
	free_authorization();
	return 1;
}