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
	//std::cout << "!!!!!" << fichier << std::endl;
    std::ostringstream oss;
	oss << DEF_PROTOCOL << " " << code;
	oss << codeTab.find(code)->second;
	oss << CACHE_STR;
    oss << CONTENT_T_STR << "text/html" << "\r\n";
	oss << CONTENT_LANG_STR << CONTENT_LANGUAGE << "\r\n";
	if (_method == PUT || (_method == POST && getContentLength(fichier) == ""))
		oss << CONTENT_L_STR << "0" << "\r\n";
	else
		oss << CONTENT_L_STR << getContentLength(fichier) << "\r\n";
	//oss << "Transfer-Encoding: deflate\r\n";
	oss << CONTENT_LOC_STR << fichier << "\r\n";
	/*if (WWW_AUTHENTICATE != NULL)
		oss << "WWW-Authenticate: " << WWW_AUTHENTICATE[0]<< " " << WWW_AUTHENTICATE[1]  << "\r\n";*/
    oss << DATE_STR << this->getDate() << "\r\n";
    oss << SERVER_STR << SERVER << "\r\n";
    oss << LAST_MOD_STR << this->getLastModified(fichier) << "\r\n";
    if (code == 503) // Not available
        oss << RETRY_STR << RETRY_AFTER << "\r\n";
	if (code == 405)
	{
		oss << ALLOW_STR << ALLOW << std::endl;
		oss << SERVER_METHODS_STR;
		 for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
		 	oss << *it << ", ";
		oss << "\r\n";
	}
	oss << "\r\n";
    return oss.str();
}

std::string Head_resp::getBuffer_cgi(int code, std::string m_body, std::string X_head)
{
    std::ostringstream oss;
	oss << DEF_PROTOCOL << " " << code;
	oss << codeTab.find(code)->second;
	oss << CONTENT_L_STR << m_body.size() << "\r\n";
    oss << CONTENT_T_STR << CONTENT_TYPE << "\r\n";
	oss << DATE_STR << this->getDate() << "\r\n";
	oss << SERVER_STR << SERVER << "\r\n";
	if (X_head != "")
 	 	oss << "http-" << X_head << "\r\n";
	oss << "\r\n";
    return oss.str();
}

Head_resp::~Head_resp(){}
Head_resp::Head_resp(const Head_resp &copy)
{
	this->CONTENT_LENGTH = copy.CONTENT_LENGTH;
}

Head_resp &Head_resp::operator=(const Head_resp &copy)
{
	this->CONTENT_LENGTH = copy.CONTENT_LENGTH;
	return *this;
}