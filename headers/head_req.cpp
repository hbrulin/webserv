#include "head_req.hpp"

// char *Head_req::get_meta()
// {
// 	char *str = NULL;
// 	// char *req_meth = ft_strjoin((const char*)"&REQUEST_METHOD=", REQUEST_METHOD.c_str());
// 	// char *serv_sft = ft_strjoin("&SERVER_SOFTWARE=", SERVER_SOFTWARE.c_str());
// 	// str = ft_strjoin(req_meth, serv_sft);
// 	return str;
// }

std::string Head_req::getMetatoParse(char *m_buffer, std::string toParse, char *Sep)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find(toParse);
	if (n != (int)std::string::npos)
	{
        n = n + std::string(toParse).size();
		int i = n;
		while (m_buffer[i] != '\0') 
		{ 
			int j = 0;
			while(m_buffer[i] != Sep[j] && Sep[j] != '\0')
			{
				j++;
			}
			std::cout << "lala" << std::endl;
			if (j != (int)ft_strlen(Sep))
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