#include "head_req.hpp"

std::string Head_req::get_meta()
{
	memset((char *) &str, 0, sizeof(str));
	// content_env = ft_strtrim(content_env, "'/t''/0'");
	// str.append(content_env);
	//ajouter http_request_method et http_content-type ???
	str.append("&HTTP_HOST=");
	str.append(HOST);
	str.append("&HTTP_REFERER=");
	str.append("");
	str.append("&HTTP_USER_AGENT=");
	str.append(USER_AGENT);
	str.append("&HTTP_ACCEPT_ENCODING=");
	str.append(ACCEPT_ENCODING);
	str.append("&HTTP_TRANSFER_ENCODING=");
	str.append(TRANSFER_ENCODING);
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
	str.append("&REMOTE_ADDR="); //AJOUTER avec l'adressse IP
	str.append(REMOTE_ADDR);
	str.append("&REMOTE_USER="); //AJOUTER en dur
	str.append("user");
	str.append("&REMOTE_IDENT=");
	str.append("login_user");
	str.append("&REQUEST_METHOD=");
	str.append(REQUEST_METHOD);
	str.append("&REQUEST_URI=");
	str.append(REQUEST_URI);
	str.append("&SCRIPT_NAME=");
	str.append(SCRIPT_NAME);
	str.append("&SERVER_NAME="); // to take
	str.append(SERVER_NAME);
	str.append("&SERVER_PORT="); // to take
	str.append(SERVER_PORT);
	str.append("&SERVER_PROTOCOL=");
	str.append(SERVER_PROTOCOL);
	str.append("&SERVER_SOFTWARE=");
	str.append("webserv"); // A modif ?
	//str.append("&HTTP_SECRET_HEADER_FOR_TEST=1");
	//str.append("&HTTP_X_SECRET_HEADER_FOR_TEST=1");
	std::string tmp = "";
	for (size_t i = 0; i != X_headers.size(); i++)
	{
		if (X_headers.at(i) == ':')
			tmp+= '=';
		else if (X_headers.at(i) == 'X' && X_headers.at(i+1) == '-')
		{
			tmp += '&';
			i++;
		}
		else if (X_headers.at(i) == '-')
			tmp += '_';
		else if (X_headers.at(i) != ' ')
			tmp += std::toupper(X_headers.at(i));
	}
	char **head_split = ft_split(tmp.c_str(), '&');
	int j = 0;
	while (head_split && head_split[j])
	{
			str.append("&HTTP_X_");
			str.append(head_split[j]);
			j++;
	}
	j = 0;
	while (head_split[j])
	{
		free(head_split[j]);
		j++;
	}
	free(head_split);
	return str;
}

void Head_req::getScriptName(std::string s) // remplacer par cgi extension
{
	std::string str_q, str_p;
	//REQUEST_URI
	REQUEST_URI = s;
	int len, i = 0;
	//SCRIPT_NAME
	int n = s.find(CGI);
	if (n != (int)std::string::npos)
	{
		n = n + 4;
		len = n;
		i = n;
		while(n > 0 && s[n - 1] != '/') {n--;}
		SCRIPT_NAME = str_q.append(&s[n], len - n); 
	}
	len = n;
	//PATH_INFO
	while (s[len] != '\0' && s[len] != '?') {len++;}
	if (s[len] != '\0') {PATH_INFO = str_p.append(&s[i], len - i);}
}

