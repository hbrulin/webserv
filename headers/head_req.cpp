#include "head_req.hpp"

std::string Head_req::get_meta(Config _Config)
{
	memset((char *) &str, 0, sizeof(str));
	// content_env = ft_strtrim(content_env, "'/t''/0'");
	// str.append(content_env);
	str.append("&AUTH_TYPE=");
	str.append(AUTH_TYPE);
	str.append("&CONTENT_LENGTH=");
	str.append(CONTENT_LENGTH);
	str.append("&CONTENT_TYPE=");
	str.append(CONTENT_TYPE);
	str.append("&GATEWAY_INTERFACE=");
	str.append("CGI/1.1");
	str.append("&PATH_INFO=");
	str.append(PATH_INFO);
	str.append("&PATH_TRANSLATED=");
	str.append(PATH_TRANSLATED);
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
		SERVER_NAME =  _Config._server_name; // changer par localhost
	else
		str.append(SERVER_NAME);
	str.append("&SERVER_PORT="); // to take
	str.append(SERVER_PORT);
	str.append("&SERVER_PROTOCOL=");
	str.append(SERVER_PROTOCOL);
	str.append("&SERVER_SOFTWARE=");
	str.append("webserv"); // A modif ?
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
		while (m_buffer[i] != '\n' && m_buffer[i] != '\r') { i++;}
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
		while (m_buffer[i] != '\r' && m_buffer[i] != '\n') { i++;}
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

std::string Head_req::contentNego(std::string root, std::string content) {
	size_t i = 0;
	if (ACCEPT_LANGUAGE.empty())
		return root + "fr/" + content;
	std::string res = "not_acceptable";
	for (std::vector<std::string>::iterator it = ACCEPT_LANGUAGE.begin(); it!=ACCEPT_LANGUAGE.end(); ++it) {
		std::string tmp = *it;
		if ((i = tmp.find(';')) != std::string::npos)
		{
			tmp = tmp.substr(0, i);
		}
		//std::cout << tmp << std::endl;
    	std::ifstream f(root + tmp + "/" + content);
		//std::cout << root + tmp + content << std::endl;

		if (f.good()) {
			res = root + tmp + "/" + content;
			f.close();
			break;
		}
		f.close();
	}
	return res;
}

void		Head_req::parse(std::vector<std::string> parsed, char *m_buffer, std::string url) {
	REQUEST_METHOD = parsed[0];
	SERVER_PROTOCOL = parsed[2];
	char **tab = ft_split(getStringtoParse(m_buffer, "Authorization: ").c_str(), ' ');
	if (tab != NULL && tab[0] != NULL)
	AUTH_TYPE = tab[0];
	CONTENT_TYPE = getStringtoParse(m_buffer, "Content-Type: ");
	CONTENT_LENGTH = getStringtoParse(m_buffer, "Content-Length: ");
	QUERY_STRING = getMetatoParse((char *)url.c_str(), "?", (char *)" #");
	getScriptName((char *)url.c_str());
	SERVER_NAME = getMetatoParse((char *)url.c_str(), "://", ":/?#");
	if (getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "")
		SERVER_PORT = getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "";
	//_head_req.SERVER_PROTOCOL = _head_req.getMetatoParse(m_url, "", "://");
	REFERER = getReferer(m_buffer);
	USER_AGENT = getUserAgent(m_buffer);
	//rest of parsing
	if (getStringtoParse(m_buffer, "Accept-Charset: ") != "")
		ACCEPT_CHARSET = ft_split(getStringtoParse(m_buffer, "Accept-Charset: ").c_str(), ',');
	if (getStringtoParse(m_buffer, "Transfer-Encoding: ") != "")
		TRANSFER_ENCODING = ft_split(getStringtoParse(m_buffer, "Transfer-Encoding: ").c_str(), ',');
	DATE = getStringtoParse(m_buffer, "Date: ");

	//parsing languages into vector
	std::string lg = getStringtoParse(m_buffer, "Accept-Language: ");
	if (lg != "")
	{
		std::stringstream s(lg);
		std::string segment;
		while(std::getline(s, segment, ','))
		{
			ACCEPT_LANGUAGE.push_back(segment);
		}
	}

}