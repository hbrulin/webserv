#include "head_req.hpp"

std::string Head_req::getStringtoParse(char *m_buffer, std::string toParse)
{
    int n;
	std::string s(m_buffer);
    std::string referer;
	n = s.find(toParse);
	if (n != std::string::npos)
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
	if (n != std::string::npos)
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
	if (n != std::string::npos)
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
	if (n != std::string::npos)
	{
        n = n + std::string("Accept-Langage: ").size();
		int i = n;
		while (m_buffer[i] != '\r') { i++;}
		referer = s.substr(n, i - n);
	}
    return referer;
}