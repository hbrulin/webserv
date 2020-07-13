#include "head_req.hpp"

std::string Head_req::get_meta(Config _Config)
{
	std::string str;
	str.append("&AUTH_TYPE="); //if nothing > ?
	str.append("lala");
	str.append("&CONTENT_LENGTH=");
	str.append(CONTENT_LENGTH);
	str.append("&CONTENT_TYPE=");
	str.append(CONTENT_TYPE);
	str.append("&GATEWAY_INTERFACE=");
	str.append("CGI/1.1");
	str.append("&PATH_INFO=");
	str.append(PATH_INFO);
	str.append("&PATH_TRANSLATED=");
	str.append("&QUERY_STRING="); // if nothing ?
	str.append(QUERY_STRING);
	str.append("&REMOTE_ADDR=");
	str.append("&REMOTE_USER=");
	str.append("&REMOTE_IDENT=");
	str.append("&REQUEST_METHOD=");
	str.append(REQUEST_METHOD);
	str.append("&REQUEST_URI="); // -----------
	str.append("&SCRIPT_NAME=");
	if (SCRIPT_NAME == "")
		str.append("/");
	else
		str.append(SCRIPT_NAME);
	str.append("&SERVER_NAME="); // to take
	if (SERVER_NAME == "")
		SERVER_NAME = _Config._server_name;
	else
		str.append(SERVER_NAME);
	str.append("&SERVER_PORT="); // to take
	str.append(SERVER_PORT);
	str.append("&SERVER_PROTOCOL=");
	str.append(SERVER_PROTOCOL);
	str.append("&SERVER_SOFTWARE=");
	str.append("webserv");
	// char *req_meth = ft_strjoin((const char*)"&REQUEST_METHOD=", REQUEST_METHOD.c_str());
	// char *serv_sft = ft_strjoin("&SERVER_SOFTWARE=", SERVER_SOFTWARE.c_str());
	// str = ft_strjoin(req_meth, serv_sft);
	std::cout << str.c_str() << std::endl;
	return str;
}

void Head_req::getScriptName(char *m_buffer) // remplacer par cgi extension
{
	std::string s(m_buffer), str_q, str_p;
	//REQUEST_URI
	REQUEST_URI = s;
	int len, i = 0;
	//SCRIPT_NAME
	int n = s.find(".cgi");
	if (n != (int)std::string::npos)
	{
		n = n + 4;
		len = n;
		i = n;
		while(n > 0 && m_buffer[n - 1] != '/') {n--;}
		SCRIPT_NAME = str_q.append(&m_buffer[n], len - n); 
	}
	len = n;
	//PATH_INFO
	while (m_buffer[len] != '\0' && m_buffer[len] != '?') {len++;}
	if (m_buffer[len] != '\0') {PATH_INFO = str_p.append(&m_buffer[i], len - i);}
}

std::string Head_req::getMetatoParse(char *m_buffer, std::string toParse, std::string Sep)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	const char *c_sep = Sep.c_str();
	n = s.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (m_buffer[i] != '\0') 
		{ 
			int j = 0;
			while(m_buffer[i] != c_sep[j] && c_sep[j] != '\0')
			{
				j++;
			}
			if (j != (int)ft_strlen(c_sep))
				break;
			i++;
		}
		referer = s.substr(n, i - n);
        return referer;
	}
    return "";
}

std::string Head_req::getStringtoParse(char *m_buffer, std::string toParse)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (m_buffer[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
        return referer;
	}
    return "";
}

std::string Head_req::getReferer(char *m_buffer)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find("Referer: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("Referer: ").size();
		int i = n;
		while (m_buffer[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::getUserAgent(char *m_buffer)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find("User-Agent: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("User-Agents: ").size();
		int i = n;
		while (m_buffer[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return USER_AGENT;
}

std::string Head_req::getAcceptLangage(char *m_buffer)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find("Accept-Langage: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("Accept-Langage: ").size();
		int i = n;
		while (m_buffer[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}