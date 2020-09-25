#include "head_resp.hpp"
#include "returnCodes.hpp"

//Header::~Header() {}

std::string Head_resp::getLastModified(const char *path) {
    if (lstat((const char *)path, &st) == 0)
	{
		char time[200];
		struct tm *tm =  localtime(&st.st_mtime);	
		if (strftime(time, sizeof(time), "%Y-%m-%d %H:%M", tm) != 0)
			LAST_MODIFIED = std::string(time) + " GMT";	
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
			if (strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", tm) != 0)
				DATE = std::string(time) + " GMT";
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

std::string Head_resp::getBuffer(int code, const char *fichier, std::vector<std::string> methods)
{
	//std::cout << "!!!!!" << fichier << std::endl;
    std::ostringstream oss;
	oss << "HTTP/1.1 " << code;
	oss << codeTab.find(code)->second;
	oss << "Cache-Control: no-cache, private\r\n";
    oss << "Content-Type: text/html" << "\r\n";
	oss << "Content-Langage: " << CONTENT_LANGUAGE << "\r\n";
    oss << "Content-Length: " << getContentLength(fichier) << "\r\n";
	//oss << "Transfer-Encoding: deflate\r\n";
	oss << "Content-Location: " << fichier << "\r\n";
	/*if (WWW_AUTHENTICATE != NULL)
		oss << "WWW-Authenticate: " << WWW_AUTHENTICATE[0]<< " " << WWW_AUTHENTICATE[1]  << "\r\n";*/
    oss << "Date: " << this->getDate() << "\r\n";
    oss << "Server: " << SERVER << "\r\n";
    oss << "Last-Modified: " << this->getLastModified(fichier) << "\r\n";
    if (code == 503) // Not available
        oss << "Retry-after: " << RETRY_AFTER << "\r\n";
	if (code == 405)
	{
		oss << "Allow: " << ALLOW << std::endl;
		oss << "Server methods: ";
		 for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
		 	oss << *it << ", ";
		oss << "\r\n";
	}
	oss << "\r\n";
    return oss.str();
}