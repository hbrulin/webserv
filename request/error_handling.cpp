#include "request.hpp"

int Request::isAuthorized(std::string str)
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
}

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

int Request::isAllowed(std::string path)
{
    if ((_head_req.REQUEST_METHOD == "POST" || _head_req.REQUEST_METHOD == "DELETE") && path.find(_loc._root) != std::string::npos)
    {
        _head_resp.ALLOW = "GET";
        return 0;
    }
    if (_head_req.REQUEST_METHOD == "GET" && path.find(".php") != std::string::npos)
        return 0;
    return 1;
}
