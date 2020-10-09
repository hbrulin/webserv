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
	str.append("&HTTP_X_SECRET_HEADER_FOR_TEST=1");
	return str;
}

void Head_req::getScriptName(std::string s) // remplacer par cgi extension
{
	std::string str_q, str_p;
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
			while(s[i] != c_sep[j] && c_sep[j] != '\0')
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
		while (s[i] != '\n' && s[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
		//std::cout << referer << std::endl;
        return referer;
	}
    return "";
}

std::string Head_req::getReferer(std::string s)
{
    int n;
    std::string referer;
	n = s.find("Referer: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("Referer: ").size();
		int i = n;
		while (s[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
	}
	//std::cout << "referer" << referer << std::endl;
    return referer;
}

std::string Head_req::getUserAgent(std::string s)
{
    int n;
    std::string referer;
	n = s.find("User-Agent: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("User-Agent: ").size();
		int i = n;
		while (s[i] != '\r' && s[i] != '\n') { i++;}
		referer = s.substr(n, i - n);
		//std::cout << referer << std::endl;
	}
    return referer;
}

std::string Head_req::getAcceptLangage(std::string s)
{
    int n;
    std::string referer;
	n = s.find("Accept-Langage: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("Accept-Langage: ").size();
		int i = n;
		while (s[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}

std::string Head_req::getTransferEncoding(std:: string s)
{
    int n;
    std::string referer;
	n = s.find("Transfer-Encoding: ");
	if (n != (int)std::string::npos)
	{
        n = n + std::string("Transfer-Encoding: ").size();
		int i = n;
		while (s[i] != '\r') { i++;}
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
		//std::cout << tmp << std::endl;
    	std::ifstream f(root + tmp + "/");
		//std::cout << root + tmp + content << std::endl;

		if (f.good()) {
			res = root + tmp + "/";
			f.close();
			break;
		}
		f.close();
	}
	return res;
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
	char **tab = ft_split(getStringtoParse(m_buffer, "Authorization: ").c_str(), ' ');
	if (tab != NULL && tab[0] != NULL)
	AUTH_TYPE = tab[0];
	ACCEPT_ENCODING = getStringtoParse(m_buffer, "Accept-Encoding: ");
	CONTENT_TYPE = getStringtoParse(m_buffer, "Content-Type: ");
	if (strstr(m_buffer.c_str(), "Content-Length: ") != NULL)
		CONTENT_LENGTH = getStringtoParse(m_buffer, "Content-Length: ");
	QUERY_STRING = getMetatoParse((char *)url.c_str(), "?", (char *)" #");
	getScriptName((char *)url.c_str());
	SERVER_NAME = getMetatoParse((char *)url.c_str(), "://", ":/?#");
	if (getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "")
		SERVER_PORT = getMetatoParse((char*)url.c_str(), SERVER_NAME + ":", "?/#") != "";
	//_head_req.SERVER_PROTOCOL = _head_req.getMetatoParse(m_url, "", "://");
	REFERER = getReferer(m_buffer);
	USER_AGENT = getUserAgent(m_buffer);
	getRemAddr();
	//rest of parsing
	if (getStringtoParse(m_buffer, "Accept-Charset: ") != "")
		ACCEPT_CHARSET = ft_split(getStringtoParse(m_buffer, "Accept-Charset: ").c_str(), ',');
	if (strstr(m_buffer.c_str(), "Transfer-Encoding: ") != NULL)
		TRANSFER_ENCODING = getTransferEncoding(m_buffer);
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