std::string Head_req::getMetatoParse(std::string s, std::string toParse, std::string Sep)
{
    int n;
    std::string referer;
	const char *c_sep = Sep.c_str();
	n = s.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (s[i] != '\0') 
		{ 
			int j = 0;
			while(s[i] && s[i] != c_sep[j] && c_sep[j] != '\0')
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

std::string Head_req::getStringtoParse(std::string s, std::string toParse)
{
    int n;
    std::string referer;
	n = s.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (s[i] && s[i] != '\n' && s[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
        return referer;
	}
    return "";
}

std::string Head_req::getReferer(std::string s)
{
    int n;
    std::string referer;
	n = s.find(REFERER_STR);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(REFERER_STR).size();
		int i = n;
		while (s[i] && s[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::getUserAgent(std::string s)
{
    int n;
    std::string referer;
	n = s.find(USER_AGENT_STR);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(USER_AGENT_STR).size();
		int i = n;
		while (s[i] && s[i] != '\r' && s[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::getAcceptLangage(std::string s)
{
    int n;
    std::string referer;
	n = s.find(ACCEPT_LAN_STR);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(ACCEPT_LAN_STR).size();
		int i = n;
		while (s[i] && s[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::getTransferEncoding(std:: string s)
{
    int n;
    std::string referer;
	n = s.find(TRANSFER_EN_STR);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(TRANSFER_EN_STR).size();
		int i = n;
		while (s[i] && s[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::contentNego(std::string root) {
	size_t i = 0;
	if (ACCEPT_LANGUAGE.empty())
		return root + "fr/";
	std::string res = root + "fr/";
	for (std::vector<std::string>::iterator it = ACCEPT_LANGUAGE.begin(); it!=ACCEPT_LANGUAGE.end(); ++it) {
		std::string tmp = *it;
		if ((i = tmp.find(';')) != std::string::npos)
		{
			tmp = tmp.substr(0, i);
		}
    	std::ifstream f(root + tmp + "/");

		if (f.good()) {
			res = root + tmp + "/";
			f.close();
			break;
		}
		f.close();
	}
	return res;
}

std::string Head_req::getXtoparse(std::string s, std::string toParse)
{
    int n;
	std::string tmp(s);
    std::string referer = "";
	n = tmp.find(toParse);
	while (n != (int)std::string::npos)
	{
		int i = n;
		while (tmp[i] != '\n' && tmp[i] != '\r') { i++;}
		referer += tmp.substr(n, i - n);
		tmp = tmp.substr(i, tmp.size() - i);
        n = tmp.find(toParse);
	}
    return referer;
}

void Head_req::getRemAddr()
{	
	int count = 0;
	std::string s(USER_AGENT);
	int begin = 0;
	int cpy = 0;
	int end = 0;

	while (!(isdigit(s[begin])) && s[begin])
		begin++;
	cpy = begin;
	while (count < 3)
	{
		end = cpy + 1;
		while (s[end] != '.' && s[end])
			end++;
		if (end - cpy < 5)
		{
			count++;
			cpy = end + 1;
		}
		else
		{
			cpy++;
			while (!(isdigit(s[cpy])))
				cpy++;
			begin = cpy;
			count = 0;
		}
	}
	if (count == 3)
	{
		while (s[end] != ' ' && s[end])
			end++;
	}
	REMOTE_ADDR = s.substr(begin, end - begin);
}


void		Head_req::parse(std::vector<std::string> parsed, std::string m_buffer, std::string url) {
	REQUEST_METHOD = parsed[0];
	REQUEST_URI = parsed[1];
	SERVER_PROTOCOL = parsed[2];
	if (strstr(m_buffer.c_str(), "X-") != NULL)
 	{
 		X_headers = getXtoparse(m_buffer, "X-");
 	}
	char **tab = ft_split(getStringtoParse(m_buffer, AUTH_STR).c_str(), ' ');
	if (tab != NULL && tab[0] != NULL)
	AUTH_TYPE = tab[0];
	ACCEPT_ENCODING = getStringtoParse(m_buffer, ACCEPT_EN_STR);
	CONTENT_TYPE = getStringtoParse(m_buffer, CONTENT_T_STR);
	if (strstr(m_buffer.c_str(), CONTENT_L_STR) != NULL)
		CONTENT_LENGTH = getStringtoParse(m_buffer, CONTENT_L_STR);
	QUERY_STRING = getMetatoParse((char *)url.c_str(), "?", (char *)" #");
	getScriptName(url);
	SERVER_NAME = getMetatoParse((char *)url.c_str(), "://", ":/?#");
	if (getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "")
		SERVER_PORT = getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "";
	//_head_req.SERVER_PROTOCOL = _head_req.getMetatoParse(m_url, "", "://");
	REFERER = getReferer(m_buffer);
	USER_AGENT = getUserAgent(m_buffer);
	getRemAddr();
	//rest of parsing
	if (getStringtoParse(m_buffer, ACCEPT_CHAR_STR) != "")
		ACCEPT_CHARSET = ft_split(getStringtoParse(m_buffer, ACCEPT_CHAR_STR).c_str(), ',');
	if (strstr(m_buffer.c_str(), TRANSFER_EN_STR) != NULL)
		TRANSFER_ENCODING = getTransferEncoding(m_buffer);
	DATE = getStringtoParse(m_buffer, DATE_STR);	
	//parsing languages into vector
	std::string lg = getStringtoParse(m_buffer, ACCEPT_LAN_STR);
	if (lg != "")
	{
		std::stringstream s(lg);
		std::string segment;
		while(std::getline(s, segment, ','))
			ACCEPT_LANGUAGE.push_back(segment);
	}
	int i = 0;
	if (tab != NULL && tab[0] != NULL)
	{
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
	}
	free(tab);
}