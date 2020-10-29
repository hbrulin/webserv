#include "head_req.hpp"

std::string Head_req::get_meta()
{
	ft_memset((char *) &str, 0, sizeof(str));
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
	CONTENT_LANGUAGE = "fr";
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
			CONTENT_LANGUAGE = tmp;
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

void		Head_req::parse(std::string s, std::string url) {	
	if (ft_strstr(s.c_str(), "X-") != NULL)
 	{
 		X_headers = getXtoparse(s, "X-");
 	}
	char **tab = ft_split(getStringtoParse(s, AUTH_STR).c_str(), ' ');
	if (tab != NULL && tab[0] != NULL)
	AUTH_TYPE = tab[0];
	ACCEPT_ENCODING = getStringtoParse(s, ACCEPT_EN_STR);
	CONTENT_TYPE = getStringtoParse(s, CONTENT_T_STR);
	if (ft_strstr(s.c_str(), CONTENT_L_STR) != NULL)
		CONTENT_LENGTH = getStringtoParse(s, CONTENT_L_STR);
	QUERY_STRING = getMetatoParse((char *)url.c_str(), "?", (char *)" #");
	SERVER_NAME = getMetatoParse((char *)url.c_str(), "://", ":/?#");
	if (getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "")
		SERVER_PORT = getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "";
	REFERER = getReferer(s);
	USER_AGENT = getUserAgent(s);
	if (getStringtoParse(s, ACCEPT_CHAR_STR) != "")
		ACCEPT_CHARSET = ft_split(getStringtoParse(s, ACCEPT_CHAR_STR).c_str(), ',');
	if (ft_strstr(s.c_str(), TRANSFER_EN_STR) != NULL)
		TRANSFER_ENCODING = getTransferEncoding(s);
	DATE = getStringtoParse(s, DATE_STR);	
	getLanguageVector(s);
	ft_tabdel((void **)tab);
}

void Head_req::getLanguageVector(std::string buffer)
{
	std::string lg = getStringtoParse(buffer, ACCEPT_LAN_STR);
	if (lg == "")
		return;
	std::string copy = lg;
	size_t pos = 0;
	while ((pos = copy.find(" ")) != std::string::npos) {
		ACCEPT_LANGUAGE.push_back(copy.substr(0, pos));
		copy.erase(0, pos + 1);
	}	
	ACCEPT_LANGUAGE.push_back(copy);
}