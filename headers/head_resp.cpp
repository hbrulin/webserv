#include "head_resp.hpp"

//Header::~Header() {}

std::string Head_resp::getLastModified(const char *path) {
    if (lstat((const char *)path, &st) == 0)
	{
		char time[200];
		struct tm *tm =  localtime(&st.st_mtime);	
		if (strftime(time, sizeof(time), TIME_FORMAT, tm) != 0)
			LAST_MODIFIED = std::string(time) + GMT;	
	}
    return LAST_MODIFIED;
}

std::string Head_resp::getDate()
{
		struct timeval tv;
		if (gettimeofday(&tv, NULL) == 0)
		{
			//std::cout << tv.tz_minuteswest << std::endl;
			time_t t = tv.tv_sec;
			struct tm *tm =  localtime(&t);
			char time[200];
			if (strftime(time, sizeof(time), TIME_FORMAT, tm) != 0)
				DATE = std::string(time) + GMT;
		}
        return DATE;
}

std::string Head_resp::getContentLength(const char *path)
{
	CONTENT_LENGTH = "";
	if (lstat((const char *)path, &st) == 0)
	{
		size_t length = (size_t)st.st_size;
		CONTENT_LENGTH = std::to_string(length);	
	}
    return CONTENT_LENGTH;
}

std::string Head_resp::getBuffer(int code, const char *fichier, std::vector<std::string> methods, std::string _method)
{
	std::string ret;
	//std::cout << "!!!!!" << fichier << std::endl;
    //std::ostringstream oss;
	ret = (std::string)DEF_PROTOCOL + " ";
	ret += std::to_string(code) + codeTab.find(code)->second;
	ret += CACHE_STR;
    ret += (std::string)CONTENT_T_STR + "text/html\r\n";
	ret += CONTENT_LANG_STR + CONTENT_LANGUAGE + "\r\n";
	if (_method == PUT || (_method == POST && getContentLength(fichier) == ""))
		ret+= (std::string)CONTENT_L_STR + "0\r\n";
	else
		ret+= (std::string) CONTENT_L_STR + getContentLength(fichier) + "\r\n";
	//oss << "Transfer-Encoding: deflate\r\n";
	if (code != 201)
		ret += CONTENT_LOC_STR + (std::string)fichier + "\r\n";
	/*if (WWW_AUTHENTICATE != NULL)
		oss << "WWW-Authenticate: " << WWW_AUTHENTICATE[0]<< " " << WWW_AUTHENTICATE[1]  << "\r\n";*/
    ret += DATE_STR + this->getDate() + "\r\n";
    ret += SERVER_STR + SERVER + "\r\n";
    ret += LAST_MOD_STR + this->getLastModified(fichier) + "\r\n";
    if (code == 503) 
        ret += RETRY_STR + RETRY_AFTER + "\r\n";
	if (code == 405)
	{
		ret += ALLOW_STR + ALLOW + "\r\n";
		ret += SERVER_METHODS_STR;
		 for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
		 	ret += *it + ", ";
		ret += "\r\n";
	}
	if (code == 201)
		ret += "Location: " + LOCATION + "\r\n";
	ret += "\r\n";
    return ret;
}

std::string Head_resp::getBuffer_cgi(int code, std::string m_body, std::string X_head)
{
    //std::ostringstream oss;
	std::string ret;
	ret = (std::string)DEF_PROTOCOL + " ";
	ret += std::to_string(code) + codeTab.find(code)->second;
	ret += CONTENT_L_STR + std::to_string(m_body.size()) + "\r\n";
    ret += CONTENT_T_STR + CONTENT_TYPE + "\r\n";
	ret += DATE_STR + this->getDate() + "\r\n";
	ret += SERVER_STR + SERVER + "\r\n";
	if (X_head != "")
 	 	ret += "http-" + X_head + "\r\n";
	ret += "\r\n";
    return ret;
}